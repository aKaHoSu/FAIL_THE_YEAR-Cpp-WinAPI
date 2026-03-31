/* =============================================================================
 * ファイル概要:
 * - EngineContext の初期化処理を実装する。
 * ============================================================================= */
#include "EngineContext.h"

// 各メンバを安全な初期値で設定する
// 実際のハンドルは WM_CREATE 内で取得される
EngineContext::EngineContext()
	: hInst(NULL)					// WM_CREATE で GetWindowLongPtr により取得
	, hWnd(NULL)					// WM_CREATE で CreateWindow の戻り値として取得
	, hDCBack(NULL) 				// WM_CREATE で CreateCompatibleDC により作成
	, tmf(0)						// タイマー割り込み防止フラグ
{}									// debugCmdLine はインクラス初期値で初期化
