#pragma once
#include "MathPCH.h"

class Vector2Int
{
public:
	int x;
	int y;

public:
	Vector2Int() {}
	Vector2Int(int nx, int ny) { x = nx; y = ny; }
	//virtual ~Vector2Int() {}

	inline void Set(int nx, int ny) { x = nx; y = ny; }
	inline float LengthSquared() const { return (float)x * x + y * y; }
	inline float Length() const { return sqrtf((float)x * x + y * y); }

	//inline Vector2Int Normalize() const {float len = Length(); if( fequal(len,0) ) return Vector2Int(x,y); len = 1.0f/len; return Vector2Int(x*len, y*len);}

	inline bool operator ==(const Vector2Int& o) const { return this->x == o.x && this->y == o.y; }
	inline bool operator !=(const Vector2Int& o) const { return this->x != o.x || this->y != o.y; }

	inline Vector2Int operator -() const { return Vector2Int(-this->x, -this->y); }
	inline Vector2Int operator +(const Vector2Int& o) const { return Vector2Int(this->x + o.x, this->y + o.y); }
	inline Vector2Int operator -(const Vector2Int& o) const { return Vector2Int(this->x - o.x, this->y - o.y); }
};

inline Vector2Int operator *(int scalar, const Vector2Int& vector) { return Vector2Int(scalar * vector.x, scalar * vector.y); }
inline Vector2Int operator /(int scalar, const Vector2Int& vector) { return Vector2Int(scalar / vector.x, scalar / vector.y); }
inline Vector2Int operator +(int scalar, const Vector2Int& vector) { return Vector2Int(scalar + vector.x, scalar + vector.y); }
inline Vector2Int operator -(int scalar, const Vector2Int& vector) { return Vector2Int(scalar - vector.x, scalar - vector.y); }
