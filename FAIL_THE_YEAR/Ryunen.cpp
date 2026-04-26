/* =============================================================================
 * ファイル概要:
 * - 留年オブジェクトの挙動と状態を実装。
 * ============================================================================= */
#include "Common.h"
#include "Ryunen.h"
#include "Scene.h"
#include "GameState.h"
#include "ObjectManager.h"
#include <cmath>
#include "DebugLog.h"
#include "Utils.h"

#pragma warning(push)
#pragma warning(disable: 4828)  // resource.h は ANSI エンコードのため UTF-8 モードでは C4828 が発生する
#include "resource.h"
#pragma warning(pop)

namespace {
	// Ryunen関連の処理に用いる定数の定義
	constexpr int16_t kFontSize = 30;  // 表示フォントサイズ
	constexpr int16_t kLandActReset = -20; // 着地時の actCnt リセット値
	constexpr int16_t kOffscreenMargin = 20;  // 画面下端の場外判定マージン (px)
	constexpr int16_t kFixTolerance = 60;  // 地面接触判定の許容誤差 (px)
	constexpr double  kBounceFactor = 0.5; // バウンド時の速度減衰率
}

Ryunen::Ryunen()
	: isSetPosition(false)
	, oldStatus(static_cast<int16_t>(RyunenStatus::None))
	, bndDir(0)
	, drawTextId(IDS_RYUNEN)
	, ryunenText{}
	, shinkyuText{}
	, drawColor(RGB(30, 30, 30))
{
	id = ID_RYUNEN;
	collision = { RYUNEN_WIDTH, RYUNEN_HEIGHT, -(float)RYUNEN_WIDTH - 7.0f, 0.0f };
}

Ryunen::~Ryunen() {}

HFONT Ryunen::sFont = nullptr;

void Ryunen::setInitData(HINSTANCE hInst, int16_t id) {
	setId(ID_RYUNEN);
	LoadStringW(hInst, IDS_RYUNEN, ryunenText, 256);
	LoadStringW(hInst, IDS_SHINKYU, shinkyuText, 256);
	setSourceText(ryunenText);
	setStatusType(RyunenStatus::None);
	setSize(RYUNEN_WIDTH, RYUNEN_HEIGHT);
	setActCnt(0);
	setPositionFlg(false);
	drawTextId = IDS_RYUNEN;
	drawColor = RGB(30, 30, 30);
}

// 状態遷移（落下/着地/画面外）と移動処理の更新
void Ryunen::Action(const SceneManager& sceneManager, GameState& gameState) {
	setActCnt(getActCnt() + 1);
	updateDrawState(sceneManager, gameState);

	// RYUNEN_STATUS_NONEのときは、Readyシーンに入るまでAction内の処理を行わない
	if (getStatusType() == RyunenStatus::None) {
		if (sceneManager.getCurrentSceneType() >= SceneId::Playing) {
			setStatusType(RyunenStatus::Falling);
		}
		return;
	}

	switch (getStatusType()) {

		// 落下中の処理
	case RyunenStatus::Falling:
		// 一度だけの処理
		if (getOldStatusType() != RyunenStatus::Falling) {
			setOldStatusType(RyunenStatus::Falling);
		}

		// 落下の重力を加算 / ヒットボックスの更新
		addPosition(0, static_cast<int16_t>(std::round(getGravity() * getActCnt())));

		// 地面にぶつかったかどうかの判定を行う関数
		// 地面にぶつかった場合は対象留年のStatusをRYUNEN_STATUS_LANDEDに変更する
		FixCheck(sceneManager, gameState);
		break;

		// 着地した留年の処理
	case RyunenStatus::Landed:
		// 一度だけの処理
		if (getOldStatusType() != RyunenStatus::Landed) {
			setActCnt(kLandActReset); // 着地したら行動カウンタをリセット(着地後の処理に再利用するため)
			setOldStatusType(RyunenStatus::Landed);
		}

		// 着地後の挙動
		addPosition(
			static_cast<int16_t>(std::round(getBndDir() * kBounceFactor)),
			static_cast<int16_t>(std::round(getGravity() * getActCnt()))
		);

		// 画面外に出たかどうかの判定を行う関数
		if (getPosition().y > WINDOW_H + kOffscreenMargin) {
			setStatusType(RyunenStatus::Offscreen);
		}
		break;

		// 画面外に出た留年の処理
	case RyunenStatus::Offscreen:
		// 一度だけの処理
		if (getOldStatusType() != RyunenStatus::Offscreen) {
			setOldStatusType(RyunenStatus::Offscreen);
			setPositionFlg(false); // 画面外に出たら、SceneLoop内で留年オブジェクトの位置を再設定するためのフラグをfalseにセット
		}
		break;

	default:
		break;
	}
}

// Fixチェック
void Ryunen::FixCheck(const SceneManager& sceneManager, GameState& gameState) {
	if (getPosition().y + getSize().height >= FIX + kFixTolerance) {
		// プレイヤーが留年を回避した回数をカウント
		if (sceneManager.getResultType() == GameResult::None
			&& sceneManager.getCurrentSceneType() != SceneId::Tutorial) {
			gameState.IncrementEvadeCnt();
			DEBUG_LOG("Current evade count : %d\n", gameState.EvadeCnt());
		}

		setStatusType(RyunenStatus::Landed);
	}
}

// 留年オブジェクト固有のフォントの作成・格納を行う関数
void Ryunen::InitializeRyunenFont(HWND hWnd) {
	if (Ryunen::sFont != NULL) return;

	HDC hDC = GetDC(hWnd);

	const int16_t FontSize = kFontSize;

	// フォントの作成、格納
	Ryunen::sFont = CreateFontW(
		-MulDiv(FontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72),
		0, 2700, 0,  // 2700は90度回転させるための値
		FW_BOLD,
		FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		L"@メイリオ"
	);
}

// 留年オブジェクト固有のフォントの破棄を行う関数
void Ryunen::FinalizeRyunenFont() {
	if (sFont) {
		DeleteObject(sFont);
		sFont = nullptr;
	}
}

// 描画状態の更新を行う関数
void Ryunen::updateDrawState(const SceneManager& sceneManager, const GameState& gameState) {

	// 進級確定していなければ「留年」、
	// 進級確定していれば「進級」と表示する
	const int16_t nextTextId = ((!gameState.CanAdv() || !gameState.CanSpawn())
		&& (sceneManager.getCurrentSceneType() == SceneId::Playing || !gameState.CanAdv()))
		? IDS_RYUNEN
		: IDS_SHINKYU;

	if (drawTextId != nextTextId) {
		drawTextId = nextTextId;
		if (drawTextId == IDS_RYUNEN) {
			setSourceText(ryunenText);
		}
		else {
			setSourceText(shinkyuText);
		}
	}

	// ステータスやゲーム状態に応じて描画色を切り替える
	if (getStatusType() == RyunenStatus::Landed) {
		drawColor = RGB(160, 160, 160);
	}
	else {
		if (gameState.CurrentDifficulty() == Difficulty::Nightmare) {
			drawColor = RGB(100, 0, 0);
		}
		else if (!gameState.IsNightmare() && !gameState.CanAdv()) {
			drawColor = RGB(30, 30, 30);
		}
		else if (gameState.CanAdv()) {
			drawColor = RGB(0, 140, 0);
		}

		if (gameState.IsGodMode()) {
			drawColor = RGB(255, 0, 255);
		}
	}
}

// 落下開始状態で指定 x 座標にスポーンさせる関数
void Ryunen::SpawnAt(float x) {
	setVisible(true);
	setStatusType(RyunenStatus::Falling);
	setActCnt(0);
	setPosition(x, 0.0f);
	setBndDir(Utils::randSigh(1));
	setPositionFlg(true);
}


