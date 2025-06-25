#pragma once

#include "UISprite.h"
#include "UIButton.h"
#include <vector>

// UI管理用クラス
class UIManager
{
public:
	enum UI_TYPE {
		SPRITE,
		BUTTON
	};

public:
	static void CreateInstance() { if (!instance_) instance_ = new UIManager; }
	static UIManager* GetInstance() { return instance_; }
	static void DeleteInstance() { if (instance_) delete instance_; instance_ = nullptr; }

private:
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	UIManager();
	~UIManager();

	// コピー・ムーブ操作を禁止
	UIManager(const UIManager&) = delete;
	UIManager& operator=(const UIManager&) = delete;
	UIManager(UIManager&&) = delete;
	UIManager& operator=(UIManager&&) = delete;

	// 下記をコンパイルエラーさせるため 上記を追加
	// UIManager copy = *UIManager::GetInstance();
	// UIManager copied(*UIManager::GetInstance());
	// UIManager moved = std::move(*UIManager::GetInstance());

private:
	static UIManager* instance_;

private:
	std::vector<UIBase*> ui_vec_;

public:
	UIBase* CreateUI(UI_TYPE type);
	UIBase* CreateUI(UI_TYPE type, float pos_x, float pos_y, const char* file_path);
	void Update();
	void Draw();
	void DeleteAll();
};
