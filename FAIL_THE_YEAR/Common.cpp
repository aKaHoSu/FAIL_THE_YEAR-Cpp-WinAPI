/* =============================================================================
 * ファイル概要:
 * - 共有インスタンスの宣言。
 * ============================================================================= */
#include "Common.h"
#include "EngineContext.h"
#include "GameState.h"
#include "ObjectManager.h"

EngineContext g_engine;  // エンジン関連
						 // （WinAPIハンドル、シーン管理、キー入力管理）

GameState g_gameState;   // ゲーム進行状態

ObjectManager g_objMgr;  // ゲームオブジェクト管理

