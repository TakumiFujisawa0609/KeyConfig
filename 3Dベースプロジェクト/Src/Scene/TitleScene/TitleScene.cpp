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

// コンストラクタ
TitleScene::TitleScene(void) 
	: isFadeOut_(false)
{}

// デストラクタ
TitleScene::~TitleScene(void)
{}

// 初期化
void TitleScene::Init(void)
{
	isFadeOut_ = false;		// フェードアウト中
}

// 読み込み
void TitleScene::Load(void)
{
	// TitleEdit で保存した CSV を読む
	std::ifstream ifs("Data/csvList/TitleScene.csv");
	if (!ifs)
	{
		char buf[512];
		snprintf(buf, sizeof(buf), "[ERROR] CSVが読み込めませんでした");
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

// 読み込み後の処理
void TitleScene::LoadEnd(void)
{
	// 初期化
	Init();

	// ロードが終了したらフェードイン
	Fade::GetInstance()->SetFade(Fade::STATE::FADE_IN);
}

// 更新
void TitleScene::Update(void)
{
	// シーン遷移
	toNextScene();
}

// 描画
void TitleScene::Draw(void)
{
	// 読み込んだものを全て描画
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

// 解放
void TitleScene::Release(void)
{
	// 読み込んだ全ての画像を解放
	for (auto& item : items_)
	{
		if (item.handle != -1) 
		{
			DeleteGraph(item.handle);
		}
	}

	// vector を空にする
	items_.clear();
	items_.shrink_to_fit();
}

// 次のシーンへ遷移
void TitleScene::toNextScene(void)
{
	// シーン遷移
	if (!isFadeOut_ && InputManager::GetInstance()->IsNew(KEY_INPUT_SPACE))
	{
		// フェードアウト
		isFadeOut_ = true;
		Fade::GetInstance()->SetFade(Fade::STATE::FADE_OUT);
	}

	// フェードアウト後に
	if (isFadeOut_ && Fade::GetInstance()->GetState() == Fade::STATE::NONE)
	{
		SceneManager::GetInstance()->ChangeScene(SceneManager::SCENE_ID::GAME);
		isFadeOut_ = false;
	}
}
