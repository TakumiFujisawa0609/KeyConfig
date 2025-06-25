#include "Player.h"
#include "../Utility/AsoUtility.h"
#include "../Input/InputManager.h"
#include "../Camera/Camera.h"

// �R���X�g���N�^
Player::Player(void){}
// �f�X�g���N�^
Player::~Player(void){}

// ������
void Player::Init(void)
{
	// ���f���̏����ʒu�ݒ�
	pos_ = { 0.0f, 0.0f, 0.0f };
	MV1SetPosition(modelId_, pos_);

	// �ړ��x�N�g�����쐬����p�x������
	moveVecRad_ = { 0.0f, 0.0f, 0.0f };

	// ���f���̊p�x�ݒ�
	rot_ = { 0.0f, 0.0f, 0.0f };

	// ���f���̊p�x
	localRot_ = INIT_MODEL_ROT_OFFSET;

	// �J�����A���O��
	cameraAngle_ = { 0.0f, 0.0f, 0.0f };
}

// �ǂݍ���
void Player::Load(void)
{
	// ���f���̓ǂݍ���
	modelId_ = MV1LoadModel("Data/Model/Human.mv1");
}

// �ǂݍ��݌�
void Player::LoadEnd(void)
{
	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(modelId_, 1);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(modelId_, animAttachNo_);
	currentAnimTime_ = 0.0f;

	// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
	MV1SetAttachAnimTime(modelId_, animAttachNo_, currentAnimTime_);
}

// �X�V
void Player::Update(VECTOR angle)
{
	// �J�����A���O���擾
	cameraAngle_ = angle;

	// �A�j���[�V��������
	UpdateAnimation();

	// �ړ�����
	ProcessMove(cameraAngle_);

	// ��]�l�ݒ�
	SetRotation();
}

// �`��
void Player::Draw(void)
{
	MV1DrawModel(modelId_);
	//DrawDebug();
}

// ���
void Player::Release(void)
{
	MV1DeleteModel(modelId_);
}

// �A�j���[�V�����X�V
void Player::UpdateAnimation(void)
{
	// �V�����A�j���[�V�����Đ����Ԃ��Z�b�g
	currentAnimTime_ += ANIM_SPEED;
	if (currentAnimTime_ > animTotalTime_)
	{
		currentAnimTime_ = 0.0f;
	}

	// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
	MV1SetAttachAnimTime(modelId_, animAttachNo_, currentAnimTime_);
}

// �ړ�����
void Player::ProcessMove(VECTOR angle)
{
	// ���͐���擾
	InputManager* ins = InputManager::GetInstance();

	// WASD�Ńv���C���[�ړ�
	moveVec_ = { 0.0f, 0.0f, 0.0f };

	// ���E�E�E��O�E���̃x�N�g�����쐬����
	VECTOR RIGHT_MOVE_VEC = { 1.0f, 0.0f, 0.0f };
	VECTOR LEFT_MOVE_VEC = { -1.0f, 0.0f, 0.0f };
	VECTOR FRONT_MOVE_VEC = { 0.0f, 0.0f, 1.0f };
	VECTOR BACK_MOVE_VEC = { 0.0f, 0.0f, -1.0f };

	// ���͕����Ɉړ��x�N�g����ǉ�����
	if (ins->IsNew(KEY_INPUT_W)) { moveVec_ = VAdd(moveVec_, FRONT_MOVE_VEC); }
	if (ins->IsNew(KEY_INPUT_A)) { moveVec_ = VAdd(moveVec_, LEFT_MOVE_VEC); }
	if (ins->IsNew(KEY_INPUT_S)) { moveVec_ = VAdd(moveVec_, BACK_MOVE_VEC); }
	if (ins->IsNew(KEY_INPUT_D)) { moveVec_ = VAdd(moveVec_, RIGHT_MOVE_VEC); }

	// �����Ă����
	if (IsMove(moveVec_))
	{
		// �J�����p�x���ݒ肷��
		MATRIX cameraMatY = MGetRotY(angle.y);
		moveVec_ = VTransform(moveVec_, cameraMatY);

		// ���W�X�V
		moveVec_ = VNorm(moveVec_);
		moveVec_ = VScale(moveVec_, WALK_SPEED);
		pos_ = VAdd(pos_, moveVec_);

		// �������p�x�ɕϊ�����(XZ���ʁ@Y��)
		moveVecRad_.y = atan2f(moveVec_.x, moveVec_.z);

		// �v�Z�p�x��ݒ�
		rot_.y = moveVecRad_.y;

		// ���W�ݒ�
		MV1SetPosition(modelId_, pos_);
	}
}

// �ړ�����
bool Player::IsMove(VECTOR _moveVec)
{
	// �ړ��x�N�g����X�EY�EZ ���ׂĈړ�����Ă��Ȃ����
	float absX = abs(_moveVec.x);
	float absY = abs(_moveVec.y);
	float absZ = abs(_moveVec.z);

	// ����Ȃ��������l��������ɏ��������0�Ɣ��肷��
	bool isNoMoveX = absX < FLT_EPSILON;
	bool isNoMoveY = absY < FLT_EPSILON;
	bool isNoMoveZ = absZ < FLT_EPSILON;

	// ���W�X�V������������
	if (isNoMoveX && isNoMoveY && isNoMoveZ)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// ��]
void Player::SetRotation(void)
{
	// ��]�s����g�p�����p�x�ݒ�

	// �P�ʍs���ݒ肷��
	MATRIX mat = MGetIdent();

	// ���f�����̂�Y����]�s����쐬����
	MATRIX mGetRotY = MGetRotY(rot_.y);

	// ���f���̕␳�pY����]�s����쐬����
	MATRIX mGetLocalRotY = MGetRotY(localRot_.y);

	// �s�������
	mat = MMult(mat, mGetRotY);
	mat = MMult(mat, mGetLocalRotY);

	// �s����g�p���ă��f���̊p�x��ݒ�
	MV1SetRotationMatrix(modelId_, mat);
}

// �f�o�b�O�`��
void Player::DrawDebug(void)
{
	// �f�o�b�O����
	DrawFormatString(0, 20, 0xffff00,
		"���f���̉�]�l(�ʓx�@):\n(X,Y,Z) = (%1.2lf, %1.2lf, %1.2lf)", rot_.x, rot_.y, rot_.z);

	DrawFormatString(0, 60, 0xffffff,
		"���f���̉�]�l(�x���@):\n(X,Y,Z) = (%1.0lf, %1.0lf, %1.0lf)",
		rot_.x * 180 / DX_PI_F,
		rot_.y * 180 / DX_PI_F,
		rot_.z * 180 / DX_PI_F);

	// ���f���̊p�x�̕␳���Ȃ������p�x
	const float INIT_MODEL_ROT = DX_PI_F / 2;
	float noOffsetRotY = rot_.y + INIT_MODEL_ROT;
	DrawFormatString(0, 120, 0x00ff00,
		"���f���̉�]�l(�ʓx�@)(���f���̕����␳�Ȃ�):\n(X,Y,Z) = (%1.2lf, %1.2lf, %1.2lf)",
		rot_.x,
		noOffsetRotY,
		rot_.z);

	DrawFormatString(0, 160, 0x00ff00,
		"���f���̉�]�l(�x���@)(���f���̕����␳�Ȃ�):\n(X,Y,Z) = (%1.2lf, %1.2lf, %1.2lf)",
		rot_.x * 180 / DX_PI_F,
		noOffsetRotY * 180 / DX_PI_F,
		rot_.z * 180 / DX_PI_F);

	DrawFormatString(0, 260, 0xffffff,
		"�ړ��x�N�g���̊p�x(�ʓx�@):(X,Y,Z) = (%1.2lf, %1.2lf, %1.2lf)",
		moveVecRad_.x, moveVecRad_.y, moveVecRad_.z);

	DrawFormatString(0, 300, 0xffffff,
		"�ړ��x�N�g���̊p�x(�x���@):(X,Y,Z) = (%1.0lf, %1.0lf, %1.0lf)",
		moveVecRad_.x * 180 / DX_PI_F,
		moveVecRad_.y * 180 / DX_PI_F,
		moveVecRad_.z * 180 / DX_PI_F);

	// ���f���̈ړ�����
	if (IsMove(moveVec_))
	{
		// �ړ��x�N�g���𐳋K��
		VECTOR debugMoveVec = VNorm(moveVec_);

		// ���̒�����ݒ�
		constexpr float DEBUG_MOVE_LINE_LENGTH = 100.0f;
		debugMoveVec = VScale(debugMoveVec, DEBUG_MOVE_LINE_LENGTH);

		// ���̏I�[���W��ݒ�
		VECTOR debugMoveVecLineEndPos = VAdd(pos_, debugMoveVec);

		// �ړ������ɐ���`��
		DrawLine3D(pos_, debugMoveVecLineEndPos, 0xffffff);
	}
}
