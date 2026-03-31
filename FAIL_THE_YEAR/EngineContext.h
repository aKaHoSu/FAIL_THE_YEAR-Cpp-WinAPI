/* =============================================================================
 * ファイル概要:
 * - WinAPI ハンドル・シーン管理・キー入力管理をまとめて保持する
 *	 ゲームエンジン全体の実行コンテキスト EngineContext を宣言する。
 * ============================================================================= */
#pragma once
#include <windows.h>
#include <cstdint>
#include <string>
#include "KeyManager.h"
#include "Scene.h"
#include "DebugCommandLine.h"

class EngineContext {
public:
	EngineContext();

	HINSTANCE hInst;	// アプリケーションインスタンスハンドル
	HWND	  hWnd;		// ウィンドウハンドル
	HDC		  hDCBack;	// バックバッファ用デバイスコンテキスト
	u_short	  tmf;		// タイマー割り込み防止フラグ

	SceneManager sceneManager;	// シーン管理
	KeyManager	 keyManager;	// キー入力管理

	DebugCommandLine debugCmdLine;	// デバッグコマンドライン状態
};
