/* =============================================================================
 * ファイル概要:
 * - キー定義と入力管理クラス KeyManager を宣言する。
 * ============================================================================= */
#pragma once
#include <iostream>

// クラスの宣言
class KeyManager {
public:
	enum KeyCode : int16_t
	{
		Up		  = (1 << 0),
		Down	  = (1 << 1),
		Left	  = (1 << 2),
		Right	  = (1 << 3),

		Shift	  = (1 << 4),
		Enter	  = (1 << 5),
		Backspace = (1 << 6),
		Esc       = (1 << 7),

		Z		  = (1 << 8),
		H		  = (1 << 9),
		G		  = (1 << 10),
		F		  = (1 << 11),
		C		  = (1 << 12),
	};

	bool IsTriggered(KeyCode code) const;	// 押した時
	bool IsPress(KeyCode code) const;		// 押しっぱなし
	bool IsRelease(KeyCode code) const;		// 離した時
	void Update();							// キー情報更新、毎フレーム呼び出し

	KeyManager();

private:
	int16_t key_;
	int16_t keyBack_;
	int16_t GetBits(int16_t nVirtKey, KeyCode code);
};
