#pragma once
#include <DxLib.h>
class Player;

class Enemy
{
public:

	// �A�j���[�V�����̍Đ����x
	static constexpr float ANIM_SPEED = 0.4f;

	// �ړ���
	static constexpr float MOVE_SPEED = 5.0f;

	// ���o�̍L��
	static constexpr float HEARING_RANGE = 400.0f;

	// �����Ă������
	static constexpr VECTOR INIT_DIR = { 0.0f, 0.0f, 1.0f };

	// ����p
	static constexpr float VIEW_ANGLE = 30.0f;

	Enemy(void);
	~Enemy(void);

	void Init(void);
	void Load(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// �A�j���[�V�����X�V
	void UpdateAnimation(void);

	// ���m�X�V
	void UpdateSearch(void);

	// �v���C���[�ݒ�
	void SetPlayer(Player* player);

	// �f�o�b�O�`��
	void DrawDebug();

private:

	// ���f��ID
	int modelId_;

	// �A�j���[�V�����̃A�^�b�`�ԍ�
	int animAttachNo_;

	// �A�j���[�V�����̑��Đ�����
	float animTotalTime_;

	// �Đ����̃A�j���[�V��������
	float currentAnimTime_;

	// �\�����W
	VECTOR pos_;

	// �v���C���[�N���X
	Player* player_;

	// ���o����t���O
	bool isNoticeHearing_;

	// �����Ă������
	VECTOR dirVec_;

	// ���f�����̂̕���
	VECTOR rot_;

	// ���o�͈͓��ɓ������t���O
	bool isNoticeView_;
};
