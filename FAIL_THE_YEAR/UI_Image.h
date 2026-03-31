/* =============================================================================
 * ファイル概要:
 * - 画像を使用するUIオブジェクトを管理する UI_Image クラスを宣言。
 * ============================================================================= */

#pragma once
#include <windows.h>
#include "ObjectData.h"

class UI_Image : public ObjectData {
public:
	UI_Image();
	~UI_Image();

	// 初期化処理。
	// ビットマップやサイズ、位置などを設定する
	void setInitData(HINSTANCE hInst, HWND hWnd, int16_t id);
};
