/* =============================================================================
 * ファイル概要:
 * - Exitシーン(thank you for playing等)の入力制御を記述。
 * ============================================================================= */
#pragma once

#include "Common.h"
#include "Scene.h"
#include "KeyManager.h"
#include "SceneServices.h"
#include "EngineContext.h"
#include "DebugLog.h"

class ExitSceneState {
public:
	inline static int16_t m_ExitCountdownFrame = 0;
	// Exit シーンの表示設定
	static void SetupExitView(SceneManager& manager, SceneServices& services) {
		if (manager.getOldSceneType() != SceneId::Exit) {
			DEBUG_LOG("currentScene : Exit\n");
			m_ExitCountdownFrame = 100; // =シーン遷移後、100フレームはテキスト表示を維持する。
			manager.setOldSceneType(SceneId::Exit);
		}

		// --- ゲームの進行状況に応じて、表示するテキストを切り替える。

		// Nightmare をクリアした場合
		if (services.gameState.HasEverClearedNightmare()) {
			services.objMgr.GetUIText(ID_TEXT_E_NM_CLEAR).setVisible(true);
		}

		// Nightmare を脱出した場合（クリアはしていない）
		else if (services.gameState.HasEverEscapedNightmare()){
			services.objMgr.GetUIText(ID_TEXT_E_NM_ESC).setVisible(true);
		}

		// ゲームを一回でも遊んだ場合（結果は不問）
		else if (!services.gameState.IsFirstPlay()) {
			services.objMgr.GetUIText(ID_TEXT_E_THANKS).setVisible(true);
		}

		// 一度も遊んでいない場合（初回起動でいきなり Exit シーンに来た場合など）
		else {
			services.objMgr.GetUIText(ID_TEXT_E_NEW_GUEST).setVisible(true);
		}

		// 一定時間経過したらウィンドウを閉じる
		m_ExitCountdownFrame--;
		DEBUG_LOG("m_ExitCountdownFrame : %d\n", m_ExitCountdownFrame);
		if (m_ExitCountdownFrame <= 0) {
			SendMessage(g_engine.hWnd, WM_DESTROY, 0, 0);
		}
	}
};
