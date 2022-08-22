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


//���L����
class Player
{
public:

	Player();
	~Player();

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw();

private:

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	Input* playerInput_ = nullptr;

	DebugText* debugText_ = nullptr;

	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

};

Player::Player()
{
}

Player::~Player()
{
}