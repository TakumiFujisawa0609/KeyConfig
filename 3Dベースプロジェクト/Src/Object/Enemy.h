#pragma once
#include <DxLib.h>
class Player;

class Enemy
{
public:

	// アニメーションの再生速度
	static constexpr float ANIM_SPEED = 0.4f;

	// 移動量
	static constexpr float MOVE_SPEED = 5.0f;

	// 聴覚の広さ
	static constexpr float HEARING_RANGE = 400.0f;

	// 向いている方向
	static constexpr VECTOR INIT_DIR = { 0.0f, 0.0f, 1.0f };

	// 視野角
	static constexpr float VIEW_ANGLE = 30.0f;

	Enemy(void);
	~Enemy(void);

	void Init(void);
	void Load(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// アニメーション更新
	void UpdateAnimation(void);

	// 検知更新
	void UpdateSearch(void);

	// プレイヤー設定
	void SetPlayer(Player* player);

	// デバッグ描画
	void DrawDebug();

private:

	// モデルID
	int modelId_;

	// アニメーションのアタッチ番号
	int animAttachNo_;

	// アニメーションの総再生時間
	float animTotalTime_;

	// 再生中のアニメーション時間
	float currentAnimTime_;

	// 表示座標
	VECTOR pos_;

	// プレイヤークラス
	Player* player_;

	// 聴覚判定フラグ
	bool isNoticeHearing_;

	// 向いている方向
	VECTOR dirVec_;

	// モデル自体の方向
	VECTOR rot_;

	// 視覚範囲内に入ったフラグ
	bool isNoticeView_;
};
