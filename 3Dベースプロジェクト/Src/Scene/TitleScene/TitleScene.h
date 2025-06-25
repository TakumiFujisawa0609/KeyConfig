#pragma once
#include "../SceneBase.h"
#include <vector>
#include <string>

// �x�[�X���p��
class TitleScene : public SceneBase
{
public:

	TitleScene(void);				// �R���X�g���N�^
	~TitleScene(void) override;		// �f�X�g���N�^

public:

	void Init(void)		override;	// ������
	void Load(void)		override;	// �ǂݍ���
	void LoadEnd(void)	override;	// �ǂݍ��݌�̏���
	void Update(void)	override;	// �X�V
	void Draw(void)		override;	// �`��
	void Release(void)	override;	// ���

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

	std::vector<Item> items_;  // �� TitleEdit�ō�������̂������Ɏ���

	// �t�F�[�h
	bool isFadeOut_;	// �t�F�[�h�A�E�g���̃t���O
};
