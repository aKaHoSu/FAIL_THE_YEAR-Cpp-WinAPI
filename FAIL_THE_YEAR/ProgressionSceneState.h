/* =============================================================================
 * ファイル概要:
 * - シーンの進行制御・シーン別のオブジェクト表示処理を記述する。
 *		（Ready, Result, AfterResult, EscNightmare シーンで使用）
 * ============================================================================= */
#pragma once

#include "Common.h"
#include "Scene.h"
#include "KeyManager.h"
#include "SceneServices.h"
#include "DebugLog.h"

class ProgressionSceneState {
public:
	// Result シーンで Enter を押すと AfterResult へ遷移する
	static void HandleResultInput(SceneManager& manager, const KeyManager& key) {
		if (key.IsTriggered(KeyManager::Enter)) {
			manager.setCurrentSceneType(SceneId::AfterResult);
		}
	}

	// Ready シーンの表示設定。
	// 難易度に応じたライフ・ボムCTを設定、
	// READY→START のアニメーション後に Playing へ遷移する。
	static void SetupReadyView(SceneManager& manager, SceneServices& services) {
		// シーン遷移直後のみ、難易度別の初期パラメータを設定する
		if (manager.getOldSceneType() != SceneId::Ready) {
			DEBUG_LOG("currentScene : Ready\n");
			if (services.gameState.CurrentDifficulty() == Difficulty::Nightmare) {
				services.gameState.SetEvadeGoal(EvadeGoal::Nightmare);
				services.objMgr.GetPlayer().setLife(1);
				services.objMgr.GetPlayer().setBombCT(BOMB_CT);
				DEBUG_LOG("\tcurrentDiff : Nightmare\n");
			}

			else if (services.gameState.CurrentDifficulty() == Difficulty::Easy) {
				services.gameState.SetEvadeGoal(EvadeGoal::Easy);
				services.objMgr.GetPlayer().setLife(5);
				DEBUG_LOG("    currentDiff : Easy\n");
			}

			else if (services.gameState.CurrentDifficulty() == Difficulty::Hard) {
				services.gameState.SetEvadeGoal(EvadeGoal::Hard);
				services.objMgr.GetPlayer().setLife(3);
				DEBUG_LOG("    currentDiff : Hard\n");
			}

			// Easy / Hard は共通のボムクールタイム（Nightmare は別設定のため除外）
			if (services.gameState.CurrentDifficulty() != Difficulty::Nightmare) {
				services.objMgr.GetPlayer().setBombCT(BOMB_CT + 1000);
			}

			manager.setOldSceneType(SceneId::Ready);
			services.objMgr.GetUIImage(ID_IMAGE_READY).setActCnt(0);
			services.objMgr.GetUIImage(ID_IMAGE_START).setActCnt(0);
			for (int i = 0; i < MAX_RYUNEN; i++) {
				services.objMgr.GetRyunen(i).setVisible(false);
			}
		}

		services.objMgr.GetPlayer().setVisible(true);

		// READY 画像を左にスライドして表示し、
		// 一定時間経過後に START 画像を表示する。
		// START画像表示終了後に Playing へ遷移する
		if (services.objMgr.GetUIImage(ID_IMAGE_READY).getActCnt() < 100) {
			services.objMgr.GetUIImage(ID_IMAGE_READY).setVisible(true);
			services.objMgr.GetUIImage(ID_IMAGE_READY).setActCnt(services.objMgr.GetUIImage(ID_IMAGE_READY).getActCnt() + 1);
			services.objMgr.GetUIImage(ID_IMAGE_READY).setPosition(services.objMgr.GetUIImage(ID_IMAGE_READY).getPosition().x - 1, 0);
		}
		else if (services.objMgr.GetUIImage(ID_IMAGE_READY).getActCnt() < 130) {
			services.objMgr.GetUIImage(ID_IMAGE_READY).setActCnt(services.objMgr.GetUIImage(ID_IMAGE_READY).getActCnt() + 1);
		}
		else if (services.objMgr.GetUIImage(ID_IMAGE_START).getActCnt() < 50) {
			services.objMgr.GetUIImage(ID_IMAGE_START).setVisible(true);
			services.objMgr.GetUIImage(ID_IMAGE_START).setActCnt(services.objMgr.GetUIImage(ID_IMAGE_START).getActCnt() + 1);
		}
		else {
			manager.setCurrentSceneType(SceneId::Playing);
		}
	}

	// Result シーンの表示設定。
	// クリア/留年確定に応じたテキストを表示し、
	// 条件を満たしていた場合、最高記録を更新する。
	static void SetupResultView(SceneManager& manager, SceneServices& services) {
		// シーン遷移直後のみ、
		// 回避数表示位置の調整と最高記録更新を行う
		if (manager.getOldSceneType() != SceneId::Result) {
			DEBUG_LOG("currentScene : Result\n");
			manager.setOldSceneType(SceneId::Result);

			if (manager.getResultType() == GameResult::Clear) {

				// 最高記録更新の条件は、今回の回避数がこれまでの最高記録を上回ること。
				if (services.gameState.BestEvadeCnt() < services.gameState.EvadeCnt()) {
					services.gameState.SetBestEvadeCnt(services.gameState.EvadeCnt());
					DEBUG_LOG("New record! the best evade count is %d\n", services.gameState.BestEvadeCnt());
				}

				// クリアフラグを立てる（Nightmareを選択可能にする）
				services.gameState.SetHasEverCleared(true);
			}

			// クリア/留年確定を問わず、初プレイフラグを下ろす
			services.gameState.SetIsFirstPlay(false);
		}

		services.objMgr.GetPlayer().setVisible(true);
		services.objMgr.GetUIShape(ID_SHAPE_VEIL).setVisible(true);

		// リザルトに応じてクリア/留年確定テキストと回避数を表示する
		if (manager.getResultType() == GameResult::Clear) {
			services.objMgr.GetUIText(ID_TEXT_RESULT_CLEAR).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_RESULT_EVADE_SHINKYU).setVisible(true);

			if (services.gameState.CurrentDifficulty() == Difficulty::Nightmare) {
				services.gameState.SetHasEverClearedNightmare(true);
				DEBUG_LOG("Cleared Nightmare difficulty.\n");
			}
		}
		else {
			services.objMgr.GetUIText(ID_TEXT_RESULT_FAIL).setVisible(true);
		}

		services.objMgr.GetUIText(ID_TEXT_RESULT_EVADE_RYUNEN).setVisible(true);

		// 『Enterキーで次へ』テキストを BLINK_INTERVAL ベースで点滅させる
		UI_Text::UpdateBlink(services.objMgr.GetUIText(ID_TEXT_RESULT_NEXT));
	}

	// AfterResult シーンの表示設定。
	// Nightmare 時は専用テキスト、通常時は Retry / BackTitle を表示する。
	static void SetupAfterResultView(SceneManager& manager, SceneServices& services) {
		if (manager.getOldSceneType() != SceneId::AfterResult) {
			DEBUG_LOG("currentScene : AfterResult\n");
			manager.setOldSceneType(SceneId::AfterResult);
		}

		if (services.gameState.CurrentDifficulty() == Difficulty::Nightmare
			&& !services.gameState.HasEverClearedNightmare()) {
			services.objMgr.GetUIText(ID_TEXT_AFTER_RESULT_NIGHTMARE).setVisible(true);
		}
		else {
			services.objMgr.GetPlayer().setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_AFTER_RESULT_RETRY).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_AFTER_RESULT_TITLE).setVisible(true);
		}

		services.objMgr.GetUIShape(ID_SHAPE_VEIL).setVisible(true);
		services.objMgr.GetUIShape(ID_SHAPE_SELECTING_TRIANGLE).setVisible(true);
		services.objMgr.GetUIShape(ID_SHAPE_SELECTING_RECTANGLE).setVisible(true);
	}

	// EscNightmare シーンの表示設定。
	// テキストを一定時間表示した後、自動的に Title へ遷移する。
	static void SetupEscNightmareView(SceneManager& manager, SceneServices& services) {
		if (manager.getOldSceneType() != SceneId::EscNightmare) {
			DEBUG_LOG("currentScene : EscNightmare\n");
			manager.setOldSceneType(SceneId::EscNightmare);
			services.objMgr.GetUIText(ID_TEXT_ESC_NM).setActCnt(0);
			services.gameState.SetHasEverEscapedNightmare(true);
		}

		services.objMgr.GetUIText(ID_TEXT_ESC_NM).setActCnt
		(services.objMgr.GetUIText(ID_TEXT_ESC_NM).getActCnt() + 1);

		if (services.objMgr.GetUIText(ID_TEXT_ESC_NM).getActCnt() <= 50) {
			services.objMgr.GetUIText(ID_TEXT_ESC_NM).setVisible(true);
		}

		if (services.objMgr.GetUIText(ID_TEXT_ESC_NM).getActCnt() >= 100)
			manager.setCurrentSceneType(SceneId::Title);
	}
};


