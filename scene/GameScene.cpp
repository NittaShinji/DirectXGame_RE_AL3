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

	float scaleX = 1.0f;
	float scaleY = 1.0f;
	float scaleZ = 1.0f;

	float translationX = 10;
	float translationY = 10;
	float translationZ = 10;

	//範囲forで全てのワールドトランスフォームを順に処理する
	for (WorldTransform& worldTransform : worldTransforms_)
	{
		//ワールドトランスフォームの初期化
		worldTransform.Initialize();

		//x.y,z方向のスケーリングを設定
		worldTransform.scale_ = { scaleX,scaleY,scaleZ };

		//スケーリング行列を宣言
		Matrix4 matScale;

		//単位行列を代入
		//worldTransform.matWorld_.IdentityMatrix();

		//スケーリング倍率を行列に設定
		matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);
		
		//x.y,z方向の回転角を乱数で設定
		worldTransform.rotation_ = { rotdist(engine),rotdist(engine),rotdist(engine) };
		//worldTransform.rotation_ = { 0.78f,0.78,0};

		//合成用回転行列を宣言
		Matrix4 matRot;

		//各軸用回転用行列を宣言
		Matrix4 matRotX, matRotY, matRotZ;

		//各軸の回転要素を設定
		matRotX.Matrix4RotationX(worldTransform.rotation_.x);
		matRotY.Matrix4RotationY(worldTransform.rotation_.y);
		matRotZ.Matrix4RotationZ(worldTransform.rotation_.z);
		//各軸の回転行列を合成
		matRotZ *= matRotX;
		matRotZ *= matRotY;
		matRot = matRotZ;
		
		// 平行移動行列を宣言
		Matrix4 matTrans = MathUtility::Matrix4Identity();

		//x.y,z方向の平行移動(座標)を乱数で設定
		worldTransform.translation_ = { posdist(engine),posdist(engine),posdist(engine) };
		matTrans.Matrix4Translation(worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z);

		//スケーリング・回転・平行移動を合成した行列を計算
		worldTransform.matWorld_.IdentityMatrix();
		worldTransform.matWorld_ *= matScale;
		worldTransform.matWorld_ *= matRot;
		worldTransform.matWorld_ *= matTrans;
		//ワールドトランスフォームに代入する
		//worldTransform.matWorld_.WorldMatrix(worldTransform.matWorld_, matScale, matRot, matTrans);
		//行列の転送
		worldTransform.TransferMatrix();
	}

	//カメラ視点座標を設定
	//viewProjection_.eye = { 0,0,-10 };

	//カメラ注視点座標を設定
	viewProjection_.target = { 10,0,0 };

	//カメラ上方向ベクトルを設定(右上45度指定)
	viewProjection_.up = { cosf(M_PI / 4.0f), sinf(M_PI / 4.0f),0.0f };

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

	//視点移動処理
	Vector3 eyeMove = { 0.0f,0.0f,0.0f};

	//視点移動の速さ
	const float kEyeSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W))
	{
		eyeMove.z += kEyeSpeed;
	}
	else if(input_->PushKey(DIK_S))
	{
		eyeMove.z -= kEyeSpeed;
	}

	//視点移動(ベクトルの加算)
	viewProjection_.eye += eyeMove;

	//注視点の移動ベクトル
	Vector3 kTargetMove = { 0,0,0 };

	//注視点の移動の速さ
	const float kTargetSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT))
	{
		kTargetMove.x -= kTargetSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		kTargetMove.x += kTargetSpeed;
	}

	//注視点移動(ベクトルの加算)
	viewProjection_.target += kTargetMove;
	
	//上方向回転処理
	
	//上方向の回転の速さ[ラジアン/frame]
	const float kUpRotSpeed = 0.05f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_SPACE))
	{
		viewAngle += kUpRotSpeed;
		//2πを超えたら0に戻す
		viewAngle = fmodf(viewAngle, M_PI * 2.0f);
	}

	//上方向ベクトルを計算(半径1の円周上の座標)
	viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };
	
	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf("up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

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

	for (WorldTransform& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}

	
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