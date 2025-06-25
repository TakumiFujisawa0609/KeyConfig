#include "Enemy.h"
#include "cmath"
#include "../Object/Player.h"

Enemy::Enemy(void){}
Enemy::~Enemy(void){}

void Enemy::Init(void)
{
	// ���f���̏����ʒu�ݒ�
	pos_ = { 300.0f, 0.0f, 300.0f };
	MV1SetPosition(modelId_, pos_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(modelId_, 1);

	// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
	animTotalTime_ = MV1SetAttachAnimTime(modelId_, animAttachNo_, currentAnimTime_);
	currentAnimTime_ = 0.0f;

	// �����Ă������������(��O����)
	dirVec_ = INIT_DIR;

	// �����Ă���������烂�f���̕�����ݒ�
	float playerRotY = atan2f(dirVec_.x, dirVec_.z);
	rot_ = { 0.0f, playerRotY, 0.0f };
	MV1SetRotationXYZ(modelId_, rot_);
}

void Enemy::Load(void)
{	
	// ���f���̓ǂݍ���
	modelId_ = MV1LoadModel("Data/Model/Human.mv1");
}

void Enemy::Update(void)
{
	// �A�j���[�V��������
	UpdateAnimation();

	// ���m�X�V
	UpdateSearch();
}

void Enemy::Draw(void)
{
	// ���o�͈͓��t���O�������Ă�����
	if (isNoticeHearing_)
	{
		// ���o�͈͓��F�f���q���[�Y�J���[�����F�ɂ���
		COLOR_F color = GetColorF(1.0f, 1.0f, 0.0f, 1.0f);
		MV1SetMaterialDifColor(modelId_, 0, color);
	}
	// ���o�͈͓��t���O���܂�Ă�����
	else
	{
		// ����͈͊O�F�f�B�q���[�Y�J���[���D�F�ɂ���
		COLOR_F color = GetColorF(0.5f, 0.5f, 0.5f, 1.0f);
		MV1SetMaterialDifColor(modelId_, 0, color);
	}

	// ���o�͈͂ɓ����Ă�����
	if (isNoticeView_)
	{
		// ����͈͓��F�f���q���[�Y�J���[��ԐF�ɂ���
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
	// �V�����A�j���[�V�����Đ����Ԃ��Z�b�g
	currentAnimTime_ += ANIM_SPEED;
	if (currentAnimTime_ > animTotalTime_)
	{
		currentAnimTime_ = 0.0f;
	}

	// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
	MV1SetAttachAnimTime(modelId_, animAttachNo_, currentAnimTime_);
}

void Enemy::UpdateSearch(void)
{
	// �v���C���[�̍��W���擾
	VECTOR pPos = player_->GetPos();

	// �G�l�~�[����v���C���[�܂ł̃x�N�g��
	VECTOR vecPlayerFromEnemy = VSub(pPos, pos_);

	// �v���C���[�ƓG�̋���(��2��)���v�Z����(�s�^�S���X�̒藝���)
	float distancePowX = std::pow(vecPlayerFromEnemy.x, 2.0f);
	float distancePowZ = std::pow(vecPlayerFromEnemy.z, 2.0f);
	float distancePow = distancePowX + distancePowZ;

	// 2��������ċ������Z�o����
	float distance = std::sqrt(distancePow);

	// ���������o�͈͓���������
	if (distance <= HEARING_RANGE)
	{
		// ���o�͈͓��ł���Ƃ����t���O�𗧂Ă�
		isNoticeHearing_ = true;
	}
	// �͈͊O�ł����
	else
	{
		// �͈͊O�ł���Ƃ����t���O��܂�
		isNoticeHearing_ = false;
	}

	// ����p����
	// ����͒��o����������͈͂ł̂ݎ���p�̔�����Ƃ�
	if (isNoticeHearing_)
	{
		// �G���v���C���[�̃x�N�g�����쐬
		float playerFromEnemyX = pPos.x - pos_.x;	// X�̋���
		float playerFromEnemyZ = pPos.z - pos_.z;	// Z�̋���
		VECTOR vecPlayerFromEnemy = { playerFromEnemyX, 0.0f, playerFromEnemyZ };

		// �������猩���v���C���[�̊p�x�����߂�
		float rad = atan2(vecPlayerFromEnemy.x, vecPlayerFromEnemy.z);

		// �p�x�̍������߂�
		float diffViewRad = rad - rot_.y;

		// ���W�A�����f�O���[�ɕϊ�(30�x�Ƃ������f�O���[�l�̕������o�Ƃ��Ă킩��₷������)
		float diffViewDeg = diffViewRad * 180.0f / DX_PI_F;

		// ���̑傫�����o��
		diffViewDeg = abs(diffViewDeg);

		// �p�x��������͈͂Ȃ��������王���������s��
		if (diffViewDeg < VIEW_ANGLE)
		{
			isNoticeView_ = true;
		}
		else
		{
			isNoticeView_ = false;
		}
	}
	// ���o�͈͂ɓ����Ă��Ȃ���Ύ��E���ɓ����Ă��Ȃ��Ƃ���
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

	// ���o����
	// �����Ă�������ɐ����o��
	dirVec_ = VNorm(dirVec_);
	VECTOR debugDirVec_ = VScale(dirVec_, HEARING_RANGE);

	// ���̕`��J�n�ʒu��������ɋ�����
	VECTOR startLinePos = pos_;
	startLinePos.y += 50.0f;

	// ���̏I���ʒu�͊J�n�ʒu�{�ړ��x�N�g��(�f�o�b�O�p�̒l
	VECTOR endLinePos = VAdd(startLinePos, debugDirVec_);

	// ���E�������������ɐ���`��
	DrawLine3D(startLinePos, endLinePos, 0xff00ff);

	// ����͈͂����W�A���ɕύX
	const float VIEW_ANGLE_RAD = VIEW_ANGLE * (DX_PI_F / 180.0f);

	// �E�����̍L���Ƀf�o�b�O�\��
	MATRIX rightSideMatY = MGetRotY(-VIEW_ANGLE_RAD);
	VECTOR rightSideDirVec = VTransform(debugDirVec_, rightSideMatY);
	VECTOR rightSideEndLinePos = VAdd(startLinePos, rightSideDirVec);
	DrawLine3D(startLinePos, rightSideEndLinePos, 0xff0000);

	// �������̍L���Ƀf�o�b�O�\��
	MATRIX leftSideMatY = MGetRotY(VIEW_ANGLE_RAD);
	VECTOR leftSideDirVec = VTransform(debugDirVec_, leftSideMatY);
	VECTOR leftSideEndLinePos = VAdd(startLinePos, leftSideDirVec);
	DrawLine3D(startLinePos, leftSideEndLinePos, 0xff0000);
}
