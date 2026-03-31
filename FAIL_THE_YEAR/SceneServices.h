/* =============================================================================
 * ファイル概要:
 * - SceneState 群が参照する GameState と ObjectManager を
 *   ひとつにまとめて渡すためのヘッダ。
 * ============================================================================= */
#pragma once

#include "GameState.h"
#include "ObjectManager.h"

struct SceneServices {
	GameState& gameState;
	ObjectManager& objMgr;
};
