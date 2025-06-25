#pragma once
#include <DxLib.h>

class ObjectBase
{
public:
	ObjectBase();
	// デストラクタ
	virtual ~ObjectBase(void) = 0;

	virtual void Init(void) = 0;
	virtual void Load(void) = 0;
	virtual void LoadEnd(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Release(void) = 0;

	// 拡大値を返す
	VECTOR GetScale(void) { return scale_; }
	// 角度を返す
	VECTOR GetRot(void) { return rot_; }
	// 座標を返す
	VECTOR GetPos(void) { return pos_; }

protected:

	// モデルID
	int modelId_;

	VECTOR scale_;	// 拡大値

	VECTOR rot_;	// 角度

	VECTOR pos_;	// 座標
};
