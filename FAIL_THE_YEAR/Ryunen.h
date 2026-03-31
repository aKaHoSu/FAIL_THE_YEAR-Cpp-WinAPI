/* =============================================================================
 * ファイル概要:
 * - 留年オブジェクトクラスと状態定義を宣言。
 * ============================================================================= */
#pragma once

#include <windows.h>
#include "ObjectData.h"

class SceneManager;
class ObjectManager;
class GameState;

enum class RyunenStatus : int16_t {
	None = -1,   // 無効
	Falling,     // 落下中
	Landed,      // 着地
	Offscreen,   // 画面外
};

class Ryunen : public ObjectData {

public:
	Ryunen();
	~Ryunen();

	void setInitData(HINSTANCE hInst, int16_t id);
	void SpawnAt(float x);  // 落下開始状態で指定 x 座標にスポーンさせる
	static void InitializeRyunenFont(HWND hWnd);
	static void FinalizeRyunenFont();
	void Action(const SceneManager& sceneManager, GameState& gameState);
	void FixCheck(const SceneManager& sceneManager, GameState& gameState);  // 地面にぶつかったかどうかの判定を行う関数。

	static const HFONT getRyunenFont() { return sFont; }

	RyunenStatus getStatusType() const { return getStatusAs<RyunenStatus>(); }
	void setStatusType(RyunenStatus next) { setStatusAs(next); }
	RyunenStatus getOldStatusType() const { return static_cast<RyunenStatus>(oldStatus); }
	void setOldStatusType(RyunenStatus prev) { oldStatus = static_cast<int16_t>(prev); }

	bool getPositionFlg() { return isSetPosition; }
	void setPositionFlg(bool flg) { isSetPosition = flg; }

	int16_t getBndDir() const { return bndDir; }
	void setBndDir(int16_t dir) { bndDir = dir; }

	const double getGravity() const { return gravity; }
	void setGravity(double gravity) { this->gravity = gravity; }

	COLORREF getDrawColor() const { return drawColor; }

protected:
	void updateDrawState(const SceneManager& sceneManager, const GameState& gameState) override;

	static HFONT sFont;  // 留年オブジェクト固有のフォントサイズ・書式
	bool isSetPosition;  // 落下開始前に位置を設定したかどうかのフラグ
	                    // SceneLoop内で位置設定時に使用
	int16_t oldStatus;   // ステータス更新時の前回状態（コンソール表示で使用）
	int16_t bndDir;      // 着地後のジャンプ方向（-1:左、0:なし、1:右）
	int16_t drawTextId;
	WCHAR ryunenText[256];
	WCHAR shinkyuText[256];
	COLORREF drawColor;

	double gravity = 0.15;
};



























