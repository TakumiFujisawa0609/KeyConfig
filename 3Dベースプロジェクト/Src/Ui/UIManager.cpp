#include "UIManager.h"

UIManager* UIManager::instance_ = nullptr;

UIManager::UIManager() {}
UIManager::~UIManager() {}

UIBase* UIManager::CreateUI(UI_TYPE type)
{
	UIBase* ui = nullptr;

	switch (type)
	{
	case UIManager::SPRITE:	ui = new UISprite; break;
	case UIManager::BUTTON: ui = new UIButton; break;
	default: break;
	}

	ui_vec_.push_back(ui);
	return ui;
}

UIBase* UIManager::CreateUI(UI_TYPE type, float pos_x, float pos_y, const char* file_path)
{
	UIBase* ui = CreateUI(type);
	ui->Init(pos_x, pos_y);
	ui->Load(file_path);

	return ui;
}

void UIManager::Update()
{
	for (auto itr = ui_vec_.begin(); itr != ui_vec_.end(); itr++)
	{
		(*itr)->Update();
	}
}

void UIManager::Draw()
{
	for (auto itr = ui_vec_.begin(); itr != ui_vec_.end(); itr++)
	{
		(*itr)->Draw();
	}
}

// vector����UI��S�č폜
void UIManager::DeleteAll()
{
	// UI���ɍ폜����
	for (auto itr = ui_vec_.begin(); itr != ui_vec_.end(); itr++)
	{
		UIBase* ui = (*itr);
		ui->Delete();
		delete ui;
	}

	// �z����폜����
	ui_vec_.clear();
	ui_vec_.shrink_to_fit();
}
