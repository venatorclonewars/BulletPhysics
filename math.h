#pragma once
#include <cmath>
#include <cstring>
#include <algorithm>    // For std::max and std::min
#include "BulletPhysics/src/btBulletDynamicsCommon.h"

using namespace std;


#define PI 3.14159265358979323846

class Vector3f
{
public:
	float x, y, z;

public:
	Vector3f() {}

	Vector3f(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	float dot(Vector3f vector) const
	{
		float out = x * vector.x + y * vector.y + z * vector.z;

		return out;
	}

	Vector3f normalize() const
	{
		Vector3f out;

		float length = sqrt(x * x + y * y + z * z);
		out.x = x / length;
		out.y = y / length;
		out.z = z / length;

		return out;
	}

	Vector3f cross(Vector3f vector)
	{
		Vector3f out;

		out.x = y * vector.z - z * vector.y;
		out.y = z * vector.x - x * vector.z;
		out.z = x * vector.y - y * vector.x;

		return out;
	}

	Vector3f negate()
	{
		Vector3f out;
		out.x = -x;
		out.y = -y;
		out.z = -z;

		return out;
	}

	float length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vector3f maxVector(const Vector3f& vector)
	{
		return Vector3f(
			max(x, vector.x),  // Compare the x components
			max(y, vector.y),  // Compare the y components
			max(z, vector.z)   // Compare the z components
		);

	}

	float maxValue()
	{
		float out = max({ x, y, z });
		return out;
	}

	float length()
	{
		return sqrt(x * x + y * y + z * z);
	}

	float distance(const Vector3f& v) const
	{
		float delta_x = x - v.x;
		float delta_y = y - v.y;
		float delta_z = z - v.z;
		float distance = sqrtf(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
		return distance;
	}


	// Multiplication operator

	void operator *= (const Vector3f& vector)
	{
		Vector3f out;

		out.x = x * vector.x;
		out.y = y * vector.y;
		out.z = z * vector.z;

		*this = out;
	}

	Vector3f operator * (const Vector3f& vector)
	{
		Vector3f out;

		out.x = x * vector.x;
		out.y = y * vector.y;
		out.z = z * vector.z;

		return out;
	}

	Vector3f operator * (float scalar)
	{
		Vector3f out;

		out.x = x * scalar;
		out.y = y * scalar;
		out.z = z * scalar;

		return out;
	}

	// Plus operator

	void operator += (const Vector3f& vector)
	{
		Vector3f out;

		out.x = x + vector.x;
		out.y = y + vector.y;
		out.z = z + vector.z;

		*this = out;
	}

	Vector3f operator + (const Vector3f& vector)
	{
		Vector3f out;

		out.x = x + vector.x;
		out.y = y + vector.y;
		out.z = z + vector.z;

		return out;
	}

	// Minus operator

	void operator -= (const Vector3f& vector)
	{
		Vector3f out;

		out.x = x - vector.x;
		out.y = y - vector.y;
		out.z = z - vector.z;

		*this = out;
	}

	Vector3f operator - (const Vector3f& vector) const
	{
		Vector3f out;

		out.x = x - vector.x;
		out.y = y - vector.y;
		out.z = z - vector.z;

		return out;
	}

	// Not equal operator

	bool operator != (const Vector3f& vector)
	{
		bool out = false;

		if (x != vector.x || y != vector.y || z != vector.z)
			out = true;

		return out;
	}

};

class Vector4f
{
public:
	Vector4f(){}

	Vector4f(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	Vector4f(const Vector3f& vector, float _w)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		w = _w;
	}

	float dot(const Vector4f& v) const
	{
		float out = x * v.x + y * v.y + z * v.z + w * v.w;
		return out;
	}

	void normalize()
	{
		Vector4f out;

		float len = sqrt(x * x + y * y + z * z + w * w);

		out.x /= len;
		out.y /= len;
		out.z /= len;
		out.w /= len;

		*this = out;
	}

	Vector4f operator * (Vector4f& vector)
	{
		Vector4f out;

		out.x = x * vector.x;
		out.y = y * vector.y;
		out.z = z * vector.z;
		out.w = w * vector.w;

		return out;
	}

	Vector4f operator + (const Vector4f& vector)
	{
		Vector4f out;

		out.x = x + vector.x;
		out.y = y + vector.y;
		out.z = z + vector.z;
		out.w = z + vector.w;

		return out;
	}

	Vector4f operator - (const Vector4f& vector)
	{
		Vector4f out;

		out.x = x - vector.x;
		out.y = y - vector.y;
		out.z = z - vector.z;
		out.w = z - vector.w;

		return out;
	}

	void cross(Vector4f& v1, Vector4f& v2, Vector4f& v3)
	{
		this->x = v1.y * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.y * v3.w - v3.y * v2.w) + v1.w * (v2.y * v3.z - v2.z * v3.y);
		this->y = -(v1.x * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.z - v3.x * v2.z));
		this->z = v1.x * (v2.y * v3.w - v3.y * v2.w) - v1.y * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.y - v3.x * v2.y);
		this->w = -(v1.x * (v2.y * v3.z - v3.y * v2.z) - v1.y * (v2.x * v3.z - v3.x * v2.z) + v1.z * (v2.x * v3.y - v3.x * v2.y));
	}

public:
	float x, y, z, w;
};

class Vector2f
{
public:
	Vector2f() {}

	Vector2f(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

public:
	float x, y;
};

#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Matrix4f
{
public:
	float mat[4][4];

public:
	Matrix4f()
	{
		setIdentity();
	}

	Matrix4f(const aiMatrix4x4& aiMat)
	{
		
		mat[0][0] = aiMat.a1; mat[0][1] = aiMat.a2; mat[0][2] = aiMat.a3; mat[0][3] = aiMat.a4;
		mat[1][0] = aiMat.b1; mat[1][1] = aiMat.b2; mat[1][2] = aiMat.b3; mat[1][3] = aiMat.b4;
		mat[2][0] = aiMat.c1; mat[2][1] = aiMat.c2; mat[2][2] = aiMat.c3; mat[2][3] = aiMat.c4;
		mat[3][0] = aiMat.d1; mat[3][1] = aiMat.d2; mat[3][2] = aiMat.d3; mat[3][3] = aiMat.d4;
	
	}

	Matrix4f(const aiMatrix3x3& aiMat)
	{

		mat[0][0] = aiMat.a1; mat[0][1] = aiMat.a2; mat[0][2] = aiMat.a3; mat[0][3] = 0.0f;
		mat[1][0] = aiMat.b1; mat[1][1] = aiMat.b2; mat[1][2] = aiMat.b3; mat[1][3] = 0.0f;
		mat[2][0] = aiMat.c1; mat[2][1] = aiMat.c2; mat[2][2] = aiMat.c3; mat[2][3] = 0.0f;
		mat[3][0] = 0.0f;	  mat[3][1] = 0.0f;     mat[3][2] = 0.0f;     mat[3][3] = 1.0f;
		
	}

	void setQuaternion(const btQuaternion& quat)
	{
		
		btMatrix3x3 rotMatrix = btMatrix3x3(quat);

		// Fill the 3x3 rotation part of the 4x4 matrix
		mat[0][0] = rotMatrix[0][0];
		mat[0][1] = rotMatrix[0][1];
		mat[0][2] = rotMatrix[0][2];

		mat[1][0] = rotMatrix[1][0];
		mat[1][1] = rotMatrix[1][1];
		mat[1][2] = rotMatrix[1][2];

		mat[2][0] = rotMatrix[2][0];
		mat[2][1] = rotMatrix[2][1];
		mat[2][2] = rotMatrix[2][2];

		mat[3][3] = 1.0f; // Homogeneous coordinate
	}

	void setTranslation(const btVector3& position)
	{
		mat[0][3] = position.x();
		mat[1][3] = position.y();
		mat[2][3] = position.z();
		mat[3][3] = 1.0f;
	}





	Matrix4f(const Matrix4f& matrix) 
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				mat[i][j] = matrix.mat[i][j];
			
	}

	void setIdentity()
	{
		::memset(mat, 0, sizeof(mat));
		mat[0][0] = 1;
		mat[1][1] = 1;
		mat[2][2] = 1;
		mat[3][3] = 1;
	}

	void setScale(const Vector3f& scale)
	{
		mat[0][0] = scale.x;
		mat[1][1] = scale.y;
		mat[2][2] = scale.z;
	}

	void setTranslation(const Vector3f& scale)
	{
		mat[0][3] = scale.x;
		mat[1][3] = scale.y;
		mat[2][3] = scale.z;
		mat[3][3] = 1.0f;
	}

	void setRotationX(float x)
	{
		mat[1][1] = cos(x);
		mat[2][1] = sin(x);
		mat[1][2] = -sin(x);
		mat[2][2] = cos(x);
	}

	void setRotationY(float y)
	{
		mat[0][0] = cos(y);
		mat[2][0] = -sin(y);
		mat[0][2] = sin(y);
		mat[2][2] = cos(y);
	}

	void setRotationZ(float z)
	{
		mat[0][0] = cos(z);
		mat[1][0] = sin(z);
		mat[0][1] = -sin(z);
		mat[1][1] = cos(z);
	}


	Vector3f getZDirection()
	{
		return Vector3f(mat[0][2], mat[1][2], mat[2][2]);
	}

	Vector3f getYDirection()
	{
		return Vector3f(mat[0][1], mat[1][1], mat[2][1]);
	}

	Vector3f getXDirection()
	{
		return Vector3f(mat[0][0], mat[1][0], mat[2][0]);
	}

	Vector3f getTranslation()
	{
		return Vector3f(mat[0][3], mat[1][3], mat[2][3]);
	}

	void setProjection(float FOV)
	{
		float radFOV = FOV * PI / 180.0;
		float tanHalfFOV = tan((FOV / 2.0f));
		float d = 1 / tanHalfFOV;

		mat[0][0] = d;
		mat[1][1] = d;
		mat[2][2] = 1;
		mat[3][2] = 1;
		mat[3][3] = 0;

	}

	void setProjection(float FOV, float aspectRatio, float nearZ, float farZ)
	{
		float radFOV = (FOV / 2.0f) * PI / 180.0;
		float tanHalfFOV = tan((radFOV));
		float d = 1 / tanHalfFOV;

		float zRange = nearZ - farZ;

		float A = (-farZ - nearZ) / zRange;
		float B = 2.0f * farZ * nearZ / zRange;

		mat[0][0] = d / aspectRatio;
		mat[1][1] = d;
		mat[2][2] = A;
		mat[2][3] = B;
		mat[3][2] = 1;
		mat[3][3] = 0;

	}

	const Matrix4f& transpose()
	{
		Matrix4f out;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				out.mat[i][j] = mat[j][i];

		return out;
	}

	void setZero()
	{
		::memset(mat, 0, sizeof(mat));
	}

	Vector4f operator* (const Vector4f& vector) const
	{
		Vector4f out;

		out.x = mat[0][0] * vector.x + mat[0][1] * vector.y + mat[0][2] * vector.z + mat[0][3] * vector.w;
		out.y = mat[1][0] * vector.x + mat[1][1] * vector.y + mat[1][2] * vector.z + mat[1][3] * vector.w;
		out.z = mat[2][0] * vector.x + mat[2][1] * vector.y + mat[2][2] * vector.z + mat[2][3] * vector.w;
		out.w = mat[3][0] * vector.x + mat[3][1] * vector.y + mat[3][2] * vector.z + mat[3][3] * vector.w;

		return out;
	}

	void operator *= (const Matrix4f& matrix)
	{
		Matrix4f out;

		for (auto i = 0; i < 4; i++)
		{
			for (auto j = 0; j < 4; j++)
			{
				out.mat[i][j] =
					mat[i][0] * matrix.mat[0][j] +
					mat[i][1] * matrix.mat[1][j] +
					mat[i][2] * matrix.mat[2][j] +
					mat[i][3] * matrix.mat[3][j];
			}
		}

		*this = out;
	}

	

	void initCameraTransform(const Vector3f& pos, const Vector3f& target, const Vector3f& up);
	void initTranslationTransform(float x, float y, float z);
	void initCameraTransform(const Vector3f& target, const Vector3f& up);

	float getDeterminant()
	{
		Vector4f minor, v1, v2, v3;
		float det;

		v1 = Vector4f(this->mat[0][0], this->mat[1][0], this->mat[2][0], this->mat[3][0]);
		v2 = Vector4f(this->mat[0][1], this->mat[1][1], this->mat[2][1], this->mat[3][1]);
		v3 = Vector4f(this->mat[0][2], this->mat[1][2], this->mat[2][2], this->mat[3][2]);


		minor.cross(v1, v2, v3);
		det = -(this->mat[0][3] * minor.x + this->mat[1][3] * minor.y + this->mat[2][3] * minor.z +
			this->mat[3][3] * minor.w);
		return det;
	}


	void inverse()
	{
		int a, i, j;
		Matrix4f out;
		Vector4f v;
		Vector4f vec[3];
		float det = 0.0f;

		det = this->getDeterminant();
		if (!det) return;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (j != i)
				{
					a = j;
					if (j > i) a = a - 1;
					vec[a].x = (this->mat[j][0]);
					vec[a].y = (this->mat[j][1]);
					vec[a].z = (this->mat[j][2]);
					vec[a].w = (this->mat[j][3]);
				}
			}
			v.cross(vec[0], vec[1], vec[2]);

			out.mat[0][i] = pow(-1.0f, i) * v.x / det;
			out.mat[1][i] = pow(-1.0f, i) * v.y / det;
			out.mat[2][i] = pow(-1.0f, i) * v.z / det;
			out.mat[3][i] = pow(-1.0f, i) * v.w / det;
		}

		*this = out;
	}

	Matrix4f operator * (const Matrix4f& matrix)
	{
		Matrix4f out;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				out.mat[i][j] =
					mat[i][0] * matrix.mat[0][j] +
					mat[i][1] * matrix.mat[1][j] +
					mat[i][2] * matrix.mat[2][j] +
					mat[i][3] * matrix.mat[3][j];
			}
		}

		return out;
	}

	void calcClipPlanes(Vector4f& l, Vector4f& r, Vector4f& b, Vector4f& t, Vector4f& n, Vector4f& f) const;
};

class Matrix3f
{
public:
	float mat[3][3];

public:
	Matrix3f()
	{
		setIdentity();
	}

	Matrix3f(const Matrix4f& matrix)
	{
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				mat[i][j] = matrix.mat[i][j];

	}

	void setIdentity()
	{
		::memset(mat, 0, sizeof(mat));
		mat[0][0] = 1;
		mat[1][1] = 1;
		mat[2][2] = 1;
	}

	const Matrix3f& transpose()
	{
		Matrix3f out;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				out.mat[i][j] = mat[j][i];

		return out;
	}

	Matrix3f operator* (const Matrix3f& matrix)
	{
		Matrix3f out;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				out.mat[i][j] =
					mat[i][0] * matrix.mat[0][j] +
					mat[i][1] * matrix.mat[1][j] +
					mat[i][2] * matrix.mat[2][j];
			}
		}

		return out;
	}

	Vector3f operator* (const Vector3f& vector)
	{
		Vector3f out;

		out.x = mat[0][0] * vector.x + mat[0][1] * vector.y + mat[0][2] * vector.z;
		out.y = mat[1][0] * vector.x + mat[1][1] * vector.y + mat[1][2] * vector.z;
		out.z = mat[2][0] * vector.x + mat[2][1] * vector.y + mat[2][2] * vector.z;

		return out;
	}
};

template<typename Type>
class Array2D
{

public:
	Array2D() {}

	Array2D(int Cols, int Rows)
	{
		setArray2D(Cols, Rows);
	}

	void setArray2D(int cols, int rows)
	{
		m_cols = cols;
		m_rows = rows;

		if (m_p) 
		{
			free(m_p);
		}

		m_p = (Type*)malloc(cols * rows * sizeof(Type));
	}

	void setArray2D(int cols, int rows, Type value)
	{
		setArray2D(cols, rows);

		for (int i = 0; i < cols * rows; i++) 
		{
			m_p[i] = value;
		}

		
	}

	void setArray2D(int cols, int rows, const unsigned char* pData)
	{
		m_cols = cols;
		m_rows = rows;

		if (m_p)
		{
			free(m_p);
		}

		m_p = (Type*)pData;
	}

	void set(int col, int row, const Type& val)
	{
		*getAddr(col, row) = val;
	}

	Type* getAddr(int col, int row) const
	{
		size_t index = row * m_cols + col;

		return &m_p[index];
	}

	const Type& get(int col, int row) const
	{
		return *getAddr(col, row);
	}

	Type& at(int col, int row)
	{
		size_t index = calcIndex(col, row);

		return m_p[index];
	}

	size_t calcIndex(int col, int row) const
	{
#ifndef NDEBUG
		if (col < 0) {
			printf("%s:%d - negative col %d\n", __FILE__, __LINE__, col);
		}

		if (col >= m_cols) {
			printf("%s:%d - column overflow (%d vs %d)\n", __FILE__, __LINE__, col, m_cols);
			exit(0);
		}

		if (row < 0) {
			printf("%s:%d - negative row %d\n", __FILE__, __LINE__, row);
		}

		if (row >= m_rows) {
			printf("%s:%d - row overflow (%d vs %d)\n", __FILE__, __LINE__, row, m_rows);
			exit(0);
		}
#endif
		size_t Index = row * m_cols + col;

		return Index;
	}


	void getMinMax(Type& min, Type& max)
	{
		max = min = m_p[0];

		for (int i = 1; i < m_rows * m_cols; i++) 
		{
			if (m_p[i] < min) 
				min = m_p[i];
			

			if (m_p[i] > max) 
				max = m_p[i];
			
		}
	}

	void normalize(Type minRange, Type maxRange)
	{
		Type min, max;

		getMinMax(min, max);

		if (max <= min) {
			return;
		}

		Type minMaxDelta = max - min;
		Type minMaxRange = maxRange - minRange;

		for (int i = 0; i < m_rows * m_cols; i++) 
		{
			m_p[i] = ((m_p[i] - min) / minMaxDelta) * minMaxRange + minRange;
		}
	}

	void print()
	{
		for (int y = 0; y < m_rows; y++) {
			printf("%d: ", y);
			for (int x = 0; x < m_cols; x++) {
				float f = (float)m_p[y * m_cols + x];
				printf("%.6f ", f);
			}
			printf("\n");
		}
	}

	~Array2D()
	{
		destroy();
	}


	void destroy()
	{
		if (m_p) 
		{
			free(m_p);
			m_p = NULL;
		}
	}
private:
	Type* m_p = NULL;
	int m_cols = 0;
	int m_rows = 0;
};

static int calcNextPowerOfTwo(int x)
{
	int ret = 1;

	if (x == 1) {
		return 2;
	}

	while (ret < x) {
		ret = ret * 2;
	}

	return ret;
}