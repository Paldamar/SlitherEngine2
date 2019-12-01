#pragma once
#include "MathPCH.h"
#include <assert.h>
#include <DirectXMath.h>

class Vector2
{
public:
	float x;
	float y;

public:
	Vector2() { x = 0; y = 0; }
	Vector2(float nxy) { x = nxy; y = nxy; }
	Vector2(float nx, float ny) { x = nx; y = ny; }

	inline void Set(float nx, float ny) { x = nx; y = ny; }
	inline float LengthSquared() const { return x * x + y * y; }
	inline float Length() const { return sqrtf(x * x + y * y); }

	inline Vector2 GetNormalized() const { float len = Length(); if (SlitherMathLibrary::fequal(len, 0)) return Vector2(x, y); len = 1.0f / len; return Vector2(x * len, y * len); }
	inline Vector2 Normalize() { float len = Length(); if (!SlitherMathLibrary::fequal(len, 0)) { x /= len; y /= len; } return *this; }
	inline float Dot(const Vector2& o) const { return x * o.x + y * o.y; }
	inline Vector2 Add(const Vector2& o) const { return Vector2(this->x + o.x, this->y + o.y); }
	inline Vector2 Sub(const Vector2& o) const { return Vector2(this->x - o.x, this->y - o.y); }
	inline Vector2 Scale(const float o) const { return Vector2(this->x * o, this->y * o); }

	inline bool operator ==(const Vector2& o) const { return SlitherMathLibrary::fequal(this->x, o.x) && SlitherMathLibrary::fequal(this->y, o.y); }
	inline bool operator !=(const Vector2& o) const { return !SlitherMathLibrary::fequal(this->x, o.x) || !SlitherMathLibrary::fequal(this->y, o.y); }

	inline Vector2 operator -() const { return Vector2(-this->x, -this->y); }
	inline Vector2 operator *(const float o) const { return Vector2(this->x * o, this->y * o); }
	inline Vector2 operator /(const float o) const { return Vector2(this->x / o, this->y / o); }
	inline Vector2 operator +(const float o) const { return Vector2(this->x + o, this->y + o); }
	inline Vector2 operator -(const float o) const { return Vector2(this->x - o, this->y - o); }
	inline Vector2 operator *(const Vector2& o) const { return Vector2(this->x * o.x, this->y * o.y); }
	inline Vector2 operator /(const Vector2& o) const { return Vector2(this->x / o.x, this->y / o.y); }
	inline Vector2 operator +(const Vector2& o) const { return Vector2(this->x + o.x, this->y + o.y); }
	inline Vector2 operator -(const Vector2& o) const { return Vector2(this->x - o.x, this->y - o.y); }

	inline Vector2 operator *=(const float o) { this->x *= o; this->y *= o; return *this; }
	inline Vector2 operator /=(const float o) { this->x /= o; this->y /= o; return *this; }
	inline Vector2 operator +=(const float o) { this->x += o; this->y += o; return *this; }
	inline Vector2 operator -=(const float o) { this->x -= o; this->y -= o; return *this; }
	inline Vector2 operator *=(const Vector2& o) { this->x *= o.x; this->y *= o.y; return *this; }
	inline Vector2 operator /=(const Vector2& o) { this->x /= o.x; this->y /= o.y; return *this; }
	inline Vector2 operator +=(const Vector2& o) { this->x += o.x; this->y += o.y; return *this; }
	inline Vector2 operator -=(const Vector2& o) { this->x -= o.x; this->y -= o.y; return *this; }

	//inline XMVECTOR GetDXVector() const { return XMVectorSet(this->x, this->y, 1.0f, 1.0f); }

	float& operator[] (int i) { assert(i >= 0 && i < 2); return *(&x + i); }

	
};