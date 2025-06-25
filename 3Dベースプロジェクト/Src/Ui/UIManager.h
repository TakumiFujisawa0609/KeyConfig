#pragma once

#include "UISprite.h"
#include "UIButton.h"
#include <vector>

// UI�Ǘ��p�N���X
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
	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	UIManager();
	~UIManager();

	// �R�s�[�E���[�u������֎~
	UIManager(const UIManager&) = delete;
	UIManager& operator=(const UIManager&) = delete;
	UIManager(UIManager&&) = delete;
	UIManager& operator=(UIManager&&) = delete;

	// ���L���R���p�C���G���[�����邽�� ��L��ǉ�
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
