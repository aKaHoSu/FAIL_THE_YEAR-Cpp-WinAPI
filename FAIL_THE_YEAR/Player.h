/* =============================================================================
 * ファイル概要:
 * - プレイヤークラスと状態定義を宣言。
 * ============================================================================= */
#pragma once

#include <windows.h>
#include "ObjectData.h"

class KeyManager;
class SceneManager;
class ObjectManager;
class GameState;

enum class PlayerStatus : int16_t {
	None = -1,   // 無効
	Easy,        // 通常
	Damage,      // ダメージを受けた状態
};

class Player : public ObjectData {

public:
	Player();
	~Player();

	void setInitData(HINSTANCE hInst);
	void Action(const KeyManager& key, const SceneManager& sceneManager, ObjectManager& objMgr, const GameState& gameState);
	void advanceMagicSignAngle();

	void setLife(int16_t life);
	void setDamageCT(int16_t damageCT);
	void setBomb(bool bomb);
	void setBombCT(int16_t bombCT);

	PlayerStatus getStatusType() const { return getStatusAs<PlayerStatus>(); }
	void setStatusType(PlayerStatus next) { setStatusAs(next); }
	PlayerStatus getOldStatusType() const { return static_cast<PlayerStatus>(oldStatus); }
	void setOldStatusType(PlayerStatus prev) { oldStatus = static_cast<int16_t>(prev); }

	const int16_t getLife() const { return life; }
	const bool isHasBomb() const { return hasBomb; }

	int16_t getDrawOffsetX() const { return drawOffsetX; }
	int16_t getDrawOffsetY() const { return drawOffsetY; }
	double getMagicSignAngle() const { return magicSignAngle; }

	void useBomb(ObjectManager& objMgr);


protected:
	void updateDrawState(const SceneManager& sceneManager, const GameState& gameState) override;

	int16_t life;      // ライフ
	int16_t damageCT;  // ダメージを受けた際のクールタイム

	bool hasBomb;    // ボムが使えるかどうか
	int16_t bombCT;  // ボム使用のクールタイム

	int16_t oldStatus;  // ステータス更新時の前回状態（コンソール表示で使用）
	int16_t drawOffsetX;
	int16_t drawOffsetY;
	double magicSignAngle;
};





































