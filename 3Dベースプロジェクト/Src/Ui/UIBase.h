#pragma once
#include "DxLib.h"

// UI�̃x�[�X�ƂȂ�N���X
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
	float pos_x_;	// ���WX
	float pos_y_;	// ���WY
	float scale_;	// �g��l
	float angle_;	// ��]�l
	float size_w_;	// ����
	float size_h_;	// �c��
	int handle_;	// �摜�n���h��
};
