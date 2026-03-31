/* =============================================================================
 * ファイル概要:
 * - ゲーム進行状態を保持する GameState の初期化の実装。
 * ============================================================================= */
#include "GameState.h"
#include "Common.h"

// 各ゲーム進行状態変数を安全な初期値で設定する
GameState::GameState()
	: gameCnt(0)								// プレイ開始からの経過カウンタ
	, ryunenCount(1)							// スポーンする留年の数
	, evadeCnt(0)								// プレイヤーが留年を回避した回数
	, evadeGoal(0)								// 進級確定に必要な回避数ノルマ（難易度で変化）
	, bestEvadeCnt(0)							// 過去の最高回避記録
	, escCnt(0)									// Nightmare AfterResult 時の Esc 連打回数
	, isFirstPlay(true)							// 初プレイフラグ
	, hasEverCleared(false)						// 一回以上クリアしているか
	, hasEverClearedNightmare(false)			// Nightmareを一回以上クリアしているか
	, hasEverEscapedNightmare(false)			// Nightmareを一回以上脱出しているか
	, isNightmare(false)						// Nightmare モードフラグ
	, canAdv(false)								// 進級確定フラグ（回避数 >= 回避数ノルマ で true）
	, canSpawn(true)							// 留年スポーン許可フラグ
	, isGodMode(false)							// ゴッドモードフラグ（デバッグ用）
	, isDspHitBox(false)						// ヒットボックス表示フラグ（デバッグ用）
	, shouldDrawNightmareFade(false)			// Nightmareフェード描画フラグ
	, nightmareFadeAlpha(0)						// Nightmareフェードのアルファ値
	, backgroundColor(0x00FFFFFF)				// 背景色（白）
	, currentDifficulty(Difficulty::Easy)		// 現在の難易度
{}
