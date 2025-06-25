#pragma once
#include <DxLib.h>
class ObjectBase;

class Camera
{

public:

	// カメラの高さ
	static constexpr float HEIGHT = 200.0f;

	// 追従対象からカメラまでの距離
	static constexpr float DIS_FOLLOW_TO_CAMERA = 500.0f;

	// 回転スピード
	static constexpr float ROT_SPEED_DEG = 1.0f;

	Camera(void);
	~Camera(void);

	void Init(void);		// 初期化	
	void Update(void);		// 更新
	void Draw(void);		// 描画
	void Release(void);		// 解放

	// 追従する座標の設定
	void SetFollowPos(ObjectBase* obj) { followObj_ = obj; }

	// カメラ回転角度の取得
	VECTOR GetAngles(void) { return localRotFrom_; }

private:
	// カメラの位置
	VECTOR pos_;

	// カメラの角度(rad)
	VECTOR angles_;

	// 追従する座標
	ObjectBase* followObj_;

	// 追従対象からのローカル座標
	VECTOR localPosFrom_;

	// 追従対象からのローカル座標の回転値
	VECTOR localRotFrom_;
};

