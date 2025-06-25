#include "UISprite.h"

UISprite::UISprite() {}
UISprite::~UISprite() {}

void UISprite::Init(float pos_x, float pos_y)
{
	pos_x_ = pos_x;
	pos_y_ = pos_y;
}

void UISprite::Load(const char* file_path)
{
	handle_ = LoadGraph(file_path);
}

void UISprite::Update()
{
	// çXêVèàóù
}

void UISprite::Draw()
{
	DrawRotaGraph(
		pos_x_,
		pos_y_,
		handle_,
		scale_,
		angle_,
		true
	);
}

void UISprite::Delete()
{
	DeleteGraph(handle_);
}
