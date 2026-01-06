#include "Engine/Math/Curve.hpp"
#include "Engine/Math/MathUtils.h"
#include <algorithm>
// -----------------------------------------------------------------------------
float LinearCurve1D::Evaluate(float t)
{
	if (t < m_start)
	{
		return 0.f;
	}
	else if (t >= m_end)
	{
		return 1.f;
	}
	else
	{
		return Interpolate(m_start, m_end, t);
	}
}
// -----------------------------------------------------------------------------
PieceWiseCurve1D::~PieceWiseCurve1D()
{
	for (int subCurveIndex = 0; subCurveIndex < static_cast<int>(m_subCurves.size()); ++subCurveIndex)
	{
		CurveSegment& subCurve = m_subCurves[subCurveIndex];
		delete subCurve.m_curve;
		subCurve.m_curve = nullptr;
	}
}

void PieceWiseCurve1D::AddCurveSegment(float startTime, Curve1D* curve)
{
	if (curve == nullptr)
	{
		return;
	}

	CurveSegment subCurve;
	subCurve.m_startTime = startTime;
	subCurve.m_curve = curve;

	m_subCurves.push_back(subCurve);

	// Sorting here to keep subcurves sorted by time
	std::sort(m_subCurves.begin(), m_subCurves.end(),
	[](CurveSegment const& firstSubCurve, CurveSegment const& secondSubCurve)
	{
		return firstSubCurve.m_startTime < secondSubCurve.m_startTime;
	});
}

float PieceWiseCurve1D::Evaluate(float t)
{
	int const numSubCurves = static_cast<int>(m_subCurves.size());

	// Check if there are enough subCurves to evaluate
	if (numSubCurves < 2)
	{
		return 0.f;
	}

	for (int subCurveIndex = 0; subCurveIndex < numSubCurves - 1; ++subCurveIndex)
	{
		CurveSegment const& start = m_subCurves[subCurveIndex];
		CurveSegment const& end   = m_subCurves[subCurveIndex + 1];

		if (t >= start.m_startTime && t < end.m_startTime)
		{
			return start.m_curve->Evaluate(Interpolate(start.m_startTime, end.m_startTime, t));
		}
	}

	if (t < m_subCurves.front().m_startTime)
	{
		return m_subCurves.front().m_curve->Evaluate(0.f);
	}
	else if (t >= m_subCurves.back().m_startTime)
	{
		return m_subCurves.back().m_curve->Evaluate(1.f);
	}

	return 0.f;
}
// -----------------------------------------------------------------------------