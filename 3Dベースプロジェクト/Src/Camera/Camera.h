#pragma once
#include <DxLib.h>
class ObjectBase;

class Camera
{

public:

	// �J�����̍���
	static constexpr float HEIGHT = 200.0f;

	// �Ǐ]�Ώۂ���J�����܂ł̋���
	static constexpr float DIS_FOLLOW_TO_CAMERA = 500.0f;

	// ��]�X�s�[�h
	static constexpr float ROT_SPEED_DEG = 1.0f;

	Camera(void);
	~Camera(void);

	void Init(void);		// ������	
	void Update(void);		// �X�V
	void Draw(void);		// �`��
	void Release(void);		// ���

	// �Ǐ]������W�̐ݒ�
	void SetFollowPos(ObjectBase* obj) { followObj_ = obj; }

	// �J������]�p�x�̎擾
	VECTOR GetAngles(void) { return localRotFrom_; }

private:
	// �J�����̈ʒu
	VECTOR pos_;

	// �J�����̊p�x(rad)
	VECTOR angles_;

	// �Ǐ]������W
	ObjectBase* followObj_;

	// �Ǐ]�Ώۂ���̃��[�J�����W
	VECTOR localPosFrom_;

	// �Ǐ]�Ώۂ���̃��[�J�����W�̉�]�l
	VECTOR localRotFrom_;
};

