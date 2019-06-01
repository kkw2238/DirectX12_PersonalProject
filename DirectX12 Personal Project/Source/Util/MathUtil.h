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
	Vector2(const XMFLOAT2& other);
	Vector2(const XMVECTOR& other);
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

	Vector2 operator/(const float scalar);

	void operator*=(const Vector2& other);
	void operator*=(const XMFLOAT2& other);
	void operator*=(const XMVECTOR& other);
	void operator*=(const float scalar);

	void operator/=(const float scalar);

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
	Vector3(const XMFLOAT3& other);
	Vector3(const XMVECTOR& other);
	~Vector3() = default;

public:
	XMVECTOR GetXMVector();

	void operator=(const Vector3& other);
	void operator=(const XMFLOAT3& other);
	void operator=(const XMVECTOR& other);

	Vector3 operator+(const Vector3& other) const;
	Vector3 operator+(const XMFLOAT3& other) const;
	Vector3 operator+(const XMVECTOR& other) const;

	void operator+=(const Vector3& other);
	void operator+=(const XMFLOAT3& other);
	void operator+=(const XMVECTOR& other);

	Vector3 operator-(const Vector3& other) const;
	Vector3 operator-(const XMFLOAT3& other) const;
	Vector3 operator-(const XMVECTOR& other) const;

	void operator-=(const Vector3& other);
	void operator-=(const XMFLOAT3& other);
	void operator-=(const XMVECTOR& other);

	Vector3 operator*(const Vector3& other) const;
	Vector3 operator*(const XMFLOAT3& other) const;
	Vector3 operator*(const XMVECTOR& other) const;
	Vector3 operator*(const float scalar);

	Vector3 operator/(const float scalar) const;

	void operator*=(const Vector3& other);
	void operator*=(const XMFLOAT3& other);
	void operator*=(const XMVECTOR& other);
	void operator*=(const float scalar);

	void operator/=(const float scalar);

	Vector3 Normalize();
	Vector3 ToRadian();
	Vector3 ToDegree();
	void Normalized();

	static float DotProduct(const Vector3& a, const XMFLOAT3& b);
	static float DotProduct(const Vector3& a, const Vector3& b);
	static float DotProduct(const Vector3& a, const XMVECTOR& b);

	static Vector3 Lerp(Vector3& a, Vector3& b, float t) { return a + (b - a) * t; }
	static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) { return a + (b - a) * t; }

#ifdef AI_VECTOR3D_H_INC
	Vector3(aiVector3D& aiVec);

	void operator=(const aiVector3D& aiVec);

	Vector3 operator+(const aiVector3D& aiVec);
	void operator+=(const aiVector3D& aiVec);

	Vector3 operator-(const aiVector3D& aiVec);
	void operator-=(const aiVector3D& aiVec);

	Vector3 operator*(const aiVector3D& aiVec);
	void operator*=(const aiVector3D& aiVec);
	
	static float DotProduct(const Vector3& a, const aiVector3D& b);

#endif

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
	Vector4(const XMFLOAT4& other);
	Vector4(const XMVECTOR& other);
	~Vector4() = default;

public:
	XMVECTOR GetXMVector();

	void operator=(const Vector4& other);
	void operator=(const XMFLOAT4& other);
	void operator=(const XMVECTOR& other);

	Vector4 operator+(const Vector4& other) const;
	Vector4 operator+(const XMFLOAT4& other) const;
	Vector4 operator+(const XMVECTOR& other) const;

	void operator+=(const Vector4& other);
	void operator+=(const XMFLOAT4& other);
	void operator+=(const XMVECTOR& other);

	Vector4 operator-(const Vector4& other) const;
	Vector4 operator-(const XMFLOAT4& other) const;
	Vector4 operator-(const XMVECTOR& other) const;

	void operator-=(const Vector4& other);
	void operator-=(const XMFLOAT4& other);
	void operator-=(const XMVECTOR& other);

	Vector4 operator*(const float scalar) const;
	Vector4 operator*(const Matrix4x4& other) const;

	void operator*=(const float scalar);
	void operator*=(const Matrix4x4& other);

	Vector4 Normalize();
	void Normalized();
	
	static Vector4 DotProduct(const Vector4& a, const XMFLOAT4& b);
	static Vector4 DotProduct(const Vector4& a, const Vector4& b);
	static Vector4 DotProduct(const Vector4& a, const XMVECTOR& b);

	static Vector4 Lerp(Vector4& a, Vector4& b, float t) { return a + (b - a) * t; }
	static Vector4 Lerp(const Vector4 & a, const Vector4 & b, float t) { return a + (b - a) * t; }

#ifdef AI_QUATERNION_H_INC
	Vector4(aiQuaternion& aiVec);

	void operator=(const aiQuaternion& aiVec);

	Vector4 operator+(const aiQuaternion& aiVec) const ;
	void operator+=(const aiQuaternion& aiVec);

	Vector4 operator-(const aiQuaternion& aiVec) const;
	void operator-=(const aiQuaternion& aiVec);

	static Vector4 Slerp(const aiQuaternion& a, const aiQuaternion& b, float t, bool normalize = false);
#endif

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

class Matrix3x3 {
public:
	Matrix3x3();
	Matrix3x3(const Matrix3x3& other);
	Matrix3x3(const XMFLOAT3X3& other);
	Matrix3x3(const XMMATRIX& other);
	Matrix3x3(const Vector3& vec1, const Vector3& vec2, const Vector3& vec3);
	~Matrix3x3() {};

public:
	XMMATRIX GetXMMatrix();

	void operator=(const Matrix3x3& other);
	void operator=(const XMFLOAT3X3& other);
	void operator=(const XMMATRIX& other);

	Matrix3x3 operator+(const Matrix3x3& other);
	Matrix3x3 operator+(const XMFLOAT3X3& other);
	Matrix3x3 operator+(const XMMATRIX& other);

	void operator+=(const Matrix3x3& other);
	void operator+=(const XMFLOAT3X3& other);
	void operator+=(const XMMATRIX& other);

	Matrix3x3 operator-(const Matrix3x3& other);
	Matrix3x3 operator-(const XMFLOAT3X3& other);
	Matrix3x3 operator-(const XMMATRIX& other);

	void operator-=(const Matrix3x3& other);
	void operator-=(const XMFLOAT3X3& other);
	void operator-=(const XMMATRIX& other);

	Matrix3x3 operator*(const Matrix3x3& other);
	Matrix3x3 operator*(const XMFLOAT3X3& other);
	Matrix3x3 operator*(const XMMATRIX& other);

	void operator*=(const float scalar);
	void operator*=(const Matrix3x3& other);
	void operator*=(const XMFLOAT3X3& other);
	void operator*=(const XMMATRIX& other);

	void SetRow(const int index, const Vector3& data);
	void SetColum(const int index, const Vector3& data);

	Vector3 Row(const int index);
	Vector3 Colum(const int index);

	Matrix3x3 Transpose();
	Matrix3x3 Inverse();

#ifdef AI_MATRIX3X3_H_INC
	Matrix3x3(const aiMatrix3x3& aiMat);

	void operator=(const aiMatrix3x3& aiMat);
	Matrix3x3 operator+(const aiMatrix3x3& aiMat);

	void operator+=(const aiMatrix3x3& aiMat);
	Matrix3x3 operator-(const aiMatrix3x3& aiMat);

	void operator-=(const aiMatrix3x3& aiMat);
	Matrix3x3 operator*(const aiMatrix3x3& aiMat);

	void operator*=(const aiMatrix3x3& aiMat);
	aiMatrix3x3 GetAiMatrix();
#endif

	friend std::ostream& operator<<(std::ostream& os, Matrix3x3& vec)
	{
		for (int i = 0; i < 3; ++i)
			os << vec.r[i];
		return os;
	}

public:
	union {
		Vector3 r[3];
		struct
		{
			float _11, _12, _13;
			float _21, _22, _23;
			float _31, _32, _33;
		};
		float m[3][3];
		XMFLOAT3X3 matrix;
	};
};

class Matrix4x4 {
public:
	Matrix4x4();
	Matrix4x4(const Vector4& vec1, const Vector4& vec2, const Vector4& vec3, const Vector4& vec4);
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

	static Matrix4x4 GetTransformMatrix(Vector3& pos);
	static Matrix4x4 GetTransformMatrix(float x, float y, float z);
	static Matrix4x4 GetScaleMatrix(Vector3& scale);
	static Matrix4x4 GetScaleMatrix(float x, float y, float z);
	static Matrix4x4 GetRotationMatrix(Vector4& quaternion);
	static Matrix4x4 GetRotationMatrix(float x, float y, float z, float w);

	Matrix3x3 GetMatrix3x3();

#ifdef AI_MATRIX4X4_H_INC
	Matrix4x4(const aiMatrix4x4& aiMat);

	void operator=(const aiMatrix4x4& aiMat);
	Matrix4x4 operator+(const aiMatrix4x4& aiMat);

	void operator+=(const aiMatrix4x4& aiMat);
	Matrix4x4 operator-(const aiMatrix4x4& aiMat);

	void operator-=(const aiMatrix4x4& aiMat);
	Matrix4x4 operator*(const aiMatrix4x4& aiMat);

	void operator*=(const aiMatrix4x4& aiMat);
	aiMatrix4x4 GetAiMatrix();
#endif

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

void CreateNormalVectors(std::vector<Vector3>& positions, std::vector<Vector3>& normals);
void CreateTangentVectors(std::vector<Vector3>& positions, std::vector<Vector3>& normals, std::vector<Vector2>& texcoords, std::vector<UINT>& indeies, UINT meshCount, std::vector<Vector3>& TBN);