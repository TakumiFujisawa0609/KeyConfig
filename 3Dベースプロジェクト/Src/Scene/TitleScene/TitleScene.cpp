#include <DxLib.h>
#include <fstream>
#include <sstream>

#include "TitleScene.h"
#include "../SceneManger/SceneManager.h"

#include "../../Application.h"
#include "../../Input/InputManager.h"
#include "../../DataManager/DataManager.h"
#include "../../Ui/UIManager.h"
#include "../../Fade/Fade.h"

#include "../../Common/Debug.h"

// �R���X�g���N�^
TitleScene::TitleScene(void) 
	: isFadeOut_(false)
{}

// �f�X�g���N�^
TitleScene::~TitleScene(void)
{}

// ������
void TitleScene::Init(void)
{
	isFadeOut_ = false;		// �t�F�[�h�A�E�g��
}

// �ǂݍ���
void TitleScene::Load(void)
{
	// TitleEdit �ŕۑ����� CSV ��ǂ�
	std::ifstream ifs("Data/csvList/TitleScene.csv");
	if (!ifs)
	{
		char buf[512];
		snprintf(buf, sizeof(buf), "[ERROR] CSV���ǂݍ��߂܂���ł���");
		LogMessage(buf);
		return;
	}

	std::string line;
	while (std::getline(ifs, line))
	{
		std::istringstream iss(line);
		std::string token;

		Item item;

		std::getline(iss, item.path, ',');
		std::getline(iss, token, ','); item.x = std::stoi(token);
		std::getline(iss, token, ','); item.y = std::stoi(token);
		std::getline(iss, token, ','); item.scale = std::stof(token);
		std::getline(iss, token, ','); item.angle = std::stof(token);

		item.handle = LoadGraph(item.path.c_str());
		if (item.handle != -1) 
		{
			GetGraphSize(item.handle, &item.width, &item.height);
			items_.push_back(item);
		}
	}
}

// �ǂݍ��݌�̏���
void TitleScene::LoadEnd(void)
{
	// ������
	Init();

	// ���[�h���I��������t�F�[�h�C��
	Fade::GetInstance()->SetFade(Fade::STATE::FADE_IN);
}

// �X�V
void TitleScene::Update(void)
{
	// �V�[���J��
	toNextScene();
}

// �`��
void TitleScene::Draw(void)
{
	// �ǂݍ��񂾂��̂�S�ĕ`��
	for (const auto& item : items_)
	{
		DrawRotaGraph2(
			item.x, item.y,
			item.width / 2, item.height / 2,
			item.scale,
			item.angle,
			item.handle,
			TRUE
		);
	}
}

// ���
void TitleScene::Release(void)
{
	// �ǂݍ��񂾑S�Ẳ摜�����
	for (auto& item : items_)
	{
		if (item.handle != -1) 
		{
			DeleteGraph(item.handle);
		}
	}

	// vector ����ɂ���
	items_.clear();
	items_.shrink_to_fit();
}

// ���̃V�[���֑J��
void TitleScene::toNextScene(void)
{
	// �V�[���J��
	if (!isFadeOut_ && InputManager::GetInstance()->IsNew(KEY_INPUT_SPACE))
	{
		// �t�F�[�h�A�E�g
		isFadeOut_ = true;
		Fade::GetInstance()->SetFade(Fade::STATE::FADE_OUT);
	}

	// �t�F�[�h�A�E�g���
	if (isFadeOut_ && Fade::GetInstance()->GetState() == Fade::STATE::NONE)
	{
		SceneManager::GetInstance()->ChangeScene(SceneManager::SCENE_ID::GAME);
		isFadeOut_ = false;
	}
}
