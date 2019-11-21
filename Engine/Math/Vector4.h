#pragma once
#include "MathPCH.h"

class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;

public:
	Vector4() {}
	Vector4(float nx, float ny, float nz, float nw) { x = nx; y = ny; z = nz; w = nw; }
	Vector4(Vector2 vec, float nz, float nw) { x = vec.x; y = vec.y; z = nz; w = nw; }
	Vector4(Vector3 vec, float nw) { x = vec.x; y = vec.y; z = vec.z; w = nw; }

	inline Vector3 XYZ() { return Vector3(x, y, z); }

	inline void Set(float nx, float ny, float nz, float nw) { x = nx; y = ny; z = nz; w = nw; }
	inline float LengthSquared() const { return x * x + y * y + z * z + w * w; }
	inline float Length() const { return sqrtf(x * x + y * y + z * z + w * w); }

	inline Vector4 GetNormalized() const { float len = Length(); if (SlitherMathLibrary::fequal(len, 0)) return Vector4(x, y, z, w); len = 1.0f / len; return Vector4(x * len, y * len, z * len, w * len); }
	inline Vector4 Normalize() { float len = Length(); if (!SlitherMathLibrary::fequal(len, 0)) { x /= len; y /= len; z /= len; w /= len; } return *this; }

	inline Vector4 operator -() const { return Vector4(-this->x, -this->y, -this->z, -this->w); }
	inline Vector4 operator *(const float o) const { return Vector4(this->x * o, this->y * o, this->z * o, this->w * o); }
	inline Vector4 operator /(const float o) const { return Vector4(this->x / o, this->y / o, this->z / o, this->w / o); }
	inline Vector4 operator +(const float o) const { return Vector4(this->x + o, this->y + o, this->z + o, this->w + o); }
	inline Vector4 operator -(const float o) const { return Vector4(this->x - o, this->y - o, this->z - o, this->w - o); }
	inline Vector4 operator *(const Vector4& o) const { return Vector4(this->x * o.x, this->y * o.y, this->z * o.z, this->w * o.w); }
	inline Vector4 operator /(const Vector4& o) const { return Vector4(this->x / o.x, this->y / o.y, this->z / o.z, this->w / o.w); }
	inline Vector4 operator +(const Vector4& o) const { return Vector4(this->x + o.x, this->y + o.y, this->z + o.z, this->w + o.w); }
	inline Vector4 operator -(const Vector4& o) const { return Vector4(this->x - o.x, this->y - o.y, this->z - o.z, this->w - o.w); }

	float& operator[] (int i) { assert(i >= 0 && i < 4); return *(&x + i); }

	//inline XMVECTOR DXVector() { return XMVectorSet(x, y, z, w); }
};

inline Vector4 operator *(float scalar, const Vector4& vector) { return Vector4(scalar * vector.x, scalar * vector.y, scalar * vector.z, scalar * vector.w); }
inline Vector4 operator /(float scalar, const Vector4& vector) { return Vector4(scalar / vector.x, scalar / vector.y, scalar / vector.z, scalar / vector.w); }
inline Vector4 operator +(float scalar, const Vector4& vector) { return Vector4(scalar + vector.x, scalar + vector.y, scalar + vector.z, scalar + vector.w); }
inline Vector4 operator -(float scalar, const Vector4& vector) { return Vector4(scalar - vector.x, scalar - vector.y, scalar - vector.z, scalar - vector.w); }