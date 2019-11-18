#pragma once
#include "MathPCH.h"

class Transform2D
{
public:
	Vector2 location;
	Vector2 rotation;
	Vector2 scale;

public:
	Transform2D() {}
	Transform2D(Vector2 loc, Vector2 rot, Vector2 nscale) { location = loc; rotation = rot; scale = nscale; }
	Transform2D(const Transform2D &ntransform) { location = ntransform.location; rotation = ntransform.rotation; scale = ntransform.scale; }

	inline void SetLocation(float nx, float ny) { location.x = nx; location.y = ny; }
	inline void SetRotation(float nx, float ny) { rotation.x = nx; location.y = ny; }
	inline void SetScale(float nx, float ny) { scale.x = nx; scale.y = ny; }

	inline void SetLocation(Vector2 nlocation) { location = nlocation; }
	inline void SetRotation(Vector2 nrotation) { rotation = nrotation; }
	inline void SetScale(Vector2 nscale) { scale = nscale; }

	inline Transform2D operator -() const { return Transform2D(-this->location, -this->rotation, -this->scale); }
	inline Transform2D operator *(const float o) { return Transform2D(this->location * o, this->rotation * o, this->scale * o); }
	inline Transform2D operator /(const float o) { return Transform2D(this->location / o, this->rotation / o, this->scale / o); }
	inline Transform2D operator +(const float o) { return Transform2D(this->location + o, this->rotation + o, this->scale + o); }
	inline Transform2D operator -(const float o) { return Transform2D(this->location - o, this->rotation - o, this->scale - o); }
	inline Transform2D operator *(const Transform2D o) { return Transform2D(this->location * o.location, this->rotation * o.rotation, this->scale * o.scale); }
	inline Transform2D operator /(const Transform2D o) { return Transform2D(this->location / o.location, this->rotation / o.rotation, this->scale / o.scale); }
	inline Transform2D operator +(const Transform2D o) { return Transform2D(this->location + o.location, this->rotation + o.rotation, this->scale + o.scale); }
	inline Transform2D operator -(const Transform2D o) { return Transform2D(this->location - o.location, this->rotation - o.rotation, this->scale - o.scale); }
	
	Vector2& operator[] (int i) { assert(i >= 0 && i < 3); return *(&location + i); }
};

