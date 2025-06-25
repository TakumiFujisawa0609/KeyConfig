#include "Camera.h"
#include "../Input/InputManager.h"
#include "../Object/ObjectBase.h"

Camera::Camera(void){}
Camera::~Camera(void){}

// ������
void Camera::Init(void)
{
	// �J�����̏����ʒu
	pos_ = { 0.0f, 0.0f, 0.0f};

	// �Ǐ]�Ώۂ���̃��[�J�����W�ݒ�
	localPosFrom_ = { 0.0f, HEIGHT, -DIS_FOLLOW_TO_CAMERA };

	// �J�����̏����p�x
	angles_ = { 0.0f, 0.0f, 0.0f };

	// �Ǐ]�Ώۂ���̃��[�J�����W�̉�]�l
	localRotFrom_ = { 0.0f, 0.0f, 0.0f };
}

// �X�V
void Camera::Update(void)
{
	// ���͐���擾
	InputManager* ins = InputManager::GetInstance();

	// ��]�X�s�[�h���f�O���[�����W�A���ɕϊ�
	float rotPow = ROT_SPEED_DEG * DX_PI_F / 180.0f;

	// ���E�L�[�ŃJ�����̍��W��ύX����
	if (ins->IsNew(KEY_INPUT_RIGHT)) { localRotFrom_.y += rotPow; }
	if (ins->IsNew(KEY_INPUT_LEFT)) { localRotFrom_.y -= rotPow; }
	if (ins->IsNew(KEY_INPUT_DOWN))	{ localRotFrom_.x -= rotPow; }
	if (ins->IsNew(KEY_INPUT_UP))	{ localRotFrom_.x += rotPow; }

	//////////////////////

	// �P�ʍs����擾
	MATRIX mat = MGetIdent();

	// X�����s���]
	MATRIX matRotX = MGetRotX(localRotFrom_.x);
	MATRIX matRotY = MGetRotY(localRotFrom_.y);

	// �P�ʍs��ɍ��������Ă���
	mat = MMult(mat, matRotX);
	mat = MMult(mat, matRotY);

	// ���[�J�����W�̉�]
	VECTOR localPos = VTransform(localPosFrom_, mat);

	//////////////////////

	// �N���b�v������ݒ肷��
	SetCameraNearFar(5.0f, 30000.0f);

	// �����_�̈ړ�
	VECTOR followPos = followObj_->GetPos();

	// ���΍��W���烏�[���h���W�ɒ����āA�J�������W�Ƃ���
	pos_ = VAdd(followPos, localPos);

	// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	VECTOR UP_VECTOR = { 0.0f, 1.0f, 0.0f };
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		followPos,
		UP_VECTOR
	);
}

// �`��
void Camera::Draw(void)
{
}

// ���
void Camera::Release(void)
{
}
