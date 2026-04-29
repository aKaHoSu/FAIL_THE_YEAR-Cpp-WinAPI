/* =============================================================================
 * ファイル概要:
 * - ウィンドウメッセージ処理とゲームループ駆動処理を記述する。
 * ============================================================================= */
#include "Common.h"
#include "WndProc.h"
#include "Draw.h"
#include "GameInitializer.h"
#include "GameContext.h"
#include "UI_Text.h"
#include "Ryunen.h"
#include "DebugLog.h"
#include "SceneServices.h"
#include <ctime>
#include <string>
#include <algorithm>
#include <cwctype>

namespace {
	// ゲームループタイマー間隔
	// これを短くすると、ゲームの更新頻度が上がる（=処理が重くなる）
	constexpr int kTimerIntervalMs = 15;

	// 実行結果メッセージを画面表示するフレーム数
	constexpr int kCommandStatusFrames = 280;

	// デバッグコマンドを実行する関数
	std::wstring ExecuteDebugCommand(const std::wstring& command, SceneServices& services) {
		// コマンドは大文字/小文字を区別しないため小文字へ正規化
		std::wstring normalized = command;
		std::transform(
			normalized.begin(),
			normalized.end(),
			normalized.begin(),
			[](wchar_t ch) { return static_cast<wchar_t>(std::towlower(ch)); });

		if (normalized == L"godmode") {
			// godmode: 無敵フラグのトグル
			services.gameState.ToggleIsGodMode();
			DEBUG_LOG("Command: godmode -> %s\n", services.gameState.IsGodMode() ? "ON" : "OFF");
			return services.gameState.IsGodMode() ? L"godmode: ON" : L"godmode: OFF";
		}

		else if (normalized == L"hitbox") {
			// hitbox: 当たり判定表示フラグのトグル
			services.gameState.ToggleIsDspHitBox();
			DEBUG_LOG("Command: hitbox -> %s\n", services.gameState.IsDspHitBox() ? "ON" : "OFF");
			return services.gameState.IsDspHitBox() ? L"hitbox: ON" : L"hitbox: OFF";
		}

		else if (normalized == L"firstplay") {
			// firstplay: 初回プレイ判定フラグのトグル
			services.gameState.ToggleIsFirstPlay();
			DEBUG_LOG("Command: firstplay -> %s\n", services.gameState.IsFirstPlay() ? "ON" : "OFF");
			return services.gameState.IsFirstPlay() ? L"firstplay: ON" : L"firstplay: OFF";
		}

		else if (normalized == L"clearflag") {
			// clearflag: クリア済みフラグのトグル
			services.gameState.ToggleHasEverCleared();
			DEBUG_LOG("Command: clearflag -> %s\n", services.gameState.HasEverCleared() ? "ON" : "OFF");
			return services.gameState.HasEverCleared() ? L"clearflag: ON" : L"clearflag: OFF";
		}

		else if (normalized == L"clearnmflag") {
			// clearnmflag: ナイトメアクリア済みフラグのトグル
			services.gameState.ToggleHasEverClearedNightmare();
			DEBUG_LOG("Command: clearnmflag -> %s\n", services.gameState.HasEverClearedNightmare() ? "ON" : "OFF");
			return services.gameState.HasEverClearedNightmare() ? L"clearnmflag: ON" : L"clearnmflag: OFF";
		}

		else if (normalized == L"escnmflag") {
			// escnmflag: ナイトメア脱出済みフラグのトグル
			services.gameState.ToggleHasEverEscapedNightmare();
			DEBUG_LOG("Command: escnmflag -> %s\n", services.gameState.HasEverEscapedNightmare() ? "ON" : "OFF");
			return services.gameState.HasEverEscapedNightmare() ? L"escnmflag: ON" : L"escnmflag: OFF";
		}

		else if (normalized == L"help") {
			// help: 利用可能コマンドを表示
			DEBUG_LOG("Available commands: godmode, hitbox, firstplay, clearflag, clearnmflag, escnmflag, help\n");
			return L"commands: godmode, hitbox, firstplay, clearflag, clearnmflag, escnmflag, help";
		}

		else if (!normalized.empty()) {
			DEBUG_LOG("Unknown command: %ls\n", normalized.c_str());
			return L"unknown command";
		}

		return L"";
	}
}


LRESULT CALLBACK WndProc(HWND hWnd,
	UINT   message,
	WPARAM wParam,
	LPARAM lParam)
{
	GameContext& context = g_gameContext;
	EngineContext& engine = context.engine;
	SceneServices services{ context.gameState, context.objMgr };

	static RECT rc;
	HBITMAP hBitmap;
	HDC hDC;
	PAINTSTRUCT ps;
	GetClientRect(hWnd, &rc);

	switch (message)
	{
	case WM_CREATE:
		DEBUG_LOG("WindowsMessage : WM_CREATE\n");
		/* ---------- バックバッファの作成 ---------- */
		hDC = GetDC(hWnd);
		engine.hDCBack = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, WINDOW_W, WINDOW_H);
		SelectObject(engine.hDCBack, hBitmap);
		DeleteObject(hBitmap);
		ReleaseDC(hWnd, hDC);
		/* ------------------------------------------ */

		engine.hInst = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
		engine.hWnd = hWnd;

		GameInitializer::InitAllGameObjects(engine.hInst, engine.hWnd, context.gameState, context.objMgr);

		srand((unsigned)time(NULL));
		SetTimer(hWnd, 1, kTimerIntervalMs, NULL);

		break;

	case WM_KEYDOWN:
#ifdef _DEBUG
		if (engine.debugCmdLine.isVisible) {
			// コマンドライン表示中は入力をゲーム側へ流さず、ここで完結させる
			if (wParam == VK_BACK) {
				// Backspace: 入力バッファの末尾を削除
				if (!engine.debugCmdLine.buffer.empty()) {
					engine.debugCmdLine.buffer.pop_back();
				}
				return 0;
			}

			if (wParam == VK_RETURN) {
				// Enter: 現在の入力バッファをコマンドとして実行
				engine.debugCmdLine.status = ExecuteDebugCommand(engine.debugCmdLine.buffer, services);
				engine.debugCmdLine.statusTimer = engine.debugCmdLine.status.empty() ? 0 : kCommandStatusFrames;
				engine.debugCmdLine.buffer.clear();
				return 0;
			}

			if (wParam == VK_ESCAPE) {
				// Esc: コマンドラインを閉じる
				engine.debugCmdLine.buffer.clear();
				engine.debugCmdLine.isVisible = false;

				// 直後の1フレームでEscがシーン遷移に使われないよう抑止フラグを立てる
				engine.debugCmdLine.suppressEscUntilRelease = true;
				return 0;
			}

			return 0;
		}
#endif
		break;

	case WM_CHAR:
#ifdef _DEBUG
		if (!engine.debugCmdLine.isVisible) {
			if (wParam == L'/') {
				// '/' でコマンドラインを開く
				engine.debugCmdLine.isVisible = true;
				engine.debugCmdLine.suppressEscUntilRelease = false;
				engine.debugCmdLine.buffer.clear();
				return 0;
			}
			break;
		}

		// 表示可能文字のみを受け付ける
		if (wParam >= 0x20 && wParam != 0x7F) {
			// 過度な入力長を防ぐため上限を設ける
			if (engine.debugCmdLine.buffer.size() < 60) {
				engine.debugCmdLine.buffer.push_back(static_cast<wchar_t>(wParam));
			}
		}
		return 0;
#endif

	case WM_TIMER:
		if (engine.tmf == 0) {
			engine.tmf++;
			// 実行結果メッセージ表示の残り時間を減算
			if (engine.debugCmdLine.statusTimer > 0) {
				engine.debugCmdLine.statusTimer--;
			}
			engine.keyManager.Update();
			engine.sceneManager.Update(
				engine.sceneManager.getCurrentSceneType(),
				engine.keyManager,
				engine.hInst,
				engine.hWnd,
				services
			);
			engine.sceneManager.SceneLoop(engine.hInst, engine.hWnd, engine.keyManager, services);
			InvalidateRect(hWnd, NULL, FALSE);
		}

		break;

	case WM_PAINT:
	{
		hDC = BeginPaint(hWnd, &ps);

		// バックバッファにすべて描画し、最後に画面へ転送する
		// バックバッファをクリア
		RECT rc = { 0, 0, WINDOW_W, WINDOW_H };
		HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
		FillRect(engine.hDCBack, &rc, hBrush);

		// すべてのゲームオブジェクトをバックバッファへ描画
		Draw::DrawAll(engine.hDCBack, context.objMgr, context.gameState);

		// バックバッファを画面に転送
		BitBlt(hDC, 0, 0, WINDOW_W, WINDOW_H, engine.hDCBack, 0, 0, SRCCOPY);

		engine.tmf = 0;
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_CLOSE:
		SendMessage(hWnd, WM_DESTROY, 0, 0);
		break;

	case WM_DESTROY:
		UI_Text::FinalizeFont();
		Ryunen::FinalizeRyunenFont();
		DeleteDC(engine.hDCBack);

		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


