/* =============================================================================
 * ファイル概要:
 * - ゲーム内オブジェクト初期化処理を実装する。
 * ============================================================================= */
#include "GameInitializer.h"
#include "Common.h"
#include "GameState.h"
#include "ObjectManager.h"

// ゲーム開始時の初期化
// フォント初期化 + オブジェクト初期化
void GameInitializer::InitAllGameObjects(HINSTANCE hInst, HWND hWnd, GameState& gameState, ObjectManager& objMgr) {

	// フォントの作成・登録
	UI_Text::InitializeFont(hWnd);
	Ryunen::InitializeRyunenFont(hWnd);

	// オブジェクトの初期化
	GameInitializer::ResetAllGameObjects(hInst, hWnd, gameState, objMgr);
}

// リトライ時の初期化
// オブジェクトのみをリセット
void GameInitializer::ResetAllGameObjects(HINSTANCE hInst, HWND hWnd, GameState& gameState, ObjectManager& objMgr) {
	InitializeUiObjects(hInst, hWnd, objMgr);
	InitializePlayerObject(hInst, objMgr);
	InitializeRyunenObjects(hInst, objMgr);
	ResetGameProgressState(gameState);
}

// オブジェクトを全て非表示にする
void GameInitializer::HiddenAllObject(HINSTANCE hInst, HWND hWnd, ObjectManager& objMgr) {
	HideUiObjects(objMgr);
	HideDynamicObjects(objMgr);
}

// UIオブジェクトの初期化
void GameInitializer::InitializeUiObjects(HINSTANCE hInst, HWND hWnd, ObjectManager& objMgr) {
	int j = 0;
	for (int i = ID_BEGIN_IMAGE; i < ID_BEGIN_IMAGE + MAX_UI_IMAGE; i++) {
		if (i == ID_BEGIN_IMAGE) { j = 0; }
		objMgr.GetUIImage(i).setInitData(hInst, hWnd, ID_BEGIN_IMAGE + j);
		j++;
	}

	for (int i = ID_BEGIN_TEXT; i < ID_BEGIN_TEXT + MAX_UI_TEXT; i++) {
		if (i == ID_BEGIN_TEXT) { j = 0; }
		objMgr.GetUIText(i).setInitData(hInst, hWnd, ID_BEGIN_TEXT + j);
		j++;
	}

	for (int i = ID_BEGIN_SHAPE; i < ID_BEGIN_SHAPE + MAX_UI_SHAPE; i++) {
		if (i == ID_BEGIN_SHAPE) { j = 0; }
		objMgr.GetUIShape(i).setInitData(hInst, hWnd, ID_BEGIN_SHAPE + j);
		j++;
	}
}

// プレイヤーオブジェクトの初期化
void GameInitializer::InitializePlayerObject(HINSTANCE hInst, ObjectManager& objMgr) {
	objMgr.GetPlayer().setInitData(hInst);
}

// 留年オブジェクトの初期化
void GameInitializer::InitializeRyunenObjects(HINSTANCE hInst, ObjectManager& objMgr) {
	for (int i = 0; i < MAX_RYUNEN; ++i) {
		objMgr.GetRyunen(i).setInitData(hInst, i);
	}
}

// ゲーム進行に関わる状態の初期化
void GameInitializer::ResetGameProgressState(GameState& gameState) {
	gameState.SetIsNightmare(false);
	gameState.SetCanAdv(false);
	gameState.SetEvadeCnt(0);
	gameState.SetCanSpawn(true);
	gameState.SetRyunenCount(5);
	gameState.SetGameCnt(0);
}

// UIオブジェクトを全て非表示にする
void GameInitializer::HideUiObjects(ObjectManager& objMgr) {
	for (int i = ID_BEGIN_IMAGE; i < ID_BEGIN_IMAGE + MAX_UI_IMAGE; i++) {
		objMgr.GetUIImage(i).setVisible(false);
	}

	for (int i = ID_BEGIN_TEXT; i < ID_BEGIN_TEXT + MAX_UI_TEXT; i++) {
		objMgr.GetUIText(i).setVisible(false);
	}

	for (int i = ID_BEGIN_SHAPE; i < ID_BEGIN_SHAPE + MAX_UI_SHAPE; i++) {
		objMgr.GetUIShape(i).setVisible(false);
	}
}

// 動的オブジェクトを全て非表示にする
void GameInitializer::HideDynamicObjects(ObjectManager& objMgr) {
	objMgr.GetPlayer().setVisible(false);
}

