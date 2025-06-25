#pragma once

class Fade
{
public:
	// フェードが進む速さ
	static constexpr float DEF_FADE_SPPED = 5.0f;

	// 状態
	enum class STATE
	{
		NONE,
		FADE_OUT,		// 徐々に暗転
		FADE_IN,		// 徐々に明転
	};

public:
	// シングルトン（生成・取得・削除）
	static void CreateInstance(void) { if (instance_ == nullptr) { instance_ = new Fade(); } };
	static Fade* GetInstance(void) { return instance_; };
	static void DeleteInstance(void) { if (instance_ != nullptr) { delete instance_; instance_ = nullptr; } }

public:

	void Init(void);	// 初期化
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 指定フェードを開始する
	void SetFade(STATE state, unsigned int color = 0x000000, float speed = DEF_FADE_SPPED);

	// 状態の取得
	STATE GetState(void) const { return state_; };

private:
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Fade(void);
	// デストラクタも同様
	~Fade(void);

private:
	// 静的インスタンス
	static Fade* instance_;

	// 状態
	STATE state_;

	// 透明度
	float alpha_;

	// 速度
	float speed_;

	// 色
	unsigned int color_;
};
