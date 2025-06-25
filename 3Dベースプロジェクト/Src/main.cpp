#include <DxLib.h>
#include "Application.h"

// WinMain関数
//---------------------------------
int WINAPI WinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// インスタンスの生成
	Application::CreateInstance();

	// インスタンスの取得
	Application::GetInstance()->Init();

	if (Application::GetInstance()->IsInitFail())
	{
		// 初期化失敗
		return -1;
	}

	// 実行
	Application::GetInstance()->Run();

	// 解放
	Application::GetInstance()->Release();

	if (Application::GetInstance()->IsReleaseFail())
	{
		// 解放失敗
		return -1;
	}

	// インスタンスの削除
	Application::DeleteInstance();

	return 0;
}
