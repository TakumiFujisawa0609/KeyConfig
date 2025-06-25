#pragma once
#include <DxLib.h>

class ObjectBase
{
public:
	ObjectBase();
	// �f�X�g���N�^
	virtual ~ObjectBase(void) = 0;

	virtual void Init(void) = 0;
	virtual void Load(void) = 0;
	virtual void LoadEnd(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Release(void) = 0;

	// �g��l��Ԃ�
	VECTOR GetScale(void) { return scale_; }
	// �p�x��Ԃ�
	VECTOR GetRot(void) { return rot_; }
	// ���W��Ԃ�
	VECTOR GetPos(void) { return pos_; }

protected:

	// ���f��ID
	int modelId_;

	VECTOR scale_;	// �g��l

	VECTOR rot_;	// �p�x

	VECTOR pos_;	// ���W
};
