#pragma once
#include "MathPCH.h"

#define PI 3.1415926535897932384626433832795f
const float FEQUALEPSILON = 0.00001f;

class SlitherMathLibrary
{
public:


	static DirectX::XMVECTOR RandUnitVec3();
	static DirectX::XMVECTOR RandHemisphereUnitVec3(DirectX::XMVECTOR n);

	static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M)
	{
		// Inverse-transpose is just applied to normals.  So zero out 
		// translation row so that it doesn't get into our inverse-transpose
		// calculation--we don't want the inverse-transpose of the translation.
		DirectX::XMMATRIX A = M;
		A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
		return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, A));
	}

	static DirectX::XMFLOAT4X4 Identity4x4()
	{
		static DirectX::XMFLOAT4X4 I(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		return I;
	}

	static DirectX::XMVECTOR SphericalToCartesian(float radius, float theta, float phi)
	{
		return DirectX::XMVectorSet(
			radius * sinf(phi) * cosf(theta),
			radius * cosf(phi),
			radius * sinf(phi) * sinf(theta),
			1.0f);
	}

	inline float randFromNegativeOneToOne()
	{
		return rand() / (RAND_MAX / 2) - 1.0f;
	}

	static float RandF()
	{
		return (float)(rand()) / (float)RAND_MAX;
	}

	// Returns random float in [a, b).
	static float RandF(float a, float b)
	{
		return a + RandF() * (b - a);
	}

	static int Rand(int a, int b)
	{
		return a + rand() % ((b - a) + 1);
	}

	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a) * t;
	}

	inline static bool fequal(const float a, const float b, const float epsilon = FEQUALEPSILON)
	{
		return fabs(a - b) <= epsilon;
	}

	inline static bool fnotequal(const float a, const float b, const float epsilon = FEQUALEPSILON)
	{
		return fabs(a - b) > epsilon;
	}

	template <class MyType> static void MySwap(MyType& v1, MyType& v2)
	{
		MyType temp = v1;
		v1 = v2;
		v2 = temp;
	}

	template <class MyType> static void MyClamp(MyType& value, MyType min, MyType max)
	{
		if (value < min)
			value = min;

		if (value > max)
			value = max;
	}

	template <class MyType> static MyType MyClamp_Return(MyType value, MyType min, MyType max)
	{
		MyType temp = value;

		if (temp < min)
			temp = min;

		if (temp > max)
			temp = max;

		return temp;
	}

	template <class MyType> static void IncreaseIfBigger(MyType& value, MyType newvalue)
	{
		if (newvalue > value)
			value = newvalue;
	}

	template <class MyType> static void DecreaseIfLower(MyType& value, MyType newvalue, bool treatzeroasinfinite = false)
	{
		if (treatzeroasinfinite && value == 0)
			value = newvalue;

		if (newvalue < value)
			value = newvalue;
	}

	static float AngleFromXY(float x, float y);

	static const float Infinity;
};


