#pragma once
#include "MathPCH.h"

class Vector3
{
public:
	float x;
	float y;
	float z;

public:
	Vector3() {}
	Vector3(float nxyz) { x = nxyz; y = nxyz; z = nxyz; }
	Vector3(float nx, float ny) { x = nx; y = ny; z = 0; }
	Vector3(float nx, float ny, float nz) { x = nx; y = ny; z = nz; }
	Vector3(Vector2 v2) { x = v2.x; y = v2.y; z = 0; }
	Vector3(Vector2 v2, float nz) { x = v2.x; y = v2.y; z = nz; }
	//virtual ~Vector3() {}

	inline Vector2 XY() { return Vector2(x, y); }

	inline void Set(float nx, float ny, float nz) { x = nx; y = ny; z = nz; }
	inline float LengthSquared() const { return x * x + y * y + z * z; }
	inline float Length() const { return sqrtf(x * x + y * y + z * z); }

	inline Vector3 GetNormalized() const { float len = Length(); if (SlitherMathLibrary::fequal(len, 0)) return Vector3(x, y, z); len = 1.0f / len; return Vector3(x * len, y * len, z * len); }
	inline Vector3 Normalize() { float len = Length(); if (!SlitherMathLibrary::fequal(len, 0)) { x /= len; y /= len; z /= len; } return *this; }
	inline Vector3 Cross(const Vector3& o) const { return Vector3((y * o.z - z * o.y), (z * o.x - x * o.z), (x * o.y - y * o.x)); }
	inline float Dot(const Vector3& o) const { return x * o.x + y * o.y + z * o.z; }
	inline Vector3 Add(const Vector3& o) const { return Vector3(this->x + o.x, this->y + o.y, this->z + o.z); }
	inline Vector3 Sub(const Vector3& o) const { return Vector3(this->x - o.x, this->y - o.y, this->z - o.z); }
	inline Vector3 Scale(const float o) const { return Vector3(this->x * o, this->y * o, this->z * o); }
	inline Vector3 MultiplyComponents(const Vector3& o) const { return Vector3(this->x * o.x, this->y * o.y, this->z * o.z); }
	inline Vector3 DivideComponents(const Vector3& o) const { return Vector3(this->x / o.x, this->y / o.y, this->z / o.z); }
	inline static Vector3 Zero() { return Vector3(0, 0, 0); }

	inline bool operator ==(const Vector3& o) const { return SlitherMathLibrary::fequal(this->x, o.x) && SlitherMathLibrary::fequal(this->y, o.y) && SlitherMathLibrary::fequal(this->z, o.z); }
	inline bool operator !=(const Vector3& o) const { return !SlitherMathLibrary::fequal(this->x, o.x) || !SlitherMathLibrary::fequal(this->y, o.y) || !SlitherMathLibrary::fequal(this->z, o.z); }

	inline Vector3 operator -() const { return Vector3(-this->x, -this->y, -this->z); }
	inline Vector3 operator *(const float o) const { return Vector3(this->x * o, this->y * o, this->z * o); }
	inline Vector3 operator /(const float o) const { return Vector3(this->x / o, this->y / o, this->z / o); }
	inline Vector3 operator +(const float o) const { return Vector3(this->x + o, this->y + o, this->z + o); }
	inline Vector3 operator -(const float o) const { return Vector3(this->x - o, this->y - o, this->z - o); }
	inline Vector3 operator *(const Vector3& o) const { return Vector3(this->x * o.x, this->y * o.y, this->z * o.z); }
	inline Vector3 operator /(const Vector3& o) const { return Vector3(this->x / o.x, this->y / o.y, this->z / o.z); }
	inline Vector3 operator +(const Vector3& o) const { return Vector3(this->x + o.x, this->y + o.y, this->z + o.z); }
	inline Vector3 operator -(const Vector3& o) const { return Vector3(this->x - o.x, this->y - o.y, this->z - o.z); }

	inline Vector3 operator *=(const float o) { this->x *= o; this->y *= o; this->z *= o; return *this; }
	inline Vector3 operator /=(const float o) { this->x /= o; this->y /= o; this->z /= o; return *this; }
	inline Vector3 operator +=(const float o) { this->x += o; this->y += o; this->z += o; return *this; }
	inline Vector3 operator -=(const float o) { this->x -= o; this->y -= o; this->z -= o; return *this; }
	inline Vector3 operator *=(const Vector3& o) { this->x *= o.x; this->y *= o.y; this->z *= o.z; return *this; }
	inline Vector3 operator /=(const Vector3& o) { this->x /= o.x; this->y /= o.y; this->z /= o.z; return *this; }
	inline Vector3 operator +=(const Vector3& o) { this->x += o.x; this->y += o.y; this->z += o.z; return *this; }
	inline Vector3 operator -=(const Vector3& o) { this->x -= o.x; this->y -= o.y; this->z -= o.z; return *this; }

	float& operator[] (int i) { assert(i >= 0 && i < 3); return *(&x + i); }

//	inline XMVECTOR DXVector() { return XMVectorSet(x, y, z, 1.0); }
};

inline Vector3 operator *(float scalar, const Vector3& vector) { return Vector3(scalar * vector.x, scalar * vector.y, scalar * vector.z); }
inline Vector3 operator /(float scalar, const Vector3& vector) { return Vector3(scalar / vector.x, scalar / vector.y, scalar / vector.z); }
inline Vector3 operator +(float scalar, const Vector3& vector) { return Vector3(scalar + vector.x, scalar + vector.y, scalar + vector.z); }
inline Vector3 operator -(float scalar, const Vector3& vector) { return Vector3(scalar - vector.x, scalar - vector.y, scalar - vector.z); }