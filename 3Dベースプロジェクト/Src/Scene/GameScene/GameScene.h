#pragma once
#include "../SceneBase.h"

// �N���X�̑O���錾
class Camera;
class Player;
class Enemy;

// �x�[�X���p��
class GameScene : public SceneBase
{
public:

	GameScene();			// �R���X�g���N�^
	~GameScene() override;	// �f�X�g���N�^

public:
	void Init(void)		override;	// ������
	void Load(void)		override;	// �ǂݍ���
	void LoadEnd(void)	override;	// �ǂݍ��݌�̏�����
	void Update(void)	override;	// �X�V
	void Draw(void)		override;	// �`��
	void Release(void)	override;	// ���

private:
	// �J����
	Camera* camera_;

	// �v���C���[
	Player* player_;

	// �G
	Enemy* enemy_;
};
