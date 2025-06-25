#include <DxLib.h>
#include "SceneManager.h"
#include "../../Scene/TitleScene/TitleScene.h"
#include "../../Scene/GameScene/GameScene.h"
#include "../../DataManager/DataManager.h"

#include "../../Ui/UIManager.h"
#include "../../Loading/Loading.h"
#include "../../Fade/Fade.h"

SceneManager* SceneManager::instance_ = nullptr;

// �R���X�g���N�^
SceneManager::SceneManager(void)
{
	sceneId_ = SCENE_ID::NONE;
	scene_ = nullptr;
	isGameEnd_ = false;
}

// �f�X�g���N�^
SceneManager::~SceneManager(void)
{
}

// ������
void SceneManager::Init(void)
{
	// �f�[�^�}�l�[�W���[����
	DataManager::CreateInstance();

	// ���[�h��ʐ���
	Loading::GetInstance()->CreateInstance();
	Loading::GetInstance()->Init();
	Loading::GetInstance()->Load();

	// �t�F�[�h�̐���
	Fade::GetInstance()->CreateInstance();
	Fade::GetInstance()->Init();

	// UI�Ǘ��̐���
	UIManager::GetInstance()->CreateInstance();

	// 3D���̏�����
	Init3D();

	// �ŏ��̓^�C�g����ʂ���
	ChangeScene(SCENE_ID::TITLE);
}

// 3D���̏�����
void SceneManager::Init3D(void)
{
	// �w�i�F�ݒ�
	SetBackgroundColor(0, 0, 0);

	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	// �o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);

	// ���C�g�̐ݒ�
	SetUseLighting(true);

	// ���ʂ���΂߉��Ɍ����������C�g
	ChangeLightTypeDir({ 0.00f, -1.00f, 1.00f });
}

// �X�V
void SceneManager::Update(void)
{
	// �V�[�����Ȃ���ΏI��
	if (scene_ == nullptr) { return; }

	// �t�F�[�h�̍X�V
	Fade::GetInstance()->Update();

	// ���[�h��
	if (Loading::GetInstance()->IsLoading())
	{
		// ���[�h�X�V
		Loading::GetInstance()->Update();

		// ��L�̍X�V�֐��Ń��[�h���I��
		if (!Loading::GetInstance()->IsLoading())
		{
			// ���[�h��̏�����
			scene_->LoadEnd();
		}
	}
	// �ʏ�̍X�V
	else
	{
		// ���݂̃V�[���̍X�V
		scene_->Update();

		// UI�̍X�V
		UIManager::GetInstance()->Update();
	}	
}

// �`��
void SceneManager::Draw(void)
{	
	// ���[�h���Ȃ烍�[�h��ʂ�`��
	if (Loading::GetInstance()->IsLoading())
	{
		// ���[�h�`��
		Loading::GetInstance()->Draw();
	}
	// �ʏ�̍X�V
	else
	{
		// ���݂̃V�[���̕`��
		scene_->Draw();

		// UI�̕`��
		UIManager::GetInstance()->Draw();
	}

	// �t�F�[�h�̕`��
	Fade::GetInstance()->Draw();
}

// ���
void SceneManager::Release(void)
{
	// ���݂̃V�[���̉���E�폜
	scene_->Release();
	delete scene_;

	// �t�F�[�h�̍폜
	Fade::GetInstance()->DeleteInstance();

	// ���[�h��ʂ̍폜
	Loading::GetInstance()->Release();
	Loading::GetInstance()->DeleteInstance();

	// UI�̍폜
	UIManager::GetInstance()->DeleteAll();
	UIManager::GetInstance()->DeleteInstance();

	// �f�[�^�}�l�[�W���[�̍폜
	DataManager::GetInstance()->ClearAll();
	DataManager::DeleteInstance();
}

// ��ԑJ�ڊ֐�
void SceneManager::ChangeScene(SCENE_ID nextId)
{
	// �V�[����ύX����
	sceneId_ = nextId;

	// ���݂̃V�[�������
	if (scene_ != nullptr)
	{
		scene_->Release();
		delete scene_;
	}

	// �e�V�[���ɐ؂�ւ�
	switch (sceneId_)
	{
	case SceneManager::SCENE_ID::NONE:
		break;
	case SceneManager::SCENE_ID::TITLE:
		scene_ = new TitleScene();
		break;
	case SceneManager::SCENE_ID::GAME:
		scene_ = new GameScene();
		break;
	case SceneManager::SCENE_ID::RESULT:
		break;
	default:
		break;
	}

	// �ǂݍ���(�񓯊�)
	Loading::GetInstance()->StartAsyncLoad();
	scene_->Load();
	Loading::GetInstance()->EndAsyncLoad();
}
