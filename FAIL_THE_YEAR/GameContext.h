/* =============================================================================
 * File overview:
 * - Game-wide shared state bundle.
 * ============================================================================= */
#pragma once

#include "EngineContext.h"
#include "GameState.h"
#include "ObjectManager.h"

struct GameContext {
	EngineContext engine;
	GameState gameState;
	ObjectManager objMgr;
};
