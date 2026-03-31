/* =============================================================================
 * ファイル概要:
 * - 画像を用いる UI オブジェクト UI_Image の実装を記述する。
 * ============================================================================= */

#include "Common.h"
#include "UI_Image.h"

#pragma warning(push)
#pragma warning(disable: 4828)  // resource.h は ANSI エンコードのため UTF-8 モードでは C4828 が発生する
#include "resource.h"
#pragma warning(pop)

// UI_Image の実装
UI_Image::UI_Image() {}
UI_Image::~UI_Image() {}

// --- 画像UI初期化用データテーブル ---
namespace {

struct ImageInitEntry {
	int16_t id;
	int16_t bmpId;
	int16_t width, height;
	float   x, y;
};

static const ImageInitEntry kImageEntries[] = {
	{ ID_IMAGE_LOGO, IDB_LOGO, LOGO_WIDTH, LOGO_HEIGHT,
		(float)(WINDOW_W / 2) - (float)(LOGO_WIDTH / 2), 70 },
	{ ID_IMAGE_READY, IDB_READY, WINDOW_W, WINDOW_H, 50, 0 },
	{ ID_IMAGE_START, IDB_START, WINDOW_W, WINDOW_H, 0, -30 },
	{ ID_IMAGE_CANADV, IDB_CANADV, WINDOW_W, WINDOW_H, 900, 0 },
};

} // namespace

// IDに対応する画像UIの初期データをテーブルから設定する
void UI_Image::setInitData(HINSTANCE hInst, HWND hWnd, int16_t id) {
	for (const auto& e : kImageEntries) {
		if (e.id == id) {
			setId(id);
			setBmp(hInst, e.bmpId);
			setSize(e.width, e.height);
			setPosition(e.x, e.y);
			return;
		}
	}
}

