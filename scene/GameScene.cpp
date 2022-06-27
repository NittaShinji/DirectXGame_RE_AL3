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

	//カメラ視点座標を設定
	viewProjection_.eye = { 0,20,-50 };

	//カメラ注視点座標を設定
	viewProjection_.target = { 0.0f,0.0f,0.0f };
	//viewProjection_.target = worldTransforms_[0].translation_;

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

	//自機
	worldTransforms_[0].Initialize();
	worldTransforms_[0].translation_ = { 0.0f,0.0f,0.0f};
	
	worldTransforms_[1].Initialize();
	worldTransforms_[1].translation_ = { 5.0f,0.0f,0.0f};
	
	worldTransforms_[2].Initialize();
	worldTransforms_[2].translation_ = { -5.0f,0.0f,0.0f};

	worldTransforms_[3].Initialize();
	worldTransforms_[3].translation_ = { 0.0f,0.0f,5.0f};

	worldTransforms_[4].Initialize();
	worldTransforms_[4].translation_ = { 0.0f,0.0f,-5.0f};

	////ワールド行列の転送
	//worldTransforms_[0].TransferMatrix();
	//worldTransforms_[1].TransferMatrix();
	//worldTransforms_[2].TransferMatrix();


	for (int i = 0; i < 5; i++)
	{
		Matrix4 matTrans = MathUtility::Matrix4Identity();

		matTrans.Matrix4Translation(
			worldTransforms_[i].translation_.x, worldTransforms_[i].translation_.y, worldTransforms_[i].translation_.z);

		worldTransforms_[i].matWorld_.IdentityMatrix();

		worldTransforms_[i].matWorld_ *= matTrans;

		worldTransforms_[i].TransferMatrix();
	}

	//Matrix4 matTrans = MathUtility::Matrix4Identity();

	//matTrans.Matrix4Translation(
	//	worldTransforms_[0].translation_.x, worldTransforms_[0].translation_.y, worldTransforms_[0].translation_.z);

	//worldTransforms_[0].matWorld_.IdentityMatrix();

	//worldTransforms_[0].matWorld_ *= matTrans;

	//worldTransforms_[0].TransferMatrix();

	//Matrix4 matTrans1 = MathUtility::Matrix4Identity();

	//matTrans1.Matrix4Translation(
	//	worldTransforms_[1].translation_.x, worldTransforms_[1].translation_.y, worldTransforms_[1].translation_.z);

	//worldTransforms_[1].matWorld_.IdentityMatrix();

	//worldTransforms_[1].matWorld_ *= matTrans1;

	//worldTransforms_[1].TransferMatrix();

	//Matrix4 matTrans2 = MathUtility::Matrix4Identity();

	//matTrans2.Matrix4Translation(
	//	worldTransforms_[2].translation_.x, worldTransforms_[2].translation_.y, worldTransforms_[2].translation_.z);

	//worldTransforms_[2].matWorld_.IdentityMatrix();

	//worldTransforms_[2].matWorld_ *= matTrans2;

	//worldTransforms_[2].TransferMatrix();

	//worldTransforms_[1].parent_ = &worldTransforms_[0];

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

	if (input_->TriggerKey(DIK_Q) == true)
	{
		if (bioMove == 0)
		{
			bioMove = 1;
		}
		else
		{
			bioMove = 0;
		}
	}

	////視点移動(ベクトルの加算)
	//viewProjection_.eye += eyeMove;

	//注視点の移動ベクトル
	//Vector3 kTargetMove = { 0,0,0 };

	////注視点の移動の速さ
	//const float kTargetSpeed = 0.2f;

	//キャラクターの速さ
	float playerSpeed = 0.1f;
	
	if (bioMove == 0)
	{
		Vector3 roate = { 0.0f, 0.0f, 0.0f };

		//回転速度
		const float RoateSpeed = 0.05f;

		if (input_->PushKey(DIK_R))
		{
			roate = { -RoateSpeed,0,  -RoateSpeed };
		}
		else if (input_->PushKey(DIK_E))
		{
			roate = { RoateSpeed, 0, RoateSpeed };
		}

		viewProjection_.target.x += roate.x;
		viewProjection_.target.z += roate.z;

		//正面移動の単位ベクトル
		Vector3 frontVec = { 0, 0, 1 };

		Vector3 target = viewProjection_.target;

		//正面ベクトルの計算
		frontVec = target -= viewProjection_.eye;

		//正面ベクトルを正規化
		MathUtility::Vector3Normalize(frontVec);

		Vector3 move = { 0.0f, 0.0f, 0.0f };

		//プレイヤーがカメラ右に進む処理
		//右ベクトルの宣言
		Vector3 rightVec;
		//仮ベクトルの宣言
		Vector3 temporaryVec = { 0,1,0 };

		//右ベクトルを作成
		rightVec = MathUtility::Vector3Cross(temporaryVec, frontVec);

		//右ベクトルの正規化
		MathUtility::Vector3Normalize(temporaryVec);

		// 移動速度
		const float moveSpeed = 0.05f;

		Vector3 resultVec = { 0,0,0 };

		if (input_->PushKey(DIK_W))
		{
			move.z += frontVec.z * playerSpeed;
		}
		if (input_->PushKey(DIK_S))
		{
			move.z += -frontVec.z * playerSpeed;
		}
		if (input_->PushKey(DIK_D))
		{
			resultVec.x += rightVec.x * playerSpeed;
			resultVec.z += rightVec.z * playerSpeed;

			//move = { resultVec.x, 0.0f, +resultVec.z };
		}
		if (input_->PushKey(DIK_A))
		{
			resultVec.x += -rightVec.x * playerSpeed;
			resultVec.z += -rightVec.z * playerSpeed;

			//move = { resultVec.x, 0.0f, +resultVec.z };
		}

		worldTransforms_->translation_.x += resultVec.x;
		worldTransforms_->translation_.z += resultVec.z;

		worldTransforms_->translation_.z += move.z;


		Matrix4 matTrans = MathUtility::Matrix4Identity();

		/*matTrans.Matrix4Translation(
			walk.x, walk.y, walk.z);*/

		matTrans.Matrix4Translation(
			worldTransforms_->translation_.x, 0, worldTransforms_->translation_.z);

		worldTransforms_[0].matWorld_.IdentityMatrix();

		worldTransforms_[0].matWorld_ *= matTrans;

		for (int i = 0; i < 3; i++)
		{
			worldTransforms_[i].TransferMatrix();
		}
	}

	if (bioMove == 1)
	{
		Vector3 roate = { 0.0f, 0.0f, 0.0f };

		//回転速度
		const float RoateSpeed = 0.02f;

		if (input_->PushKey(DIK_A))
		{
			roate = { 0, -RoateSpeed, 0 };
		}
		else if (input_->PushKey(DIK_D))
		{
			roate = { 0, RoateSpeed, 0 };
		}

		worldTransforms_[0].rotation_.y += roate.y; 

		//Y軸用回転用行列を宣言
		Matrix4 matRotY;

		//Y軸の回転要素を設定
		matRotY.Matrix4RotationY(worldTransforms_[0].rotation_.y);

		//-----------------------------------

		//単位ベクトル(逆)
		Vector3 frontVec = { 0.0f, 0.0f, -1.0f };
		//結果用のベクトル
		Vector3 resultVec = { 0.0f, 0.0f, 0.0f };

		resultVec.x = (cos(worldTransforms_[0].rotation_.y) * frontVec.x +
			sin(worldTransforms_[0].rotation_.y) * frontVec.z);
		resultVec.z = (-sinf(worldTransforms_[0].rotation_.y) * frontVec.x +
			cosf(worldTransforms_[0].rotation_.y) * frontVec.z);

		//キャラクターの移動速さ
		//キャラクターの移動速さ
		const float kCharacterSpeed = 0.2f;

		Vector3 move = { 0.0f, 0.0f, 0.0f };

		Vector3 direction = { 0.0f, 0.0f, 0.0f };

		/*if (input_->PushKey(DIK_W))
		{
			move = { 0.0f, 0.0f, +playerSpeed };
		}
		if (input_->PushKey(DIK_S))
		{
			move = { 0.0f, 0.0f, -playerSpeed };
		}*/

		if (input_->PushKey(DIK_W))
		{
			direction.x += resultVec.x * kCharacterSpeed;
			direction.z += resultVec.z * kCharacterSpeed;
		}

		move = { direction.x, 0.0f, direction.z };

		if (input_->PushKey(DIK_S))
		{
			direction.x += -resultVec.x * kCharacterSpeed;
			direction.z += -resultVec.z * kCharacterSpeed;
		}

		worldTransforms_[0].translation_.x += direction.x;
		worldTransforms_[0].translation_.z += direction.z;

		Matrix4 matTrans = MathUtility::Matrix4Identity();

		matTrans.Matrix4Translation(
			worldTransforms_[0].translation_.x,0, worldTransforms_[0].translation_.z);

		worldTransforms_[0].matWorld_.IdentityMatrix();
		worldTransforms_[0].matWorld_ *= matRotY;
		worldTransforms_[0].matWorld_ *= matTrans;

		for (int i = 0; i < 5; i++)
		{
			worldTransforms_[i].TransferMatrix();
		}
	}

	//視点移動の速さ
	const float CameraSpeed = 0.1f;

	//視点移動処理
	if (input_->PushKey(DIK_UP))
	{
		viewProjection_.eye.z += CameraSpeed;
		viewProjection_.target.z += CameraSpeed;
	}
	else if (input_->PushKey(DIK_DOWN))
	{
		viewProjection_.eye.z -= CameraSpeed;
		viewProjection_.target.z -= CameraSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		viewProjection_.eye.x += CameraSpeed;
		viewProjection_.target.x += CameraSpeed;
	}
	else if (input_->PushKey(DIK_LEFT))
	{
		viewProjection_.eye.x -= CameraSpeed;
		viewProjection_.target.x -= CameraSpeed;
	}

	//ビュー行列の更新
	viewProjection_.UpdateMatrix();

	debugCamera_->Update();

	// デバッグ用表示
	/*debugText_->SetPos(50, 110);
	debugText_->Printf(
		"forAngleY(Degree):%f", ToAngle(viewProjection_.fovAngleY));

	debugText_->SetPos(50, 130);
	debugText_->Printf(
		"worldTranslation:(%f,%f,%f)", worldTransforms_[0].translation_.x,
		worldTransforms_[0].translation_.y, worldTransforms_[0].translation_.z);

	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"worldTranslation:(%f,%f,%f)", worldTransforms_[1].translation_.x,
		worldTransforms_[1].translation_.y, worldTransforms_[1].translation_.z);

	debugText_->SetPos(50, 180);
	debugText_->Printf(
		"viewProjection_.target(x,z):(%f,%f) [E][R]", viewProjection_.target.x, viewProjection_.target.z);*/


	debugText_->SetPos(50, 210);
	debugText_->Printf(
		"[W][A][S][D]: move,[E][R] CameraRoate", bioMove);

	debugText_->SetPos(50, 250);
	debugText_->Printf(
		"bioMove[Q]:%d",bioMove );

	debugText_->SetPos(50, 270);
	debugText_->Printf("bioMove [A][D] Roate, [W][S] move");

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

	/*for (int i = 0; i < 3; i++)
	{
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}*/

	model_->Draw(worldTransforms_[0], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[1], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[2], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[3], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[4], viewProjection_, textureHandle_);
	
	//for (int i = 0; i < 100; i++)
	//{

	//	//変数で描くバージョン
	//	//RedStart = { 0.0f, 0.0f, 0.0f + (0.5f * i) };
	//	//RedEnd = { 20.0f,0.0f, 0.0f + (0.5f * i) };
	//	//
	//	//BlueStart = { 0.0f + (0.5f * i),0.0f,0.0f };
	//	//BlueEnd = { 0.0f + (0.5f * i),0.0f,20.0f };
	//	////ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	//	//PrimitiveDrawer::GetInstance()->DrawLine3d(RedStart, RedEnd, RedColor);
	//	//PrimitiveDrawer::GetInstance()->DrawLine3d(BlueStart,BlueEnd,BlueColor);

	//	//変数作らないバージョン
	//	PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(0, 0, 0 - i), Vector3(100, 0, 0 - i), Vector4(256, 0, 0, 1));
	//	PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(0 + i, 0, 0), Vector3(0 + i, 0, -100), Vector4(0, 128, 0, 1));
	//}

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