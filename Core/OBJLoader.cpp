#include "Engine/Core/OBJLoader.hpp"
#include "Engine/Core/EngineCommon.h"
#include "Engine/Core/VertexUtils.h"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Math/MathUtils.h"

bool LoadOBJMeshFile(std::vector<Vertex_PCUTBN>& meshVerts, char const* objFilePath)
{
	bool success = ParseOBJMeshFile(meshVerts, objFilePath);
	GUARANTEE_OR_DIE(success == true, Stringf("Failed to load OBJ file \"%s\"", objFilePath));

	if (success)
	{
		ComputeMissingNormals(meshVerts);
		ComputeMissingTangentsAndBitangents(meshVerts);
	}

	return success;
}

void ComputeMissingNormals(std::vector<Vertex_PCUTBN>& meshVerts)
{
	for (int vertIndex = 0; vertIndex < static_cast<int>(meshVerts.size()); ++vertIndex)
	{
		Vertex_PCUTBN& vert = meshVerts[vertIndex];

		// Check if our obj has a normal or not
		if (vert.m_normal == Vec3::ZERO)
		{
			Vec3 newNormal = Vec3::ZERO;
			for (int faceIndex = 0; faceIndex < static_cast<int>(meshVerts.size()); faceIndex += 3)
			{
				Vec3 p0 = meshVerts[faceIndex].m_position;
				Vec3 p1 = meshVerts[faceIndex + 1].m_position;
				Vec3 p2 = meshVerts[faceIndex + 2].m_position;

				Vec3 triSide1 = p1 - p0;
				Vec3 triSide2 = p2 - p0;
				Vec3 triNormal = CrossProduct3D(triSide1, triSide2).GetNormalized();

				for (int objTriIndex = 0; objTriIndex < 3; ++objTriIndex)
				{
					if (meshVerts[faceIndex + objTriIndex].m_normal == Vec3::ZERO)
					{
						meshVerts[faceIndex + objTriIndex].m_normal += triNormal;
					}
				}
			}
			vert.m_normal = newNormal.GetNormalized();
		}
	}
}

void ComputeMissingTangentsAndBitangents(std::vector<Vertex_PCUTBN>& meshVerts)
{
	// Iterate over the triangles.
	for (int vertIndex = 0; vertIndex < static_cast<int>(meshVerts.size()); vertIndex += 3)
	{
		Vertex_PCUTBN& v0 = meshVerts[vertIndex];
		Vertex_PCUTBN& v1 = meshVerts[vertIndex + 1];
		Vertex_PCUTBN& v2 = meshVerts[vertIndex + 2];

		// Positions
		Vec3 p0 = v0.m_position;
		Vec3 p1 = v1.m_position;
		Vec3 p2 = v2.m_position;

		// UVs
		Vec2 uv0 = v0.m_uvTexCoords;
		Vec2 uv1 = v1.m_uvTexCoords;
		Vec2 uv2 = v2.m_uvTexCoords;

		// Edges and deltas
		Vec3 edge0 = p1 - p0;
		Vec3 edge1 = p2 - p0;
		float deltaU0 = uv1.x - uv0.x;
		float deltaV0 = uv1.y - uv0.y;
		float deltaU1 = uv2.x - uv0.x;
		float deltaV1 = uv2.y - uv0.y;

		// Calculate the reciprocal r of our determinant
		float r = 1.f / (deltaU0 * deltaV1 - deltaU1 * deltaV0);

		// Tangent
		Vec3 tangent = r * (deltaV1 * edge0 - deltaV0 * edge1).GetNormalized();

		// Bitangent
		Vec3 biTangent = r * (deltaU0 * edge1 - deltaU1 * edge0).GetNormalized();

		// Normalize add T and B to all three vertex tangents and biTangents.
		v0.m_tangent = tangent;
		v0.m_bitangent = biTangent;

		v1.m_tangent = tangent;
		v1.m_bitangent = biTangent;

		v2.m_tangent = tangent;
		v2.m_bitangent = biTangent;
	}

	for (int vertIndex = 0; vertIndex < static_cast<int>(meshVerts.size()); ++vertIndex)
	{
		Vertex_PCUTBN& vert = meshVerts[vertIndex];

		// TBN basis vectors
		Vec3& T = vert.m_tangent;
		Vec3& B = vert.m_bitangent;
		Vec3& N = vert.m_normal;

		// Apply Gram–Schmidt orthonormalization keeping N unchanged.
		T = (T - DotProduct3D(N, T) * N).GetNormalized();
		B = CrossProduct3D(N, T);
	}
}

bool ParseOBJWithSplitStrings(std::vector<Vertex_PCUTBN>& meshVerts, char const* objFilePath)
{
	std::string fileText;
	int result = FileReadToString(fileText, objFilePath);
	GUARANTEE_OR_DIE(result == FILE_SUCCESS, Stringf("Failed to read OBJ file \"%s\"", objFilePath));

	Strings textLines = SplitStringOnDelimiter(fileText, '\n');

	std::vector<Vec3> positions;
	std::vector<Vec2> uvs;
	std::vector<Vec3> normals;
	std::vector<OBJTriIndexes> triIndexes;

	size_t numLines = textLines.size();
	meshVerts.reserve(numLines / 2);
	positions.reserve(numLines / 4);
	uvs.reserve(numLines / 4);
	normals.reserve(numLines / 4);
	triIndexes.reserve(numLines / 2);

	for (int lineIndex = 0; lineIndex < static_cast<int>(textLines.size()); ++lineIndex)
	{
		std::string const& line = textLines[lineIndex];
		if (line.empty() || line[0] == '#')
		{
			continue;
		}

		Strings args = SplitStringOnWhiteSpace(line);
		if (args.empty())
		{
			continue;
		}

		std::string const& command = args[0];
		if (command == "v")
		{
			GUARANTEE_OR_DIE(args.size() >= 4, Stringf("Malformed vertex position in OBJ \"%s\": \"%s\"", objFilePath, line.c_str()));
			Vec3 vPositions = CreateVec3FromStrings(args[1], args[2], args[3]);
			positions.push_back(vPositions);
		}
		else if (command == "vt")
		{
			GUARANTEE_OR_DIE(args.size() >= 3, Stringf("Malformed vertex uvs in OBJ \"%s\": \"%s\"", objFilePath, line.c_str()));
			Vec2 vtUVs = CreateVec2FromStrings(args[1], args[2]);
			uvs.push_back(vtUVs);
		}
		else if (command == "vn")
		{
			GUARANTEE_OR_DIE(args.size() == 4, Stringf("Malformed face in OBJ \"%s\": \"%s\"", objFilePath, line.c_str()));
			Vec3 vnNormals = CreateVec3FromStrings(args[1], args[2], args[3]);
			normals.push_back(vnNormals);
		}
		else if (command == "f")
		{
			int numVerts = static_cast<int>(args.size()) - 1;
			GUARANTEE_OR_DIE(numVerts >= 3, Stringf("Malformed face in OBJ \"%s\": \"%s\"", objFilePath, line.c_str()));

			for (int startIndex = 1; startIndex < numVerts - 1; ++startIndex)
			{
				OBJTriIndexes tris;
				for (int triIndex = 0; triIndex < 3; ++triIndex)
				{
					Strings fTris0 = SplitStringOnDelimiter(args[1], '/');

					// Positions
					tris.v1[triIndex] = CreateIntFromStrings(fTris0[0]) - 1;

					// Uvs
					if (fTris0.size() > 1 && !fTris0[1].empty())
					{
						tris.v2[triIndex] = CreateIntFromStrings(fTris0[1]) - 1;
					}

					// Normals
					if (fTris0.size() > 2 && !fTris0[2].empty())
					{
						tris.v3[triIndex] = CreateIntFromStrings(fTris0[2]) - 1;
					}
				}

				Strings fTris1 = SplitStringOnDelimiter(args[startIndex + 1], '/');

				// Positions
				tris.v1[0] = CreateIntFromStrings(fTris1[0]) - 1;

				// Uvs
				if (fTris1.size() > 1 && !fTris1[1].empty())
				{
					tris.v2[0] = CreateIntFromStrings(fTris1[1]) - 1;
				}

				// Normals
				if (fTris1.size() > 2 && !fTris1[2].empty())
				{
					tris.v3[0] = CreateIntFromStrings(fTris1[2]) - 1;
				}

				Strings fTris2 = SplitStringOnDelimiter(args[startIndex + 2], '/');

				// Positions
				tris.v1[1] = CreateIntFromStrings(fTris2[0]) - 1;

				// Uvs
				if (fTris2.size() > 1 && !fTris2[1].empty())
				{
					tris.v2[1] = CreateIntFromStrings(fTris2[1]) - 1;
				}

				// Normals
				if (fTris2.size() > 2 && !fTris2[2].empty())
				{
					tris.v3[1] = CreateIntFromStrings(fTris2[2]) - 1;
				}

				triIndexes.push_back(tris);
			}
		}
	}

	for (int objTriIndex = 0; objTriIndex < static_cast<int>(triIndexes.size()); ++objTriIndex)
	{
		OBJTriIndexes const& tri = triIndexes[objTriIndex];

		for (int numTris = 0; numTris < 3; ++numTris)
		{
			// Positions
			Vec3 position = positions[tri.v1[numTris]];

			// UVs
			Vec2 uv = Vec2::ZERO;
			if (tri.v2[numTris] >= 0 && tri.v2[numTris] < static_cast<int>(uvs.size()))
			{
				uv = uvs[tri.v2[numTris]];
			}

			// Normals
			Vec3 normal = Vec3::ZERO;
			if (tri.v3[numTris] >= 0 && tri.v3[numTris] < static_cast<int>(normals.size()))
			{
				normal = normals[tri.v3[numTris]];
			}

			meshVerts.push_back(Vertex_PCUTBN(position, Rgba8::WHITE, uv, Vec3::ZERO, Vec3::ZERO, normal));
		}
	}

	return true;
}

bool ParseOBJMeshFile(std::vector<Vertex_PCUTBN>& meshVerts, char const* objFilePath)
{
	return ParseOBJWithSplitStrings(meshVerts, objFilePath);
}
