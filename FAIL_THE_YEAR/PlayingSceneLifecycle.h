/* =============================================================================
 * ファイル概要:
 * - Playing シーンのライフサイクル処理（遷移時初期化・ポーズ表示）を管理する。
 * ============================================================================= */
#pragma once

#include "Scene.h"
#include "SceneServices.h"
#include "UI_Text.h"
#include "DebugLog.h"

class PlayingSceneLifecycle {
public:
	// Playing シーン遷移直後に1回だけ実行する初期化
	static void InitOnEnter(SceneManager& manager, SceneServices& services) {
		if (manager.getCurrentSceneType() == SceneId::Playing &&
			manager.getOldSceneType() != SceneId::Playing) {
			DEBUG_LOG("currentScene : Playing\n");

			// 以降、同一シーン滞在中は初期化を再実行しないため oldScene を更新
			manager.setOldSceneType(SceneId::Playing);

			// 新規プレイ開始時の制御状態をリセット
			manager.SetIsPaused(false);
			manager.setResultType(GameResult::None);
			services.gameState.SetGameCnt(0);

			// 演出用カウンタをリセット
			services.objMgr.GetUIImage(ID_IMAGE_CANADV).setActCnt(0);
			services.objMgr.GetUIText(ID_TEXT_PAUSE).setActCnt(0);
			DEBUG_LOG("evadeCnt : %d\n", services.gameState.EvadeCnt());
			DEBUG_LOG("evadeGoal : %d\n", services.gameState.EvadeGoal());
		}
	}

	// ポーズ中の表示状態を維持する（ロジック更新はしない）
	static void SetupPausedView(SceneServices& services) {
		// ポーズ演出で必要な表示要素のみを可視化
		services.objMgr.GetPlayer().setVisible(true);
		services.objMgr.GetUIShape(ID_SHAPE_VEIL).setVisible(true);
		// PAUSE テキストの点滅を更新
		UI_Text::UpdateBlink(services.objMgr.GetUIText(ID_TEXT_PAUSE));
	}
};
