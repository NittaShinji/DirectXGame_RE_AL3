#include "Player.h"
#include "input.h"
#include <cassert>


void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);
	assert(textureHandle);


	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	playerInput_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϐ��̏�����
	//�L�����N�^�[�̑匳
	worldTransform_.Initialize();
	////�Ґ�
	//worldTransforms_[PartId::kSpine].Initialize();
	//worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];
	//worldTransforms_[PartId::kSpine].translation_ = { 0.0f,0.5f,0.0f };

	////�㔼�g
	//worldTransforms_[PartId::kChest].Initialize();
	//worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];
	//worldTransforms_[PartId::kChest].translation_ = { 0.0f,0.0f,0.0f };

	////��
	//worldTransforms_[PartId::kHead].Initialize();
	//worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
	//worldTransforms_[PartId::kHead].translation_ = { 0.0f,5.0f,0.0f };

	////���r
	//worldTransforms_[PartId::kArmL].Initialize();
	//worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];
	//worldTransforms_[PartId::kArmL].translation_ = { -5.0f,0.0f,0.0f };

	////�E�r
	//worldTransforms_[PartId::kArmR].Initialize();
	//worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];
	//worldTransforms_[PartId::kArmR].translation_ = { 5.0f,0.0f,0.0f };

	////�����g
	//worldTransforms_[PartId::kHip].Initialize();
	//worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];
	//worldTransforms_[PartId::kHip].translation_ = { 0.0f,-4.5f,0.0f };

	////����
	//worldTransforms_[PartId::kLegL].Initialize();
	//worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];
	//worldTransforms_[PartId::kLegL].translation_ = { -5.0f,-4.5f,0.0f };
	////�E��
	//worldTransforms_[PartId::kLegR].Initialize();
	//worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];
	//worldTransforms_[PartId::kLegR].translation_ = { 5.0f,-4.5f,0.0f };

	////�͈�for�őS�Ẵ��[���h�g�����X�t�H�[�������ɏ�������
	//for (WorldTransform& worldTransform : worldTransforms_)
	//{
	//	//���[���h�g�����X�t�H�[���̏�����
	//	worldTransform.Initialize();

	//	//x.y,z�����̃X�P�[�����O��ݒ�
	//	worldTransform.scale_ = { 1.0f,1.0f,1.0f };

	//	//�X�P�[�����O�s���錾
	//	Matrix4 matScale;

	//	//�P�ʍs�����
	//	//worldTransform.matWorld_.IdentityMatrix();

	//	//�X�P�[�����O�{�����s��ɐݒ�
	//	//matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);
	//	matScale.Matrix4Scaling(1.0f, 1.0f, 1.0f);

	//
	//	//x.y,z�����̉�]�p�𗐐��Őݒ�
	//	worldTransform.rotation_ = { 0.0f,0.0f,0.0f };
	//	//worldTransform.rotation_ = { 0.78f,0.78,0};

	//	//�����p��]�s���錾
	//	Matrix4 matRot;

	//	//�e���p��]�p�s���錾
	//	Matrix4 matRotX, matRotY, matRotZ;

	//	//�e���̉�]�v�f��ݒ�
	//	matRotX.Matrix4RotationX(worldTransform.rotation_.x);
	//	matRotY.Matrix4RotationY(worldTransform.rotation_.y);
	//	matRotZ.Matrix4RotationZ(worldTransform.rotation_.z);
	//	//�e���̉�]�s�������
	//	matRotZ *= matRotX;
	//	matRotZ *= matRotY;
	//	matRot = matRotZ;
	//	
	//	// ���s�ړ��s���錾
	//	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//	//x.y,z�����̕��s�ړ�(���W)�𗐐��Őݒ�
	//	worldTransform.translation_ = { 0.0f,0.0f,0.0f };
	//	matTrans.Matrix4Translation(worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z);

	//	//�X�P�[�����O�E��]�E���s�ړ������������s����v�Z
	//	//���[���h�g�����X�t�H�[���ɑ������
	//	worldTransform.matWorld_.IdentityMatrix();
	//	worldTransform.matWorld_ *= matScale;
	//	worldTransform.matWorld_ *= matRot;
	//	worldTransform.matWorld_ *= matTrans;
	//	
	//	//worldTransform.matWorld_.WorldMatrix(worldTransform.matWorld_, matScale, matRot, matTrans);
	//	//�s��̓]��
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
