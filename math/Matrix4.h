﻿#pragma once
/// <summary>
/// 行列
/// </summary>
class Matrix4 {
  public:
	// 行x列
	float m[4][4];
	// コンストラクタ
	Matrix4();
	// 成分を指定しての生成
	Matrix4(
	  float m00, float m01, float m02, float m03,
	  float m10, float m11, float m12, float m13,
	  float m20, float m21, float m22, float m23,
	  float m30, float m31, float m32, float m33);

	// 代入演算子オーバーロード
	Matrix4& operator*=(const Matrix4& m2);

	//単位行列
	void IdentityMatrix();

	//X軸回りの回転
	void roateX();
	//Y軸回りの回転
	void roateY();
	//Z軸回りの回転
	void roateZ();

	//度数法からラジアンに変換
	float Toradian(float angle);

	//スケーリング行列を生成
	void Matrix4Scaling(float sx, float sy, float sz);

	//X軸回りの回転行列
	void Matrix4RotationX(float worldTransformRotationX);

	//Y軸回りの回転行列
	void Matrix4RotationY(float worldTransformRotationY);

	//Z軸回りの回転行列
	void Matrix4RotationZ(float worldTransformRotationZ);

	//平行移動行列
	void Matrix4Translation(float tx, float ty, float tz);
	
	//ワールド行列
	void WorldMatrix(Matrix4 world,Matrix4 matScale, Matrix4 matRot, Matrix4 matTran);
};