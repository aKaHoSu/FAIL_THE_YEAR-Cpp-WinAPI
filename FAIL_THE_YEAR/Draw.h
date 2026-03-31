/* =============================================================================
 * ファイル概要:
 * - 描画処理クラス Draw を宣言する。
 * ============================================================================= */
#pragma once
#include "Common.h"

class GameState;
class ObjectManager;

class Draw {
public:
	// 配列内のすべてのオブジェクトを描画する関数を 記述順に呼び出す
	static void DrawAll(HDC hDC, const ObjectManager& objMgr, const GameState& gameState);

private:
	// 背景を描画する関数
	static void DrawBackground(HDC hDC, const GameState& gameState);

	// 画像を用いるオブジェクトを描画する関数
	static void DrawImageObject(
		HDC hDC,
		const ObjectData& obj,
		int16_t offsetX = 0,
		int16_t offsetY = 0);

	// 図形を用いるオブジェクトを描画する関数
	static void DrawShapeObject(HDC hDC, const UI_Shape& obj);

	// テキストを用いるオブジェクトを描画する関数
	static void DrawTextObject(HDC hDC, const UI_Text& obj, const ObjectManager& objMgr);

	// 留年オブジェクトを描画する関数
	static void DrawRyunenObject(HDC hDC, const Ryunen& obj);

	// ヒットボックスを描画する関数
	static void DrawHitBox(HDC hDC, const ObjectData& obj, const GameState& gameState);

	// ボム所持時のエフェクトを描画する関数
	static void DrawMagicSign(HDC hDC, const Player& obj);

	// フェードを描画する関数
	// DrawAll 関数の一番最後に記述する
	static void DrawFadeLayer(HDC hDC, const GameState& gameState);

	// デバッグ用コマンドラインを描画する関数
	static void DrawDebugCommandLine(HDC hDC);
};
