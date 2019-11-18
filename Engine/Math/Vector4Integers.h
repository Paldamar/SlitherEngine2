#pragma once
#include "MathPCH.h"

class Vector4Int
{
public:
	int x;
	int y;
	int z;
	int w;

public:
	Vector4Int() {}
	Vector4Int(int nx, int ny, int nz, int nw) { x = nx; y = ny; z = nz; w = nw; }
	//virtual ~Vector4Int() {}

	inline void Set(int nx, int ny, int nz, int nw) { x = nx; y = ny; z = nz; w = nw; }
	inline float LengthSquared() const { return (float)x * x + y * y + z * z + w * w; }
	inline float Length() const { return sqrtf((float)x * x + y * y + z * z + w * w); }

	inline bool operator ==(const Vector4Int& o) const { return this->x == o.x && this->y == o.y && this->z == o.z && this->w == o.w; }
	inline bool operator !=(const Vector4Int& o) const { return this->x != o.x || this->y != o.y || this->z != o.z || this->w != o.w; }

	inline Vector4Int operator -() const { return Vector4Int(-this->x, -this->y, -this->z, -this->w); }
	inline Vector4Int operator +(const Vector4Int& o) const { return Vector4Int(this->x + o.x, this->y + o.y, this->z + o.z, this->w + o.w); }
	inline Vector4Int operator -(const Vector4Int& o) const { return Vector4Int(this->x - o.x, this->y - o.y, this->z - o.z, this->w - o.w); }
};

inline Vector4Int operator *(int scalar, const Vector4Int& vector) { return Vector4Int(scalar * vector.x, scalar * vector.y, scalar * vector.z, scalar * vector.w); }
inline Vector4Int operator /(int scalar, const Vector4Int& vector) { return Vector4Int(scalar / vector.x, scalar / vector.y, scalar / vector.z, scalar / vector.w); }
inline Vector4Int operator +(int scalar, const Vector4Int& vector) { return Vector4Int(scalar + vector.x, scalar + vector.y, scalar + vector.z, scalar + vector.w); }
inline Vector4Int operator -(int scalar, const Vector4Int& vector) { return Vector4Int(scalar - vector.x, scalar - vector.y, scalar - vector.z, scalar - vector.w); }
