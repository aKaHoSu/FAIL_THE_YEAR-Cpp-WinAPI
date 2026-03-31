/* =============================================================================
 * ファイル概要:
 * - ゲーム進行状態を保持する GameState の宣言。
 * ============================================================================= */
#pragma once
#include <cstdint>
#include "Common.h"

class GameState {
public:
	GameState();

	int16_t GameCnt() const { return gameCnt; }
	void SetGameCnt(int16_t value) { gameCnt = value; }
	void IncrementGameCnt() {
		if (gameCnt < INT16_MAX) {
			++gameCnt;
		}
	}

	int16_t RyunenCount() const { return ryunenCount; }
	void SetRyunenCount(int16_t value) { ryunenCount = (value < 0) ? 0 : value; }

	int16_t EvadeCnt() const { return evadeCnt; }
	void SetEvadeCnt(int16_t value) { evadeCnt = (value < 0) ? 0 : value; }
	void IncrementEvadeCnt() {
		if (evadeCnt < INT16_MAX) {
			++evadeCnt;
		}
	}

	int16_t EvadeGoal() const { return evadeGoal; }
	void SetEvadeGoal(int16_t value) { evadeGoal = (value < 0) ? 0 : value; }

	int16_t BestEvadeCnt() const { return bestEvadeCnt; }
	void SetBestEvadeCnt(int16_t value) { bestEvadeCnt = (value < 0) ? 0 : value; }

	int16_t EscCnt() const { return escCnt; }
	void SetEscCnt(int16_t value) { escCnt = (value < 0) ? 0 : value; }
	void IncrementEscCnt() {
		if (escCnt < INT16_MAX) {
			++escCnt;
		}
	}
	void DecrementEscCnt() {
		if (escCnt > 0) {
			--escCnt;
		}
	}

	bool IsFirstPlay() const { return isFirstPlay; }
	void SetIsFirstPlay(bool value) { isFirstPlay = value; }
	void ToggleIsFirstPlay() { isFirstPlay = !isFirstPlay; }

	bool HasEverCleared() const { return hasEverCleared; }
	void SetHasEverCleared(bool value) { hasEverCleared = value; }
	void ToggleHasEverCleared() { hasEverCleared = !hasEverCleared; }

	bool HasEverClearedNightmare() const { return hasEverClearedNightmare; }
	void SetHasEverClearedNightmare(bool value) { hasEverClearedNightmare = value; }
	void ToggleHasEverClearedNightmare() { hasEverClearedNightmare = !hasEverClearedNightmare; }

	bool HasEverEscapedNightmare() const { return hasEverEscapedNightmare; }
	void SetHasEverEscapedNightmare(bool value) { hasEverEscapedNightmare = value; }
	void ToggleHasEverEscapedNightmare() { hasEverEscapedNightmare = !hasEverEscapedNightmare; }

	bool IsNightmare() const { return isNightmare; }
	void SetIsNightmare(bool value) { isNightmare = value; }

	bool CanAdv() const { return canAdv; }
	void SetCanAdv(bool value) { canAdv = value; }

	bool CanSpawn() const { return canSpawn; }
	void SetCanSpawn(bool value) { canSpawn = value; }

	bool IsGodMode() const { return isGodMode; }
	void SetIsGodMode(bool value) { isGodMode = value; }
	void ToggleIsGodMode() { isGodMode = !isGodMode; }

	bool IsDspHitBox() const { return isDspHitBox; }
	void SetIsDspHitBox(bool value) { isDspHitBox = value; }
	void ToggleIsDspHitBox() { isDspHitBox = !isDspHitBox; }

	bool ShouldDrawNightmareFade() const { return shouldDrawNightmareFade; }
	void SetShouldDrawNightmareFade(bool value) { shouldDrawNightmareFade = value; }

	uint8_t NightmareFadeAlpha() const { return nightmareFadeAlpha; }
	void SetNightmareFadeAlpha(uint8_t value) { nightmareFadeAlpha = value; }

	uint32_t BackgroundColor() const { return backgroundColor; }
	void SetBackgroundColor(uint32_t value) { backgroundColor = value; }

	Difficulty CurrentDifficulty() const { return currentDifficulty; }
	void SetCurrentDifficulty(Difficulty value) { currentDifficulty = value; }

private:
	int16_t gameCnt;		// プレイ開始からの経過時間カウンタ
	int16_t ryunenCount;	// 現在の留年オブジェクト数
	int16_t evadeCnt;		// 留年を回避した回数
	int16_t evadeGoal;		// 進級確定に必要な回避数
	int16_t bestEvadeCnt;	// 最高回避記録
	int16_t escCnt;         // Nightmare・AfterResult 時の Esc 連打数

	bool	isFirstPlay;    // 初プレイか否か
	bool	hasEverCleared; // 一回以上クリアしているか
	bool	hasEverClearedNightmare; // Nightmareを一回以上クリアしているか
	bool	hasEverEscapedNightmare; // Nightmareを一回以上脱出しているか
	bool	isNightmare;    // Nightmare モードフラグ
	bool	canAdv;         // 進級確定フラグ
	bool	canSpawn;       // 留年オブジェクトの再スポーン許可フラグ
	bool	isGodMode;      // God モードフラグ
	bool	isDspHitBox;    // ヒットボックス表示フラグ

	bool		shouldDrawNightmareFade; // Nightmareフェード描画フラグ（更新側で確定）
	uint8_t		nightmareFadeAlpha;		 // Nightmareフェードの透明度（更新側で確定）
	uint32_t	backgroundColor;         // 背景色（更新側で確定）
	Difficulty		currentDifficulty;       // 現在選択中の難易度
};
