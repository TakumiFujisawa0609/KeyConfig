#include <DxLib.h>
#include "../Application.h"
#include "Fade.h"

Fade* Fade::instance_ = nullptr;

// コンストラクタ
Fade::Fade(void)
	: alpha_(0.0f)
	, color_(0x000000)
	, speed_(DEF_FADE_SPPED)
	, state_(STATE::NONE)
{
}

// デストラクタ
Fade::~Fade(void)
{
}

// 初期化
void Fade::Init(void)
{
	state_ = STATE::NONE;		// 状態
	alpha_ = 0;					// アルファ値
	color_ = 0x000000;			// 色
	speed_ = DEF_FADE_SPPED;	// スピード
}

// 更新
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
			// フェード終了
			Init();
		}
		break;

	case STATE::FADE_IN:
		alpha_ -= speed_;
		if (alpha_ < 0)
		{
			// フェード終了
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

// フェードの開始
void Fade::SetFade(STATE state, unsigned int color, float speed)
{
	// 状態を更新
	state_ = state;

	// フェードの状態
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
