#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <math.h>
#include <WinApp.h>
#include <cassert>
#include <random>

#ifndef M_PI
#define M_PI 3.14159 /* 桁数はもっと多い方がいいかも */
#endif

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	winApp_ = WinApp::GetInstance();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//回転角用の乱数範囲の指定
	std::uniform_real_distribution<float> rotdist(0.0f, M_PI);
	//座標用の乱数範囲の指定
	std::uniform_real_distribution<float> posdist(-10.0f, 10.0f);

	float rotValue = rotdist(engine);
	float posValue = posdist(engine);

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

	float scaleX = 5.0f;
	float scaleY = 5.0f;
	float scaleZ = 5.0f;

	float translationX = 10;
	float translationY = 10;
	float translationZ = 10;

	//スケーリング行列を宣言
	Matrix4 matScale;
	//合成用回転行列を宣言
	Matrix4 matRot;
	//各軸用回転用行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;
	// 平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();


	////範囲forで全てのワールドトランスフォームを順に処理する
	//for (WorldTransform& worldTransform : worldTransforms_)
	//{
	//	//ワールドトランスフォームの初期化
	//	worldTransform.Initialize();

	//	//x.y,z方向のスケーリングを設定
	//	worldTransform.scale_ = { 3.0f,3.0f,3.0f };
	//	//スケーリング倍率を行列に設定
	//	matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);

	//	//x.y,z方向の回転角を乱数で設定
	//	//worldTransform.rotation_ = { rotdist(engine),rotdist(engine),rotdist(engine) };
	//	worldTransform.rotation_ = { 0.78f,0.78,0};

	//	//x.y,z方向の平行移動(座標)を乱数で設定
	//	worldTransform.translation_ = { posdist(engine),0,0 };
	//	matTrans.Matrix4Translation(worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z);

	//	//各軸の回転要素を設定
	//	matRotX.Matrix4RotationX(worldTransform.rotation_.x);
	//	matRotY.Matrix4RotationY(worldTransform.rotation_.y);
	//	matRotZ.Matrix4RotationZ(worldTransform.rotation_.z);
	//	//各軸の回転行列を合成
	//	matRotZ *= matRotX;
	//	matRotZ *= matRotY;
	//	matRot = matRotZ;
	//	
	//	//スケーリング・回転・平行移動を合成した行列を計算
	//	worldTransform.matWorld_.IdentityMatrix();
	//	/*worldTransform.matWorld_ *= matScale;
	//	worldTransform.matWorld_ *= matRot;
	//	worldTransform.matWorld_ *= matTrans;*/
	//	//ワールドトランスフォームに代入する
	//	worldTransform.matWorld_.WorldMatrix(worldTransform.matWorld_, matScale, matRot, matTrans);
	//	//行列の転送
	//	worldTransform.Initialize();
	//}

	for (size_t i = 0; i < _countof(worldTransforms_); i++)
	{
		//x.y,z方向のスケーリングを設定
		worldTransforms_[i].scale_ = {3.0f,3.0f,3.0f};
		//スケーリング倍率を行列に設定
		matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);

		//x.y,z方向の回転角を乱数で設定
		worldTransforms_[i].rotation_ = {0.78f,0.78,0};

		//各軸の回転要素を設定
		matRotX.Matrix4RotationX(worldTransforms_[i].rotation_.x);
		matRotY.Matrix4RotationY(worldTransforms_[i].rotation_.y);
		matRotZ.Matrix4RotationZ(worldTransforms_[i].rotation_.z);

		//各軸の回転行列を合成
		matRotZ *= matRotX;
		matRotZ *= matRotY;
		matRot = matRotZ;

		worldTransforms_[i].translation_ = { posdist(engine),posdist(engine),posdist(engine) };
		matTrans.Matrix4Translation(worldTransforms_[i].translation_.x, worldTransforms_[i].translation_.y, worldTransforms_[i].translation_.z);

		//スケーリング・回転・平行移動を合成した行列を計算
		//worldTransforms_[i].matWorld_.IdentityMatrix();
		worldTransforms_[i].matWorld_ *= matScale;
		worldTransforms_[i].matWorld_ *= matRot;
		worldTransforms_[i].matWorld_ *= matTrans;
		//ワールドトランスフォームに代入する
		worldTransforms_[i].matWorld_.WorldMatrix(worldTransforms_[i].matWorld_, matScale, matRot, matTrans);
		//行列の転送
		worldTransforms_[i].Initialize();
	}

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(winApp_->kWindowWidth, winApp_->kWindowHeight);

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画が参照するビュープロジェクションを生成する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//
	////x,y,z方向のスケーリングを設定
	//for (int i = 0; i < _countof(worldTransforms_); i++)
	//{
	//	worldTransforms_[i].scale_ = {scaleX,scaleY,scaleZ};
	//}
	//
	////スケーリング行列を宣言
	//Matrix4 matScale;

	////スケーリング倍率を行列に設定
	//matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);

	/*worldTransform_.matWorld_.IdentityMatrix();

	worldTransform_.matWorld_ *= matScale;*/

	////行列の転送
	//worldTransform_.TransferMatrix();

	////Z軸回りの回転角を設定
	//worldTransform_.rotation_ = { 0.0f,0.0f,0.78f };

	////Z軸回転行列を宣言
	//Matrix4 matRotZ;

	////Z軸回転行列を各要素に設定する。
	//matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	//matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	//matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	//matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);
	//matRotZ.m[2][2] = 1;
	//matRotZ.m[3][3] = 1;

	//worldTransform_.matWorld_.IdentityMatrix();
	//worldTransform_.matWorld_ *= matRotZ;

	////行列の転送
	//worldTransform_.TransferMatrix();

	////X軸回りの回転角を設定
	//worldTransform_.rotation_ = { 0.78f,0.0f,0.0f };

	////X軸回転行列を宣言
	//Matrix4 matRotX;

	////X軸回転行列を各要素に設定する。
	//matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
	//matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
	//matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	//matRotX.m[2][2] = cos(worldTransform_.rotation_.x);
	//matRotX.m[0][0] = 1;
	//matRotX.m[3][3] = 1;

	//worldTransform_.matWorld_.IdentityMatrix();
	//worldTransform_.matWorld_ *= matRotX;

	////行列の転送
	//worldTransform_.TransferMatrix();

	////Y軸回りの回転角を設定
	//worldTransform_.rotation_ = { 0.0f,0.78f,0.0f };

	////Y軸回転行列を宣言
	//Matrix4 matRotY;

	////Y軸回転行列を各要素に設定する。
	//matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
	//matRotY.m[0][2] = -sin(worldTransform_.rotation_.y);
	//matRotY.m[2][0] = sin(worldTransform_.rotation_.y);
	//matRotY.m[2][2] = cos(worldTransform_.rotation_.y);
	//matRotY.m[1][1] = 1;
	//matRotY.m[3][3] = 1;

	//worldTransform_.matWorld_.IdentityMatrix();
	//worldTransform_.matWorld_ *= matRotY;

	////行列の転送
	//worldTransform_.TransferMatrix();

	//X,Y,Z軸回りの回転角を設定
	//worldTransforms_.rotation_ = { 0.78,0.78,0.0f };
	
	//各軸の回転要素を設定
	//matRotX.Matrix4RotationX(worldTransforms_.rotation_.x);
	//matRotY.Matrix4RotationY(worldTransforms_.rotation_.y);
	//matRotZ.Matrix4RotationZ(worldTransforms_.rotation_.z);

	////各軸の回転行列を合成
	//matRotZ *= matRotX;
	//matRotZ *= matRotY;
	//matRot = matRotZ;

	//
	////　X,Y,Z軸回りの平行移動を設定
	//worldTransforms_.translation_ = {translationX,translationY,translationZ };
	//// 平行移動行列を宣言
	//Matrix4 matTrans = MathUtility::Matrix4Identity();

	//matTrans.Matrix4Translation(translationX, translationY, translationZ);

	//worldTransforms_.matWorld_.IdentityMatrix();
	//worldTransforms_.matWorld_ *= matScale;
	//worldTransforms_.matWorld_ *= matRot;
	//worldTransforms_.matWorld_ *= matTrans;

	//worldTransforms_.matWorld_.WorldMatrix(worldTransforms_.matWorld_, matScale, matRot, matTrans);

	////行列の転送
	//worldTransforms_.TransferMatrix();
}

void GameScene::Update() {

	debugCamera_->Update();
}

void GameScene::Draw() {

	//描画用の線のベクトル変数
	Vector3 RedStart = { 0.0f, 0.0f, 0.0f };
	Vector3 RedEnd = { 128.0f, 0.0f, 128.0f };

	Vector3 BlueStart = { 0.0f, 0.0f, 0.0f };
	Vector3 BlueEnd = { 128.0f, 0.0f, 128.0f };

	Vector4 RedColor = { 128.0f, 0.0f, 0.0f, 1.0f };
	Vector4 BlueColor = { 0.0f, 0.0f, 128.0f, 1.0f };

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//	3Dモデル描画
	//model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	/*for (WorldTransform& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, debugCamera_->GetViewProjection(), textureHandle_);
	}*/

	
	for (int i = 0; i < 100; i++)
	{
		model_->Draw(worldTransforms_[i], debugCamera_->GetViewProjection(), textureHandle_);
	}


	for (int i = 0; i < 100; i++)
	{

		//変数で描くバージョン
		//RedStart = { 0.0f, 0.0f, 0.0f + (0.5f * i) };
		//RedEnd = { 20.0f,0.0f, 0.0f + (0.5f * i) };
		//
		//BlueStart = { 0.0f + (0.5f * i),0.0f,0.0f };
		//BlueEnd = { 0.0f + (0.5f * i),0.0f,20.0f };
		////ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
		//PrimitiveDrawer::GetInstance()->DrawLine3d(RedStart, RedEnd, RedColor);
		//PrimitiveDrawer::GetInstance()->DrawLine3d(BlueStart,BlueEnd,BlueColor);

		//変数作らないバージョン
		PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(0, 0, 0 - i), Vector3(100, 0, 0 - i), Vector4(256, 0, 0, 1));
		PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(0 + i, 0, 0), Vector3(0 + i, 0, -100), Vector4(0, 128, 0, 1));
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}