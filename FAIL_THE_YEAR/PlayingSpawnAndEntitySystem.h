/* =============================================================================
 * ファイル概要:
 * - Playing シーンのスポーン・エンティティ更新処理を管理する。
 * ============================================================================= */
#pragma once

#include <cstdlib>
#include "Common.h"
#include "Scene.h"
#include "SceneServices.h"
#include "KeyManager.h"

class PlayingSpawnAndEntitySystem {
public:
	// 難易度と進級確定状態に応じたスポーン間隔を算出する
	static int16_t CalcSpawnInterval(const GameState& gameState) {
		// 進級確定後は Easy/Hard でもスポーン間隔を短縮する
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
		// gameCnt / spawnInterval により、時間経過に応じて出現上限を段階的に増やす
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
			// 未表示または未配置のスロットを探し、プレイ中のみ1体ずつ補充する
			if ((services.objMgr.GetRyunen(i).isVisible() == false ||
				services.objMgr.GetRyunen(i).getPositionFlg() == false) &&
				(services.gameState.CanSpawn() && manager.getResultType() == GameResult::None)) {
				float spawnX;
				// 先頭2体は端固定、3体目以降はランダム配置
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
				// 1フレームに1体のみ生成する
				break;
			}
		}
	}

	// プレイヤーと留年の行動を1フレーム分更新する
	static void UpdateEntities(SceneManager& manager, const KeyManager& key, SceneServices& services) {
		// 勝敗未確定中のみプレイヤー操作を更新
		if (manager.getCurrentSceneType() == SceneId::Playing && manager.getResultType() == GameResult::None) {
			services.objMgr.GetPlayer().Action(key, manager, services.objMgr, services.gameState);
		}

		// 表示中の留年のみ挙動更新
		for (int i = 0; i < MAX_RYUNEN; ++i) {
			if (services.objMgr.GetRyunen(i).isVisible()) {
				services.objMgr.GetRyunen(i).Action(manager, services.gameState);
			}
		}
	}
};
