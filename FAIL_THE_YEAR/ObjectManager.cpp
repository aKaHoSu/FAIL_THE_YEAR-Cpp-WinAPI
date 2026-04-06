/* =============================================================================
 * ファイル概要:
 * - ObjectManager クラスの実装
 * ============================================================================= */
#include "ObjectManager.h"
#include "GameState.h"

namespace {
	// プレイヤーと留年の衝突判定を行う際に使用する関数。
	bool isCollision(const ObjectData& lhs, const ObjectData& rhs) {
		const OBJHITBOX::Rect lhsRect = lhs.getCollision().GetRect(
			lhs.getPosition().x, 
			lhs.getPosition().y);
		const OBJHITBOX::Rect rhsRect = rhs.getCollision().GetRect(
			rhs.getPosition().x, 
			rhs.getPosition().y);

		const bool xOverlap = (lhsRect.x1 < rhsRect.x2) && (lhsRect.x2 > rhsRect.x1);
		const bool yOverlap = (lhsRect.y1 < rhsRect.y2) && (lhsRect.y2 > rhsRect.y1);
		return xOverlap && yOverlap;
	}
}

ObjectManager::ObjectManager() {}

// プレイヤーと落下中の留年オブジェクトの当たり判定を実行し、
// 必要なステータス更新（ダメージ・着地・ゲームオーバー）を反映する
bool ObjectManager::checkHitBetweenPlayerAndRyunen(const GameState& gameState) {
	for (int i = 0; i < MAX_RYUNEN; ++i) {

		// 留年が無効または非表示の場合はスキップ
		if (!ryunen[i].isVisible()) { continue; }

		// 留年のステータスが落下中以外の場合はスキップ
		if (ryunen[i].getStatusType() != RyunenStatus::Falling) { continue; }

		// player と留年の衝突判定
		if (isCollision(player, ryunen[i]) && !gameState.IsGodMode()) {

			// 留年の状態を着地に変更（二重ヒットを防ぐ）
			ryunen[i].setStatusType(RyunenStatus::Landed);

			// Nightmare モードかつ進級未確定の場合はゲームオーバー、
			if (gameState.IsNightmare() && !gameState.CanAdv()) {
				player.setLife(0);
			}

			// それ以外はダメージ
			else {
				player.setStatusType(PlayerStatus::Damage);
			}

			// 条件を通った場合は true を返す
			return true;
		}
	}

	// すべての条件を通らなかった場合は false を返す
	return false;
}


