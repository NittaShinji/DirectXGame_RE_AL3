#include "Player.h"
#include "input.h"
#include <cassert>


void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);
	assert(textureHandle);


	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	playerInput_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変数の初期化
	//キャラクターの大元
	worldTransform_.Initialize();
	////脊髄
	//worldTransforms_[PartId::kSpine].Initialize();
	//worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];
	//worldTransforms_[PartId::kSpine].translation_ = { 0.0f,0.5f,0.0f };

	////上半身
	//worldTransforms_[PartId::kChest].Initialize();
	//worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];
	//worldTransforms_[PartId::kChest].translation_ = { 0.0f,0.0f,0.0f };

	////頭
	//worldTransforms_[PartId::kHead].Initialize();
	//worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
	//worldTransforms_[PartId::kHead].translation_ = { 0.0f,5.0f,0.0f };

	////左腕
	//worldTransforms_[PartId::kArmL].Initialize();
	//worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];
	//worldTransforms_[PartId::kArmL].translation_ = { -5.0f,0.0f,0.0f };

	////右腕
	//worldTransforms_[PartId::kArmR].Initialize();
	//worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];
	//worldTransforms_[PartId::kArmR].translation_ = { 5.0f,0.0f,0.0f };

	////下半身
	//worldTransforms_[PartId::kHip].Initialize();
	//worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];
	//worldTransforms_[PartId::kHip].translation_ = { 0.0f,-4.5f,0.0f };

	////左足
	//worldTransforms_[PartId::kLegL].Initialize();
	//worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];
	//worldTransforms_[PartId::kLegL].translation_ = { -5.0f,-4.5f,0.0f };
	////右足
	//worldTransforms_[PartId::kLegR].Initialize();
	//worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];
	//worldTransforms_[PartId::kLegR].translation_ = { 5.0f,-4.5f,0.0f };

	////範囲forで全てのワールドトランスフォームを順に処理する
	//for (WorldTransform& worldTransform : worldTransforms_)
	//{
	//	//ワールドトランスフォームの初期化
	//	worldTransform.Initialize();

	//	//x.y,z方向のスケーリングを設定
	//	worldTransform.scale_ = { 1.0f,1.0f,1.0f };

	//	//スケーリング行列を宣言
	//	Matrix4 matScale;

	//	//単位行列を代入
	//	//worldTransform.matWorld_.IdentityMatrix();

	//	//スケーリング倍率を行列に設定
	//	//matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);
	//	matScale.Matrix4Scaling(1.0f, 1.0f, 1.0f);

	//
	//	//x.y,z方向の回転角を乱数で設定
	//	worldTransform.rotation_ = { 0.0f,0.0f,0.0f };
	//	//worldTransform.rotation_ = { 0.78f,0.78,0};

	//	//合成用回転行列を宣言
	//	Matrix4 matRot;

	//	//各軸用回転用行列を宣言
	//	Matrix4 matRotX, matRotY, matRotZ;

	//	//各軸の回転要素を設定
	//	matRotX.Matrix4RotationX(worldTransform.rotation_.x);
	//	matRotY.Matrix4RotationY(worldTransform.rotation_.y);
	//	matRotZ.Matrix4RotationZ(worldTransform.rotation_.z);
	//	//各軸の回転行列を合成
	//	matRotZ *= matRotX;
	//	matRotZ *= matRotY;
	//	matRot = matRotZ;
	//	
	//	// 平行移動行列を宣言
	//	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//	//x.y,z方向の平行移動(座標)を乱数で設定
	//	worldTransform.translation_ = { 0.0f,0.0f,0.0f };
	//	matTrans.Matrix4Translation(worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z);

	//	//スケーリング・回転・平行移動を合成した行列を計算
	//	//ワールドトランスフォームに代入する
	//	worldTransform.matWorld_.IdentityMatrix();
	//	worldTransform.matWorld_ *= matScale;
	//	worldTransform.matWorld_ *= matRot;
	//	worldTransform.matWorld_ *= matTrans;
	//	
	//	//worldTransform.matWorld_.WorldMatrix(worldTransform.matWorld_, matScale, matRot, matTrans);
	//	//行列の転送
	//	worldTransform.TransferMatrix();
	//}

}

void Player::Update()
{

}

void Player::Draw()
{
	for (int i = 0; i < 2; i++)
	{
		/*if (i == kRoot) {
			continue;
		}
		else if (i == kSpine)
		{
			continue;
		}
		else
		{

		}*/

		//model_->Draw(worldTransforms_[0], debugCamera_->GetViewProjection(), textureHandle_);
		//
		//if (rayCollision == 1)
		//{
		//	model_->Draw(worldTransforms_[1], debugCamera_->GetViewProjection(), textureHandle_);
		//}
	}

}
