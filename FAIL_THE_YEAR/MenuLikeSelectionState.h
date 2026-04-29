/* =============================================================================
 * ファイル概要:
 * - メニュー系シーンの入力遷移処理を記述する。
 *		（Menu, SelectDifficulty, AfterResultシーンで使用）
 * ============================================================================= */
#pragma once
#include "Common.h"
#include "Scene.h"
#include "KeyManager.h"
#include "GameInitializer.h"
#include "SceneServices.h"
#include "DebugLog.h"

class MenuLikeSelectionState {
public:
	// Menu / SelectDifficulty / AfterResult シーン共通の
	// キー入力処理。
	// カーソル移動、難易度選択、Nightmare の Esc 連打を担当。
	static void HandleInput(
		SceneManager& manager,
		int16_t			  sceneNow,
		const KeyManager& key,
		HINSTANCE		  hInst,
		HWND			  hWnd,
		SceneServices& services
	)
	{
		const SceneId sceneNowType = static_cast<SceneId>(sceneNow);
		constexpr int16_t kCursorX = 310;	// 選択カーソルのX座標（矢印・枠共通）
		constexpr int16_t kFrameX = 350;	// 選択枠のX座標

		const int16_t TIMER_MAX = 7;		// Esc 連打の減算遅延タイマーの最大値
		const int16_t ESC_MAX = 30;	// Esc 連打の最大値（これに達すると Nightmare を脱出する）

		// 上下キーで選択カーソルを移動する。
		// グリッチ感演出の為、
		// Nightmare・留年確定時の AfterResult では常に移動可能。
		if (key.IsTriggered(KeyManager::Up)) {
			if (m_menuSelection == 1 ||
				(manager.getCurrentSceneType() == SceneId::AfterResult &&
					services.gameState.CurrentDifficulty() == Difficulty::Nightmare &&
					!services.gameState.HasEverClearedNightmare())) {
				m_menuSelection--;
			}
		}

		if (key.IsTriggered(KeyManager::Down)) {
			if (m_menuSelection == 0 ||
				(manager.getCurrentSceneType() == SceneId::AfterResult &&
					services.gameState.CurrentDifficulty() == Difficulty::Nightmare &&
					!services.gameState.HasEverClearedNightmare())) {
				m_menuSelection++;
			}
		}

		// 非クリア以外、かつ SelectDifficulty 時のみ、
		// 左右キーで Nightmare 選択を可能にする。
		if (services.gameState.HasEverCleared() && sceneNowType == SceneId::SelectDifficulty) {
			if (key.IsTriggered(KeyManager::Right)) {
				m_menuSelection = 2;
			}

			if (key.IsTriggered(KeyManager::Left)) {
				if (m_menuSelection != 1)
					m_menuSelection = 0;
			}
		}

		// Nightmare・留年確定時の AfterResult における Enter は
		// カーソル位置に関わらずリトライ専用の処理を行う。
		if (key.IsTriggered(KeyManager::Enter)) {
			if (manager.getCurrentSceneType() == SceneId::AfterResult &&
				services.gameState.CurrentDifficulty() == Difficulty::Nightmare &&
				!services.gameState.HasEverClearedNightmare()) {

				manager.setCurrentSceneType(SceneId::Ready);
				GameInitializer::ResetAllGameObjects(hInst, hWnd, services.gameState, services.objMgr);
			}
		}

		// カーソル位置(menuSelection)に応じて選択カーソル UI を移動し、
		// Enter 時のシーン遷移を処理する。
		switch (m_menuSelection) {
		case 0:
			// カーソルを上項目（Play / Easy / Retry）に置く。
			services.objMgr.getUIShape(ID_SHAPE_SELECTING_TRIANGLE).setPosition(kCursorX, 300);
			services.objMgr.getUIShape(ID_SHAPE_SELECTING_RECTANGLE).setPosition(kFrameX, 268);

			// Enter キーでシーン遷移する。
			if (key.IsTriggered(KeyManager::Enter)) {
				if (manager.getCurrentSceneType() == SceneId::Menu) {
					manager.setCurrentSceneType(SceneId::SelectDifficulty);
				}
				else if (manager.getCurrentSceneType() == SceneId::SelectDifficulty) {
					services.gameState.SetCurrentDifficulty(Difficulty::Easy);
					manager.setCurrentSceneType(SceneId::Ready);
					GameInitializer::InitAllGameObjects(hInst, hWnd, services.gameState, services.objMgr);					if (!services.gameState.HasEverCleared()) {
						GameInitializer::ResetAllGameObjects(hInst, hWnd, services.gameState, services.objMgr);
					}
				}
				else if (manager.getCurrentSceneType() == SceneId::AfterResult) {
					manager.setCurrentSceneType(SceneId::Ready);
					GameInitializer::ResetAllGameObjects(hInst, hWnd, services.gameState, services.objMgr);
				}
				m_menuSelection = 0;
			}
			break;

		case 1:
			// カーソルを下項目（Tutorial / Hard / Title）に置く。
			services.objMgr.getUIShape(ID_SHAPE_SELECTING_TRIANGLE).setPosition(kCursorX, 400);
			services.objMgr.getUIShape(ID_SHAPE_SELECTING_RECTANGLE).setPosition(kFrameX, 368);

			if (key.IsTriggered(KeyManager::Enter)) {
				if (manager.getCurrentSceneType() == SceneId::Menu) {
					manager.setCurrentSceneType(SceneId::Tutorial);
				}
				else if (manager.getCurrentSceneType() == SceneId::SelectDifficulty) {
					services.gameState.SetCurrentDifficulty(Difficulty::Hard);
					manager.setCurrentSceneType(SceneId::Ready);
					GameInitializer::InitAllGameObjects(hInst, hWnd, services.gameState, services.objMgr);
					if (!services.gameState.IsFirstPlay()) {
						GameInitializer::ResetAllGameObjects(hInst, hWnd, services.gameState, services.objMgr);
					}
				}
				else if (manager.getCurrentSceneType() == SceneId::AfterResult) {
					manager.setCurrentSceneType(SceneId::Title);
				}
				m_menuSelection = 0;
			}
			break;

		case 2:
			// Nightmare 選択状態。
			// 不具合でmenuSelectionが2になってしまった場合の保険も兼ねて、条件を記述
			if (!services.gameState.HasEverCleared() || sceneNowType != SceneId::SelectDifficulty) {
				m_menuSelection = 0;
				break;
			}

			services.objMgr.getUIShape(ID_SHAPE_SELECTING_TRIANGLE).setPosition(1080, 352);
			services.objMgr.getUIShape(ID_SHAPE_SELECTING_RECTANGLE).setPosition(1120, 320);

			if (key.IsTriggered(KeyManager::Enter)) {
				services.gameState.SetCurrentDifficulty(Difficulty::Nightmare);
				services.gameState.SetIsNightmare(true);
				manager.setCurrentSceneType(SceneId::Ready);
				GameInitializer::InitAllGameObjects(hInst, hWnd, services.gameState, services.objMgr);
				GameInitializer::ResetAllGameObjects(hInst, hWnd, services.gameState, services.objMgr);
				m_menuSelection = 0;
			}
			break;

		default:
			break;
		}

		// Esc で一つ前のシーンに戻る。
		if (key.IsTriggered(KeyManager::Esc)) {
			// if文：Nightmare・留年確定時の AfterResult 以外
			if (!(manager.getCurrentSceneType() == SceneId::AfterResult &&
				services.gameState.CurrentDifficulty() == Difficulty::Nightmare) ||
				services.gameState.HasEverClearedNightmare()) {

				manager.setCurrentSceneType(GetPreviousScene(manager.getCurrentSceneType()));
				m_menuSelection = 0;
			}
		}

		// Nightmare AfterResult 時の Esc 連打処理。
		// escCnt が ESC_MAX に達したら EscNightmare へ遷移し、
		// Nightmare を解除する。
		if (manager.getCurrentSceneType() == SceneId::AfterResult
			&& services.gameState.CurrentDifficulty() == Difficulty::Nightmare) {
			if (key.IsTriggered(KeyManager::Esc)) {
				services.gameState.IncrementEscCnt();
				m_escapeTimer = TIMER_MAX;
				DEBUG_LOG("Triggered Esc key. Current EscCnt : %d\n", services.gameState.EscCnt());

				if (services.gameState.EscCnt() >= ESC_MAX) {
					manager.setCurrentSceneType(SceneId::EscNightmare);
					services.gameState.SetIsNightmare(false);
					services.gameState.SetCurrentDifficulty(Difficulty::Easy);
					services.gameState.SetEscCnt(0);
				}
			}
			else {
				if (m_escapeTimer > 0) {
					m_escapeTimer--;
					DEBUG_LOG("Current value of timer : %d\n", m_escapeTimer);
				}
				else {
					if (services.gameState.EscCnt() > 0) {
						services.gameState.DecrementEscCnt();
						m_escapeTimer = 10;
						DEBUG_LOG("Decrement EscCnt. Current EscCnt : %d\n", services.gameState.EscCnt());
					}
				}
			}
		}
	}

	// メニューシーンの表示項目（Play・Tutorial）と
	// 選択カーソル UI を表示する。
	static void SetupMenuView(SceneManager& manager, SceneServices& services) {
		if (manager.getOldSceneType() != SceneId::Menu) {
			DEBUG_LOG("currentScene : Menu\n");
			manager.setOldSceneType(SceneId::Menu);
		}

		services.objMgr.getUIText(ID_TEXT_MENU_PLAY).setVisible(true);
		services.objMgr.getUIText(ID_TEXT_MENU_TUTORIAL).setVisible(true);
		services.objMgr.getUIText(ID_TEXT_MENU_BACK).setVisible(true);
		services.objMgr.getUIShape(ID_SHAPE_SELECTING_TRIANGLE).setVisible(true);
		services.objMgr.getUIShape(ID_SHAPE_SELECTING_RECTANGLE).setVisible(true);
	}

	// 難易度選択シーンの表示項目を設定する。
	// 初プレイ済みの場合のみ Nightmare 選択肢を表示する。
	static void SetupSelectDifficultyView(SceneManager& manager, SceneServices& services) {
		if (manager.getOldSceneType() != SceneId::SelectDifficulty) {
			DEBUG_LOG("currentScene : SelectDifficulty\n");
			manager.setOldSceneType(SceneId::SelectDifficulty);
		}

		services.objMgr.getUIText(ID_TEXT_SELDIFF).setVisible(true);
		services.objMgr.getUIText(ID_TEXT_SELDIFF_BACK).setVisible(true);
		services.objMgr.getUIText(ID_TEXT_SELDIFF_NORMAL).setVisible(true);
		services.objMgr.getUIText(ID_TEXT_SELDIFF_HARD).setVisible(true);
		if (services.gameState.HasEverCleared())
			services.objMgr.getUIText(ID_TEXT_SELDIFF_NIGHTMARE).setVisible(true);

		services.objMgr.getUIShape(ID_SHAPE_FRAME_SELDIFF).setVisible(true);
		services.objMgr.getUIShape(ID_SHAPE_SELECTING_TRIANGLE).setVisible(true);
		services.objMgr.getUIShape(ID_SHAPE_SELECTING_RECTANGLE).setVisible(true);
	}

private:
	// Esc キーで戻る際の遷移先シーンを返す。
	static SceneId GetPreviousScene(SceneId scene) {
		switch (scene) {
		case SceneId::Menu:
			return SceneId::Title;
		case SceneId::SelectDifficulty:
			return SceneId::Menu;
		case SceneId::AfterResult:
			return SceneId::Result;
		default:
			return scene;
		}
	}

	// 現在のカーソル選択位置
	inline static int m_menuSelection = 0;
	// Esc 連打後の escCnt 減算遅延タイマー
	inline static int16_t m_escapeTimer = 0;
};


