/* =============================================================================
 * ファイル概要:
 * - Result シーンの入力制御と表示設定を記述。
 * ============================================================================= */
#pragma once

#include "Common.h"
#include "Scene.h"
#include "KeyManager.h"
#include "SceneServices.h"
#include "UiVisibilityBatchHelper.h"
#include "DebugLog.h"

class ResultSceneState {
public:
	// Result シーンの入力処理。
	   // Enter で AfterResult へ進む。
	static void HandleInput(SceneManager& manager, const KeyManager& key) {
		if (key.IsTriggered(KeyManager::Enter)) {
			manager.setCurrentSceneType(SceneId::AfterResult);
		}
	}

	// Result シーンの表示設定。
	   // 記録更新・フラグ更新・リザルト文言表示を行う。
	static void SetupView(SceneManager& manager, SceneServices& services) {
		// シーン遷移直後に1回だけ記録更新とフラグ更新を行う
		if (manager.getOldSceneType() != SceneId::Result) {
			DEBUG_LOG("currentScene : Result\n");
			manager.setOldSceneType(SceneId::Result);

			if (manager.getResultType() == GameResult::Clear) {
				// クリア時はベスト回避数を更新
				if (services.gameState.BestEvadeCnt() < services.gameState.EvadeCnt()) {
					services.gameState.SetBestEvadeCnt(services.gameState.EvadeCnt());
					DEBUG_LOG("New record! the best evade count is %d\n", services.gameState.BestEvadeCnt());
				}
				// クリア履歴フラグを更新
				services.gameState.SetHasEverCleared(true);
			}
			// 初回プレイ状態を解除
			services.gameState.SetIsFirstPlay(false);
		}

		// 共通表示（プレイヤーとヴェール）
		UiVisibilityBatchHelper::SetPlayerVisible(services.objMgr, true);
		UiVisibilityBatchHelper::SetShapesVisible(services.objMgr, { ID_SHAPE_VEIL });

		if (manager.getResultType() == GameResult::Clear) {
			// クリア表示
			UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, {
				 ID_TEXT_RESULT_CLEAR,
				 ID_TEXT_RESULT_EVADE_SHINKYU
				});
			if (services.gameState.CurrentDifficulty() == Difficulty::Nightmare) {
				// Nightmare クリア履歴を記録
				services.gameState.SetHasEverClearedNightmare(true);
				DEBUG_LOG("Cleared Nightmare difficulty.\n");
			}
		}
		else {
			// 留年確定表示
			UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, { ID_TEXT_RESULT_FAIL });
		}

		// 回避数表示 + 次へテキスト点滅
		UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, { ID_TEXT_RESULT_EVADE_RYUNEN });
		UI_Text::UpdateBlink(services.objMgr.getUIText(ID_TEXT_RESULT_NEXT));
	}
};
