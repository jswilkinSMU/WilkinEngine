#pragma once
#include "Engine/Math/Vec2.hpp"
//-------------------------------------------------------
struct AABB2
{
	static const AABB2 ZERO_TO_ONE;
public:
	Vec2 m_mins;
	Vec2 m_maxs;

public:
	//Constructors and destructors
	AABB2() {}
	~AABB2() {}
	AABB2(AABB2 const& copyFrom);
	explicit AABB2(float minX, float minY, float maxX, float maxY);
	explicit AABB2(Vec2 const& mins, Vec2 const& maxs);

	//Accessors
	bool        IsPointInside(Vec2 const& point) const;
	Vec2 const  GetCenter() const;
	Vec2 const  GetDimensions() const;
	Vec2 const  GetNearestPoint(Vec2 const& referencePosition) const;
	Vec2 const  GetPointAtUV(Vec2 const& uv) const;
	Vec2 const  GetUVForPoint(Vec2 const& point) const;
	AABB2 const GetBoxAtUVs(Vec2 const& uvMins, Vec2 const& uvMaxs);

	//Mutators
	void Translate(Vec2 const& translationToApply);
	void SetCenter(Vec2 const& newCenter);
	void SetDimensions(Vec2 const& newDimensions);
	void StretchToIncludePoint(Vec2 const& point);

	// UI helpers
	void  AddPadding(float xToAddOnBothSides, float yToAddToTopAndBottom);
	void  ReduceToNewAspect(float newAspect);
	void  EnlargeToNewAspect(float newAspect);
	AABB2 ChopOffTop(float percentOfOriginalToChop, float extraHeightOfOriginalToChop);
	AABB2 ChopOffBottom(float percentOfOriginalToChop, float extraHeightOfOriginalToChop);
	AABB2 ChopOffLeft(float percentOfOriginalToChop, float extraHeightOfOriginalToChop);
	AABB2 ChopOffRight(float percentOfOriginalToChop, float extraHeightOfOriginalToChop);
};