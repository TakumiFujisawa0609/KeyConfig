#pragma once

class Fade
{
public:
	// �t�F�[�h���i�ޑ���
	static constexpr float DEF_FADE_SPPED = 5.0f;

	// ���
	enum class STATE
	{
		NONE,
		FADE_OUT,		// ���X�ɈÓ]
		FADE_IN,		// ���X�ɖ��]
	};

public:
	// �V���O���g���i�����E�擾�E�폜�j
	static void CreateInstance(void) { if (instance_ == nullptr) { instance_ = new Fade(); } };
	static Fade* GetInstance(void) { return instance_; };
	static void DeleteInstance(void) { if (instance_ != nullptr) { delete instance_; instance_ = nullptr; } }

public:

	void Init(void);	// ������
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �w��t�F�[�h���J�n����
	void SetFade(STATE state, unsigned int color = 0x000000, float speed = DEF_FADE_SPPED);

	// ��Ԃ̎擾
	STATE GetState(void) const { return state_; };

private:
	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	Fade(void);
	// �f�X�g���N�^�����l
	~Fade(void);

private:
	// �ÓI�C���X�^���X
	static Fade* instance_;

	// ���
	STATE state_;

	// �����x
	float alpha_;

	// ���x
	float speed_;

	// �F
	unsigned int color_;
};
