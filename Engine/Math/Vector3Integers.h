#pragma once
#include "MathPCH.h"

class Vector3Int
{
public:
	int x;
	int y;
	int z;

public:
	Vector3Int() {}
	Vector3Int(int nx, int ny, int nz) { x = nx; y = ny; z = nz; }
	//virtual ~Vector3Int() {}

	inline void Set(int nx, int ny, int nz) { x = nx; y = ny; z = nz; }
	inline void Set(Vector3Int vec) { x = vec.x; y = vec.y; z = vec.z; }
	inline int LengthSquared() const { return x * x + y * y + z * z; }
	inline float Length() const { return sqrtf((float)x * x + y * y + z * z); }

	//inline Vector3Int Normalize() const {float len = Length(); if( fequal(len,0) ) return Vector3Int(x,y,z); len = 1.0f/len; return Vector3Int(x*len, y*len, z*len);}
	//inline Vector3Int Cross(const Vector3Int& o) const {return Vector3Int( (y*o.z - z*o.y), (z*o.x - x*o.z), (x*o.y - y*o.x) );}

	inline Vector3 MultiplyComponents(const Vector3& o) const { return Vector3(this->x * o.x, this->y * o.y, this->z * o.z); }
	inline Vector3Int MultiplyComponents(const Vector3Int& o) const { return Vector3Int(this->x * o.x, this->y * o.y, this->z * o.z); }

	inline bool operator ==(const Vector3Int& o) const { return this->x == o.x && this->y == o.y && this->z == o.z; }
	inline bool operator !=(const Vector3Int& o) const { return this->x != o.x || this->y != o.y || this->z != o.z; }

	inline Vector3Int operator -() const { return Vector3Int(-this->x, -this->y, -this->z); }
	inline Vector3 operator *(const float o) const { return Vector3(this->x * o, this->y * o, this->z * o); }
	inline Vector3 operator /(const float o) const { return Vector3(this->x / o, this->y / o, this->z / o); }
	inline Vector3 operator +(const float o) const { return Vector3(this->x + o, this->y + o, this->z + o); }
	inline Vector3 operator -(const float o) const { return Vector3(this->x - o, this->y - o, this->z - o); }
	inline Vector3Int operator *(const int o) const { return Vector3Int(this->x * o, this->y * o, this->z * o); }
	inline Vector3Int operator /(const int o) const { return Vector3Int(this->x / o, this->y / o, this->z / o); }
	inline Vector3Int operator +(const int o) const { return Vector3Int(this->x + o, this->y + o, this->z + o); }
	inline Vector3Int operator -(const int o) const { return Vector3Int(this->x - o, this->y - o, this->z - o); }
	inline Vector3 operator *(const Vector3& o) const { return Vector3(this->x * o.x, this->y * o.y, this->z * o.z); }
	inline Vector3 operator /(const Vector3& o) const { return Vector3(this->x / o.x, this->y / o.y, this->z / o.z); }
	inline Vector3 operator +(const Vector3& o) const { return Vector3(this->x + o.x, this->y + o.y, this->z + o.z); }
	inline Vector3 operator -(const Vector3& o) const { return Vector3(this->x - o.x, this->y - o.y, this->z - o.z); }
	inline Vector3Int operator *(const Vector3Int& o) const { return Vector3Int(this->x * o.x, this->y * o.y, this->z * o.z); }
	inline Vector3Int operator /(const Vector3Int& o) const { return Vector3Int(this->x / o.x, this->y / o.y, this->z / o.z); }
	inline Vector3Int operator +(const Vector3Int& o) const { return Vector3Int(this->x + o.x, this->y + o.y, this->z + o.z); }
	inline Vector3Int operator -(const Vector3Int& o) const { return Vector3Int(this->x - o.x, this->y - o.y, this->z - o.z); }

	inline Vector3Int operator +=(const Vector3Int& o) { this->x += o.x; this->y += o.y; this->z += o.z; return *this; }
	inline Vector3Int operator -=(const Vector3Int& o) { this->x -= o.x; this->y -= o.y; this->z -= o.z; return *this; }

	int& operator[] (int i) { assert(i >= 0 && i < 3); return *(&x + i); }
};

inline Vector3 operator *(float scalar, const Vector3Int& vector) { return Vector3(scalar * vector.x, scalar * vector.y, scalar * vector.z); }
inline Vector3 operator /(float scalar, const Vector3Int& vector) { return Vector3(scalar / vector.x, scalar / vector.y, scalar / vector.z); }
inline Vector3 operator +(float scalar, const Vector3Int& vector) { return Vector3(scalar + vector.x, scalar + vector.y, scalar + vector.z); }
inline Vector3 operator -(float scalar, const Vector3Int& vector) { return Vector3(scalar - vector.x, scalar - vector.y, scalar - vector.z); }
inline Vector3Int operator *(int scalar, const Vector3Int& vector) { return Vector3Int(scalar * vector.x, scalar * vector.y, scalar * vector.z); }
inline Vector3Int operator /(int scalar, const Vector3Int& vector) { return Vector3Int(scalar / vector.x, scalar / vector.y, scalar / vector.z); }
inline Vector3Int operator +(int scalar, const Vector3Int& vector) { return Vector3Int(scalar + vector.x, scalar + vector.y, scalar + vector.z); }
inline Vector3Int operator -(int scalar, const Vector3Int& vector) { return Vector3Int(scalar - vector.x, scalar - vector.y, scalar - vector.z); }
