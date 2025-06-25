#pragma once
#include "../SceneBase.h"
#include <vector>
#include <string>

// ベースを継承
class TitleScene : public SceneBase
{
public:

	TitleScene(void);				// コンストラクタ
	~TitleScene(void) override;		// デストラクタ

public:

	void Init(void)		override;	// 初期化
	void Load(void)		override;	// 読み込み
	void LoadEnd(void)	override;	// 読み込み後の処理
	void Update(void)	override;	// 更新
	void Draw(void)		override;	// 描画
	void Release(void)	override;	// 解放

	void toNextScene(void);

private:
	struct Item {
		std::string path;
		int handle;
		int x, y;
		int width, height;
		float scale;
		float angle;
	};

	std::vector<Item> items_;  // ← TitleEditで作ったものをここに持つ

	// フェード
	bool isFadeOut_;	// フェードアウト中のフラグ
};
