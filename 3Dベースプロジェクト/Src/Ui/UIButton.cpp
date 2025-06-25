#include "UIButton.h"
#include "../Collision/Collision.h"

UIButton::UIButton(){}
UIButton::~UIButton() {}

void UIButton::Init(float pos_x, float pos_y)
{
	pos_x_ = pos_x;
	pos_y_ = pos_y;
}

void UIButton::Load(const char* file_path)
{
	handle_ = LoadGraph(file_path);
}

void UIButton::Update()
{
	// マウスの座標を取得
	int mousePosX = 0;
	int mousePosY = 0;

	GetMousePoint(&mousePosX, &mousePosY);

	// デフォルト状態
	buttonState_ = BUTTON_STATE::DEFAULT;

	// ボタンの上にマウスポインタがあるか？
	if (CollCheckPointRect(pos_x_, pos_y_, size_w_, size_h_, mousePosX, mousePosY))
	{
		// ボタンの上にマウスポインタがある
		buttonState_ = BUTTON_STATE::HOVER;

		// クリックされているのか？
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
		{
			// クリックされている
			buttonState_ = BUTTON_STATE::DISABLED;
		}
	}
}

void UIButton::Draw()
{
	DrawGraphF(pos_x_, pos_y_, handle_, true);
}

void UIButton::Delete()
{
	DeleteGraph(handle_);
}

void UIButton::SetSize(float w, float h)
{
	size_w_ = w;
	size_h_ = h;
}

UIButton::BUTTON_STATE UIButton::GetButtonState(void)
{
	return buttonState_;
}
