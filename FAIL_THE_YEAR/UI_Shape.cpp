/* =============================================================================
 * ファイル概要:
 * - 図形 UI オブジェクト UI_Shape の挙動実装を記述する。
 * ============================================================================= */
#include "UI_Shape.h"
#include "Common.h"

// UI_Shape の実装
UI_Shape::UI_Shape() {
	// 未設定 ID が描画されても壊れない状態にしておく
	m_shapeType = ShapeType::None;
	m_fillColor = RGB(0, 0, 0);
	m_borderColor = RGB(0, 0, 0);
	m_borderWidth = 1;
	m_filled = true;
	m_alpha = 0;
}
UI_Shape::~UI_Shape() {}

namespace {

// チュートリアル見出し枠の共通初期化ヘルパー
void initTutorialFrame(UI_Shape& self, int16_t id, int16_t width) {
	self.setId(id);
	self.setShapeType(UI_Shape::ShapeType::Rectangle);
	self.setSize(width, 90);
	self.setPosition(65, 50);
	self.setFilled(false);
	self.setBorderColor(RGB(30, 30, 30));
	self.setBorderWidth(3);
}

} // namespace

void UI_Shape::setInitData(HINSTANCE hInst, HWND hWnd, int16_t id) {
	switch (id) {
	case ID_SHAPE_SELECTING_TRIANGLE:
		// 項目の現在選択を示すカーソル（右向き三角形）
		setId(id);
		setShapeType(ShapeType::TriangleRight);
		setSize(36, 24);
		setPosition(320, 292);
		setFilled(true);
		setFillColor(RGB(255, 215, 0));
		setBorderColor(RGB(120, 90, 0));
		setBorderWidth(2);
		setActCnt(0);
		break;

	case ID_SHAPE_SELECTING_RECTANGLE:
		// 項目を囲う枠
		setId(id);
		setShapeType(ShapeType::Rectangle);
		setSize(483, 96);
		setPosition(300, 332);
		setFilled(false);
		setFillColor(RGB(255, 255, 255));
		setBorderColor(RGB(30, 30, 30));
		setBorderWidth(3);
		break;

	case ID_SHAPE_FRAME_SELDIFF:
		// SelectDifficultyシーンの、「難易度を選択」テキストを囲う枠
		setId(id);
		setShapeType(ShapeType::Rectangle);
		setSize(270, 75);
		setPosition(458, 80);
		setFilled(true);
		setFillColor(RGB(200, 200, 200));
		setBorderColor(RGB(30, 30, 30));
		setBorderWidth(3);
		break;

	case ID_SHAPE_VEIL:
		// ゲーム終了後の、プレイ画面を薄く覆う矩形
		setId(id);
		setShapeType(ShapeType::Rectangle);
		setSize(WINDOW_W, WINDOW_H);
		setPosition(0, 0);
		setFilled(true);
		setFillColor(RGB(100, 100, 100));
		setBorderColor(RGB(100, 100, 100));
		setBorderWidth(0);
		setAlpha(128);
		break;

	case ID_SHAPE_T_TITLE:
		// 「チュートリアル」を囲う枠
		setId(id);
		setShapeType(ShapeType::Rectangle);
		setSize(1040, 170);
		setPosition(70, 60);
		setFilled(false);
		setBorderColor(RGB(30, 30, 30));
		setBorderWidth(3);
		break;

	// チュートリアル各ステップの見出し枠（共通ヘルパーで初期化）
	case ID_SHAPE_T_MOVE:         initTutorialFrame(*this, id, 280); break;
	case ID_SHAPE_T_RYUNEN_BOMB:  initTutorialFrame(*this, id, 410); break;
	case ID_SHAPE_T_LIFE:         initTutorialFrame(*this, id, 495); break;
	case ID_SHAPE_T_CLEAR:        initTutorialFrame(*this, id, 610); break;
	case ID_SHAPE_T_END:          initTutorialFrame(*this, id, 710); break;

	default:
		break;
	}
}

// 図形UIの種別を設定する
void UI_Shape::setShapeType(ShapeType type) {
	m_shapeType = type;
}

UI_Shape::ShapeType UI_Shape::getShapeType() const {
	return m_shapeType;
}


// 塗りつぶし色を設定する
void UI_Shape::setFillColor(COLORREF color) {
	m_fillColor = color;
}

COLORREF UI_Shape::getFillColor() const {
	return m_fillColor;
}


// 枠線色を設定する
void UI_Shape::setBorderColor(COLORREF color) {
	m_borderColor = color;
}

COLORREF UI_Shape::getBorderColor() const {
	return m_borderColor;
}


// 枠線の太さを設定する
void UI_Shape::setBorderWidth(int16_t borderWidth) {
	m_borderWidth = borderWidth;
}

int16_t UI_Shape::getBorderWidth() const {
	return m_borderWidth;
}


// 塗りつぶし有無を設定する
void UI_Shape::setFilled(bool filled) {
	m_filled = filled;
}

bool UI_Shape::isFilled() const {
	return m_filled;
}


// 図形描画時の透過値を設定する
void UI_Shape::setAlpha(int16_t val) {
	m_alpha = val;
}

int16_t UI_Shape::getAlpha() const {
	return m_alpha;
}


