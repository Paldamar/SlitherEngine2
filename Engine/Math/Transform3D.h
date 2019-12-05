#pragma once
#include "MathPCH.h"

class Transform3D
{
public:
	/*Vector3 location;
	Vector3 rotation;
	Vector3 scale;*/

	MyMatrix matrix;

public:
	Transform3D() {}

	Transform3D(Vector3 loc) { matrix.CreateTranslation(loc); }
	Transform3D(Vector3 loc, Vector3 rot, Vector3 scale) { matrix.CreateSRT(scale, rot, loc); }
	Transform3D(const Transform3D& newTransform) { matrix = newTransform.matrix; }

	inline void SetLocation(Vector3 newLoc) { matrix.SetTranslation(newLoc); }
	inline void SetRotation(Vector3 eulerAngles)
	{
		Vector3 scale = matrix.GetScale();
		Vector3 translation = matrix.GetTranslation();
		matrix.CreateSRT(scale, eulerAngles, translation);
	}
	inline SetScale(Vector3 scale)
	{
		Vector3 translation = matrix.GetTranslation();
		Vector3 rotation = matrix.GetEulerAngles();
		matrix.CreateSRT(scale, rotation, translation);
	}

	/*Transform3D(Vector3 loc) { location = loc; rotation = Vector3(0,0,0), scale = Vector3(0,0,0); }
	Transform3D(Vector3 loc, Vector3 rot, Vector3 nscale) { location = loc; rotation = rot; scale = nscale; }
	Transform3D(const Transform3D& ntransform) { location = ntransform.location; rotation = ntransform.rotation; scale = ntransform.scale; }

	inline Transform2D TwoDimensional() { return Transform2D(location.XY(), rotation.XY(), scale.XY()); }

	inline void SetLocation(float nx, float ny) { location.x = nx; location.y = ny; }
	inline void SetRotation(float nx, float ny) { rotation.x = nx; location.y = ny; }
	inline void SetScale(float nx, float ny) { scale.x = nx; scale.y = ny; }

	inline void SetLocation(Vector3 nlocation) { location = nlocation; }
	inline void SetRotation(Vector3 nrotation) { rotation = nrotation; }
	inline void SetScale(Vector3 nscale) { scale = nscale; }

	inline Transform3D operator -() const { return Transform3D(-this->location, -this->rotation, -this->scale); }
	inline Transform3D operator *(const float o) { return Transform3D(this->location * o, this->rotation * o, this->scale * o); }
	inline Transform3D operator /(const float o) { return Transform3D(this->location / o, this->rotation / o, this->scale / o); }
	inline Transform3D operator +(const float o) { return Transform3D(this->location + o, this->rotation + o, this->scale + o); }
	inline Transform3D operator -(const float o) { return Transform3D(this->location - o, this->rotation - o, this->scale - o); }
	inline Transform3D operator *(const Transform3D o) { return Transform3D(this->location * o.location, this->rotation * o.rotation, this->scale * o.scale); }
	inline Transform3D operator /(const Transform3D o) { return Transform3D(this->location / o.location, this->rotation / o.rotation, this->scale / o.scale); }
	inline Transform3D operator +(const Transform3D o) { return Transform3D(this->location + o.location, this->rotation + o.rotation, this->scale + o.scale); }
	inline Transform3D operator -(const Transform3D o) { return Transform3D(this->location - o.location, this->rotation - o.rotation, this->scale - o.scale); }

	Vector3& operator[] (int i) { assert(i >= 0 && i < 3); return *(&location + i); }*/
};

