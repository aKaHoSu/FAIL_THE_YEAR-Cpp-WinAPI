/* =============================================================================
 * ファイル概要:
 * - デバッグ用コンソールウィンドウの生成/破棄。
 * ============================================================================= */
#include <windows.h>
#include "ConsoleWindow.h"
#include "DebugLog.h"

ConsoleWindow::ConsoleWindow()
{
	// デバッグ用コンソールを作成、を使えるようにする
	AllocConsole();
	SetConsoleTitleA("FAIL_THE_YEAR Debug Console");

	freopen_s(&m_fIn, "CON", "r", stdin);    // 標準入力の割り当て
	freopen_s(&m_fOut, "CON", "w", stdout);  // 標準出力の割り当て

	DEBUG_LOG(
		"It succeeded in starting of a console screen.\n"
		"A standard input/output can be used.\n\n");
}

ConsoleWindow::~ConsoleWindow()
{
	// コンソールの削除
	fclose(m_fIn);
	fclose(m_fOut);
	FreeConsole();
}

