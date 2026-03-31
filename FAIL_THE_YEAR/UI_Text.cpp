/* =============================================================================
 * ファイル概要:
 * - テキスト UI オブジェクト UI_Text の実装を記述する。
 * ============================================================================= */
#include "Common.h"
#include "UI_Text.h"
#include "ObjectManager.h"
#include "GameState.h"
#include <cwchar>
#include <cstdlib>

#pragma warning(push)
#pragma warning(disable: 4828)  // resource.h は ANSI エンコードのため UTF-8 モードでは C4828 が発生する
#include "resource.h"
#pragma warning(pop)

// UI_Text の実装
UI_Text::UI_Text() {
	m_SizeFont = nullptr;
	m_drawColor = RGB(0, 0, 0);
	m_drawOffsetX = 0;
	m_drawOffsetY = 0;
	m_repeatCount = 1;
	m_repeatOffsetY = 0;
	for (int i = 0; i < 256; i++) {
		m_renderText[i] = L'\0';
	}
}
UI_Text::~UI_Text() {}

// 静的フォント変数の定義
HFONT UI_Text::s_hFont_Small = nullptr;
HFONT UI_Text::s_hFont_Easy = nullptr;
HFONT UI_Text::s_hFont_Large = nullptr;
HFONT UI_Text::s_hFont_ExtraLarge = nullptr;
HFONT UI_Text::s_hFont_Lines = nullptr;

// --- テキストUI初期化用データテーブル ---
namespace {

	// フォントサイズの識別子
	enum class FontType : int16_t { 
		Small, 
		Easy, 
		Large, 
		ExtraLarge, 
		Lines };

	struct TextInitEntry {
		int16_t   id;
		FontType  font;
		int16_t   stringId;
		float     x, y;
	};

	static const TextInitEntry kTextEntries[] = {

		// --- メインメニュー ---
		{ ID_TEXT_TITLE,                  FontType::Easy,       IDS_TITLE,                  180, 500 },
		{ ID_TEXT_MENU_PLAY,              FontType::Large,      IDS_MENU_PLAY,              390, 270 },
		{ ID_TEXT_MENU_TUTORIAL,          FontType::Large,      IDS_MENU_TUTORIAL,          355, 370 },
		{ ID_TEXT_MENU_BACK,              FontType::Small,      IDS_MENU_BACK,                0,   0 },

		// --- 難易度選択 ---
		{ ID_TEXT_SELDIFF,                FontType::Easy,       IDS_SELDIFF,                473,  90 },
		{ ID_TEXT_SELDIFF_BACK,           FontType::Small,      IDS_SELDIFF_BACK,             0,   0 },
		{ ID_TEXT_SELDIFF_NORMAL,         FontType::Large,      IDS_SELDIFF_NORMAL,         385, 270 },
		{ ID_TEXT_SELDIFF_HARD,           FontType::Large,      IDS_SELDIFF_HARD,           385, 370 },

		// --- リザルト ---
		{ ID_TEXT_RESULT_CLEAR,           FontType::ExtraLarge, IDS_RESULT_CLEAR,           155,  50 },
		{ ID_TEXT_RESULT_FAIL,            FontType::ExtraLarge, IDS_RESULT_FAIL,            295,  50 },
		{ ID_TEXT_RESULT_EVADE_RYUNEN,    FontType::Large,      IDS_RESULT_EVADE_RYUNEN,    240, 280 },
		{ ID_TEXT_RESULT_EVADE_SHINKYU,   FontType::Large,      IDS_RESULT_EVADE_SHINKYU,   240, 370 },
		{ ID_TEXT_RESULT_NEXT,            FontType::Easy,       IDS_RESULT_NEXT,            430, 520 },
		{ ID_TEXT_AFTER_RESULT_RETRY,     FontType::Large,      IDS_AFTER_RESULT_RETRY,     390, 270 },
		{ ID_TEXT_AFTER_RESULT_TITLE,     FontType::Large,      IDS_AFTER_RESULT_TITLE,     355, 370 },
		{ ID_TEXT_BEST_EVADE_CNT,         FontType::Small,      IDS_BEST_EVADE_CNT,         800,   0 },

		// --- チュートリアル ---
		{ ID_TEXT_T_TITLE,                FontType::ExtraLarge, IDS_T_TITLE,                 70,  50 },
		{ ID_TEXT_T_CONSEPT1,             FontType::Large,      IDS_T_CONSEPT1,              70, 290 },
		{ ID_TEXT_T_CONSEPT2,             FontType::Easy,       IDS_T_CONSEPT2,             180, 470 },
		{ ID_TEXT_T_MOVE,                 FontType::Large,      IDS_T_MOVE,                  70,  50 },
		{ ID_TEXT_T_M_EXPL1,              FontType::Easy,       IDS_T_M_EXPL1,               70, 200 },
		{ ID_TEXT_T_M_EXPL2,              FontType::Easy,       IDS_T_M_EXPL2,               70, 400 },
		{ ID_TEXT_T_M_EXPL3,              FontType::Easy,       IDS_T_M_EXPL3,              170, 450 },
		{ ID_TEXT_T_RYUNEN,               FontType::Large,      IDS_T_RYUNEN,                70,  50 },
		{ ID_TEXT_T_R_EXPL1,              FontType::Easy,       IDS_T_R_EXPL1,               70, 200 },
		{ ID_TEXT_T_R_EXPL2,              FontType::Easy,       IDS_T_R_EXPL2,               70, 280 },
		{ ID_TEXT_T_R_EXPL3,              FontType::Easy,       IDS_T_R_EXPL3,               70, 470 },
		{ ID_TEXT_T_LIFE,                 FontType::Large,      IDS_T_LIFE,                  70,  50 },
		{ ID_TEXT_T_L_EXPL1,              FontType::Easy,       IDS_T_L_EXPL1,               70, 200 },
		{ ID_TEXT_T_L_EXPL2,              FontType::Easy,       IDS_T_L_EXPL2,              170, 250 },
		{ ID_TEXT_T_L_EXPL3,              FontType::Easy,       IDS_T_L_EXPL3,               70, 330 },
		{ ID_TEXT_T_L_EXPL4,              FontType::Easy,       IDS_T_L_EXPL4,               70, 500 },
		{ ID_TEXT_T_L_EXPL5,              FontType::Easy,       IDS_T_L_EXPL5,              170, 550 },
		{ ID_TEXT_T_BOMB,                 FontType::Large,      IDS_T_BOMB,                  70,  50 },
		{ ID_TEXT_T_B_EXPL1,              FontType::Easy,       IDS_T_B_EXPL1,               70, 200 },
		{ ID_TEXT_T_B_EXPL2,              FontType::Easy,       IDS_T_B_EXPL2,              170, 250 },
		{ ID_TEXT_T_B_EXPL3,              FontType::Easy,       IDS_T_B_EXPL3,               70, 450 },
		{ ID_TEXT_T_CLEAR,                FontType::Large,      IDS_T_CLEAR,                 70,  50 },
		{ ID_TEXT_T_C_EXPL1,              FontType::Easy,       IDS_T_C_EXPL1,               70, 200 },
		{ ID_TEXT_T_C_EXPL2,              FontType::Easy,       IDS_T_C_EXPL2,              170, 250 },
		{ ID_TEXT_T_C_EXPL3,              FontType::Small,      IDS_T_C_EXPL3,              490, 350 },
		{ ID_TEXT_T_C_EXPL4,              FontType::Lines,      IDS_T_C_EXPL4,              350, 463 },
		{ ID_TEXT_T_AFTER_CLEAR,          FontType::Large,      IDS_T_AFTER_CLEAR,           70,  50 },
		{ ID_TEXT_T_AC_EXPL1,             FontType::Easy,       IDS_T_AC_EXPL1,              70, 230 },
		{ ID_TEXT_T_AC_EXPL2,             FontType::Easy,       IDS_T_AC_EXPL2,              70, 350 },
		{ ID_TEXT_T_AC_EXPL3,             FontType::Easy,       IDS_T_AC_EXPL3,              70, 410 },
		{ ID_TEXT_T_END,                  FontType::Large,      IDS_T_END,                   70,  50 },
		{ ID_TEXT_T_E_EXPL1,              FontType::Easy,       IDS_T_E_EXPL1,               70, 230 },
		{ ID_TEXT_T_E_EXPL2,              FontType::Lines,      IDS_T_E_EXPL2,              580, 370 },
		{ ID_TEXT_T_BACK,                 FontType::Small,      IDS_T_BACK,                   0, 620 },
		{ ID_TEXT_T_NEXT,                 FontType::Small,      IDS_T_NEXT,                 935, 620 },
		{ ID_TEXT_T_BACK_TITLE,           FontType::Small,      IDS_T_BACK_TITLE,           854, 620 },

		// --- Nightmare ---
		{ ID_TEXT_SELDIFF_NIGHTMARE,      FontType::Large,      IDS_SELDIFF_NIGHTMARE,     1150, 320 },
		{ ID_TEXT_AFTER_RESULT_NIGHTMARE, FontType::Large,      IDS_AFTER_RESULT_NIGHTMARE, -40, -40 },
		{ ID_TEXT_ESC_NM,                 FontType::Lines,      IDS_ESC_NM,                 490, 305 },

		// --- Exit ---
		{ ID_TEXT_E_NEW_GUEST,           FontType::Lines,      IDS_E_NEW_GUEST,             510, 305 },
		{ ID_TEXT_E_THANKS,              FontType::Lines,      IDS_E_THANKS,                440, 305 },
		{ ID_TEXT_E_NM_ESC,              FontType::Lines,      IDS_E_NM_ESC,                510, 305 },
		{ ID_TEXT_E_NM_CLEAR,            FontType::Lines,      IDS_E_NM_CLEAR,              470, 305 },

		// --- ポーズ ---
		{ ID_TEXT_PAUSE,                 FontType::ExtraLarge, IDS_PAUSE,                   158, 230 },
	};

} // namespace

// 初期描画
void UI_Text::setInitData(HINSTANCE hInst, HWND hWnd, int16_t id) {
	clearSourceText();
	wcscpy_s(m_renderText, 256, L"");
	m_drawColor = RGB(0, 0, 0);
	m_drawOffsetX = 0;
	m_drawOffsetY = 0;
	m_repeatCount = 1;
	m_repeatOffsetY = 0;

	// FontType → HFONT 変換用の配列
	const HFONT fonts[] = {
		s_hFont_Small,
		s_hFont_Easy, 
		s_hFont_Large,
		s_hFont_ExtraLarge, 
		s_hFont_Lines
	};

	for (const auto& e : kTextEntries) {
		if (e.id == id) {
			setId(id);
			setSizeFont(fonts[static_cast<int>(e.font)]);
			setText(hInst, e.stringId);
			setPosition(e.x, e.y);
			return;
		}
	}
}

// 可変表示が必要なテキストの描画文字列/色/震え情報を更新する
void UI_Text::refreshDrawState(const GameState& gameState) {
	m_drawColor = (getId() == ID_TEXT_RESULT_CLEAR) ? RGB(0, 255, 0) : RGB(0, 0, 0);
	m_drawOffsetX = 0;
	m_drawOffsetY = 0;
	m_repeatCount = 1;
	m_repeatOffsetY = 0;

	// 描画する文字列を更新（回避数などの値を表示するテキスト）
	if (getId() == ID_TEXT_BEST_EVADE_CNT) {
		swprintf_s(m_renderText, sourceText(), gameState.BestEvadeCnt());
	}
	else if (getId() == ID_TEXT_RESULT_EVADE_RYUNEN) {
		if (gameState.CanAdv()) {
			swprintf_s(m_renderText, sourceText(), gameState.EvadeGoal());
		}
		else {
			swprintf_s(m_renderText, sourceText(), gameState.EvadeCnt());
		}
	}
	else if (getId() == ID_TEXT_RESULT_EVADE_SHINKYU) {
		swprintf_s(m_renderText, sourceText(), gameState.EvadeCnt() - gameState.EvadeGoal());
	}
	else {
		wcscpy_s(m_renderText, 256, sourceText());
	}

	// 文字の震え
	if (getId() == ID_TEXT_AFTER_RESULT_NIGHTMARE) {
		m_repeatCount = 15;
		m_repeatOffsetY = 50;

		const int16_t intensity = gameState.EscCnt();
		if (intensity == 0) {
			m_drawOffsetX = 0;
			m_drawOffsetY = 0;
		}
		else if (intensity == 1) {
			m_drawOffsetX = (rand() % 8) - (8 / 2);
			m_drawOffsetY = (rand() % 8) - (8 / 2);
		}
		else {
			m_drawOffsetX = ((rand() % intensity) - (intensity / 2)) * 2;
			m_drawOffsetY = ((rand() % intensity) - (intensity / 2)) * 2;
		}
	}
}

// すべてのテキストUIに対して描画状態更新を適用する
void UI_Text::RefreshAllDrawStates(ObjectManager& objMgr, const GameState& gameState) {
	for (int i = ID_BEGIN_TEXT; i < ID_BEGIN_TEXT + MAX_UI_TEXT; ++i) {
		objMgr.GetUIText(i).refreshDrawState(gameState);
	}
}

// ActCnt が BLINK_INTERVAL を超えたら非表示にし、
// さらに超えたらリセットして点滅を繰り返す
void UI_Text::UpdateBlink(UI_Text& text) {
	if (text.getActCnt() > BLINK_INTERVAL) {
		text.setVisible(false);
		if (text.getActCnt() > BLINK_INTERVAL * 2 - BLINK_INTERVAL / 2) {
			text.setActCnt(0);
		}
	}
	else if (text.getActCnt() > 0) {
		text.setVisible(true);
	}
	text.setActCnt(text.getActCnt() + 1);
}

// フォントの作成・登録
void UI_Text::InitializeFont(HWND hWnd) {
	if (UI_Text::s_hFont_Small != NULL &&
		UI_Text::s_hFont_Easy != NULL &&
		UI_Text::s_hFont_Large != NULL &&
		UI_Text::s_hFont_ExtraLarge != NULL) return;

	HDC hDC = GetDC(hWnd);

	// --- フォントの作成、格納
	// Small
	UI_Text::s_hFont_Small = CreateFontW(
		// 20pt を DPI に応じてピクセルに変換
		// （px = pt × DPI / 72）
		-MulDiv(20, GetDeviceCaps(hDC, LOGPIXELSY), 72),

		0, 0, 0,
		FW_BOLD,
		FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"メイリオ"
	);

	// Easy
	UI_Text::s_hFont_Easy = CreateFontW(
		// 30pt を DPI に応じてピクセルに変換
		// （px = pt × DPI / 72）`
		-MulDiv(30, GetDeviceCaps(hDC, LOGPIXELSY), 72),

		0, 0, 0,
		FW_BOLD,
		FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"メイリオ"
	);

	// Large
	UI_Text::s_hFont_Large = CreateFontW(
		// 50pt を DPI に応じてピクセルに変換
		// （px = pt × DPI / 72）
		-MulDiv(50, GetDeviceCaps(hDC, LOGPIXELSY), 72),

		0, 0, 0,
		FW_BOLD,
		FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"メイリオ"
	);

	// ExtraLarge
	UI_Text::s_hFont_ExtraLarge = CreateFontW(
		// 110pt を DPI に応じてピクセルに変換
		// （px = pt × DPI / 72）
		-MulDiv(110, GetDeviceCaps(hDC, LOGPIXELSY), 72),

		0, 0, 0,
		FW_HEAVY,
		FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"游明朝"
	);

	// s_hFont_Lines
	UI_Text::s_hFont_Lines = CreateFontW(
		// 20pt を DPI に応じてピクセルに変換
		// （px = pt × DPI / 72）
		-MulDiv(20, GetDeviceCaps(hDC, LOGPIXELSY), 72),

		0, 0, 0,
		FW_BOLD,
		FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"游明朝"
	);

	ReleaseDC(hWnd, hDC);
}

// 各テキストにどのサイズのフォントを適応するかセットする
void UI_Text::setSizeFont(HFONT font) {
	m_SizeFont = font;
}

// 各テキストのフォント設定を呼び出す
const HFONT UI_Text::getSizeFont(const ObjectManager& objMgr, int16_t id) {
	return objMgr.GetUIText(id).m_SizeFont;
}

// フォントの破棄
void UI_Text::FinalizeFont() {
	if (UI_Text::s_hFont_Small) {
		DeleteObject(UI_Text::s_hFont_Small);
		UI_Text::s_hFont_Small = nullptr;
	}
	if (UI_Text::s_hFont_Easy) {
		DeleteObject(UI_Text::s_hFont_Easy);
		UI_Text::s_hFont_Easy = nullptr;
	}
	if (UI_Text::s_hFont_Large) {
		DeleteObject(UI_Text::s_hFont_Large);
		UI_Text::s_hFont_Large = nullptr;
	}
	if (UI_Text::s_hFont_ExtraLarge) {
		DeleteObject(UI_Text::s_hFont_ExtraLarge);
		UI_Text::s_hFont_ExtraLarge = nullptr;
	}
	if (UI_Text::s_hFont_Lines) {
		DeleteObject(UI_Text::s_hFont_Lines);
		UI_Text::s_hFont_Lines = nullptr;
	}
}


