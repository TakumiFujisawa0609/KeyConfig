#pragma once

#include "UIBase.h"

// UI�X�v���C�g�N���X
class UISprite : public UIBase
{
public:
	UISprite();
	~UISprite();

public:
	void Init(float pos_x, float pos_y) override;
	void Load(const char* file_path) override;
	void Update() override;
	void Draw() override;
	void Delete() override;

private:
};
