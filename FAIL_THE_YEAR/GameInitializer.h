/* =============================================================================
 * ファイル概要:
 * - ゲーム内オブジェクト初期化関数を宣言する。
 * ============================================================================= */

#pragma once
#include <windows.h>

class GameState;
class ObjectManager;

class GameInitializer {
public:
	// インスタンス化禁止
	GameInitializer() = delete;

	// ゲーム開始時の初期化（フォント + オブジェクト）
	static void InitAllGameObjects(HINSTANCE hInst, HWND hWnd, GameState& gameState, ObjectManager& objMgr);

	// リトライ時の初期化（オブジェクトのみ）
	static void ResetAllGameObjects(HINSTANCE hInst, HWND hWnd, GameState& gameState, ObjectManager& objMgr);

	// オブジェクトを全て非表示にする
	static void HiddenAllObject(HINSTANCE hInst, HWND hWnd, ObjectManager& objMgr);

private:
	static void InitializeUiObjects(HINSTANCE hInst, HWND hWnd, ObjectManager& objMgr);
	static void InitializePlayerObject(HINSTANCE hInst, ObjectManager& objMgr);
	static void InitializeRyunenObjects(HINSTANCE hInst, ObjectManager& objMgr);
	static void ResetGameProgressState(GameState& gameState);
	static void HideUiObjects(ObjectManager& objMgr);
	static void HideDynamicObjects(ObjectManager& objMgr);
};
