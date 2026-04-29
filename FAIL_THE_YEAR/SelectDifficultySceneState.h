/* =============================================================================
 * ファイル概要:
 * - SelectDifficulty シーンの入力制御と表示設定を記述。
 * ============================================================================= */
#pragma once

#include "Common.h"
#include "Scene.h"
#include "KeyManager.h"
#include "GameInitializer.h"
#include "SceneServices.h"
#include "SelectionUiHelper.h"
#include "UiVisibilityBatchHelper.h"
#include "DebugLog.h"

class SelectDifficultySceneState {
public:
	// SelectDifficulty シーンの入力処理。
	// 難易度の選択移動、決定、戻る操作を扱う。
	static void HandleInput(
		SceneManager& manager,
		const KeyManager& key,
		HINSTANCE hInst,
		HWND hWnd,
		SceneServices& services)
	{
		// 上下入力で Easy / Hard を切り替える
		SelectionUiHelper::MoveTwoChoiceSelection(key, m_selection);

		// クリア済みの場合のみ Nightmare 選択を左右入力で有効化
		if (services.gameState.HasEverCleared()) {
			if (key.IsTriggered(KeyManager::Right)) {
				m_selection = 2;
			}
			if (key.IsTriggered(KeyManager::Left)) {
				if (m_selection != 1) {
					m_selection = 0;
				}
			}
		}

		switch (m_selection) {
		case 0:
			// Easy 項目にカーソルを合わせる
			SelectionUiHelper::ApplyTwoChoiceCursor(services.objMgr, 0);
			if (key.IsTriggered(KeyManager::Enter)) {
				// Easy でゲーム開始準備
				services.gameState.SetCurrentDifficulty(Difficulty::Easy);
				manager.setCurrentSceneType(SceneId::Ready);
				GameInitializer::InitAllGameObjects(hInst, hWnd, services.gameState, services.objMgr);
				if (!services.gameState.HasEverCleared()) {
					GameInitializer::ResetAllGameObjects(hInst, hWnd, services.gameState, services.objMgr);
				}
				m_selection = 0;
			}
			break;

		case 1:
			// Hard 項目にカーソルを合わせる
			SelectionUiHelper::ApplyTwoChoiceCursor(services.objMgr, 1);
			if (key.IsTriggered(KeyManager::Enter)) {
				// Hard でゲーム開始準備
				services.gameState.SetCurrentDifficulty(Difficulty::Hard);
				manager.setCurrentSceneType(SceneId::Ready);
				GameInitializer::InitAllGameObjects(hInst, hWnd, services.gameState, services.objMgr);
				if (!services.gameState.IsFirstPlay()) {
					GameInitializer::ResetAllGameObjects(hInst, hWnd, services.gameState, services.objMgr);
				}
				m_selection = 0;
			}
			break;

		case 2:
			// Nightmare は解放条件を満たさない場合に無効化する
			if (!services.gameState.HasEverCleared()) {
				m_selection = 0;
				break;
			}
			// Nightmare 項目にカーソルを合わせる
			services.objMgr.getUIShape(ID_SHAPE_SELECTING_TRIANGLE).setPosition(1080, 352);
			services.objMgr.getUIShape(ID_SHAPE_SELECTING_RECTANGLE).setPosition(1120, 320);
			if (key.IsTriggered(KeyManager::Enter)) {
				// Nightmare でゲーム開始準備
				services.gameState.SetCurrentDifficulty(Difficulty::Nightmare);
				services.gameState.SetIsNightmare(true);
				manager.setCurrentSceneType(SceneId::Ready);
				GameInitializer::InitAllGameObjects(hInst, hWnd, services.gameState, services.objMgr);
				GameInitializer::ResetAllGameObjects(hInst, hWnd, services.gameState, services.objMgr);
				m_selection = 0;
			}
			break;

		default:
			break;
		}

		// 1つ前のシーン（Menu）へ戻る
		if (key.IsTriggered(KeyManager::Esc)) {
			manager.setCurrentSceneType(SceneId::Menu);
			m_selection = 0;
		}
	}

	// SelectDifficulty シーンの表示設定。
	   // 難易度関連テキストと選択 UI を可視化する。
	static void SetupView(SceneManager& manager, SceneServices& services) {
		if (manager.getOldSceneType() != SceneId::SelectDifficulty) {
			DEBUG_LOG("currentScene : SelectDifficulty\n");
			manager.setOldSceneType(SceneId::SelectDifficulty);
		}

		UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, {
			ID_TEXT_SELDIFF,
			ID_TEXT_SELDIFF_BACK,
			ID_TEXT_SELDIFF_NORMAL,
			ID_TEXT_SELDIFF_HARD
			});
		if (services.gameState.HasEverCleared()) {
			UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, { ID_TEXT_SELDIFF_NIGHTMARE });
		}
		UiVisibilityBatchHelper::SetShapesVisible(services.objMgr, {
			ID_SHAPE_FRAME_SELDIFF,
			ID_SHAPE_SELECTING_TRIANGLE,
			ID_SHAPE_SELECTING_RECTANGLE
			});
	}

private:
	// SelectDifficulty シーン内の現在選択位置（0:Easy, 1:Hard, 2:Nightmare）
	inline static int m_selection = 0;
};
