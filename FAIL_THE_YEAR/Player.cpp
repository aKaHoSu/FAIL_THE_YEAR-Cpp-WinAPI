/* =============================================================================
 * ファイル概要:
 * - プレイヤーオブジェクトの機能を実装。
 * ============================================================================= */
#include "Common.h"
#include "Player.h"
#include "Scene.h"
#include "KeyManager.h"
#include "GameState.h"
#include "DebugLog.h"
#include "ObjectManager.h"

#pragma warning(push)
#pragma warning(disable: 4828)  // resource.h は ANSI エンコードのため UTF-8 モードでは C4828 が発生する
#include "resource.h"
#pragma warning(pop)

namespace {
	// Player関連の処理に用いる定数の定義
	constexpr int16_t kInitialLife		 = 3;		// 初期ライフ
	constexpr int16_t kSpeedNormal		 = 8;		// 通常移動速度
	constexpr int16_t kSpeedSlow         = 4;		// Shift押下時
	constexpr int16_t kInitialY          = 650;		// 初期Y座標基準値
	constexpr int16_t kBlinkDivisor      = 5;		// ダメージ点滅周期
	constexpr int16_t kShakeIntensityNightmare = 8; // Nightmareモードの揺れ強度
	constexpr int16_t kShakeIntensityUsually = 3;		// Nightmareモード以外の揺れ強度
	constexpr double  kMagicSignAngleStep = 0.04;	// 魔法陣の回転角度ステップ( = 回転速度)
	constexpr double  kMagicSignAngleWrap = 6.283185307179586; // 魔法陣の回転角度のラップ値(360度)
}

Player::Player()
	: life(kInitialLife)
	, damageCT(DMG_CT)
	, hasBomb(true)
	, bombCT(BOMB_CT)
	, oldStatus(static_cast<int16_t>(PlayerStatus::None))
	, drawOffsetX(0)
	, drawOffsetY(0)
	, magicSignAngle(0.0)
{
	id = ID_PLAYER;
	status = static_cast<int16_t>(PlayerStatus::None);
	collision = { 40, 46, 8.0f, 8.0f };
}

Player::~Player() {}

void Player::setInitData(HINSTANCE hInst) {
	setId(ID_PLAYER);
	setBmp(hInst, IDB_PLAYER);
	setStatusType(PlayerStatus::None);
	setSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	setPosition((float)(WINDOW_W / 2) - (float)(size.width / 2) - 9, (float)(kInitialY - size.height));
	setSpeed(kSpeedNormal, kSpeedNormal);
	setActCnt(0);
	setLife(kInitialLife);
	setDamageCT(DMG_CT);
	setBomb(true);
	setBombCT(BOMB_CT);
	drawOffsetX = 0;
	drawOffsetY = 0;
	magicSignAngle = 0.0;
}

// 入力・状態遷移・移動・ボムCTを含むプレイヤー1フレーム更新を行う
void Player::Action(const KeyManager& key, const SceneManager& sceneManager, ObjectManager& objMgr, const GameState& gameState) {
	updateDrawState(sceneManager, gameState);

	switch (getStatusType()) {
		// PLAYER_STATUS_NONEのときは、Readyシーンに入るまでAction内の処理を行わない
	case PlayerStatus::None:
		if (sceneManager.getCurrentSceneType() >= SceneId::Ready) {
			setStatusType(PlayerStatus::Normal);
		}
		return;

	case PlayerStatus::Normal:
		if (getOldStatusType() != PlayerStatus::Normal) {
			setDamageCT(DMG_CT);
			DEBUG_LOG("Current player status : PLAYER_STATUS_NORMAL\n");
			setOldStatusType(PlayerStatus::Normal);
		}
		break;

	case PlayerStatus::Damage:
		if (getOldStatusType() != PlayerStatus::Damage) {
			setDamageCT(DMG_CT);
			if (sceneManager.getCurrentSceneType() != SceneId::Tutorial)
					setLife(getLife() - 1);
				DEBUG_LOG("Current player status : PLAYER_STATUS_DAMAGE\n");
				DEBUG_LOG("Remaining player life : %d\n", getLife());
			setOldStatusType(PlayerStatus::Damage);
		}

		// ダメージクールタイム中は点滅させる
		if (damageCT > 0) {
			setVisible(damageCT / kBlinkDivisor % 2 == 0);
			damageCT--;
		}

		// ダメージクールタイムが0になったら通常状態に戻す
		else {
			setStatusType(PlayerStatus::Normal);
			setVisible(true);
		}
		break;

	default:
		break;
	}

	/*---------------------------移動処理---------------------------*/
	// Shiftキーを押しているときは移動速度を半分にする
	if (key.IsPress(KeyManager::Shift)) {
		speed.x = kSpeedSlow;
		speed.y = kSpeedSlow;
	}
	else {
		speed.x = kSpeedNormal;
		speed.y = kSpeedNormal;
	}

	// 十字キーで移動、画面外に行けないようにする
	if (key.IsPress(KeyManager::Up)) {
		if (getPosition().y >= 0)
			addPosition(0, static_cast<float>(-speed.y));
	}
	if (key.IsPress(KeyManager::Down)) {
		if (getPosition().y <= FIX)
			addPosition(0, static_cast<float>(speed.y));
	}
	if (key.IsPress(KeyManager::Left)) {
		if (getPosition().x >= 0)
			addPosition(static_cast<float>(-speed.x), 0);
	}
	if (key.IsPress(KeyManager::Right)) {
		if (getPosition().x <= RIGHT_FIX)
			addPosition(static_cast<float>(speed.x), 0);
	}
	/*--------------------------------------------------------------*/

	// ボムが使用できない状態の場合、リチャージ処理を行う
	if (!hasBomb &&
		(sceneManager.getCurrentSceneType() != SceneId::Tutorial ||
			sceneManager.getCurrentTutorialStepType() == TutorialStepId::Bomb)) {
		bombCT--;
		if (bombCT <= 0) {
			hasBomb = true;
			bombCT = BOMB_CT;
		}
	}
}

// プレイヤーの残りライフを更新する
void Player::setLife(int16_t life) {
	this->life = life;
	DEBUG_LOG("Setting life : %d\n", life);
}

// ダメージクールタイムを更新する
void Player::setDamageCT(int16_t damageCT) {
	this->damageCT = damageCT;
}

// ボム所持状態を更新する
void Player::setBomb(bool bomb) {
	this->hasBomb = bomb;
}

// ボム再使用までのクールタイムを更新する
void Player::setBombCT(int16_t bombCT) {
	this->bombCT = bombCT;
}

// ボムを発動し、落下中の留年オブジェクトを着地状態へ遷移させる
void Player::useBomb(ObjectManager& objMgr) {
	// ボムが使用できない場合はreturn
	if (!hasBomb) {
		DEBUG_LOG("Bomb is currently unavailable. Remaining Bomb CT : %d\n", bombCT);
		return;
	}

	if (objMgr.getPlayer().getStatusType() == PlayerStatus::Damage) {
		DEBUG_LOG("Bomb is currently unavailable.\n");
		return;
	}

	// ボム所持フラグを落とす
	this->hasBomb = false;
	DEBUG_LOG("Bomb was used. Remaining Bomb CT : %d\n", bombCT);

	// 表示されているすべての留年を強制的に着地状態(=無効状態)にする
	for (int i = 0; i < MAX_RYUNEN; i++) {
		if (objMgr.getRyunen(i).getStatusType() == RyunenStatus::Falling) {
			objMgr.getRyunen(i).setStatusType(RyunenStatus::Landed);
		}
	}
}

// チュートリアルのコンセプト紹介時、プレイヤーを震わせる処理
void Player::updateDrawState(const SceneManager& sceneManager, const GameState& gameState) {
	const int16_t intensity = (gameState.CurrentDifficulty() == Difficulty::Nightmare)
		? kShakeIntensityNightmare
		: kShakeIntensityUsually;

	if ((sceneManager.getCurrentSceneType() == SceneId::Tutorial &&
		sceneManager.getCurrentTutorialStepType() == TutorialStepId::Consept) ||
		getLife() == 1) {
		drawOffsetX = (rand() % intensity) - (intensity / 2);
		drawOffsetY = (rand() % intensity) - (intensity / 2);
	}
	else {
		drawOffsetX = 0;
		drawOffsetY = 0;
	}
}

// プレイヤーの魔法陣の回転を更新する関数
void Player::advanceMagicSignAngle() {
	if (isVisible() && hasBomb) {
		magicSignAngle += kMagicSignAngleStep;
		if (magicSignAngle >= kMagicSignAngleWrap) {
			magicSignAngle -= kMagicSignAngleWrap;
		}
	}
}


