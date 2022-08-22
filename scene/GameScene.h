#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "WinApp.h""
//#include "Player.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public:

	//パーツID
	enum PartId {

		kRoot = 0,	//大元
		kSpine,	//脊髄
		kChest,	//胸
		kHead,	//頭
		kArmL,	//左腕
		kArmR,	//右腕
		kHip,		//尻
		kLegL,	//左足
		kLegR,	//左足

		kNumPartId
	};

	// メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	//Matrix4 WorldMatrix(Matrix4 matworld, Matrix4 matScale, Matrix4 matRot, Matrix4 matTrans);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	
	
	/// <summary>
	/// ゲームシーン用

	//単位行列
	Matrix4 frontVec;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t textureRayHandle_ = 0;
	uint32_t textureGroundHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;
	Model* rayModel = nullptr;

	////ワールドトランスフォーム
	WorldTransform worldTransforms_[100] = { 0 };

	//光線
	WorldTransform  ray = { 0 };

	//始点
	Vector3 startray = { 1,0,0 };
	//終点
	Vector3 endaray = { 1,0,1 };

	//オブジェクトの半径
	float objectRadius = 0.0f;

	//当たり判定フラグ
	bool rayCollision = 0;

	float moveCount = 0.0f;


	//WorldTransform rays = { 0 };

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//プレイヤー
	//Player* player_ = nullptr;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//カメラ上方向の角度
	float viewAngle = 0.0f;

	/// </summary>
};