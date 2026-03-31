/* =============================================================================
 * ファイル概要:
 * - テキストを使用するUIオブジェクトを管理する UI_Text クラスを宣言する。
 * ============================================================================= */
#pragma once
#include <windows.h>
#include "ObjectData.h"

class GameState;
class ObjectManager;

// UI_Text クラスはテキスト表示を行う UI オブジェクト。
// Common.h を直接含めずに宣言する。
class UI_Text : public ObjectData {
public:
	UI_Text();
	~UI_Text();

	// 各テキストの初期化処理
	void setInitData(HINSTANCE hInst, HWND hWnd, int16_t id);

	// フォントの作成・格納・取得を行う関数と、
	// 破棄する関数
	static void InitializeFont(HWND hWnd);
	void setSizeFont(HFONT font);
	static const HFONT getSizeFont(const ObjectManager& objMgr, int16_t id);

	static void FinalizeFont();	// WM_DESTROYで呼び出される

	void refreshDrawState(const GameState& gameState);
	static void RefreshAllDrawStates(ObjectManager& objMgr, const GameState& gameState);

	// ActCnt が BLINK_INTERVAL を超えたら非表示にし、
	// さらに超えたらリセットして点滅を繰り返す
	static void UpdateBlink(UI_Text& text);
	const WCHAR* getRenderText() const { return m_renderText; }
	COLORREF getDrawColor() const { return m_drawColor; }
	int16_t getDrawOffsetX() const { return m_drawOffsetX; }
	int16_t getDrawOffsetY() const { return m_drawOffsetY; }
	int16_t getRepeatCount() const { return m_repeatCount; }
	int16_t getRepeatOffsetY() const { return m_repeatOffsetY; }

protected:
	// フォント(小/中/大/特大)
	static HFONT s_hFont_Small;
	static HFONT s_hFont_Easy;
	static HFONT s_hFont_Large;
	static HFONT s_hFont_ExtraLarge;
	static HFONT s_hFont_Lines;

	// そのテキストがどのサイズを用いるのかが格納される変数
	HFONT m_SizeFont;
	WCHAR m_renderText[256];
	COLORREF m_drawColor;
	int16_t m_drawOffsetX;
	int16_t m_drawOffsetY;
	int16_t m_repeatCount;
	int16_t m_repeatOffsetY;
};

