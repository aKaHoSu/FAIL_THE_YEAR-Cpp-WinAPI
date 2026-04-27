/* =============================================================================
 * ファイル概要:
 * - プレイ中シーンの更新処理を記述する。
 * - 生成・行動・判定・遷移を管理する。
 * ============================================================================= */
#pragma once

#include "Common.h"
#include "Scene.h"
#include "KeyManager.h"
#include "SceneServices.h"
#include "PlayingSceneLifecycle.h"
#include "PlayingSpawnAndEntitySystem.h"
#include "PlayingOutcomeSystem.h"

class PlayingSceneState {
public:
	// Zキー入力を受け取り、プレイヤーのボムを発動する
	static void HandleInput(SceneManager& manager, const KeyManager& key, SceneServices& services) {
		if (key.IsTriggered(KeyManager::Z) || key.IsTriggered(KeyManager::P)) {
			// ボム使用可否の判定は Player::useBomb 側で行う
			services.objMgr.GetPlayer().useBomb(services.objMgr);
		}
	}

	// Playing中の1フレーム更新（生成・行動・判定・遷移）を行う
	static void Run(SceneManager& manager, const KeyManager& key, HINSTANCE hInst, SceneServices& services) {
		// シーン遷移直後の初期化（ポーズ解除・カウンタ初期化など）
		PlayingSceneLifecycle::InitOnEnter(manager, services);

		// ポーズ中はロジックを停止し、現在の表示状態だけを維持する
		if (manager.IsPaused()) {
			// ポーズ中は表示更新のみ行って処理を打ち切る
			PlayingSceneLifecycle::SetupPausedView(services);
			return;
		}

		// 難易度・進行状態に応じたスポーン進行を更新
		const int16_t spawnInterval = PlayingSpawnAndEntitySystem::CalcSpawnInterval(services.gameState);
		services.gameState.IncrementGameCnt();
		PlayingSpawnAndEntitySystem::UpdateRyunenCount(spawnInterval, services);

		// エンティティ更新（プレイヤー/留年）と衝突判定
		services.objMgr.GetPlayer().setVisible(true);
		PlayingSpawnAndEntitySystem::SpawnRyunen(manager, services);
		PlayingSpawnAndEntitySystem::UpdateEntities(manager, key, services);
		services.objMgr.checkHitBetweenPlayerAndRyunen(services.gameState);

		// 勝敗判定・進級確定判定・カットイン演出更新
		PlayingOutcomeSystem::EvaluateGameResult(manager, services);
		PlayingOutcomeSystem::EvaluateCanAdv(manager, services);
		PlayingOutcomeSystem::UpdateCanAdvCutIn(manager, services);

		// 勝敗確定後、全留年が画面外になったら Result へ遷移
		if (PlayingOutcomeSystem::CheckAllOffScreen(services) &&
			(manager.getResultType() == GameResult::Clear || manager.getResultType() == GameResult::Fail)) {
			manager.setCurrentSceneType(SceneId::Result);
		}
	}
};


