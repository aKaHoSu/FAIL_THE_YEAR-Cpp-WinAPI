/* =============================================================================
 * ファイル概要:
 * - タイトルシーンの入力制御を記述。
 * ============================================================================= */
#pragma once

#include "Common.h"
#include "Scene.h"
#include "KeyManager.h"
#include "SceneServices.h"
#include "DebugLog.h"

class TitleSceneState {
public:
	// Enter キーでメニューシーンへ遷移し、
	// Esc キーでゲーム終了へ遷移する
	static void HandleInput(SceneManager& manager, const KeyManager& key) {
		if (key.IsTriggered(KeyManager::Enter)) {
			manager.setCurrentSceneType(SceneId::Menu);
		}

		if (key.IsTriggered(KeyManager::Esc)) {
			manager.setCurrentSceneType(SceneId::Exit);
		}
	}

	// ロゴ画像を表示し、タイトルテキストを
	// BLINK_INTERVAL ベースで点滅させる
	static void SetupView(SceneManager& manager, SceneServices& services) {
		// シーン遷移直後のみ ActCnt をリセットし、
		// 点滅を最初から始める
		if (manager.getOldSceneType() != SceneId::Title) {
			DEBUG_LOG("currentScene : Title\n");
			manager.setOldSceneType(SceneId::Title);
			services.objMgr.GetUIText(ID_TEXT_TITLE).setActCnt(0);
			services.gameState.SetCurrentDifficulty(Difficulty::Easy);
		}

		services.objMgr.GetUIImage(ID_IMAGE_LOGO).setVisible(true);

		// ActCnt が BLINK_INTERVAL を超えたら非表示にし、
		// さらに超えたらリセットして点滅を繰り返す
		UI_Text::UpdateBlink(services.objMgr.GetUIText(ID_TEXT_TITLE));
	}
};


