/* =============================================================================
 * ファイル概要:
 * - SceneManager の処理を実装する。
 * ============================================================================= */
#include "Common.h"
#include "UI_Text.h"
#include "GameInitializer.h"
#include "DebugLog.h"
#include "TitleSceneState.h"
#include "MenuSceneState.h"
#include "SelectDifficultySceneState.h"
#include "ReadySceneState.h"
#include "PlayingSceneState.h"
#include "ResultSceneState.h"
#include "AfterResultSceneState.h"
#include "TutorialSceneState.h"
#include "EscNightmareSceneState.h"
#include "ExitSceneState.h"
#include "SceneServices.h"
#include "EngineContext.h"
#include <algorithm>

SceneManager::SceneManager() {
	SceneManager::Init();
}

SceneManager::~SceneManager() {}

void SceneManager::Init() {
	setCurrentSceneType(SceneId::Blank);
	setOldSceneType(SceneId::Title);
	setCurrentTutorialStepType(TutorialStepId::None);
	setOldTutorialStepType(TutorialStepId::Consept);

	setResultType(GameResult::None);
	SetIsPaused(false);
}

// 全体、あるいはシーンごとのキー入力処理
void SceneManager::Update(
	SceneId sceneNow, // 引数 sceneNow は SceneLoop 内で getCurrentSceneType() を渡す
	const KeyManager& key,
	HINSTANCE hInst,
	HWND hWnd,
	SceneServices& services) {

	// Escでコマンドラインを閉じた直後は、Escキーが離されるまで
	// シーン入力を止めて遷移誤発火を防ぐ
	if (g_engine.debugCmdLine.suppressEscUntilRelease) {
		if (!key.IsPress(KeyManager::Esc)) {
			g_engine.debugCmdLine.suppressEscUntilRelease = false;
		}
		return;
	}

	// コマンドライン表示中はゲーム側入力を停止する
	if (g_engine.debugCmdLine.isVisible) {
		return;
	}

	switch (sceneNow) {

	case SceneId::Title:
       // Title シーン専用入力
		TitleSceneState::HandleInput(*this, key);
		break;

	case SceneId::Menu:
     // Menu シーン専用入力
		MenuSceneState::HandleInput(*this, key, services);
		break;

	case SceneId::SelectDifficulty:
       // SelectDifficulty シーン専用入力
		SelectDifficultySceneState::HandleInput(*this, key, hInst, hWnd, services);
		break;

	case SceneId::AfterResult:
      // AfterResult シーン専用入力
		AfterResultSceneState::HandleInput(*this, key, hInst, hWnd, services);
		break;

	case SceneId::Ready:
		break;

	case SceneId::Playing:
		// Esc: プレイ中かつプレイヤー生存中のみポーズを切り替える
		if (key.IsTriggered(KeyManager::Esc) &&
			getResultType() == GameResult::None &&
			services.objMgr.getPlayer().getLife() >= 1) {
			TogglePaused();
		}
		if (!IsPaused()) {
			PlayingSceneState::HandleInput(*this, key, services);
		}
		break;

	case SceneId::Result:
       // Result シーン専用入力
		ResultSceneState::HandleInput(*this, key);
		break;

	case SceneId::Tutorial:
		TutorialSceneState::HandleInput(*this, key);
		break;

	default:
		break;
	}
}

// シーン遷移とシーンごとの表示物の設定
void SceneManager::SceneLoop(HINSTANCE hInst, HWND hWnd, const KeyManager& key, SceneServices& services) {

	// 一度すべてのオブジェクトを非表示にする。
	// Scene ごとに指定した表示物の設定を行うこと。
	GameInitializer::HiddenAllObject(hInst, hWnd, services.objMgr);

	// シーン管理
	switch (getCurrentSceneType()) {
	case SceneId::Blank:
		// シーン遷移時に一度だけ処理
		if (getOldSceneType() != SceneId::Blank) {
			DEBUG_LOG("currentScene : Blank\n");
			setOldSceneType(SceneId::Blank);
		}

		// タイトルシーンへ移行
		setCurrentSceneType(SceneId::Title);
		break;

	case SceneId::Title:
        // Title シーン表示設定
		TitleSceneState::SetupView(*this, services);
		break;

	case SceneId::Menu:
        // Menu シーン表示設定
		MenuSceneState::SetupView(*this, services);
		break;

	case SceneId::SelectDifficulty:
        // SelectDifficulty シーン表示設定
		SelectDifficultySceneState::SetupView(*this, services);
		break;

	case SceneId::Ready:
       // Ready シーン表示設定
		ReadySceneState::SetupView(*this, services);
		break;

	case SceneId::Playing:
		PlayingSceneState::Run(*this, key, hInst, services);
		break;

	case SceneId::Result:
       // Result シーン表示設定
		ResultSceneState::SetupView(*this, services);
		break;

	case SceneId::AfterResult:
       // AfterResult シーン表示設定
		AfterResultSceneState::SetupView(*this, services);
		break;

	case SceneId::Tutorial:
		// シーン遷移時に一度だけ処理
		if (getOldSceneType() != SceneId::Tutorial) {
			DEBUG_LOG("currentScene : Tutorial\n");
			setOldSceneType(SceneId::Tutorial);
		}
		// チュートリアルのステップ遷移を管理する関数を呼び出す
		TutorialStepLoop(hInst, hWnd, key, services);
		break;

	case SceneId::EscNightmare:
       // EscNightmare シーン表示設定
		EscNightmareSceneState::SetupView(*this, services);
		break;

	case SceneId::Exit:
		ExitSceneState::SetupExitView(*this, services);

		// 時間経過でウィンドウを閉じる

		break;

	default:
		DEBUG_LOG("*** Scene error : An unexpected scene was called ***\n");
		break;
	}

	// プレイ中以外のシーンで、
	// 回避数の最高記録がゼロでなければ表示する
	if (services.gameState.BestEvadeCnt() != 0) {
		const SceneId currentSceneType = getCurrentSceneType();

		// 表示しない場合の条件
		if (currentSceneType != SceneId::Playing &&
			currentSceneType != SceneId::Ready &&
			currentSceneType != SceneId::EscNightmare &&
			currentSceneType != SceneId::Exit &&
			!(currentSceneType == SceneId::AfterResult &&
				services.gameState.CurrentDifficulty() == Difficulty::Nightmare)) {

			services.objMgr.getUIText(ID_TEXT_BEST_EVADE_CNT).setVisible(true);
		}
	}

	// 描画で使う状態は更新側で確定する
	services.objMgr.getPlayer().advanceMagicSignAngle();
	services.gameState.SetShouldDrawNightmareFade(
		(getCurrentSceneType() == SceneId::AfterResult &&
			services.gameState.CurrentDifficulty() == Difficulty::Nightmare));
	const int16_t fadeAlphaRaw = static_cast<int16_t>(services.gameState.EscCnt() * 8.5);
	services.gameState.SetNightmareFadeAlpha(static_cast<uint8_t>(std::clamp<int16_t>(fadeAlphaRaw, 0, 255)));
	services.gameState.SetBackgroundColor(
		(services.gameState.CurrentDifficulty() == Difficulty::Nightmare)
		? static_cast<uint32_t>(RGB(30, 30, 30))
		: static_cast<uint32_t>(RGB(255, 255, 255)));

	// 描画で使う可変テキスト/色は更新側で確定する
	UI_Text::RefreshAllDrawStates(services.objMgr, services.gameState);
}

// Tutorial シーン中のステップ進行更新を委譲する
void SceneManager::TutorialStepLoop(HINSTANCE hInst, HWND hWnd, const KeyManager& key, SceneServices& services) {
	TutorialSceneState::RunStepLoop(*this, hInst, hWnd, key, services);
}

