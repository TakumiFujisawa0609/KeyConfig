#pragma once
#include <DxLib.h>
#include "ObjectBase.h"

class Player : public ObjectBase
{

public:

	// アニメーションの再生速度
	static constexpr float ANIM_SPEED = 0.4f;	

	// 移動スピード
	static constexpr float WALK_SPEED = 5.0f;
	static constexpr float DASH_SPEED = 10.0f;

	// 初期モデル補正角度
	static constexpr VECTOR INIT_MODEL_ROT_OFFSET = { 0.0f, DX_PI_F, 0.0f };

	Player(void);					// コンストラクタ
	~Player(void) override;			// デストラクタ

	void Init(void) override;		// 初期化
	void Load(void) override;		// 読み込み
	void LoadEnd(void) override;	// 読み込み後
	void Update()override {};		// 更新
	void Update(VECTOR angle);		// 更新
	void Draw(void) override;		// 描画
	void Release(void) override;	// 解放

private:
	// 移動ベクトル
	VECTOR moveVec_;

	// 移動ベクトルが生成する角度
	VECTOR moveVecRad_;

	// 調整用初期角度
	VECTOR localRot_;

	// カメラの方向取得用
	VECTOR cameraAngle_;

	// 回転設定
	void SetRotation(void);

	// 移動関連
	// 移動操作
	void ProcessMove(VECTOR angle);

	// ベクトルの移動判定
	bool IsMove(VECTOR _moveVec);

	// アニメーション関連
	// アニメーションのアタッチ番号
	int animAttachNo_;

	// アニメーションの総再生時間
	float animTotalTime_;

	// 再生中のアニメーション時間
	float currentAnimTime_;

	// アニメーション更新
	void UpdateAnimation(void);

	// デバッグ
	// デバッグ文字描画
	void DrawDebug(void);
};