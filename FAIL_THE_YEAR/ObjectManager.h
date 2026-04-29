/* =============================================================================
 * ファイル概要:
 * - 全オブジェクトを一元管理、当たり判定処理を持つ ObjectManager を宣言。
 * ============================================================================= */
#pragma once
#include "Common.h"
#include "Player.h"
#include "Ryunen.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "UI_Shape.h"

class GameState;

class ObjectManager {
public:
	ObjectManager();

	Player& getPlayer() { return player; }
	const Player& getPlayer() const { return player; }

	Ryunen& getRyunen(int index) { return ryunen[index]; }
	const Ryunen& getRyunen(int index) const { return ryunen[index]; }

	UI_Image& getUIImage(int index) { return UIimage[index]; }
	const UI_Image& getUIImage(int index) const { return UIimage[index]; }

	UI_Text& getUIText(int index) { return UItext[index]; }
	const UI_Text& getUIText(int index) const { return UItext[index]; }

	UI_Shape& getUIShape(int index) { return UIshape[index]; }
	const UI_Shape& getUIShape(int index) const { return UIshape[index]; }

	// プレイヤーと留年の衝突判定を行う関数。
	// 衝突時にステータスを更新し true を返す
	bool checkHitBetweenPlayerAndRyunen(const GameState& gameState);

private:
	Player player;                    // プレイヤーオブジェクト（1体のみ）
	Ryunen ryunen[MAX_RYUNEN];        // 留年オブジェクト配列

	UI_Image UIimage[MAX_UI_IMAGE];   // 画像UIオブジェクト配列
	UI_Text  UItext[MAX_UI_TEXT];     // テキストUIオブジェクト配列
	UI_Shape UIshape[MAX_UI_SHAPE];   // 図形UIオブジェクト配列
};
