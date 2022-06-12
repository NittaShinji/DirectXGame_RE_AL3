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

float ToRadian(float angle);
float ToAngle(float radian);

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

	//親(0番)
	worldTransforms_[0].Initialize();
	//子(1番)
	worldTransforms_[1].Initialize();
	worldTransforms_[1].translation_ = { 0.0f,4.5f,0.0f };
	worldTransforms_[1].parent_ = &worldTransforms_[0];

	////範囲forで全てのワールドトランスフォームを順に処理する
	//for (WorldTransform& worldTransform : worldTransforms_)
	//{
	//	//ワールドトランスフォームの初期化
	//	worldTransform.Initialize();

	//	//x.y,z方向のスケーリングを設定
	//	worldTransform.scale_ = { scaleX,scaleY,scaleZ };

	//	//スケーリング行列を宣言
	//	Matrix4 matScale;

	//	//単位行列を代入
	//	//worldTransform.matWorld_.IdentityMatrix();

	//	//スケーリング倍率を行列に設定
	//	matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);
	//	
	//	//x.y,z方向の回転角を乱数で設定
	//	worldTransform.rotation_ = { rotdist(engine),rotdist(engine),rotdist(engine) };
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
	//	worldTransform.translation_ = { posdist(engine),posdist(engine),posdist(engine) };
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

	//カメラ視点座標を設定
	//viewProjection_.eye = { 0,0,-10 };

	//カメラ注視点座標を設定
	//viewProjection_.target = { 10,0,0 };

	//カメラ上方向ベクトルを設定
	//viewProjection_.up = { 0.0f,1.0f,0.0f };

	//カメラ垂直方向視野角を設定
	//viewProjection_.fovAngleY = ToRadian(10.0f);

	//アスペクト比を設定
	//viewProjection_.aspectRatio = 1.0f;

	////ニアクリップ距離を設定
	//viewProjection_.nearZ = 52.0f;
	////ファークリップ距離を設定
	//viewProjection_.farZ = 53.0f;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(winApp_->kWindowWidth, winApp_->kWindowHeight);

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを生成する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

}

void GameScene::Update()
{
	debugCamera_->Update();

	////視点移動処理
	//Vector3 eyeMove = { 0.0f,0.0f,0.0f};

	////視点移動の速さ
	//const float kEyeSpeed = 0.2f;

	////押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_W))
	//{
	//	eyeMove.z += kEyeSpeed;
	//}
	//else if(input_->PushKey(DIK_S))
	//{
	//	eyeMove.z -= kEyeSpeed;
	//}

	////視点移動(ベクトルの加算)
	//viewProjection_.eye += eyeMove;

	////注視点の移動ベクトル
	//Vector3 kTargetMove = { 0,0,0 };

	////注視点の移動の速さ
	//const float kTargetSpeed = 0.2f;

	////押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_LEFT))
	//{
	//	kTargetMove.x -= kTargetSpeed;
	//}
	//else if (input_->PushKey(DIK_RIGHT))
	//{
	//	kTargetMove.x += kTargetSpeed;
	//}

	////注視点移動(ベクトルの加算)
	//viewProjection_.target += kTargetMove;
	
	//キャラクター移動処理
	float scaleX = 2.0f;
	float scaleY = 2.0f;
	float scaleZ = 2.0f;

	//x.y,z方向のスケーリングを設定
	worldTransforms_[0].scale_ = { scaleX,scaleY,scaleZ };

	//スケーリング行列を宣言
	Matrix4 matScale;

	//スケーリング倍率を行列に設定
	matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);

	//x.y,z方向の回転角を乱数で設定
	worldTransforms_[1].rotation_ = { 0.0f,0.0f,0.0f };

	//合成用回転行列を宣言
	Matrix4 matRot;

	//各軸用回転用行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	//各軸の回転要素を設定
	matRotX.Matrix4RotationX(worldTransforms_[0].rotation_.x);
	matRotY.Matrix4RotationY(worldTransforms_[0].rotation_.y);
	matRotZ.Matrix4RotationZ(worldTransforms_[0].rotation_.z);
	//各軸の回転行列を合成
	matRotZ *= matRotX;
	matRotZ *= matRotY;
	matRot = matRotZ;

	//キャラクターの移動ベクトル
	Vector3 playerMove = { 0,0,0 };

	//キャラクターの速さ
	//float playerSpeed = 0.01f;

	if (input_->PushKey(DIK_RIGHT))
	{
		playerMove.x += 0.1;
	}
	else if (input_->PushKey(DIK_LEFT))
	{
		playerMove.x -= 0.1;
	}

	worldTransforms_[0].translation_ = playerMove;

	//ワールド行列の計算
	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.Matrix4Translation(worldTransforms_[0].translation_.x, worldTransforms_[0].translation_.y, worldTransforms_[0].translation_.z);

	worldTransforms_[0].matWorld_.IdentityMatrix();

	worldTransforms_[0].matWorld_ *= matTrans;

	//行列の転送
	worldTransforms_[0].TransferMatrix();

	////子の更新

	//x.y,z方向のスケーリングを設定
	worldTransforms_[1].scale_ = {2.0f,2.0f,2.0f };

	//スケーリング行列を宣言
	Matrix4 matScale2;

	//スケーリング倍率を行列に設定
	matScale2.Matrix4Scaling(scaleX, scaleY, scaleZ);

	//x.y,z方向の回転角を乱数で設定
	worldTransforms_[1].rotation_ = { 0.0f,0.0f,0.0f};
	
	//合成用回転行列を宣言
	Matrix4 matRot2;

	//各軸用回転用行列を宣言
	Matrix4 matRotX2, matRotY2, matRotZ2;

	//各軸の回転要素を設定
	matRotX2.Matrix4RotationX(worldTransforms_[1].rotation_.x);
	matRotY2.Matrix4RotationY(worldTransforms_[1].rotation_.y);
	matRotZ2.Matrix4RotationZ(worldTransforms_[1].rotation_.z);
	//各軸の回転行列を合成
	matRotZ2 *= matRotX2;
	matRotZ2 *= matRotY2;
	matRot2 = matRotZ2;

	//x.y,z方向の平行移動(座標)を乱数で設定
	worldTransforms_[1].translation_ = { 0.0f,0.0f,0.0f };

	Matrix4 matTrans2 = MathUtility::Matrix4Identity();

	matTrans2.Matrix4Translation(worldTransforms_[1].translation_.x, worldTransforms_[1].translation_.y, worldTransforms_[1].translation_.z);

	//スケーリング・回転・平行移動を合成した行列を計算
	//ワールドトランスフォームに代入する
	worldTransforms_[1].matWorld_.IdentityMatrix();
	worldTransforms_[1].matWorld_ *= matScale2;
	worldTransforms_[1].matWorld_ *= matRot2;
	worldTransforms_[1].matWorld_ *= matTrans2;

	worldTransforms_[1].matWorld_ *= worldTransforms_[0].matWorld_;
	worldTransforms_[1].TransferMatrix();

	//行列の再計算
	viewProjection_.UpdateMatrix();

	debugText_->SetPos(50, 110);
	debugText_->Printf("forAngleY(Degree):%f", ToAngle(viewProjection_.fovAngleY));

	debugText_->SetPos(50, 130);
	debugText_->Printf("worldTranslation:(%f,%f,%f)", worldTransforms_[0].translation_.x, worldTransforms_[0].translation_.y, worldTransforms_[0].translation_.z);
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
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}*/

	model_->Draw(worldTransforms_[0], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[1], viewProjection_, textureHandle_);


	/*for (int i = 0; i < 100; i++)
	{
		model_->Draw(worldTransforms_[i], debugCamera_->GetViewProjection(), textureHandle_);
	}*/


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

float ToRadian(float angle)
{
	return angle / 180;
}

float ToAngle(float radian)
{
	return  (180 / M_PI) * radian;
}