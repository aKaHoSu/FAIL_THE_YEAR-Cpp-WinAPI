/* =============================================================================
 * ファイル概要:
 * - 共通ユーティリティ関数 Utils の宣言を記述する。
 * ============================================================================= */
#pragma once
#include <cstdint>
#include <cstdlib>

namespace Utils {
	// 任意の整数型に対応した、val の符号をランダムに決定する関数。
	// 主に留年オブジェクト着地後のバウンド方向決定に使用する。
	template<typename T>
	T randSigh(T val) {
		return val * (rand() % 2 == 0 ? static_cast<T>(1) : static_cast<T>(-1));
	}
}

