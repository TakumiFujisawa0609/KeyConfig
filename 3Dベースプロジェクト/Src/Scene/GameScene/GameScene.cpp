#include <DxLib.h>
#include "GameScene.h"
#include "../SceneManger/SceneManager.h"

#include "../../Camera/Camera.h"
#include "../../Input/InputManager.h"
#include "../../Object/Player.h"
#include "../../Object/Enemy.h"
#include "../../Fade/Fade.h"

// �R���X�g���N�^
GameScene::GameScene() :
	camera_(nullptr),
	player_(nullptr),
	enemy_(nullptr)
{
}

// �f�X�g���N�^
GameScene::~GameScene()
{
}

// ������
void GameScene::Init(void)
{
	// �v���C���[������
	player_->Init();

	// �G������
	enemy_->Init();

	// �G�Ƀv���C���[����ݒ�
	enemy_->SetPlayer(player_);

	// �J�����̐���
	camera_->Init();

	// �J�����̒Ǐ]�Ώۂ�ݒ�
	camera_->SetFollowPos(player_);
}

// �ǂݍ���
void GameScene::Load(void)
{
	// ��������
	player_ = new Player();
	enemy_ = new Enemy();
	camera_ = new Camera();

	// ���[�h����
	player_->Load();
	enemy_->Load();
}

// �ǂݍ��݌�̏�����
void GameScene::LoadEnd(void)
{
	// ������
	Init();

	// ���[�h�㏈��
	player_->LoadEnd();

	// ���[�h��Ƀt�F�[�h�C��
	Fade::GetInstance()->SetFade(Fade::STATE::FADE_IN);
}

// �X�V
void GameScene::Update(void)
{
	// �v���C���[�X�V
	player_->Update(camera_->GetAngles());

	// �G�̍X�V
	enemy_->Update();

	// �J�����X�V
	camera_->Update();
}

// �`��
void GameScene::Draw(void)
{
	// �v���C���[�`��
	player_->Draw();

	// �G�`��
	enemy_->Draw();
}

// ���
void GameScene::Release(void)
{
	// �v���C���[���
	player_->Release();
	delete player_;

	// �G�̉��
	enemy_->Release();
	delete enemy_;

	// �J�����̉��
	camera_->Release();
	delete camera_;
}
