#pragma once

//#include "Model.h"
//#include "Input.h"
//#include "Debugtext.h"
//#include "TextureManager.h"
//#include "AxisIndicator.h"
//#include "PrimitiveDrawer.h"
//#include <math.h>
//#include <WinApp.h>
//#include <cassert>

#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"


//自キャラ
class Player
{
public:

	Player();
	~Player();

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw();

private:

	//ワールド変換データ
	WorldTransform worldTransform_;

	Input* playerInput_ = nullptr;

	DebugText* debugText_ = nullptr;

	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};

Player::Player()
{
}

Player::~Player()
{
}