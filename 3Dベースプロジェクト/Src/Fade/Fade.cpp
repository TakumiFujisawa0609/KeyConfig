#include <DxLib.h>
#include "../Application.h"
#include "Fade.h"

Fade* Fade::instance_ = nullptr;

// �R���X�g���N�^
Fade::Fade(void)
	: alpha_(0.0f)
	, color_(0x000000)
	, speed_(DEF_FADE_SPPED)
	, state_(STATE::NONE)
{
}

// �f�X�g���N�^
Fade::~Fade(void)
{
}

// ������
void Fade::Init(void)
{
	state_ = STATE::NONE;		// ���
	alpha_ = 0;					// �A���t�@�l
	color_ = 0x000000;			// �F
	speed_ = DEF_FADE_SPPED;	// �X�s�[�h
}

// �X�V
void Fade::Update(void)
{
	switch (state_)
	{
	case STATE::NONE:
		return;

	case STATE::FADE_OUT:
		alpha_ += speed_;
		if (alpha_ > 255)
		{
			// �t�F�[�h�I��
			Init();
		}
		break;

	case STATE::FADE_IN:
		alpha_ -= speed_;
		if (alpha_ < 0)
		{
			// �t�F�[�h�I��
			Init();
		}
		break;

	default:
		return;
	}
}

void Fade::Draw(void)
{
	switch (state_)
	{
	case STATE::NONE:
		return;
	case STATE::FADE_OUT:
	case STATE::FADE_IN:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
		DrawBox(
			0, 0,
			Application::SCREEN_SIZE_X,
			Application::SCREEN_SIZE_Y,
			color_, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	}
}

// �t�F�[�h�̊J�n
void Fade::SetFade(STATE state, unsigned int color, float speed)
{
	// ��Ԃ��X�V
	state_ = state;

	// �t�F�[�h�̏��
	switch (state)
	{
	case Fade::STATE::NONE:
		break;
	case Fade::STATE::FADE_OUT:
		alpha_ = 0;
		color_ = color;
		speed_ = speed;
		break;
	case Fade::STATE::FADE_IN:
		alpha_ = 255;
		color_ = color;
		speed_ = speed;
		break;
	default:
		break;
	}
}
