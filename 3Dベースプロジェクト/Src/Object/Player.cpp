#include "Player.h"
#include "../Utility/AsoUtility.h"
#include "../Input/InputManager.h"
#include "../Camera/Camera.h"

// コンストラクタ
Player::Player(void){}
// デストラクタ
Player::~Player(void){}

// 初期化
void Player::Init(void)
{
	// モデルの初期位置設定
	pos_ = { 0.0f, 0.0f, 0.0f };
	MV1SetPosition(modelId_, pos_);

	// 移動ベクトルが作成する角度初期化
	moveVecRad_ = { 0.0f, 0.0f, 0.0f };

	// モデルの角度設定
	rot_ = { 0.0f, 0.0f, 0.0f };

	// モデルの角度
	localRot_ = INIT_MODEL_ROT_OFFSET;

	// カメラアングル
	cameraAngle_ = { 0.0f, 0.0f, 0.0f };
}

// 読み込み
void Player::Load(void)
{
	// モデルの読み込み
	modelId_ = MV1LoadModel("Data/Model/Human.mv1");
}

// 読み込み後
void Player::LoadEnd(void)
{
	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(modelId_, 1);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(modelId_, animAttachNo_);
	currentAnimTime_ = 0.0f;

	// モデルに指定時間のアニメーションを設定する
	MV1SetAttachAnimTime(modelId_, animAttachNo_, currentAnimTime_);
}

// 更新
void Player::Update(VECTOR angle)
{
	// カメラアングル取得
	cameraAngle_ = angle;

	// アニメーション処理
	UpdateAnimation();

	// 移動操作
	ProcessMove(cameraAngle_);

	// 回転値設定
	SetRotation();
}

// 描画
void Player::Draw(void)
{
	MV1DrawModel(modelId_);
	//DrawDebug();
}

// 解放
void Player::Release(void)
{
	MV1DeleteModel(modelId_);
}

// アニメーション更新
void Player::UpdateAnimation(void)
{
	// 新しいアニメーション再生時間をセット
	currentAnimTime_ += ANIM_SPEED;
	if (currentAnimTime_ > animTotalTime_)
	{
		currentAnimTime_ = 0.0f;
	}

	// モデルに指定時間のアニメーションを設定する
	MV1SetAttachAnimTime(modelId_, animAttachNo_, currentAnimTime_);
}

// 移動操作
void Player::ProcessMove(VECTOR angle)
{
	// 入力制御取得
	InputManager* ins = InputManager::GetInstance();

	// WASDでプレイヤー移動
	moveVec_ = { 0.0f, 0.0f, 0.0f };

	// 左・右・手前・奥のベクトルを作成する
	VECTOR RIGHT_MOVE_VEC = { 1.0f, 0.0f, 0.0f };
	VECTOR LEFT_MOVE_VEC = { -1.0f, 0.0f, 0.0f };
	VECTOR FRONT_MOVE_VEC = { 0.0f, 0.0f, 1.0f };
	VECTOR BACK_MOVE_VEC = { 0.0f, 0.0f, -1.0f };

	// 入力方向に移動ベクトルを追加する
	if (ins->IsNew(KEY_INPUT_W)) { moveVec_ = VAdd(moveVec_, FRONT_MOVE_VEC); }
	if (ins->IsNew(KEY_INPUT_A)) { moveVec_ = VAdd(moveVec_, LEFT_MOVE_VEC); }
	if (ins->IsNew(KEY_INPUT_S)) { moveVec_ = VAdd(moveVec_, BACK_MOVE_VEC); }
	if (ins->IsNew(KEY_INPUT_D)) { moveVec_ = VAdd(moveVec_, RIGHT_MOVE_VEC); }

	// 動いていれば
	if (IsMove(moveVec_))
	{
		// カメラ角度分設定する
		MATRIX cameraMatY = MGetRotY(angle.y);
		moveVec_ = VTransform(moveVec_, cameraMatY);

		// 座標更新
		moveVec_ = VNorm(moveVec_);
		moveVec_ = VScale(moveVec_, WALK_SPEED);
		pos_ = VAdd(pos_, moveVec_);

		// 方向を角度に変換する(XZ平面　Y軸)
		moveVecRad_.y = atan2f(moveVec_.x, moveVec_.z);

		// 計算角度を設定
		rot_.y = moveVecRad_.y;

		// 座標設定
		MV1SetPosition(modelId_, pos_);
	}
}

// 移動判定
bool Player::IsMove(VECTOR _moveVec)
{
	// 移動ベクトルがX・Y・Z すべて移動されていなければ
	float absX = abs(_moveVec.x);
	float absY = abs(_moveVec.y);
	float absZ = abs(_moveVec.z);

	// 限りなく小さい値よりもさらに小さければ0と判定する
	bool isNoMoveX = absX < FLT_EPSILON;
	bool isNoMoveY = absY < FLT_EPSILON;
	bool isNoMoveZ = absZ < FLT_EPSILON;

	// 座標更新をせず抜ける
	if (isNoMoveX && isNoMoveY && isNoMoveZ)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// 回転
void Player::SetRotation(void)
{
	// 回転行列を使用した角度設定

	// 単位行列を設定する
	MATRIX mat = MGetIdent();

	// モデル自体のY軸回転行列を作成する
	MATRIX mGetRotY = MGetRotY(rot_.y);

	// モデルの補正用Y軸回転行列を作成する
	MATRIX mGetLocalRotY = MGetRotY(localRot_.y);

	// 行列を合成
	mat = MMult(mat, mGetRotY);
	mat = MMult(mat, mGetLocalRotY);

	// 行列を使用してモデルの角度を設定
	MV1SetRotationMatrix(modelId_, mat);
}

// デバッグ描画
void Player::DrawDebug(void)
{
	// デバッグ文字
	DrawFormatString(0, 20, 0xffff00,
		"モデルの回転値(弧度法):\n(X,Y,Z) = (%1.2lf, %1.2lf, %1.2lf)", rot_.x, rot_.y, rot_.z);

	DrawFormatString(0, 60, 0xffffff,
		"モデルの回転値(度数法):\n(X,Y,Z) = (%1.0lf, %1.0lf, %1.0lf)",
		rot_.x * 180 / DX_PI_F,
		rot_.y * 180 / DX_PI_F,
		rot_.z * 180 / DX_PI_F);

	// モデルの角度の補正をなくした角度
	const float INIT_MODEL_ROT = DX_PI_F / 2;
	float noOffsetRotY = rot_.y + INIT_MODEL_ROT;
	DrawFormatString(0, 120, 0x00ff00,
		"モデルの回転値(弧度法)(モデルの方向補正なし):\n(X,Y,Z) = (%1.2lf, %1.2lf, %1.2lf)",
		rot_.x,
		noOffsetRotY,
		rot_.z);

	DrawFormatString(0, 160, 0x00ff00,
		"モデルの回転値(度数法)(モデルの方向補正なし):\n(X,Y,Z) = (%1.2lf, %1.2lf, %1.2lf)",
		rot_.x * 180 / DX_PI_F,
		noOffsetRotY * 180 / DX_PI_F,
		rot_.z * 180 / DX_PI_F);

	DrawFormatString(0, 260, 0xffffff,
		"移動ベクトルの角度(弧度法):(X,Y,Z) = (%1.2lf, %1.2lf, %1.2lf)",
		moveVecRad_.x, moveVecRad_.y, moveVecRad_.z);

	DrawFormatString(0, 300, 0xffffff,
		"移動ベクトルの角度(度数法):(X,Y,Z) = (%1.0lf, %1.0lf, %1.0lf)",
		moveVecRad_.x * 180 / DX_PI_F,
		moveVecRad_.y * 180 / DX_PI_F,
		moveVecRad_.z * 180 / DX_PI_F);

	// モデルの移動方向
	if (IsMove(moveVec_))
	{
		// 移動ベクトルを正規化
		VECTOR debugMoveVec = VNorm(moveVec_);

		// 線の長さを設定
		constexpr float DEBUG_MOVE_LINE_LENGTH = 100.0f;
		debugMoveVec = VScale(debugMoveVec, DEBUG_MOVE_LINE_LENGTH);

		// 線の終端座標を設定
		VECTOR debugMoveVecLineEndPos = VAdd(pos_, debugMoveVec);

		// 移動方向に線を描画
		DrawLine3D(pos_, debugMoveVecLineEndPos, 0xffffff);
	}
}
