#include "Camera.h"
#include "../Input/InputManager.h"
#include "../Object/ObjectBase.h"

Camera::Camera(void){}
Camera::~Camera(void){}

// 初期化
void Camera::Init(void)
{
	// カメラの初期位置
	pos_ = { 0.0f, 0.0f, 0.0f};

	// 追従対象からのローカル座標設定
	localPosFrom_ = { 0.0f, HEIGHT, -DIS_FOLLOW_TO_CAMERA };

	// カメラの初期角度
	angles_ = { 0.0f, 0.0f, 0.0f };

	// 追従対象からのローカル座標の回転値
	localRotFrom_ = { 0.0f, 0.0f, 0.0f };
}

// 更新
void Camera::Update(void)
{
	// 入力制御取得
	InputManager* ins = InputManager::GetInstance();

	// 回転スピードをデグリー→ラジアンに変換
	float rotPow = ROT_SPEED_DEG * DX_PI_F / 180.0f;

	// 左右キーでカメラの座標を変更する
	if (ins->IsNew(KEY_INPUT_RIGHT)) { localRotFrom_.y += rotPow; }
	if (ins->IsNew(KEY_INPUT_LEFT)) { localRotFrom_.y -= rotPow; }
	if (ins->IsNew(KEY_INPUT_DOWN))	{ localRotFrom_.x -= rotPow; }
	if (ins->IsNew(KEY_INPUT_UP))	{ localRotFrom_.x += rotPow; }

	//////////////////////

	// 単位行列を取得
	MATRIX mat = MGetIdent();

	// X軸を行列回転
	MATRIX matRotX = MGetRotX(localRotFrom_.x);
	MATRIX matRotY = MGetRotY(localRotFrom_.y);

	// 単位行列に合成をしていく
	mat = MMult(mat, matRotX);
	mat = MMult(mat, matRotY);

	// ローカル座標の回転
	VECTOR localPos = VTransform(localPosFrom_, mat);

	//////////////////////

	// クリップ距離を設定する
	SetCameraNearFar(5.0f, 30000.0f);

	// 注視点の移動
	VECTOR followPos = followObj_->GetPos();

	// 相対座標からワールド座標に直して、カメラ座標とする
	pos_ = VAdd(followPos, localPos);

	// カメラの設定(位置と注視点による制御)
	VECTOR UP_VECTOR = { 0.0f, 1.0f, 0.0f };
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		followPos,
		UP_VECTOR
	);
}

// 描画
void Camera::Draw(void)
{
}

// 解放
void Camera::Release(void)
{
}
