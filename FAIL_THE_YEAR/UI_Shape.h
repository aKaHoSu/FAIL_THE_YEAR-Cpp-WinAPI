/* =============================================================================
 * ファイル概要:
 * - 図形を使用するUIオブジェクトを管理する UI_Shape クラスを宣言。
 * ============================================================================= */
#pragma once
#include <windows.h>
#include "ObjectData.h"

class UI_Shape : public ObjectData {
public:
	// 図形の描画方法を分岐するための種類。
	// Draw::DrawShapeObject ではこの値を見て WinApi の描画 API を切り替える。
	enum class ShapeType : int16_t {
		None = 0,
		Rectangle,
		TriangleRight,
	};

	UI_Shape();
	~UI_Shape();

	// 初期化処理。
	// 図形の種類やサイズ、位置を設定する
	void setInitData(HINSTANCE hInst, HWND hWnd, int16_t id);

	// 描画する図形の種類を設定する
	void setShapeType(ShapeType type);
	ShapeType getShapeType() const;

	// 塗りつぶし色（m_filled=true の時に使用）
	void setFillColor(COLORREF color);
	COLORREF getFillColor() const;

	// 枠線色
	void setBorderColor(COLORREF color);
	COLORREF getBorderColor() const;

	// 枠線太さ（ピクセル）
	void setBorderWidth(int16_t borderWidth);
	int16_t getBorderWidth() const;

	// true:塗りつぶしあり / false:枠線のみ
	void setFilled(bool filled);
	bool isFilled() const;

	// 透明度を表すalpha値
	void setAlpha(int16_t val);
	int16_t getAlpha() const;

private:
	ShapeType m_shapeType;
	COLORREF m_fillColor;
	COLORREF m_borderColor;
	int16_t m_borderWidth;
	bool m_filled;
	int16_t m_alpha;
};
