/* =============================================================================
 * File overview:
 * - Shared instance definitions.
 * ============================================================================= */
#include "Common.h"
#include "GameContext.h"

GameContext g_gameContext;
EngineContext& g_engine = g_gameContext.engine;
GameState& g_gameState = g_gameContext.gameState;
ObjectManager& g_objMgr = g_gameContext.objMgr;
