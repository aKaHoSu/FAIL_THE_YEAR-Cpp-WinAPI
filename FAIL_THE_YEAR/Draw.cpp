/* =============================================================================
 * ファイル概要:
 * - 背景/オブジェクト/UIの描画処理。
 * ============================================================================= */
#pragma comment(lib, "Msimg32.lib")
#include "Common.h"
#include "Draw.h"
#include "Player.h"
#include "Ryunen.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "UI_Shape.h"
#include "GameState.h"
#include "ObjectManager.h"
#include "EngineContext.h"
#include <cmath>
#include <string>

// 各オブジェクトが持つisVisibleフラグを用いて、実際に描画する関数を呼び出す
// 描画順は、DrawAll内の記述順 -> ID_xxx の定義順 に依存
void Draw::DrawAll(HDC hDC, const ObjectManager& objMgr, const GameState& gameState) {

	/* --- 各オブジェクトの描画処理をラムダ関数として定義 --- */
	auto drawRyunenLayer = [&]() {
		for (int i = 0; i < MAX_RYUNEN; ++i) {
			if (objMgr.GetRyunen(i).isVisible()) {
				DrawRyunenObject(hDC, objMgr.GetRyunen(i));
				DrawHitBox(hDC, objMgr.GetRyunen(i), gameState);
			}
		}
	};

	auto drawUiImageLayer = [&]() {
		for (int i = ID_BEGIN_IMAGE; i < ID_BEGIN_IMAGE + MAX_UI_IMAGE; ++i) {
			if (objMgr.GetUIImage(i).isVisible()) {
				DrawImageObject(hDC, objMgr.GetUIImage(i));
			}
		}
	};

	auto drawPlayerLayer = [&]() {
		if (!objMgr.GetPlayer().isVisible()) {
			return;
		}

		if (objMgr.GetPlayer().isHasBomb()) {
			DrawMagicSign(hDC, objMgr.GetPlayer());
		}

		DrawHitBox(hDC, objMgr.GetPlayer(), gameState);
		DrawImageObject(
			hDC,
			objMgr.GetPlayer(),
			objMgr.GetPlayer().getDrawOffsetX(),
			objMgr.GetPlayer().getDrawOffsetY());
	};

	auto drawUiShapeLayer = [&]() {
		for (int i = 0; i < MAX_UI_SHAPE; ++i) {
			if (objMgr.GetUIShape(i).isVisible()) {
				DrawShapeObject(hDC, objMgr.GetUIShape(i));
			}
		}
	};

	auto drawUiTextLayer = [&]() {
		for (int i = ID_BEGIN_TEXT; i < ID_BEGIN_TEXT + MAX_UI_TEXT; ++i) {
			if (objMgr.GetUIText(i).isVisible()) {
				DrawTextObject(hDC, objMgr.GetUIText(i), objMgr);
			}
		}
	};
	/* ------------------------------------------------------ */


	// 0) 背景
	DrawBackground(hDC, gameState);

	// 1) 留年
	drawRyunenLayer();

	// 2) 画像UI
	drawUiImageLayer();

	// 3) プレイヤー周辺
	drawPlayerLayer();

	// 4) 図形UI
	drawUiShapeLayer();

	// 5) テキストUI
	drawUiTextLayer();

	// 6) Nightmare・AfterResult時のフェード
	if (gameState.ShouldDrawNightmareFade()) {
		DrawFadeLayer(hDC, gameState);
	}

	// 7) デバッグコマンドライン
	DrawDebugCommandLine(hDC);
}

// 背景の描画
void Draw::DrawBackground(HDC hDC, const GameState& gameState) {
	RECT backRect = { 0, 0, WINDOW_W, WINDOW_H };
	HBRUSH brush = CreateSolidBrush(static_cast<COLORREF>(gameState.BackgroundColor()));
	FillRect(hDC, &backRect, brush);
	DeleteObject(brush);
}

// 画像を用いるオブジェクト
void Draw::DrawImageObject(
	HDC hDC,
	const ObjectData& obj,
	int16_t offsetX,
	int16_t offsetY)
{
	// 無効なID、またはビットマップが無いオブジェクトはスキップ
	if (obj.getId() < 0) return;
	HBITMAP hBmp = obj.getHBitmap();
	if (hBmp == NULL) return;

	// 互換DCの作成・ビットマップの選択
	HDC memDC = CreateCompatibleDC(hDC);
	HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, hBmp);

	// サイズと位置を取得
	const OBJSIZE& sz = obj.getSize();
	const OBJPOSITION& pos = obj.getPosition();

	// 描画
	BitBlt(
		hDC,
		(int)pos.x + offsetX,
		(int)pos.y + offsetY,
		sz.width,
		sz.height,
		memDC,
		0,
		0,
		SRCAND);

	// リーク防止
	SelectObject(memDC, oldBmp);
	DeleteDC(memDC);
}

// 図形を用いるオブジェクト
void Draw::DrawShapeObject(HDC hDC, const UI_Shape& obj) {
	// 無効IDは描画対象外
	if (obj.getId() < 0) return;

	// サイズと位置を取得
	const OBJSIZE& sz = obj.getSize();
	const OBJPOSITION& pos = obj.getPosition();

	// UI_Shape別に、枠線ペンを作成
	HPEN hPen = CreatePen(PS_SOLID, obj.getBorderWidth(), obj.getBorderColor());
	HPEN oldPen = (HPEN)SelectObject(hDC, hPen);

	// 塗りつぶしペンを作成
	// 塗りなしの場合は HOLLOW_BRUSH を使って輪郭のみ描画
	HBRUSH hBrush = obj.isFilled() ? CreateSolidBrush(obj.getFillColor()) : (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	// 図形種別に応じて WinAPI の描画関数を切り替える
	switch (obj.getShapeType()) {
	case UI_Shape::ShapeType::Rectangle:

		// アルファ値が指定されている場合は AlphaBlend で描画
		if (obj.getAlpha() != 0) {
			HDC hDCmem = CreateCompatibleDC(hDC);
			HBITMAP hBmp = CreateCompatibleBitmap(hDC, obj.getSize().width, obj.getSize().height);
			HBITMAP oldBmp = (HBITMAP)SelectObject(hDCmem, hBmp);

			// 互換DCの背景を透明にするため白で塗りつぶし
			HBRUSH hMemBrush = CreateSolidBrush(obj.getFillColor());
			RECT memRect = { 0, 0, obj.getSize().width, obj.getSize().height };
			FillRect(hDCmem, &memRect, hMemBrush);
			DeleteObject(hMemBrush);

			// Alpha付きで描画するための構造体を設定
			BLENDFUNCTION bf;
			bf.BlendOp = AC_SRC_OVER;
			bf.BlendFlags = 0;
			bf.SourceConstantAlpha = (BYTE)obj.getAlpha();
			bf.AlphaFormat = 0;

			// 描画
			AlphaBlend(hDC, (int)pos.x, (int)pos.y,
				obj.getSize().width, obj.getSize().height, hDCmem,
				0, 0, obj.getSize().width, obj.getSize().height, bf);

			SelectObject(hDCmem, oldBmp);
			DeleteObject(hBmp);
			DeleteDC(hDCmem);
		}

		// アルファ値が指定されていない場合は通常の描画
		else {
			Rectangle(hDC, (int)pos.x, (int)pos.y, (int)pos.x + sz.width, (int)pos.y + sz.height);
		}

		break;

	case UI_Shape::ShapeType::TriangleRight: {

		// 右向きの三角形を描画するための3点を定義
		POINT points[3] = {
			{ (LONG)pos.x + sz.width, (LONG)pos.y + (sz.height / 2) },
			{ (LONG)pos.x, (LONG)pos.y },
			{ (LONG)pos.x, (LONG)pos.y + sz.height },
		};

		// 描画
		Polygon(hDC, points, 3);
		break;
	}

	default:
		// 未対応の図形は何も描かない
		break;
	}

	// リーク防止
	SelectObject(hDC, oldBrush);
	SelectObject(hDC, oldPen);

	if (obj.isFilled()) {
		DeleteObject(hBrush);
	}
	DeleteObject(hPen);
}

// テキストを用いるオブジェクト
void Draw::DrawTextObject(HDC hDC, const UI_Text& obj, const ObjectManager& objMgr) {
	// 無効なIDはスキップ
	if (obj.getId() < 0) return;

	// 文字の色を設定
	SetTextColor(hDC, obj.getDrawColor());

	// 文字の背景を描画しないための設定
	SetBkMode(hDC, TRANSPARENT);

	// 描画位置はオブジェクトの位置を使う
	const OBJPOSITION& pos = obj.getPosition();

	// 共有フォントを使い回す（GameInitializer で作成される）
	HFONT hFont = UI_Text::getSizeFont(objMgr, obj.getId());
	HFONT oldFont = NULL;
	if (hFont) {
		oldFont = (HFONT)SelectObject(hDC, hFont);
	}

	for (int i = 0; i < obj.getRepeatCount(); i++) {
		TextOutW(
			hDC,
			(int)pos.x + obj.getDrawOffsetX(),
			(int)pos.y + obj.getDrawOffsetY() + obj.getRepeatOffsetY() * i,
			obj.getRenderText(),
			lstrlenW(obj.getRenderText())
		);
	}

	if (oldFont) {
		SelectObject(hDC, oldFont);
	}
}

// 留年オブジェクトの描画
void Draw::DrawRyunenObject(HDC hDC, const Ryunen& obj) {
	// オブジェクトのIdを取得
	const int16_t& id = obj.getId();

	// 無効なID、またはテキストがないオブジェクトはスキップ
	if (id < 0) return;

	// 文字の色を設定
	SetTextColor(hDC, obj.getDrawColor());

	// 文字の背景を描画しないための設定
	SetBkMode(hDC, TRANSPARENT);

	// 描画位置はオブジェクトの位置を使う
	const OBJPOSITION& pos = obj.getPosition();

	// 留年専用フォントの選択
	HFONT hFont = Ryunen::getRyunenFont();
	HFONT oldFont = NULL;
	if (hFont) {
		oldFont = (HFONT)SelectObject(hDC, hFont);
	}

	// 描画
	TextOutW(hDC, (int)pos.x, (int)pos.y, obj.getText(), lstrlenW(obj.getText()));

	// 後始末（共有フォントは破棄しない）
	if (oldFont) {
		SelectObject(hDC, oldFont);
	}
}

// ヒットボックスの可視化（デバッグ用）
void Draw::DrawHitBox(HDC hDC, const ObjectData& obj, const GameState& gameState) {

	// 表示物の設定がfalse、または無効なIDは描画対象外
	if (obj.getId() < 0 || gameState.IsDspHitBox() == false) { return; }

	// ヒットボックスの矩形を取得する変数
	OBJHITBOX::Rect r;

	// getCollision() アクセサ経由でヒットボックスを取得する
	if (obj.getId() == ID_PLAYER) {
		r = obj.getCollision().GetRect(obj.getPosition().x + 8, obj.getPosition().y + 8);
	}
	// 留年オブジェクトのみ、描画位置から左にずらして描画する（テキストの表示方法の影響）
	else {
		r = obj.getCollision().GetRect(obj.getPosition().x - (float)obj.getSize().width - 7.0f,
			obj.getPosition().y);
	}

	// 赤い点線のペンを作成して選択
	HPEN hPen = CreatePen(PS_DOT, 1, RGB(255, 0, 0));
	HPEN oldPen = (HPEN)SelectObject(hDC, hPen);

	//	塗りなしのブラシを選択
	HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	// ヒットボックスを矩形で描画
	Rectangle(hDC, static_cast<int>(r.x1), static_cast<int>(r.y1), static_cast<int>(r.x2), static_cast<int>(r.y2));

	// リーク防止
	SelectObject(hDC, oldBrush);
	SelectObject(hDC, oldPen);
	DeleteObject(hPen);
}

// プレイヤー中心に魔法陣を回転描画する
void Draw::DrawMagicSign(HDC hDC, const Player& obj) {
	// 回転角度を取得
	const double angle = obj.getMagicSignAngle();

	// SEGMENTS：紋をいくつの線分に分割するか( = 何角形か決める)
	const int SEGMENTS = 3;

	// DOT_STEP：点線の間隔(1の場合は直線になる)
	const int DOT_STEP = 1;

	// 赤いペンを使用
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(200, 0, 0));
	HPEN oldPen = (HPEN)SelectObject(hDC, hPen);

	// MoveToEx / LineToで描画
	for (double i = 0; i < SEGMENTS; i += (double)DOT_STEP)
	{
		/*------- 六芒星(二つの三角形をずらして配置することで六芒星を表現しています) -------*/

		// --- 一つ目の三角形
		// 最初の角度(線分の始点)
		double a1 = (2 * 3.14 / SEGMENTS) * i + angle;

		// 次の角度（線分の終点）
	    double a2 = (2 * 3.14 / SEGMENTS) * (i + 1) + angle;

		// 楕円のパラメトリック方程式([中心の座標] + [cos / sin(a1 / a2)] * [幅 / 高さ])
		int x1 = (int)obj.getPosition().x + (obj.getSize().width / 2) + (int)(cos(a1) * 43);
		int y1 = (int)obj.getPosition().y + obj.getSize().height - 1 + (int)(sin(a1) * 10);

		// 線分の終点
		int x2 = (int)obj.getPosition().x + (obj.getSize().width / 2) + (int)(cos(a2) * 43);
		int y2 = (int)obj.getPosition().y + obj.getSize().height - 1 + (int)(sin(a2) * 10);

		// 描画開始位置を移動
		MoveToEx(hDC, x1, y1, NULL);

		// 終点まで線を描く
		LineTo(hDC, x2, y2);

		// --- 二つ目の三角形
		// 最初の角度(線分の始点)
		double a3 = (2 * 3.14 / SEGMENTS) * (i + 0.5) + angle;

		// 次の角度（線分の終点）
		double a4 = (2 * 3.14 / SEGMENTS) * (i + 1.5) + angle;

		// 楕円のパラメトリック方程式([中心の座標] + [cos / sin(a1 / a2)] * [幅 / 高さ])
		int x3 = (int)obj.getPosition().x + (obj.getSize().width / 2) + (int)(cos(a3) * 43);
		int y3 = (int)obj.getPosition().y + obj.getSize().height - 1 + (int)(sin(a3) * 10);

		// 線分の終点
		int x4 = (int)obj.getPosition().x + (obj.getSize().width / 2) + (int)(cos(a4) * 43);
		int y4 = (int)obj.getPosition().y + obj.getSize().height - 1 + (int)(sin(a4) * 10);

		// 描画開始位置を移動
		MoveToEx(hDC, x3, y3, NULL);

		// 終点まで線を描く
		LineTo(hDC, x4, y4);
		/*----------------------------------------------------------------------------------*/

	}

	// リーク防止
	SelectObject(hDC, oldPen);
	DeleteObject(hPen);
}


// フェードを描画
void Draw::DrawFadeLayer(HDC hDC, const GameState& gameState) {
	const BYTE fadeAlpha = gameState.NightmareFadeAlpha();

	// 互換DCとビットマップを作成
	HDC memDC = CreateCompatibleDC(hDC);
	HBITMAP memBitmap = CreateCompatibleBitmap(hDC, WINDOW_W, WINDOW_H);
	SelectObject(memDC, memBitmap);

	// 白で塗りつぶし、範囲の指定
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); // 白
	RECT rect = { 0, 0, WINDOW_W, WINDOW_H };
	FillRect(memDC, &rect, hBrush);
	DeleteObject(hBrush);

	// Alpha付きで描画するための構造体を設定
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = fadeAlpha; // 透明度を指定
	bf.AlphaFormat = 0;

	// 描画
	AlphaBlend(hDC, 0, 0, WINDOW_W, WINDOW_H, memDC, 0, 0, WINDOW_W, WINDOW_H, bf);

	// リーク防止
	DeleteObject(memBitmap);
	DeleteDC(memDC);
}

// デバッグコマンドラインを左下に描画
void Draw::DrawDebugCommandLine(HDC hDC) {
	if (!g_engine.debugCmdLine.isVisible) {
		return;
	}

	const int boxHeight = 34;
	const int boxMargin = 10;
	// 左下にコマンド入力欄を表示する矩形
	RECT boxRect = {
		boxMargin,
		650 - boxHeight,
		1170,
		650
	};

	HBRUSH bgBrush = CreateSolidBrush(RGB(235, 235, 235));
	FillRect(hDC, &boxRect, bgBrush);
	DeleteObject(bgBrush);

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(0, 0, 0));

	const std::wstring commandText = L"> " + g_engine.debugCmdLine.buffer;
   // 現在入力中のコマンドを表示
	TextOutW(
		hDC,
		boxRect.left + 8,
		boxRect.top + 7,
		commandText.c_str(),
		static_cast<int>(commandText.length()));

	if (g_engine.debugCmdLine.statusTimer > 0 && !g_engine.debugCmdLine.status.empty()) {
	   // Enter実行後の結果を短時間だけ入力欄の上に表示
		TextOutW(
			hDC,
			boxRect.left + 8,
			boxRect.top - 24,
			g_engine.debugCmdLine.status.c_str(),
			static_cast<int>(g_engine.debugCmdLine.status.length()));
	}
}


