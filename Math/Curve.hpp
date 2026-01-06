#pragma once
#include <vector>
// -----------------------------------------------------------------------------
class Curve1D
{
public:
	// Base class with one pure virtual function
	virtual ~Curve1D() = default;
	virtual float Evaluate(float t) = 0;
};
// -----------------------------------------------------------------------------
struct CurveSegment
{
	float    m_startTime = 0.f;
	Curve1D* m_curve = nullptr;
};
// -----------------------------------------------------------------------------
class LinearCurve1D : public Curve1D
{
public:
	float Evaluate(float t) override;

public:
	float m_start = 0.f;
	float m_end   = 1.f;
};
// -----------------------------------------------------------------------------
class PieceWiseCurve1D : public Curve1D
{
public:
	~PieceWiseCurve1D();
	void  AddCurveSegment(float startTime, Curve1D* curve);
	float Evaluate(float t) override;

public:
	std::vector<CurveSegment> m_subCurves;
};
// -----------------------------------------------------------------------------