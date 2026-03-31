/* =============================================================================
 * ファイル概要:
 * - Win32アプリのエントリーポイントと
 *   メインメッセージループを記述する。
 * ============================================================================= */
#include "Common.h"
#include "WndProc.h"

#ifdef _DEBUG
#include "ConsoleWindow.h"
#endif

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	WNDCLASSEX wc;
	HWND hWnd;
	MSG msg;

#ifdef _DEBUG
	// デバッグ表示用コンソール
	ConsoleWindow consoleWindow;
#endif

	/* ---------- ウィンドウクラスの設定 ---------- */
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("MainWnd");
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// 設定したウィンドウクラスを登録
	RegisterClassEx(&wc);

	// ウィンドウスタイルから
	// WS_MINIMIZEBOX / WS_MAXIMIZEBOX / WS_THICKFRAME を除外
	// -> ウィンドウのサイズ変更・最小化・最大化を無効化
	DWORD style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);

	// 登録したウィンドウクラスを元にウィンドウを生成
	hWnd = CreateWindow(wc.lpszClassName,
		TEXT("FAIL THE YEAR"),
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_W,
		WINDOW_H,
		NULL,
		NULL,
		hInstance,
		NULL);

	// ウィンドウを画面に表示し、最初の描画を行う
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	// メッセージループ。
	// OSからメッセージを受け取り、WndProcへ転送し続ける
	while (GetMessage(&msg, NULL, 0, 0))
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

