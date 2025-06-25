#pragma once
#include "DxLib.h"

// UIのベースとなるクラス
class UIBase
{
public:
	UIBase();
	virtual ~UIBase();

public:
	virtual void Init(float pos_x, float pos_y) = 0;
	virtual void Load(const char* file_path) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Delete() = 0;

	float GetPosX() const { return pos_x_; }
	float GetPosY() const { return pos_y_; }

protected:
	float pos_x_;	// 座標X
	float pos_y_;	// 座標Y
	float scale_;	// 拡大値
	float angle_;	// 回転値
	float size_w_;	// 横幅
	float size_h_;	// 縦幅
	int handle_;	// 画像ハンドル
};
