#pragma once
#include "Source/D3DUtil.h"

class Matrix4x4;

class Vector3 {
public:
	Vector3();
	Vector3(float vecx, float vecy, float vecz);
	Vector3(XMFLOAT3& other);
	~Vector3() = default;

public:
	void operator=(const Vector3& other);
	void operator=(const XMFLOAT3& other);
	void operator=(const XMVECTOR& other);

	Vector3 operator+(const Vector3& other);
	Vector3 operator+(const XMFLOAT3& other);
	Vector3 operator+(const XMVECTOR& other);

	void operator+=(const Vector3& other);
	void operator+=(const XMFLOAT3& other);
	void operator+=(const XMVECTOR& other);

	Vector3 operator-(const Vector3& other);
	Vector3 operator-(const XMFLOAT3& other);
	Vector3 operator-(const XMVECTOR& other);

	void operator-=(const Vector3& other);
	void operator-=(const XMFLOAT3& other);
	void operator-=(const XMVECTOR& other);

	Vector3 operator*(const Vector3& other);
	Vector3 operator*(const XMFLOAT3& other);
	Vector3 operator*(const XMVECTOR& other);
	Vector3 operator*(const float other);

	void operator*=(const Vector3& other);
	void operator*=(const XMFLOAT3& other);
	void operator*=(const XMVECTOR& other);
	void operator*=(const float scalar);

	static Vector3 DotProduct(const Vector3& a, const XMFLOAT3& b);
	static Vector3 DotProduct(const Vector3& a, const Vector3& b);
	static Vector3 DotProduct(const Vector3& a, const XMVECTOR& b);

	static Vector3 Lerp(Vector3& a, Vector3& b, float t) { return a + (b - a) * t; }

protected:
	union {
		struct {
			float x, y, z;
		};
		XMFLOAT3 xmf3Vector;
	};
};

class Vector4 {
public:
	Vector4();
	Vector4(float vecx, float vecy, float vecz, float vecw);
	Vector4(XMFLOAT4& other);
	~Vector4() = default;

public:
	void operator=(const Vector4& other);
	void operator=(const XMFLOAT4& other);
	void operator=(const XMVECTOR& other);

	Vector4 operator+(const Vector4& other);
	Vector4 operator+(const XMFLOAT4& other);
	Vector4 operator+(const XMVECTOR& other);

	void operator+=(const Vector4& other);
	void operator+=(const XMFLOAT4& other);
	void operator+=(const XMVECTOR& other);

	Vector4 operator-(const Vector4& other);
	Vector4 operator-(const XMFLOAT4& other);
	Vector4 operator-(const XMVECTOR& other);

	void operator-=(const Vector4& other);
	void operator-=(const XMFLOAT4& other);
	void operator-=(const XMVECTOR& other);

	Vector4 operator*(const float other);
	Vector4 operator*(const Matrix4x4& other);

	void operator*=(const float scalar);
	void operator*=(const Matrix4x4& other);
	
	static Vector4 DotProduct(const Vector4& a, const XMFLOAT4& b);
	static Vector4 DotProduct(const Vector4& a, const Vector4& b);
	static Vector4 DotProduct(const Vector4& a, const XMVECTOR& b);

protected:
	union {
		struct {
			float x, y, z, w;
		};
		XMFLOAT4 xmf4Vector;
	};
};

class Matrix4x4 {
public:
	Matrix4x4();
	Matrix4x4(XMFLOAT4X4& other);
	Matrix4x4(XMMATRIX& other);
	~Matrix4x4() {};

public:
	void operator=(const Matrix4x4& other);
	void operator=(const XMFLOAT4X4& other);

	Matrix4x4 operator+(const Matrix4x4& other);
	Matrix4x4 operator+(const XMFLOAT4X4& other);

	void operator+=(const Matrix4x4& other);
	void operator+=(const XMFLOAT4X4& other);

	Matrix4x4 operator-(const Matrix4x4& other);
	Matrix4x4 operator-(const XMFLOAT4X4& other);

	void operator-=(const Matrix4x4& other);
	void operator-=(const XMFLOAT4X4& other);

	Matrix4x4 operator*(const Matrix4x4& other);
	Matrix4x4 operator*(const XMFLOAT4X4& other);

	void operator*=(const float scalar);
	void operator*=(const Matrix4x4& other);
	void operator*=(const XMFLOAT4X4& other);

	Vector4 Row(const int index); 
	Vector4 Colum(const int index);

	Matrix4x4 Transpose();

public:
	union {
		Vector4 r[4];
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		struct
		{
			Vector3 _xyz1; float _14;
			Vector3 _xyz2; float _24;
			Vector3 _xyz3; float _34;
			Vector3 _xyz4; float _44;
		};
		float m[4][4];
		XMFLOAT4X4 matrix;
	};
};