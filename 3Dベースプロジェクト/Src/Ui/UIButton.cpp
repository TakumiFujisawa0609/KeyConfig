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
	// �}�E�X�̍��W���擾
	int mousePosX = 0;
	int mousePosY = 0;

	GetMousePoint(&mousePosX, &mousePosY);

	// �f�t�H���g���
	buttonState_ = BUTTON_STATE::DEFAULT;

	// �{�^���̏�Ƀ}�E�X�|�C���^�����邩�H
	if (CollCheckPointRect(pos_x_, pos_y_, size_w_, size_h_, mousePosX, mousePosY))
	{
		// �{�^���̏�Ƀ}�E�X�|�C���^������
		buttonState_ = BUTTON_STATE::HOVER;

		// �N���b�N����Ă���̂��H
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
		{
			// �N���b�N����Ă���
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
