/* =============================================================================
 * ファイル概要:
 * - Ready シーンの表示設定と遷移処理を記述。
 * ============================================================================= */
#pragma once

#include "Common.h"
#include "Scene.h"
#include "SceneServices.h"
#include "UiVisibilityBatchHelper.h"
#include "DebugLog.h"

class ReadySceneState {
public:
	// Ready シーンの表示更新。
	   // 難易度別パラメータ設定と READY→START 演出後の Playing 遷移を行う。
	static void SetupView(SceneManager& manager, SceneServices& services) {
		// シーン遷移直後に1回だけ初期設定を行う
		if (manager.getOldSceneType() != SceneId::Ready) {
			DEBUG_LOG("currentScene : Ready\n");
			if (services.gameState.CurrentDifficulty() == Difficulty::Nightmare) {
				// Nightmare の初期値
				services.gameState.SetEvadeGoal(EvadeGoal::Nightmare);
				services.objMgr.getPlayer().setLife(1);
				services.objMgr.getPlayer().setBombCT(BOMB_CT);
				DEBUG_LOG("\tcurrentDiff : Nightmare\n");
			}
			else if (services.gameState.CurrentDifficulty() == Difficulty::Easy) {
				// Easy の初期値
				services.gameState.SetEvadeGoal(EvadeGoal::Easy);
				services.objMgr.getPlayer().setLife(5);
				DEBUG_LOG("    currentDiff : Easy\n");
			}
			else if (services.gameState.CurrentDifficulty() == Difficulty::Hard) {
				// Hard の初期値
				services.gameState.SetEvadeGoal(EvadeGoal::Hard);
				services.objMgr.getPlayer().setLife(3);
				DEBUG_LOG("    currentDiff : Hard\n");
			}

			// Easy / Hard は共通のボムCT補正を適用
			if (services.gameState.CurrentDifficulty() != Difficulty::Nightmare) {
				services.objMgr.getPlayer().setBombCT(BOMB_CT + 1000);
			}

			// 演出カウンタと留年表示状態をリセット
			manager.setOldSceneType(SceneId::Ready);
			services.objMgr.getUIImage(ID_IMAGE_READY).setActCnt(0);
			services.objMgr.getUIImage(ID_IMAGE_START).setActCnt(0);
			for (int i = 0; i < MAX_RYUNEN; i++) {
				services.objMgr.getRyunen(i).setVisible(false);
			}
		}

		UiVisibilityBatchHelper::SetPlayerVisible(services.objMgr, true);
		services.objMgr.getPlayer().updateDrawState(manager, services.gameState);

		// READY→待機→START の順で演出を進める
		if (services.objMgr.getUIImage(ID_IMAGE_READY).getActCnt() < 100) {
			// READY 表示中（左へスライド）
			UiVisibilityBatchHelper::SetImagesVisible(services.objMgr, { ID_IMAGE_READY });
			services.objMgr.getUIImage(ID_IMAGE_READY).setActCnt(services.objMgr.getUIImage(ID_IMAGE_READY).getActCnt() + 1);
			services.objMgr.getUIImage(ID_IMAGE_READY).setPosition(services.objMgr.getUIImage(ID_IMAGE_READY).getPosition().x - 1, 0);
		}
		else if (services.objMgr.getUIImage(ID_IMAGE_READY).getActCnt() < 130) {
			// READY 表示後の短い待機
			services.objMgr.getUIImage(ID_IMAGE_READY).setActCnt(services.objMgr.getUIImage(ID_IMAGE_READY).getActCnt() + 1);
		}
		else if (services.objMgr.getUIImage(ID_IMAGE_START).getActCnt() < 50) {
			// START 表示
			UiVisibilityBatchHelper::SetImagesVisible(services.objMgr, { ID_IMAGE_START });
			services.objMgr.getUIImage(ID_IMAGE_START).setActCnt(services.objMgr.getUIImage(ID_IMAGE_START).getActCnt() + 1);
		}
		else {
			// 演出完了で Playing へ遷移
			manager.setCurrentSceneType(SceneId::Playing);
		}
	}
};
