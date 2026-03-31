/* =============================================================================
 * ファイル概要:
 * - デバッグコマンドライン機能に関連する状態をまとめた構造体を定義する。
 * - EngineContext から分離し、単一責務の原則に従う。
 * ============================================================================= */
#pragma once
#include <string>

// デバッグコマンドライン関連の状態をまとめた構造体
struct DebugCommandLine {
	bool         isVisible               = false;	// コマンドライン表示状態
	std::wstring buffer;							// コマンド入力バッファ
	std::wstring status;							// 実行結果表示メッセージ
	int          statusTimer             = 0;		// 実行結果表示の残りフレーム
	bool         suppressEscUntilRelease = false;	// コマンドライン終了直後のEscシーン遷移抑止
};
