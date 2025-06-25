#include "Enemy.h"
#include "cmath"
#include "../Object/Player.h"

Enemy::Enemy(void){}
Enemy::~Enemy(void){}

void Enemy::Init(void)
{
	// モデルの初期位置設定
	pos_ = { 300.0f, 0.0f, 300.0f };
	MV1SetPosition(modelId_, pos_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(modelId_, 1);

	// モデルに指定時間のアニメーションを設定する
	animTotalTime_ = MV1SetAttachAnimTime(modelId_, animAttachNo_, currentAnimTime_);
	currentAnimTime_ = 0.0f;

	// 向いている方向初期化(手前向き)
	dirVec_ = INIT_DIR;

	// 向いている方向からモデルの方向を設定
	float playerRotY = atan2f(dirVec_.x, dirVec_.z);
	rot_ = { 0.0f, playerRotY, 0.0f };
	MV1SetRotationXYZ(modelId_, rot_);
}

void Enemy::Load(void)
{	
	// モデルの読み込み
	modelId_ = MV1LoadModel("Data/Model/Human.mv1");
}

void Enemy::Update(void)
{
	// アニメーション処理
	UpdateAnimation();

	// 検知更新
	UpdateSearch();
}

void Enemy::Draw(void)
{
	// 聴覚範囲内フラグがたっていたら
	if (isNoticeHearing_)
	{
		// 聴覚範囲内：デュヒューズカラーを黄色にする
		COLOR_F color = GetColorF(1.0f, 1.0f, 0.0f, 1.0f);
		MV1SetMaterialDifColor(modelId_, 0, color);
	}
	// 聴覚範囲内フラグが折れていたら
	else
	{
		// 視野範囲外：ディヒューズカラーを灰色にする
		COLOR_F color = GetColorF(0.5f, 0.5f, 0.5f, 1.0f);
		MV1SetMaterialDifColor(modelId_, 0, color);
	}

	// 視覚範囲に入っていたら
	if (isNoticeView_)
	{
		// 視野範囲内：デュヒューズカラーを赤色にする
		COLOR_F color = GetColorF(1.0f, 0.0f, 0.0f, 1.0f);
		MV1SetMaterialDifColor(modelId_, 0, color);
	}

	MV1DrawModel(modelId_);

	// DrawDebug();
}

void Enemy::Release(void)
{
}

void Enemy::UpdateAnimation(void)
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

void Enemy::UpdateSearch(void)
{
	// プレイヤーの座標を取得
	VECTOR pPos = player_->GetPos();

	// エネミーからプレイヤーまでのベクトル
	VECTOR vecPlayerFromEnemy = VSub(pPos, pos_);

	// プレイヤーと敵の距離(の2乗)を計算する(ピタゴラスの定理より)
	float distancePowX = std::pow(vecPlayerFromEnemy.x, 2.0f);
	float distancePowZ = std::pow(vecPlayerFromEnemy.z, 2.0f);
	float distancePow = distancePowX + distancePowZ;

	// 2乗を解いて距離を算出する
	float distance = std::sqrt(distancePow);

	// 距離が聴覚範囲内だったら
	if (distance <= HEARING_RANGE)
	{
		// 聴覚範囲内であるというフラグを立てる
		isNoticeHearing_ = true;
	}
	// 範囲外であれば
	else
	{
		// 範囲外であるというフラグを折る
		isNoticeHearing_ = false;
	}

	// 視野角判定
	// 今回は聴覚が聞こえる範囲でのみ視野角の判定をとる
	if (isNoticeHearing_)
	{
		// 敵→プレイヤーのベクトルを作成
		float playerFromEnemyX = pPos.x - pos_.x;	// Xの距離
		float playerFromEnemyZ = pPos.z - pos_.z;	// Zの距離
		VECTOR vecPlayerFromEnemy = { playerFromEnemyX, 0.0f, playerFromEnemyZ };

		// 自分から見たプレイヤーの角度を求める
		float rad = atan2(vecPlayerFromEnemy.x, vecPlayerFromEnemy.z);

		// 角度の差を求める
		float diffViewRad = rad - rot_.y;

		// ラジアンをデグリーに変換(30度といったデグリー値の方が感覚としてわかりやすいため)
		float diffViewDeg = diffViewRad * 180.0f / DX_PI_F;

		// 差の大きさを出す
		diffViewDeg = abs(diffViewDeg);

		// 角度差が視野範囲ないだったら視野内判定を行う
		if (diffViewDeg < VIEW_ANGLE)
		{
			isNoticeView_ = true;
		}
		else
		{
			isNoticeView_ = false;
		}
	}
	// 聴覚範囲に入っていなければ視界内に入っていないとする
	else
	{
		isNoticeView_ = false;
	}
}

void Enemy::SetPlayer(Player* player)
{
	player_ = player;
}

void Enemy::DrawDebug()
{
	DrawSphere3D(pos_, HEARING_RANGE, 32, GetColor(255, 255, 255), GetColor(255, 255, 255), false);

	// 視覚判定
	// 向いている方向に線を出す
	dirVec_ = VNorm(dirVec_);
	VECTOR debugDirVec_ = VScale(dirVec_, HEARING_RANGE);

	// 線の描画開始位置を少し上に挙げる
	VECTOR startLinePos = pos_;
	startLinePos.y += 50.0f;

	// 線の終了位置は開始位置＋移動ベクトル(デバッグ用の値
	VECTOR endLinePos = VAdd(startLinePos, debugDirVec_);

	// 視界判定をする方向に線を描画
	DrawLine3D(startLinePos, endLinePos, 0xff00ff);

	// 視野範囲をラジアンに変更
	const float VIEW_ANGLE_RAD = VIEW_ANGLE * (DX_PI_F / 180.0f);

	// 右向きの広さにデバッグ表示
	MATRIX rightSideMatY = MGetRotY(-VIEW_ANGLE_RAD);
	VECTOR rightSideDirVec = VTransform(debugDirVec_, rightSideMatY);
	VECTOR rightSideEndLinePos = VAdd(startLinePos, rightSideDirVec);
	DrawLine3D(startLinePos, rightSideEndLinePos, 0xff0000);

	// 左向きの広さにデバッグ表示
	MATRIX leftSideMatY = MGetRotY(VIEW_ANGLE_RAD);
	VECTOR leftSideDirVec = VTransform(debugDirVec_, leftSideMatY);
	VECTOR leftSideEndLinePos = VAdd(startLinePos, leftSideDirVec);
	DrawLine3D(startLinePos, leftSideEndLinePos, 0xff0000);
}
