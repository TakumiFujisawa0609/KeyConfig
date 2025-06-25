#include <DxLib.h>
#include "GameScene.h"
#include "../SceneManger/SceneManager.h"

#include "../../Camera/Camera.h"
#include "../../Input/InputManager.h"
#include "../../Object/Player.h"
#include "../../Object/Enemy.h"
#include "../../Fade/Fade.h"

// コンストラクタ
GameScene::GameScene() :
	camera_(nullptr),
	player_(nullptr),
	enemy_(nullptr)
{
}

// デストラクタ
GameScene::~GameScene()
{
}

// 初期化
void GameScene::Init(void)
{
	// プレイヤー初期化
	player_->Init();

	// 敵初期化
	enemy_->Init();

	// 敵にプレイヤー情報を設定
	enemy_->SetPlayer(player_);

	// カメラの生成
	camera_->Init();

	// カメラの追従対象を設定
	camera_->SetFollowPos(player_);
}

// 読み込み
void GameScene::Load(void)
{
	// 生成処理
	player_ = new Player();
	enemy_ = new Enemy();
	camera_ = new Camera();

	// ロード処理
	player_->Load();
	enemy_->Load();
}

// 読み込み後の初期化
void GameScene::LoadEnd(void)
{
	// 初期化
	Init();

	// ロード後処理
	player_->LoadEnd();

	// ロード後にフェードイン
	Fade::GetInstance()->SetFade(Fade::STATE::FADE_IN);
}

// 更新
void GameScene::Update(void)
{
	// プレイヤー更新
	player_->Update(camera_->GetAngles());

	// 敵の更新
	enemy_->Update();

	// カメラ更新
	camera_->Update();
}

// 描画
void GameScene::Draw(void)
{
	// プレイヤー描画
	player_->Draw();

	// 敵描画
	enemy_->Draw();
}

// 解放
void GameScene::Release(void)
{
	// プレイヤー解放
	player_->Release();
	delete player_;

	// 敵の解放
	enemy_->Release();
	delete enemy_;

	// カメラの解放
	camera_->Release();
	delete camera_;
}
