#include"Matrix4.h"
#include "math.h"

void Matrix4(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{}

//単位行列の代入
void Matrix4::IdentityMatrix()
{
	m[0][0] = 1;
	m[1][1] = 1;
	m[2][2] = 1;
	m[3][3] = 1;
}

void Matrix4::roateX()
{
	m[0][0] = 1;
	m[3][3] = 1;

}

void Matrix4::roateY()
{
	m[1][1] = 1;
	m[3][3] = 1;
}

void Matrix4::roateZ()
{
	m[2][2] = 1;
	m[3][3] = 1;
}

//度数法変換(ラジアン→度数法)
float Matrix4::Toradian(float angle)
{
	return angle / 180;
}

//スケーリング行列
void Matrix4::Matrix4Scaling(float sx, float sy, float sz)
{
	m[0][0] = sx;
	m[1][1] = sy;
	m[2][2] = sz;
	m[3][3] = 1;
}

//X軸回りの回転行列
void Matrix4::Matrix4RotationX(float worldTransformRotationX)
{
	m[1][1] = cos(worldTransformRotationX);
	m[1][2] = sin(worldTransformRotationX);
	m[2][1] = -sin(worldTransformRotationX);
	m[2][2] = cos(worldTransformRotationX);
	m[0][0] = 1;
	m[3][3] = 1;
}

//Y軸回りの回転行列
void Matrix4::Matrix4RotationY(float worldTransformRotationY)
{
	m[0][0] = cos(worldTransformRotationY);
	m[0][2] = -sin(worldTransformRotationY);
	m[2][0] = sin(worldTransformRotationY);
	m[2][2] = cos(worldTransformRotationY);
	m[1][1] = 1;
	m[3][3] = 1;
}

//Y軸回りの回転行列
void Matrix4::Matrix4RotationZ(float worldTransformRotationZ) 
{
	m[0][0] = cos(worldTransformRotationZ);
	m[0][1] = sin(worldTransformRotationZ);
	m[1][0] = -sin(worldTransformRotationZ);
	m[1][1] = cos(worldTransformRotationZ);
	m[2][2] = 1;
	m[3][3] = 1;
}

//平行移動行列
void Matrix4::Matrix4Translation(float tx, float ty, float tz)
{
	m[3][0] = tx;
	m[3][1] = ty;
	m[3][2] = tz;
}

//ワールド行列の生成
void Matrix4::WorldMatrix(Matrix4 matworld, Matrix4 matScale, Matrix4 matRot, Matrix4 matTrans)
{
	matworld *= matScale;
	matworld *= matRot;
	matworld *= matTrans;
}
