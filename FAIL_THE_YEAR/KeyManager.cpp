/* =============================================================================
 * ファイル概要:
 * - キー入力取得と押下状態判定処理を実装する。
 * ============================================================================= */
#include <windows.h>
#include <stdio.h>
#include "KeyManager.h"

KeyManager::KeyManager()
	: key_(0)
	, keyBack_(0)
{}

// 押されたとき（１度だけ）
// 前フレームで未押下かつ今フレームで押下されたビットのみが立つ
bool KeyManager::IsTriggered(KeyCode code) const
{
	return ((key_ & (~keyBack_)) & (int)code) != 0;
}

// 押されたとき(そのまま押しっぱなしも込み)
// 現フレームの入力ビットに対象キーのビットが立っているか確認する
bool KeyManager::IsPress(KeyCode code) const
{
	return (key_ & (int)code) != 0;
}

// 離された時
// 前フレームで押されていて今フレームでは押されていないビットが立つ
bool KeyManager::IsRelease(KeyCode code) const
{
	return (((~key_) & keyBack_) & (int)code) != 0;
}

// 毎フレーム呼び出す。
// 前フレームの入力を保存し、現フレームの全キー状態を取得する
void KeyManager::Update()
{
	keyBack_ = key_;  // 前フレームの入力状態を保存
	key_ = 0;        // 現フレームの入力ビットをリセット

	key_ |= GetBits(VK_UP, Up);
	key_ |= GetBits('W', Up);

	key_ |= GetBits(VK_DOWN, Down);
	key_ |= GetBits('S', Down);

	key_ |= GetBits(VK_LEFT, Left);
	key_ |= GetBits('A', Left);

	key_ |= GetBits(VK_RIGHT, Right);
	key_ |= GetBits('D', Right);

	key_ |= GetBits(VK_SHIFT, Shift);
	key_ |= GetBits(VK_RETURN, Enter);
	key_ |= GetBits(VK_BACK, Backspace);
	key_ |= GetBits(VK_ESCAPE, Esc);

	key_ |= GetBits('Z', Z);
	key_ |= GetBits('H', H);
	key_ |= GetBits('G', G);
	key_ |= GetBits('F', F);
	key_ |= GetBits('C', C);

}

// 仮想キーコード nVirtKey の押下状態を取得し、
// 押されていれば code のビットを返す
// GetKeyState の戻り値が負（符号ビットが立っている）とき、
// そのキーは押されている
int16_t KeyManager::GetBits(int16_t nVirtKey, KeyCode code)
{
	return GetKeyState(nVirtKey) < 0 ? (int16_t)code : 0;
}
