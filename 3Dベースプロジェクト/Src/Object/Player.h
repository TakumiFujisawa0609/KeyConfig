#pragma once
#include <DxLib.h>
#include "ObjectBase.h"

class Player : public ObjectBase
{

public:

	// �A�j���[�V�����̍Đ����x
	static constexpr float ANIM_SPEED = 0.4f;	

	// �ړ��X�s�[�h
	static constexpr float WALK_SPEED = 5.0f;
	static constexpr float DASH_SPEED = 10.0f;

	// �������f���␳�p�x
	static constexpr VECTOR INIT_MODEL_ROT_OFFSET = { 0.0f, DX_PI_F, 0.0f };

	Player(void);					// �R���X�g���N�^
	~Player(void) override;			// �f�X�g���N�^

	void Init(void) override;		// ������
	void Load(void) override;		// �ǂݍ���
	void LoadEnd(void) override;	// �ǂݍ��݌�
	void Update()override {};		// �X�V
	void Update(VECTOR angle);		// �X�V
	void Draw(void) override;		// �`��
	void Release(void) override;	// ���

private:
	// �ړ��x�N�g��
	VECTOR moveVec_;

	// �ړ��x�N�g������������p�x
	VECTOR moveVecRad_;

	// �����p�����p�x
	VECTOR localRot_;

	// �J�����̕����擾�p
	VECTOR cameraAngle_;

	// ��]�ݒ�
	void SetRotation(void);

	// �ړ��֘A
	// �ړ�����
	void ProcessMove(VECTOR angle);

	// �x�N�g���̈ړ�����
	bool IsMove(VECTOR _moveVec);

	// �A�j���[�V�����֘A
	// �A�j���[�V�����̃A�^�b�`�ԍ�
	int animAttachNo_;

	// �A�j���[�V�����̑��Đ�����
	float animTotalTime_;

	// �Đ����̃A�j���[�V��������
	float currentAnimTime_;

	// �A�j���[�V�����X�V
	void UpdateAnimation(void);

	// �f�o�b�O
	// �f�o�b�O�����`��
	void DrawDebug(void);
};