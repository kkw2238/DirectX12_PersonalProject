#pragma once
/*
 *	MathUtil에는 앞으로 사용할 Vector, Matrix등을 편하게 사용하기 위한 클래스가 선언되어 있음,
 *  주로 사용하는 XMFLOAT와 XMVECTOR, XMFLOAT4x4 XMMATRIX들과 호환 가능하게 설계
 */

#include "D3DUtil.h"

class Matrix4x4;

class Vector2 {
public:
	Vector2();
	Vector2(float vecx, float vecy);
	Vector2(XMFLOAT2& other);
	Vector2(XMVECTOR& other);
	~Vector2() = default;

public:
	XMVECTOR GetXMVector();

	void operator=(const Vector2& other);
	void operator=(const XMFLOAT2& other);
	void operator=(const XMVECTOR& other);

	Vector2 operator+(const Vector2& other);
	Vector2 operator+(const XMFLOAT2& other);
	Vector2 operator+(const XMVECTOR& other);

	void operator+=(const Vector2& other);
	void operator+=(const XMFLOAT2& other);
	void operator+=(const XMVECTOR& other);

	Vector2 operator-(const Vector2& other);
	Vector2 operator-(const XMFLOAT2& other);
	Vector2 operator-(const XMVECTOR& other);

	void operator-=(const Vector2& other);
	void operator-=(const XMFLOAT2& other);
	void operator-=(const XMVECTOR& other);

	Vector2 operator*(const Vector2& other);
	Vector2 operator*(const XMFLOAT2& other);
	Vector2 operator*(const XMVECTOR& other);
	Vector2 operator*(const float scalar);

	void operator*=(const Vector2& other);
	void operator*=(const XMFLOAT2& other);
	void operator*=(const XMVECTOR& other);
	void operator*=(const float scalar);

	Vector2 Normalize();
	void Normalized();

	static float DotProduct(const Vector2& a, const XMFLOAT2& b);
	static float DotProduct(const Vector2& a, const Vector2& b);
	static float DotProduct(const Vector2& a, const XMVECTOR& b);

	static Vector2 Lerp(Vector2& a, Vector2& b, float t) { return a + (b - a) * t; }

	friend std::ostream & operator<<(std::ostream & os, const Vector2 & vec)
	{
		os << "[" << vec.x << ", " << vec.y << "]" << std::endl;
		return os;
	}

public:
	union {
		struct {
			float x, y;
		};
		XMFLOAT2 xmf2Vector;
	};
};

class Vector3 {
public:
	Vector3();
	Vector3(float vecx, float vecy, float vecz);
	Vector3(XMFLOAT3& other);
	Vector3(XMVECTOR& other);
	~Vector3() = default;

public:
	XMVECTOR GetXMVector();

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
	Vector3 operator*(const float scalar);

	void operator*=(const Vector3& other);
	void operator*=(const XMFLOAT3& other);
	void operator*=(const XMVECTOR& other);
	void operator*=(const float scalar);

	Vector3 Normalize();
	void Normalized();

	static float DotProduct(const Vector3& a, const XMFLOAT3& b);
	static float DotProduct(const Vector3& a, const Vector3& b);
	static float DotProduct(const Vector3& a, const XMVECTOR& b);

	static Vector3 Lerp(Vector3& a, Vector3& b, float t) { return a + (b - a) * t; }

	friend std::ostream& operator<<(std::ostream& os, const Vector3& vec)
	{
		os << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]" << std::endl;
		return os;
	}

public:
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
	Vector4(XMVECTOR& other);
	~Vector4() = default;

public:
	XMVECTOR GetXMVector();

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

	Vector4 operator*(const float scalar);
	Vector4 operator*(const Matrix4x4& other);

	void operator*=(const float scalar);
	void operator*=(const Matrix4x4& other);

	Vector4 Normalize();
	void Normalized();
	
	static Vector4 DotProduct(const Vector4& a, const XMFLOAT4& b);
	static Vector4 DotProduct(const Vector4& a, const Vector4& b);
	static Vector4 DotProduct(const Vector4& a, const XMVECTOR& b);

	friend std::ostream& operator<<(std::ostream& os, Vector4& vec)
	{
		os << "[" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << "]" << std::endl;
		return os;
	}

public:
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
	Matrix4x4(const Matrix4x4& other);
	Matrix4x4(const XMFLOAT4X4& other);
	Matrix4x4(const XMMATRIX& other);
	~Matrix4x4() {};

public:
	XMMATRIX GetXMMatrix();

	void operator=(const Matrix4x4& other);
	void operator=(const XMFLOAT4X4& other);
	void operator=(const XMMATRIX& other);

	Matrix4x4 operator+(const Matrix4x4& other);
	Matrix4x4 operator+(const XMFLOAT4X4& other);
	Matrix4x4 operator+(const XMMATRIX& other);

	void operator+=(const Matrix4x4& other);
	void operator+=(const XMFLOAT4X4& other);
	void operator+=(const XMMATRIX& other);

	Matrix4x4 operator-(const Matrix4x4& other);
	Matrix4x4 operator-(const XMFLOAT4X4& other);
	Matrix4x4 operator-(const XMMATRIX& other);

	void operator-=(const Matrix4x4& other);
	void operator-=(const XMFLOAT4X4& other);
	void operator-=(const XMMATRIX& other);

	Matrix4x4 operator*(const Matrix4x4& other);
	Matrix4x4 operator*(const XMFLOAT4X4& other);
	Matrix4x4 operator*(const XMMATRIX& other);

	void operator*=(const float scalar);
	void operator*=(const Matrix4x4& other);
	void operator*=(const XMFLOAT4X4& other);
	void operator*=(const XMMATRIX& other);

	void SetRow(const int index, const Vector4& data);
	void SetColum(const int index, const Vector4& data);

	Vector4 Row(const int index); 
	Vector4 Colum(const int index);

	Matrix4x4 Transpose();
	Matrix4x4 Inverse();

	friend std::ostream& operator<<(std::ostream& os, Matrix4x4& vec)
	{
		for (int i = 0; i < 4; ++i)
			os << vec.r[i];
		return os;
	}


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

void CreateTangentVectors(std::vector<Vector3>& positions, std::vector<Vector3>& normals, std::vector<Vector2>& texcoords, std::vector<UINT>& indeies, UINT meshCount, std::vector<Vector3>& TBN);