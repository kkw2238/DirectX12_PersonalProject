#include "MathUtil.h"

Vector2::Vector2() : xmf2Vector(XMFLOAT2())
{ }

Vector2::Vector2(float vecx, float vecy) : xmf2Vector(XMFLOAT2(vecx, vecy))
{ }

Vector2::Vector2(XMFLOAT2& other)
{
	xmf2Vector = other;
}

Vector2::Vector2(XMVECTOR& other)
{
	XMStoreFloat2(&xmf2Vector, other);
}

XMVECTOR Vector2::GetXMVector()
{
	return XMLoadFloat2(&xmf2Vector);
}

void Vector2::operator=(const Vector2& other)
{
	xmf2Vector = other.xmf2Vector;
}

void Vector2::operator=(const XMFLOAT2& other)
{
	xmf2Vector = other;
}

void Vector2::operator=(const XMVECTOR& other)
{
	XMStoreFloat2(&xmf2Vector, other);
}

Vector2 Vector2::operator+(const Vector2& other)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMLoadFloat2(&xmf2Vector) + XMLoadFloat2(&XMFLOAT2(other.x, other.y)));
	return result;
}

Vector2 Vector2::operator+(const XMFLOAT2& other)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMLoadFloat2(&xmf2Vector) + XMLoadFloat2(&other));
	return result;
}

Vector2 Vector2::operator+(const XMVECTOR& other)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMLoadFloat2(&xmf2Vector) + other);
	return result;
}

void Vector2::operator+=(const Vector2& other)
{
	XMStoreFloat2(&xmf2Vector, XMLoadFloat2(&xmf2Vector) + XMLoadFloat2(&other.xmf2Vector));
}

void Vector2::operator+=(const XMFLOAT2& other)
{
	XMStoreFloat2(&xmf2Vector, XMLoadFloat2(&xmf2Vector) + XMLoadFloat2(&other));
}

void Vector2::operator+=(const XMVECTOR& other)
{
	XMStoreFloat2(&xmf2Vector, XMLoadFloat2(&xmf2Vector) + other);
}

Vector2 Vector2::operator-(const Vector2& other)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMLoadFloat2(&xmf2Vector) - XMLoadFloat2(&XMFLOAT2(other.x, other.y)));
	return result;
}

Vector2 Vector2::operator-(const XMFLOAT2& other)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMLoadFloat2(&xmf2Vector) - XMLoadFloat2(&other));
	return result;
}

Vector2 Vector2::operator-(const XMVECTOR& other)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMLoadFloat2(&xmf2Vector) - other);
	return result;
}

void Vector2::operator-=(const Vector2& other)
{
	XMStoreFloat2(&xmf2Vector, XMLoadFloat2(&xmf2Vector) - XMLoadFloat2(&other.xmf2Vector));
}

void Vector2::operator-=(const XMFLOAT2& other)
{
	XMStoreFloat2(&xmf2Vector, XMLoadFloat2(&xmf2Vector) - XMLoadFloat2(&other));
}

void Vector2::operator-=(const XMVECTOR& other)
{
	XMStoreFloat2(&xmf2Vector, XMLoadFloat2(&xmf2Vector) + other);
}

Vector2 Vector2::operator*(const Vector2& other)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMVector2Cross(XMLoadFloat2(&xmf2Vector), XMLoadFloat2(&XMFLOAT2(other.x, other.y))));
	return result;
}

Vector2 Vector2::operator*(const XMFLOAT2& other)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMVector2Cross(XMLoadFloat2(&xmf2Vector), XMLoadFloat2(&other)));
	return result;
}

Vector2 Vector2::operator*(const XMVECTOR& other)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMVector2Cross(XMLoadFloat2(&xmf2Vector), other));
	return result;
}

Vector2 Vector2::operator*(const float scalar)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMLoadFloat2(&xmf2Vector) * scalar);
	return result;
}

Vector2 Vector2::operator/(const float scalar)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMLoadFloat2(&xmf2Vector) / scalar);
	return result;
}

void Vector2::operator*=(const Vector2& other)
{
	XMStoreFloat2(&xmf2Vector, XMVector2Cross(XMLoadFloat2(&xmf2Vector), XMLoadFloat2(&XMFLOAT2(other.x, other.y))));
}

void Vector2::operator*=(const XMFLOAT2& other)
{
	XMStoreFloat2(&xmf2Vector, XMVector2Cross(XMLoadFloat2(&xmf2Vector), XMLoadFloat2(&other)));
}

void Vector2::operator*=(const XMVECTOR& other)
{
	XMStoreFloat2(&xmf2Vector, XMVector2Cross(XMLoadFloat2(&xmf2Vector), other));
}

void Vector2::operator*=(const float scalar)
{
	XMStoreFloat2(&xmf2Vector, XMLoadFloat2(&xmf2Vector) * scalar);
}

void Vector2::operator/=(const float scalar)
{
	XMStoreFloat2(&xmf2Vector, XMLoadFloat2(&xmf2Vector) / scalar);
}

Vector2 Vector2::Normalize()
{
	XMVECTOR result = XMVector2Normalize(GetXMVector());
	return result;
}

void Vector2::Normalized()
{
	XMStoreFloat2(&xmf2Vector, XMVector2Normalize(GetXMVector()));
}

float Vector2::DotProduct(const Vector2& a, const XMFLOAT2& b)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMVector2Dot(XMLoadFloat2(&XMFLOAT2(a.x, a.y)), XMLoadFloat2(&b)));
	return result.x;
}

float Vector2::DotProduct(const Vector2& a, const Vector2& b)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMVector2Dot(XMLoadFloat2(&XMFLOAT2(a.x, a.y)), XMLoadFloat2(&XMFLOAT2(b.x, b.y))));
	return result.x;
}

float Vector2::DotProduct(const Vector2& a, const XMVECTOR& b)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMVector2Dot(XMLoadFloat2(&XMFLOAT2(a.x, a.y)), b));
	return result.x;
}

////////////////////////////////////////

Vector3::Vector3() :
	xmf3Vector(XMFLOAT3())
{}

Vector3::Vector3(float vecx, float vecy, float vecz) :
	xmf3Vector(XMFLOAT3(vecx, vecy, vecz))
{}

Vector3::Vector3(XMFLOAT3 & other) : 
	xmf3Vector(other)
{}

Vector3::Vector3(XMVECTOR & other)
{
	XMStoreFloat3(&xmf3Vector, other);
}

XMVECTOR Vector3::GetXMVector()
{
	return XMLoadFloat3(&xmf3Vector);
}

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

Vector3 Vector3::operator/(const float scalar)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMLoadFloat3(&xmf3Vector) / scalar);
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

void Vector3::operator/=(const float scalar)
{
	XMStoreFloat3(&xmf3Vector, XMLoadFloat3(&xmf3Vector) / scalar);
}

Vector3 Vector3::Normalize()
{
	XMVECTOR result = XMVector3Normalize(GetXMVector());
	return result;
}

Vector3 Vector3::ToRadian()
{
	Vector3 result;
	result = XMLoadFloat3(&xmf3Vector) * (XM_PI / 180.0f);
	return result;
}

Vector3 Vector3::ToDegree()
{
	Vector3 result;
	result = XMLoadFloat3(&xmf3Vector) * (180.0f / XM_PI);
	return result;
}

void Vector3::Normalized()
{
	XMStoreFloat3(&xmf3Vector, XMVector3Normalize(GetXMVector()));
}

float Vector3::DotProduct(const Vector3 & a, const XMFLOAT3 & b)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Dot(XMLoadFloat3(&XMFLOAT3(a.x, a.y, a.z)), XMLoadFloat3(&b)));
	return result.x;
}

float Vector3::DotProduct(const Vector3 & a, const Vector3 & b)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Dot(XMLoadFloat3(&XMFLOAT3(a.x, a.y, a.z)), XMLoadFloat3(&XMFLOAT3(b.x, b.y, b.z))));
	return result.x;
}

float Vector3::DotProduct(const Vector3 & a, const XMVECTOR & b)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Dot(XMLoadFloat3(&XMFLOAT3(a.x, a.y, a.z)), b));
	return result.x;
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

Vector4::Vector4(XMVECTOR & other)
{
	XMStoreFloat4(&xmf4Vector, other);
}

XMVECTOR Vector4::GetXMVector()
{
	return XMLoadFloat4(&xmf4Vector);
}

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

Vector4 Vector4::Normalize()
{
	XMVECTOR result = XMVector4Normalize(GetXMVector());
	return result;
}

void Vector4::Normalized()
{
	XMStoreFloat4(&xmf4Vector, XMVector4Normalize(GetXMVector()));
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

XMMATRIX Matrix4x4::GetXMMatrix()
{
	return XMLoadFloat4x4(&matrix);
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

void Matrix4x4::SetRow(const int index, const Vector4& data)
{
	r[index] = data;
}

void Matrix4x4::SetColum(const int index, const Vector4& data)
{
	m[0][index] = data.x; m[1][index] = data.y; m[2][index] = data.z; m[3][index] = data.w;
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


void CreateNormalVectors(std::vector<Vector3>& positions, std::vector<Vector3>& normals)
{
	normals.resize(positions.size());

	for (size_t i = 0; i < positions.size(); ++i) {
		normals[i] = positions[i].Normalize();
	}
}

void CreateTangentVectors(std::vector<Vector3>& positions, std::vector<Vector3>& normals, std::vector<Vector2>& texcoords, std::vector<UINT>& indeies, UINT meshCount, std::vector<Vector3>& Tangent )
{
	UINT vertexCount = positions.size();

	std::vector<Vector3> tan1(vertexCount);
	std::vector<Vector3> tan2(vertexCount);

	for (UINT i = 0; i < meshCount; ++i) {
		std::array<UINT, 3> index{ indeies[i * 3], indeies[i * 3 + 1], indeies[i * 3 + 2] };
		std::array<Vector3, 3> vertexPos{ positions[index[0]], positions[index[1]], positions[index[2]] };
		std::array<Vector2, 3> texCoord{ texcoords[index[0]], texcoords[index[1]], texcoords[index[2]] };

		Vector3 deltaVertex1 = vertexPos[1] - vertexPos[0];	
		Vector3 deltaVertex2 = vertexPos[2] - vertexPos[0];

		Vector2 deltaTexCoord1 = texCoord[1] - texCoord[0];
		Vector2 deltaTexCoord2 = texCoord[2] - texCoord[0];

		float r = 1.0f / (deltaTexCoord1.x * deltaTexCoord2.y - deltaTexCoord2.x * deltaTexCoord1.y);
		Vector2 inverse1 = Vector2(deltaTexCoord2.y, -deltaTexCoord1.y) * r;
		Vector3 T = deltaVertex1 * inverse1.x + deltaVertex2 * inverse1.y;
		
		for (UINT j = 0; j < 3; ++j)
			tan1[index[j]] += T;
	}

	for (UINT i = 0; i < vertexCount; ++i) {
		Tangent.emplace_back(Vector3(tan1[i] - (normals[i] * Vector3::DotProduct(normals[i], tan1[i]))).Normalize());
	}
}

Matrix4x4 aiMatrixConverter(const aiMatrix4x4& mat)
{
	Matrix4x4 result;
	::memcpy(&result, &mat, sizeof(float) * 16);

	return result;
}
