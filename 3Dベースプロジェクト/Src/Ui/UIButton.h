#pragma once

#include "UIBase.h"

// UI�{�^���N���X
class UIButton : public UIBase
{
public:
	// �{�^���̏��
	enum class BUTTON_STATE
	{
		DEFAULT,	// �ʏ�
		HOVER,		// �N���b�N�\
		DISABLED,	// �N���b�N�s��

		STATE_MAX,	// ��Ԃ̍ő吔
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

	void SetSize(float w, float h);	// �T�C�Y��ݒ�

	// �{�^���̏�Ԃ�Ԃ�
	BUTTON_STATE GetButtonState(void);

private:
	BUTTON_STATE buttonState_;	// �{�^���̏��
};
