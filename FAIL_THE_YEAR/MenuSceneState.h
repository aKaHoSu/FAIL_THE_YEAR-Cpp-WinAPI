/* =============================================================================
 * ファイル概要:
 * - Menu シーンの入力制御と表示設定を記述。
 * ============================================================================= */
#pragma once

#include "Common.h"
#include "Scene.h"
#include "KeyManager.h"
#include "SceneServices.h"
#include "SelectionUiHelper.h"
#include "UiVisibilityBatchHelper.h"
#include "DebugLog.h"

class MenuSceneState {
public:
	// Menu シーンの入力処理。
	// 上下で選択を切り替え、Enter で遷移、Esc で Title に戻る。
	static void HandleInput(SceneManager& manager, const KeyManager& key, SceneServices& services) {
		// 上下入力でメニュー選択位置を更新
		SelectionUiHelper::MoveTwoChoiceSelection(key, m_menuSelection);

		switch (m_menuSelection) {
		case 0:
			// 上段項目（Play）にカーソルを合わせる
			SelectionUiHelper::ApplyTwoChoiceCursor(services.objMgr, 0);
			if (key.IsTriggered(KeyManager::Enter)) {
				// 難易度選択へ遷移
				manager.setCurrentSceneType(SceneId::SelectDifficulty);
				m_menuSelection = 0;
			}
			break;

		case 1:
			// 下段項目（Tutorial）にカーソルを合わせる
			SelectionUiHelper::ApplyTwoChoiceCursor(services.objMgr, 1);
			if (key.IsTriggered(KeyManager::Enter)) {
				// Tutorial へ遷移
				manager.setCurrentSceneType(SceneId::Tutorial);
				m_menuSelection = 0;
			}
			break;

		default:
			break;
		}

		// 1つ前のシーン（Title）へ戻る
		if (key.IsTriggered(KeyManager::Esc)) {
			manager.setCurrentSceneType(SceneId::Title);
			m_menuSelection = 0;
		}
	}

	// Menu シーンの表示設定。
	   // このシーンで使うテキストと選択 UI を可視化する。
	static void SetupView(SceneManager& manager, SceneServices& services) {
		if (manager.getOldSceneType() != SceneId::Menu) {
			DEBUG_LOG("currentScene : Menu\n");
			manager.setOldSceneType(SceneId::Menu);
		}

		UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, {
			  ID_TEXT_MENU_PLAY,
			  ID_TEXT_MENU_TUTORIAL,
			  ID_TEXT_MENU_BACK,
			});

		UiVisibilityBatchHelper::SetShapesVisible(services.objMgr, {
			ID_SHAPE_SELECTING_TRIANGLE,
			ID_SHAPE_SELECTING_RECTANGLE
			});
	}

private:
	// Menu シーン内の現在選択位置（0:Play, 1:Tutorial）
	inline static int m_menuSelection = 0;
};
