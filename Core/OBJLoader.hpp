#pragma once
#include "Engine/Core/Vertex_PCUTBN.hpp"
#include <vector>
// -----------------------------------------------------------------------------
struct OBJTriIndexes
{
	// TRIANGLE FACES (f) IN FORMAT: v1 v2 v3
	int v1[3];  
	int v2[3];  
	int v3[3];

	OBJTriIndexes() 
	{
		for (int triIndex = 0; triIndex < 3; ++triIndex) 
		{
			v1[triIndex] = -1;
			v2[triIndex] = -1;
			v3[triIndex] = -1;
		}
	};
};
// -----------------------------------------------------------------------------
bool LoadOBJMeshFile(std::vector<Vertex_PCUTBN>& meshVerts, char const* objFilePath); // This is what gets called from Game, calls ParseOBJMeshFile
void ComputeMissingNormals(std::vector<Vertex_PCUTBN>& meshVerts);
void ComputeMissingTangentsAndBitangents(std::vector<Vertex_PCUTBN>& meshVerts);
bool ParseOBJWithSplitStrings(std::vector<Vertex_PCUTBN>& meshVerts, char const* objFilePath); // This is doing most of the work, may need more arguments
bool ParseOBJMeshFile(std::vector<Vertex_PCUTBN>& meshVerts, char const* objFilePath); // This will only be one to two lines calling ParseOBJWithSplitStrings