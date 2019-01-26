#include "MathUtil.h"


Vector3::Vector3() :
	xmf3Vector(XMFLOAT3())
{}

Vector3::Vector3(float vecx, float vecy, float vecz) :
	xmf3Vector(XMFLOAT3(vecx, vecy, vecz))
{}

Vector3::Vector3(XMFLOAT3 & other) : 
	xmf3Vector(other)
{}

void Vector3::operator=(const Vector3 & other)
{
	xmf3Vector = other.xmf3Vector;
}

void Vector3::operator=(const XMFLOAT3 & other)
{
	xmf3Vector = other;
}

void Vector3::operator=(const XMVECTOR & other)
{
	XMStoreFloat3(&xmf3Vector, other);
}

Vector3 Vector3::operator+(const Vector3 & other)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMLoadFloat3(&xmf3Vector) + XMLoadFloat3(&XMFLOAT3(other.x, other.y, other.z)));
	return result;
}

Vector3 Vector3::operator+(const XMFLOAT3 & other)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMLoadFloat3(&xmf3Vector) + XMLoadFloat3(&other));
	return result;
}

Vector3 Vector3::operator+(const XMVECTOR & other)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMLoadFloat3(&xmf3Vector) + other);
	return result;
}

void Vector3::operator+=(const Vector3 & other)
{
	XMStoreFloat3(&xmf3Vector, XMLoadFloat3(&xmf3Vector) + XMLoadFloat3(&XMFLOAT3(other.x, other.y, other.z)));
}

void Vector3::operator+=(const XMFLOAT3 & other)
{
	XMStoreFloat3(&xmf3Vector, XMLoadFloat3(&xmf3Vector) + XMLoadFloat3(&other));
}

void Vector3::operator+=(const XMVECTOR & other)
{
	XMStoreFloat3(&xmf3Vector, XMLoadFloat3(&xmf3Vector) + other);
}

Vector3 Vector3::operator-(const Vector3 & other)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMLoadFloat3(&xmf3Vector) - XMLoadFloat3(&XMFLOAT3(other.x, other.y, other.z)));
	return result;
}

Vector3 Vector3::operator-(const XMFLOAT3 & other)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMLoadFloat3(&xmf3Vector) - XMLoadFloat3(&other));
	return result;
}

Vector3 Vector3::operator-(const XMVECTOR & other)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMLoadFloat3(&xmf3Vector) - other);
	return result;
}

void Vector3::operator-=(const Vector3 & other)
{
	XMStoreFloat3(&xmf3Vector, XMLoadFloat3(&xmf3Vector) - XMLoadFloat3(&XMFLOAT3(other.x, other.y, other.z)));
}

void Vector3::operator-=(const XMFLOAT3 & other)
{
	XMStoreFloat3(&xmf3Vector, XMLoadFloat3(&xmf3Vector) - XMLoadFloat3(&other));
}

void Vector3::operator-=(const XMVECTOR & other)
{
	XMStoreFloat3(&xmf3Vector, XMLoadFloat3(&xmf3Vector) - other);
}

Vector3 Vector3::operator*(const Vector3 & other)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Cross(XMLoadFloat3(&xmf3Vector), XMLoadFloat3(&XMFLOAT3(other.x, other.y, other.z))));
	return result;
}

Vector3 Vector3::operator*(const XMFLOAT3 & other)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Cross(XMLoadFloat3(&xmf3Vector), XMLoadFloat3(&other)));
	return result;
}

Vector3 Vector3::operator*(const XMVECTOR & other)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Cross(XMLoadFloat3(&xmf3Vector),other));
	return result;
}

Vector3 Vector3::operator*(const float other)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMLoadFloat3(&xmf3Vector) * other);
	return result;
}

void Vector3::operator*=(const Vector3 & other)
{
	XMStoreFloat3(&xmf3Vector, XMVector3Cross(XMLoadFloat3(&xmf3Vector), XMLoadFloat3(&XMFLOAT3(other.x, other.y, other.z))));
}

void Vector3::operator*=(const XMFLOAT3 & other)
{
	XMStoreFloat3(&xmf3Vector, XMVector3Cross(XMLoadFloat3(&xmf3Vector), XMLoadFloat3(&other)));
}

void Vector3::operator*=(const XMVECTOR & other)
{
	XMStoreFloat3(&xmf3Vector, XMVector3Cross(XMLoadFloat3(&xmf3Vector), other));
}

void Vector3::operator*=(const float scalar)
{
	XMStoreFloat3(&xmf3Vector, XMLoadFloat3(&xmf3Vector) * scalar);
}

Vector3 Vector3::DotProduct(const Vector3 & a, const XMFLOAT3 & b)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Dot(XMLoadFloat3(&XMFLOAT3(a.x, a.y, a.z)), XMLoadFloat3(&b)));
	return result;
}

Vector3 Vector3::DotProduct(const Vector3 & a, const Vector3 & b)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Dot(XMLoadFloat3(&XMFLOAT3(a.x, a.y, a.z)), XMLoadFloat3(&XMFLOAT3(b.x, b.y, b.z))));
	return result;
}

Vector3 Vector3::DotProduct(const Vector3 & a, const XMVECTOR & b)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Dot(XMLoadFloat3(&XMFLOAT3(a.x, a.y, a.z)), b));
	return result;
}

////////////////////////////////////////

Vector4::Vector4()
	: xmf4Vector(XMFLOAT4())
{ }

Vector4::Vector4(float vecx, float vecy, float vecz, float vecw)
	: xmf4Vector(XMFLOAT4(vecx, vecy, vecz, vecw))
{ }

Vector4::Vector4(XMFLOAT4 & other)
	: xmf4Vector(other)
{ }

void Vector4::operator=(const Vector4 & other)
{
	xmf4Vector = other.xmf4Vector;
}

void Vector4::operator=(const XMFLOAT4 & other)
{
	xmf4Vector = other;
}

void Vector4::operator=(const XMVECTOR & other)
{
	XMStoreFloat4(&xmf4Vector, other);
}

Vector4 Vector4::operator+(const Vector4 & other)
{
	XMFLOAT4 result;
	XMStoreFloat4(&result, XMLoadFloat4(&xmf4Vector) + XMLoadFloat4(&XMFLOAT4(other.x, other.y, other.z, other.w)));
	return result;
}

Vector4 Vector4::operator+(const XMFLOAT4 & other)
{
	XMFLOAT4 result;
	XMStoreFloat4(&result, XMLoadFloat4(&xmf4Vector) + XMLoadFloat4(&other));
	return result;
}

Vector4 Vector4::operator+(const XMVECTOR & other)
{
	XMFLOAT4 result;
	XMStoreFloat4(&result, XMLoadFloat4(&xmf4Vector) + other);
	return result;
}

void Vector4::operator+=(const Vector4 & other)
{
	XMStoreFloat4(&xmf4Vector, XMLoadFloat4(&xmf4Vector) + XMLoadFloat4(&XMFLOAT4(other.x, other.y, other.z, other.w)));
}

void Vector4::operator+=(const XMFLOAT4 & other)
{
	XMStoreFloat4(&xmf4Vector, XMLoadFloat4(&xmf4Vector) + XMLoadFloat4(&other));
}

void Vector4::operator+=(const XMVECTOR & other)
{
	XMStoreFloat4(&xmf4Vector, XMLoadFloat4(&xmf4Vector) + other);
}

Vector4 Vector4::operator-(const Vector4 & other)
{
	XMFLOAT4 result;
	XMStoreFloat4(&result, XMLoadFloat4(&xmf4Vector) - XMLoadFloat4(&XMFLOAT4(other.x, other.y, other.z, other.w)));
	return result;
}

Vector4 Vector4::operator-(const XMFLOAT4 & other)
{
	XMFLOAT4 result;
	XMStoreFloat4(&result, XMLoadFloat4(&xmf4Vector) - XMLoadFloat4(&other));
	return result;
}

Vector4 Vector4::operator-(const XMVECTOR & other)
{
	XMFLOAT4 result;
	XMStoreFloat4(&result, XMLoadFloat4(&xmf4Vector) - other);
	return result;
}

void Vector4::operator-=(const Vector4 & other)
{
	XMStoreFloat4(&xmf4Vector, XMLoadFloat4(&xmf4Vector) - XMLoadFloat4(&XMFLOAT4(other.x, other.y, other.z, other.w)));
}

void Vector4::operator-=(const XMFLOAT4 & other)
{
	XMStoreFloat4(&xmf4Vector, XMLoadFloat4(&xmf4Vector) - XMLoadFloat4(&other));
}

void Vector4::operator-=(const XMVECTOR & other)
{
	XMStoreFloat4(&xmf4Vector, XMLoadFloat4(&xmf4Vector) - other);
}

Vector4 Vector4::operator*(const float other)
{
	XMFLOAT4 result;
	XMStoreFloat4(&result, XMLoadFloat4(&xmf4Vector) * other);
	return result;
}

Vector4 Vector4::operator*(const Matrix4x4 & other)
{
	XMFLOAT4 result;
	XMStoreFloat4(&result, XMVector4Transform(XMLoadFloat4(&xmf4Vector) , XMLoadFloat4x4(&other.matrix)));
	return result;
}

void Vector4::operator*=(const float scalar)
{
	XMStoreFloat4(&xmf4Vector, XMLoadFloat4(&xmf4Vector) * scalar);
}

void Vector4::operator*=(const Matrix4x4 & other)
{
	XMStoreFloat4(&xmf4Vector, XMVector4Transform(XMLoadFloat4(&xmf4Vector), XMLoadFloat4x4(&other.matrix)));
}

Vector4 Vector4::DotProduct(const Vector4 & a, const XMFLOAT4 & b)
{
	XMFLOAT4 result;
	XMStoreFloat4(&result, XMVector4Dot(XMLoadFloat4(&a.xmf4Vector), XMLoadFloat4(&b)));
	return result;
}

Vector4 Vector4::DotProduct(const Vector4 & a, const Vector4 & b)
{
	XMFLOAT4 result;
	XMStoreFloat4(&result, XMVector4Dot(XMLoadFloat4(&a.xmf4Vector), XMLoadFloat4(&b.xmf4Vector)));
	return result;
}

Vector4 Vector4::DotProduct(const Vector4 & a, const XMVECTOR & b)
{
	XMFLOAT4 result;
	XMStoreFloat4(&result, XMVector4Dot(XMLoadFloat4(&a.xmf4Vector), b));
	return result;
}

////////////////////////////////////////

Matrix4x4::Matrix4x4()
{
	XMStoreFloat4x4(&matrix, DirectX::XMMatrixIdentity());
}

Matrix4x4::Matrix4x4(const Matrix4x4& other)
{
	this->matrix = other.matrix;
}

Matrix4x4::Matrix4x4(const XMFLOAT4X4 & other)
{
	matrix = other;
}

Matrix4x4::Matrix4x4(const XMMATRIX & other)
{
	XMStoreFloat4x4(&matrix, other);
}

void Matrix4x4::operator=(const Matrix4x4 & other)
{
	matrix = other.matrix;
}

void Matrix4x4::operator=(const XMFLOAT4X4 & other)
{
	matrix = other;
}

void Matrix4x4::operator=(const XMMATRIX& other)
{
	XMStoreFloat4x4(&matrix, other);
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4 & other)
{
	return XMLoadFloat4x4(&matrix) + XMLoadFloat4x4(&other.matrix);
}

Matrix4x4 Matrix4x4::operator+(const XMFLOAT4X4 & other)
{
	return XMLoadFloat4x4(&matrix) + XMLoadFloat4x4(&other);
}

Matrix4x4 Matrix4x4::operator+(const XMMATRIX& other)
{
	return XMLoadFloat4x4(&matrix) + other;
}

void Matrix4x4::operator+=(const Matrix4x4 & other)
{
	XMStoreFloat4x4(&matrix, XMLoadFloat4x4(&matrix) + XMLoadFloat4x4(&other.matrix));
}

void Matrix4x4::operator+=(const XMFLOAT4X4 & other)
{
	XMStoreFloat4x4(&matrix, XMLoadFloat4x4(&matrix) + XMLoadFloat4x4(&other));
}

void Matrix4x4::operator+=(const XMMATRIX& other)
{
	XMStoreFloat4x4(&matrix, XMLoadFloat4x4(&matrix) +other);
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4 & other)
{
	return XMLoadFloat4x4(&matrix) - XMLoadFloat4x4(&other.matrix);
}

Matrix4x4 Matrix4x4::operator-(const XMFLOAT4X4 & other)
{
	return XMLoadFloat4x4(&matrix) - XMLoadFloat4x4(&other);
}

Matrix4x4 Matrix4x4::operator-(const XMMATRIX& other)
{
	return XMLoadFloat4x4(&matrix) - other;
}

void Matrix4x4::operator-=(const Matrix4x4 & other)
{
	XMStoreFloat4x4(&matrix, XMLoadFloat4x4(&matrix) - XMLoadFloat4x4(&other.matrix));
}

void Matrix4x4::operator-=(const XMFLOAT4X4 & other)
{
	XMStoreFloat4x4(&matrix, XMLoadFloat4x4(&matrix) - XMLoadFloat4x4(&other));
}

void Matrix4x4::operator-=(const XMMATRIX& other)
{
	XMStoreFloat4x4(&matrix, XMLoadFloat4x4(&matrix) - other);
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 & other)
{
	return XMLoadFloat4x4(&matrix)* XMLoadFloat4x4(&other.matrix);
}

Matrix4x4 Matrix4x4::operator*(const XMFLOAT4X4 & other)
{
	return XMLoadFloat4x4(&matrix)* XMLoadFloat4x4(&other);
}

Matrix4x4 Matrix4x4::operator*(const XMMATRIX& other)
{
	return XMLoadFloat4x4(&matrix)* other;
}

void Matrix4x4::operator*=(const float scalar)
{
	XMStoreFloat4x4(&matrix, XMLoadFloat4x4(&matrix) * scalar);
}

void Matrix4x4::operator*=(const Matrix4x4 & other)
{
	XMStoreFloat4x4(&matrix, XMLoadFloat4x4(&matrix) * XMLoadFloat4x4(&other.matrix));
}

void Matrix4x4::operator*=(const XMFLOAT4X4 & other)
{
	XMStoreFloat4x4(&matrix, XMLoadFloat4x4(&matrix) * XMLoadFloat4x4(&other));
}

void Matrix4x4::operator*=(const XMMATRIX& other)
{
	XMStoreFloat4x4(&matrix, XMLoadFloat4x4(&matrix) * other);
}

Vector4 Matrix4x4::Row(const int index)
{
	return r[index];
}

Vector4 Matrix4x4::Colum(const int index)
{
	return Vector4(m[index][0], m[index][1], m[index][2], m[index][4]);
}

Matrix4x4 Matrix4x4::Transpose()
{
	XMMATRIX result = XMMatrixTranspose(XMLoadFloat4x4(&matrix));
	return Matrix4x4(result);
}

Matrix4x4 Matrix4x4::Inverse()
{
	return XMMatrixInverse(&XMMatrixDeterminant(XMLoadFloat4x4(&matrix)), XMLoadFloat4x4(&matrix));
}
