#pragma once
// ----------------------------------------------------------------------------------------------
struct Vec3;
// ----------------------------------------------------------------------------------------------
struct Vec2
{
	static const Vec2 ZERO;
	static const Vec2 ONE;
	static const Vec2 ONEHALF;
	static const Vec2 ZERO_TO_ONE;
	static const Vec2 ONE_TO_ZERO;

public: // NOTE: this is one of the few cases where we break both the "m_" naming rule AND the avoid-public-members rule
	float x = 0.f;
	float y = 0.f;

public:
	// Construction/Destruction
	~Vec2() {}												// destructor (do nothing)
	Vec2() {}												// default constructor (do nothing)
	Vec2( Vec2 const& copyFrom );							// copy constructor (from another vec2)
	explicit Vec2( float initialX, float initialY );		// explicit constructor (from x, y)

	// Operators (const)
	bool		operator==( Vec2 const& compare ) const;		// vec2 == vec2
	bool		operator!=( Vec2 const& compare ) const;		// vec2 != vec2
	Vec2 const	operator+( Vec2 const& vecToAdd ) const;		// vec2 + vec2
	Vec2 const	operator-( Vec2 const& vecToSubtract ) const;	// vec2 - vec2
	Vec2 const	operator-() const;								// -vec2, i.e. "unary negation"
	Vec2 const	operator*( float uniformScale ) const;			// vec2 * float
	Vec2 const	operator*( Vec2 const& vecToMultiply ) const;	// vec2 * vec2
	Vec2 const	operator/( float inverseScale ) const;			// vec2 / float

	// Operators (self-mutating / non-const)
	void		operator+=( Vec2 const& vecToAdd );				// vec2 += vec2
	void		operator-=( Vec2 const& vecToSubtract );		// vec2 -= vec2
	void		operator*=( const float uniformScale );			// vec2 *= float
	void		operator/=( const float uniformDivisor );		// vec2 /= float
	void		operator=( Vec2 const& copyFrom );				// vec2 = vec2

	// Standalone "friend" functions that are conceptually, but not actually, part of Vec2::
	friend Vec2 const operator*( float uniformScale, Vec2 const& vecToScale );	// float * vec2

	//-----------------------------------------------------------------------------------------------

	// Static methods, Creation functions
	static Vec2 const MakeFromPolarRadians(float orientationRadians, float distance = 1.f);		// Orientation in (Theta, R)
	static Vec2 const MakeFromPolarDegrees(float orientationDegrees, float distance = 1.f);		// Orientation in (x, y)

	//-----------------------------------------------------------------------------------------------

	// Accessors read only const methods (getters)
	float GetLength() const;
	float GetLengthSquared() const;
	float GetOrientationRadians() const;
	float GetOrientationDegrees() const;
	Vec2 const GetRotated90Degrees() const;
	Vec2 const GetRotatedMinus90Degrees() const;
	Vec2 const GetRotatedRadians(float deltaRadians) const;
	Vec2 const GetRotatedDegrees(float deltaDegrees) const;
	bool IsNearlyZero(float nearlyZeroValue = 0.001f) const;

	// Mutators non const setters
	void  SetOrientationRadians(float newOrientationRadians);
	void  SetOrientationDegrees(float newOrientationDegrees);
	void  SetPolarRadians(float newOrientationRadians, float newLength);
	void  SetPolarDegrees(float newOrientationDegrees, float newLength);
	void  Rotate90Degrees();
	void  RotateMinus90Degrees();
	void  RotateRadians(float deltaRadians);
	void  RotateDegrees(float deltaDegrees);

	// Accessors read only const methods (getters)
	Vec2 const GetClamped(float maxLength) const;
	Vec2 const GetNormalized() const;
	Vec2 const GetReflected(Vec2 const& normalOfSurfaceToReflectOffOf ) const;
	Vec3 const GetAsVec3(float z = 0.0f) const;

	// Mutators non const setters
	void  SetLength(float newlength);
	void  SetFromText(char const* text); // Parses “6,4” or “ -.3 , 0.05 ” to (x,y)
	void  ClampLength(float maxLength);
	void  Normalize();
	void  Reflect(Vec2 const& normalOfSurfaceToReflectOffOf);
	float NormalizeAndGetPreviousLength();
};


