/* =============================================================================
 * ファイル概要:
 * - 当たり判定サイズ/オフセット構造体を定義する。
 * ============================================================================= */
#pragma once

struct OBJHITBOX {
	float width;
	float height;
	float offsetX;
	float offsetY;

	// AABB判定用ヘルパー（左上・右下の座標を返す）
	struct Rect {
		float x1;
		float y1;
		float x2;
		float y2;
	};

	// キャラクターの位置を基準に当たり判定の矩形を取得する
	Rect GetRect(float charX, float charY) const {
		return {
			charX + offsetX,
			charY + offsetY,
			charX + offsetX + width,
			charY + offsetY + height
		};
	}
};

