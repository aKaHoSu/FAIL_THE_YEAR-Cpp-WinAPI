/* =============================================================================
 * ファイル概要:
 * - プレイ中シーンの更新処理を記述する。
 * - 生成・行動・判定・遷移を管理する。
 * ============================================================================= */
#pragma once

#include "Common.h"
#include "Scene.h"
#include "KeyManager.h"
#include "Utils.h"
#include "SceneServices.h"
#include "DebugLog.h"

class PlayingSceneState {
public:
	// Zキー入力を受け取り、プレイヤーのボムを発動する
	static void HandleInput(SceneManager& manager, const KeyManager& key, SceneServices& services) {
		if (key.IsTriggered(KeyManager::Z)) {
			services.objMgr.GetPlayer().useBomb(services.objMgr);
		}
	}

	// Playing中の1フレーム更新（生成・行動・判定・遷移）を行う
	static void Run(SceneManager& manager, const KeyManager& key, HINSTANCE hInst, SceneServices& services) {
		InitSceneState(manager, services);

		// ポーズ中はロジックを停止し、現在の表示状態だけを維持する
		if (manager.IsPaused()) {
			SetupPausedView(services);
			return;
		}

		const int16_t spawnInterval = CalcSpawnInterval(services.gameState);
		services.gameState.IncrementGameCnt();
		UpdateRyunenCount(spawnInterval, services);

		services.objMgr.GetPlayer().setVisible(true);
		SpawnRyunen(manager, services);
		UpdateEntities(manager, key, services);
		services.objMgr.checkHitBetweenPlayerAndRyunen(services.gameState);

		EvaluateGameResult(manager, services);
		EvaluateCanAdv(manager, services);
		UpdateCanAdvCutIn(manager, services);

		if (CheckAllOffScreen(services) &&
			(manager.getResultType() == GameResult::Clear || manager.getResultType() == GameResult::Fail)) {
			manager.setCurrentSceneType(SceneId::Result);
		}
	}

private:
	// ポーズ中の表示状態を維持する（HiddenAllObject 後の復元。ロジック更新は行わない）
	static void SetupPausedView(SceneServices& services) {
		services.objMgr.GetPlayer().setVisible(true);
		services.objMgr.GetUIShape(ID_SHAPE_VEIL).setVisible(true);
		//services.objMgr.GetUIText(ID_TEXT_PAUSE).setVisible(true);
		UI_Text::UpdateBlink(services.objMgr.GetUIText(ID_TEXT_PAUSE));
	}

	// シーン遷移直後の初期化（Playing / Fail / Clear）
	static void InitSceneState(SceneManager& manager, SceneServices& services) {
		const SceneId currentSceneType = manager.getCurrentSceneType();
		const SceneId oldSceneType     = manager.getOldSceneType();

		if (currentSceneType == SceneId::Playing && oldSceneType != SceneId::Playing) {
			DEBUG_LOG("currentScene : Playing\n");
			manager.setOldSceneType(SceneId::Playing);
			manager.SetIsPaused(false);		// 新セッション開始時にポーズを解除
			services.gameState.SetGameCnt(0);
			services.objMgr.GetUIImage(ID_IMAGE_CANADV).setActCnt(0);
			services.objMgr.GetUIText(ID_TEXT_PAUSE).setActCnt(0);
			manager.setResultType(GameResult::None);
			DEBUG_LOG("evadeCnt : %d\n", services.gameState.EvadeCnt());
			DEBUG_LOG("evadeGoal : %d\n", services.gameState.EvadeGoal());
		}
		else if (currentSceneType == SceneId::Fail && oldSceneType != SceneId::Fail) {
			DEBUG_LOG("currentScene : Fail\n");
			manager.setOldSceneType(SceneId::Fail);
			services.gameState.SetCanSpawn(false);
		}
		else if (currentSceneType == SceneId::Clear && oldSceneType != SceneId::Clear) {
			DEBUG_LOG("currentScene : Clear\n");
			manager.setOldSceneType(SceneId::Clear);
			services.gameState.SetCanSpawn(false);
		}
	}

	// 難易度と進級確定状態に応じたスポーン間隔を算出する
	static int16_t CalcSpawnInterval(const GameState& gameState) {
		if (gameState.CurrentDifficulty() == Difficulty::Easy) {
			return gameState.CanAdv() ? 50 : 100;
		}
		else if (gameState.CurrentDifficulty() == Difficulty::Hard) {
			return gameState.CanAdv() ? 50 : 70;
		}
		return 50;
	}

	// 経過時間に応じて同時出現数を更新する（難易度ごとに上限あり）
	static void UpdateRyunenCount(int16_t spawnInterval, SceneServices& services) {
		if (services.gameState.CurrentDifficulty() == Difficulty::Easy) {
			if (services.gameState.RyunenCount() < MAX_RYUNEN - 45)
				services.gameState.SetRyunenCount(static_cast<int16_t>(services.gameState.GameCnt() / spawnInterval));
		}
		else if (services.gameState.CurrentDifficulty() == Difficulty::Hard) {
			if (services.gameState.RyunenCount() < MAX_RYUNEN - 20)
				services.gameState.SetRyunenCount(static_cast<int16_t>(services.gameState.GameCnt() / spawnInterval));
		}
		else {
			if (services.gameState.RyunenCount() < MAX_RYUNEN)
				services.gameState.SetRyunenCount(static_cast<int16_t>(services.gameState.GameCnt() / spawnInterval));
		}
	}

	// スポーン条件を満たす留年を1体スポーンする
	static void SpawnRyunen(SceneManager& manager, SceneServices& services) {
		for (int i = 0; i < services.gameState.RyunenCount(); ++i) {
			if ((services.objMgr.GetRyunen(i).isVisible() == false ||
				services.objMgr.GetRyunen(i).getPositionFlg() == false) &&
				(services.gameState.CanSpawn() && manager.getResultType() == GameResult::None)) {
				float spawnX;
				if (i == 0) {
					spawnX = 50.0f;
				}
				else if (i == 1) {
					spawnX = 1190.0f;
				}
				else {
					spawnX = static_cast<float>(rand() % (RIGHT_FIX - 100) + 100);
				}
				services.objMgr.GetRyunen(i).SpawnAt(spawnX);
				break;
			}
		}
	}

	// プレイヤーと留年の行動を1フレーム分更新する
	static void UpdateEntities(SceneManager& manager, const KeyManager& key, SceneServices& services) {
		if (manager.getCurrentSceneType() == SceneId::Playing && manager.getResultType() == GameResult::None) {
			services.objMgr.GetPlayer().Action(key, manager, services.objMgr, services.gameState);
		}

		for (int i = 0; i < MAX_RYUNEN; ++i) {
			if (services.objMgr.GetRyunen(i).isVisible()) {
				services.objMgr.GetRyunen(i).Action(manager, services.gameState);
			}
		}
	}

	// ライフが尽きたタイミングでリザルトを確定する
	static void EvaluateGameResult(SceneManager& manager, SceneServices& services) {
		if (services.objMgr.GetPlayer().getLife() <= 0 &&
			manager.getCurrentSceneType() == SceneId::Playing &&
			manager.getResultType() == GameResult::None) {
			if (services.gameState.CanAdv()) {
				manager.setResultType(GameResult::Clear);
			}
			else {
				manager.setResultType(GameResult::Fail);
				// 留年確定時は追い込み数を表示しないため、
				// 回避数表示を下にずらして調整する
				services.objMgr.GetUIText(ID_TEXT_RESULT_EVADE_RYUNEN).addPosition(0, 45);
			}
			services.gameState.SetCanSpawn(false);
			services.gameState.SetGameCnt(0);
		}
	}

	// 回避目標達成時に進級確定状態へ移行する
	static void EvaluateCanAdv(SceneManager& manager, SceneServices& services) {
		if (services.gameState.EvadeCnt() >= services.gameState.EvadeGoal() &&
			!services.gameState.CanAdv() &&
			manager.getCurrentSceneType() == SceneId::Playing) {
			for (int i = 0; i < MAX_RYUNEN; i++) {
				services.objMgr.GetRyunen(i).setStatusType(RyunenStatus::Landed);
			}
			services.gameState.SetCanSpawn(false);
			services.gameState.SetRyunenCount(5);
			services.gameState.SetGameCnt(0);
			services.gameState.SetCanAdv(true);
			services.objMgr.GetPlayer().setLife(1);
		}
	}

	// 進級確定時のカットイン演出を1フレーム分更新する
	static void UpdateCanAdvCutIn(SceneManager& manager, SceneServices& services) {
		if (services.gameState.CanAdv() &&
			!services.gameState.CanSpawn() &&
			manager.getCurrentSceneType() == SceneId::Playing) {
			auto& img = services.objMgr.GetUIImage(ID_IMAGE_CANADV);
			img.setVisible(true);
			img.setActCnt(img.getActCnt() + 1);
			if (img.getActCnt() <= 11) {
				img.setPosition(img.getPosition().x - 80.0f, img.getPosition().y - 2.3f);
			}
			else if (img.getActCnt() <= 70) {
				img.setPosition(img.getPosition().x - 1.2f, img.getPosition().y - 0.2f);
			}
			else if (img.getActCnt() <= 120) {
				img.setPosition(img.getPosition().x - 80.0f, img.getPosition().y - 2.3f);
			}
			else {
				services.gameState.SetCanSpawn(true);
			}
		}
	}

	// 全留年が画面外かどうかを確認する
	static bool CheckAllOffScreen(SceneServices& services) {
		for (int i = 0; i < MAX_RYUNEN; i++) {
			if (services.objMgr.GetRyunen(i).isVisible()) {
				if (services.objMgr.GetRyunen(i).getStatusType() != RyunenStatus::Offscreen) {
					return false;
				}
			}
		}
		return true;
	}
};


