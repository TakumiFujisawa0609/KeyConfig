#pragma once

#include "UIBase.h"

// UIボタンクラス
class UIButton : public UIBase
{
public:
	// ボタンの状態
	enum class BUTTON_STATE
	{
		DEFAULT,	// 通常
		HOVER,		// クリック可能
		DISABLED,	// クリック不可

		STATE_MAX,	// 状態の最大数
	};
public:
	UIButton();
	~UIButton();

public:
	void Init(float pos_x, float pos_y) override;
	void Load(const char* file_path) override;
	void Update() override;
	void Draw() override;
	void Delete() override;

	void SetSize(float w, float h);	// サイズを設定

	// ボタンの状態を返す
	BUTTON_STATE GetButtonState(void);

private:
	BUTTON_STATE buttonState_;	// ボタンの状態
};
