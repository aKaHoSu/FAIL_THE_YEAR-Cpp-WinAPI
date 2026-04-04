/* =============================================================================
 * ファイル概要:
 * - チュートリアルの段階進行処理を記述。
 * ============================================================================= */
#pragma once

#include "Common.h"
#include "Scene.h"
#include "KeyManager.h"
#include "Utils.h"
#include "SceneServices.h"
#include "DebugLog.h"

class TutorialSceneState {
public:
	// キー入力
	static void HandleInput(SceneManager& manager, const KeyManager& key) {
		// Enter で次のステップへ進む（EndTutorial 時はタイトルへ戻る）、
		if (key.IsTriggered(KeyManager::Enter)) {
			if (manager.getCurrentTutorialStepType() == TutorialStepId::EndTutorial) {
				manager.setCurrentSceneType(SceneId::Title);
				manager.setCurrentTutorialStepType(TutorialStepId::None);
			}
			else if (manager.getCurrentTutorialStepType() != TutorialStepId::None) {
				manager.setCurrentTutorialStepType(NextStep(manager.getCurrentTutorialStepType()));
			}
		}

		// Esc で前のステップへ戻る（Consept 時はメニューへ）
		if (key.IsTriggered(KeyManager::Esc)) {
			if (manager.getCurrentTutorialStepType() != TutorialStepId::Consept) {
				manager.setCurrentTutorialStepType(PrevStep(manager.getCurrentTutorialStepType()));
			}
			else {
				manager.setCurrentSceneType(SceneId::Menu);
			}
		}
	}

	// チュートリアルの各ステップの表示設定と演出を管理する
	static void RunStepLoop(SceneManager& manager, HINSTANCE hInst, HWND hWnd, const KeyManager& key, SceneServices& services) {
		// ステップが変化した直後の初期化処理
		if (manager.getOldTutorialStepType() != manager.getCurrentTutorialStepType()) {
			services.gameState.SetCanAdv(false);
			services.objMgr.GetPlayer().setStatusType(PlayerStatus::Normal);
			services.objMgr.GetPlayer().setLife(3);
			for (int i = 0; i < MAX_RYUNEN; i++) {
				services.objMgr.GetRyunen(i).setActCnt(0);
			}
		}

		// Back と Next は常に表示（EndTutorial 時は BackTitle に切り替え）
		services.objMgr.GetUIText(ID_TEXT_T_BACK).setVisible(true);
		if (manager.getCurrentTutorialStepType() != TutorialStepId::EndTutorial) {
			services.objMgr.GetUIText(ID_TEXT_T_NEXT).setVisible(true);
		}
		else {
			services.objMgr.GetUIText(ID_TEXT_T_BACK_TITLE).setVisible(true);
		}

		// チュートリアルの段階に応じた表示設定と演出
		switch (manager.getCurrentTutorialStepType()) {
		case TutorialStepId::None:
			if (manager.getOldTutorialStepType() != TutorialStepId::None) {
				DEBUG_LOG("Current tutorial step : None\n");
				manager.setOldTutorialStepType(TutorialStepId::None);
			}
			manager.setCurrentTutorialStepType(TutorialStepId::Consept);
			break;

		case TutorialStepId::Consept:
			if (manager.getOldTutorialStepType() != TutorialStepId::Consept) {
				DEBUG_LOG("Current tutorial step : Consept\n");
				manager.setOldTutorialStepType(TutorialStepId::Consept);
				services.objMgr.GetPlayer().setPosition(900, 310);
				services.objMgr.GetPlayer().setBomb(false);
			}
			services.objMgr.GetUIText(ID_TEXT_T_TITLE).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_CONSEPT1).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_CONSEPT2).setVisible(true);
			services.objMgr.GetUIShape(ID_SHAPE_T_TITLE).setVisible(true);
			services.objMgr.GetPlayer().setVisible(true);
			services.objMgr.GetPlayer().Action(key, manager, services.objMgr, services.gameState);
			break;

		case TutorialStepId::Move:
			if (manager.getOldTutorialStepType() != TutorialStepId::Move) {
				DEBUG_LOG("Current tutorial step : Move\n");
				manager.setOldTutorialStepType(TutorialStepId::Move);
				services.objMgr.GetRyunen(0).setVisible(false);
			}
			services.objMgr.GetPlayer().setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_MOVE).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_M_EXPL1).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_M_EXPL2).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_M_EXPL3).setVisible(true);
			services.objMgr.GetUIShape(ID_SHAPE_T_MOVE).setVisible(true);
			services.objMgr.GetPlayer().Action(key, manager, services.objMgr, services.gameState);
			break;

		case TutorialStepId::RyunenObject:
			if (manager.getOldTutorialStepType() != TutorialStepId::RyunenObject) {
				DEBUG_LOG("Current tutorial step : RyunenObject\n");
				manager.setOldTutorialStepType(TutorialStepId::RyunenObject);
				services.objMgr.GetPlayer().setPosition(900, 310);
				m_oneHit = false;
			}
			services.objMgr.GetPlayer().setVisible(true);
			services.objMgr.GetRyunen(0).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_RYUNEN).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_R_EXPL1).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_R_EXPL2).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_R_EXPL3).setVisible(true);
			services.objMgr.GetUIShape(ID_SHAPE_T_RYUNEN_BOMB).setVisible(true);
			if (!services.objMgr.GetRyunen(0).getPositionFlg()) {
				services.objMgr.GetRyunen(0).SpawnAt(952.0f);
			}
			if (services.objMgr.checkHitBetweenPlayerAndRyunen(services.gameState)) m_oneHit = true;
			if (m_oneHit) services.objMgr.GetPlayer().Action(key, manager, services.objMgr, services.gameState);
			services.objMgr.GetRyunen(0).Action(manager, services.gameState);
			break;

		case TutorialStepId::Life:
			if (manager.getOldTutorialStepType() != TutorialStepId::Life) {
				DEBUG_LOG("Current tutorial step : Life\n");
				manager.setOldTutorialStepType(TutorialStepId::Life);
				services.objMgr.GetPlayer().setPosition(900, 310);
				services.objMgr.GetRyunen(0).SpawnAt(952.0f);
				m_oneHit = false;
				for (int i = 1; i <= 6; i++) services.objMgr.GetRyunen(i).setVisible(false);
			}

			services.objMgr.GetPlayer().setVisible(true);

			if ((!m_oneHit) || (m_oneHit && services.objMgr.GetRyunen(0).getStatusType() != RyunenStatus::Offscreen))
				services.objMgr.GetRyunen(0).setVisible(true);

			services.objMgr.GetUIText(ID_TEXT_T_LIFE).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_L_EXPL1).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_L_EXPL2).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_L_EXPL3).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_L_EXPL4).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_L_EXPL5).setVisible(true);
			services.objMgr.GetUIShape(ID_SHAPE_T_LIFE).setVisible(true);

			if (!services.objMgr.GetRyunen(0).getPositionFlg()) {
				services.objMgr.GetRyunen(0).SpawnAt(952.0f);
			}
			if (services.objMgr.checkHitBetweenPlayerAndRyunen(services.gameState))
				m_oneHit = true;

			if (m_oneHit) {
				services.objMgr.GetPlayer().Action(key, manager, services.objMgr, services.gameState);
				services.objMgr.GetPlayer().setLife(1);
			}

			if (!m_oneHit ||
				(m_oneHit && services.objMgr.GetRyunen(0).getStatusType() != RyunenStatus::Offscreen))
				services.objMgr.GetRyunen(0).Action(manager, services.gameState);

			break;

		case TutorialStepId::Bomb:
			if (manager.getOldTutorialStepType() != TutorialStepId::Bomb) {
				DEBUG_LOG("Current tutorial step : Bomb\n");
				manager.setOldTutorialStepType(TutorialStepId::Bomb);
				services.objMgr.GetPlayer().setLife(3);
				services.objMgr.GetPlayer().setBomb(true);
				services.objMgr.GetRyunen(0).setVisible(false);
				for (int i = 1; i < 6; i++) services.objMgr.GetRyunen(i).setInitData(hInst, i);
			}

			services.objMgr.GetPlayer().setVisible(true);

			for (int i = 1; i < 6; i++) services.objMgr.GetRyunen(i).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_BOMB).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_B_EXPL1).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_B_EXPL2).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_B_EXPL3).setVisible(true);
			services.objMgr.GetUIShape(ID_SHAPE_T_RYUNEN_BOMB).setVisible(true);
			services.objMgr.GetPlayer().Action(key, manager, services.objMgr, services.gameState);

			for (int i = 1; i < 6; i++) {
				services.objMgr.GetRyunen(i).Action(manager, services.gameState);
			}

			if (key.IsTriggered(KeyManager::Z)) {
				services.objMgr.GetPlayer().useBomb(services.objMgr); services.objMgr.GetPlayer().setBombCT(100);
			}

			{
				static constexpr float kBombSpawnX[] = {
					0.0f, // ダミー
					900.0f,
					950.0f,
					1000.0f,
					1050.0f,
					1100.0f
				};

				for (int i = 1; i < 6; i++) {
					if (!services.objMgr.GetRyunen(i).getPositionFlg()) {
						services.objMgr.GetRyunen(i).SpawnAt(kBombSpawnX[i]);
					}
				}
			}

			services.objMgr.checkHitBetweenPlayerAndRyunen(services.gameState);
			break;

		case TutorialStepId::ClearCondition:
			if (manager.getOldTutorialStepType() != TutorialStepId::ClearCondition) {
				DEBUG_LOG("Current tutorial step : ClearCondition\n");
				manager.setOldTutorialStepType(TutorialStepId::ClearCondition);
				services.objMgr.GetPlayer().setPosition(790, 470);
				for (int i = 1; i <= 6; i++) services.objMgr.GetRyunen(i).setVisible(false);
			}

			services.objMgr.GetPlayer().setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_CLEAR).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_C_EXPL1).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_C_EXPL2).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_C_EXPL3).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_C_EXPL4).setVisible(true);
			services.objMgr.GetUIShape(ID_SHAPE_T_CLEAR).setVisible(true);
			break;

		case TutorialStepId::AfterCanAdv:
			if (manager.getOldTutorialStepType() != TutorialStepId::AfterCanAdv) {
				DEBUG_LOG("Current tutorial step : AfterCanAdv\n");
				manager.setOldTutorialStepType(TutorialStepId::AfterCanAdv);
				services.objMgr.GetPlayer().setPosition(790, 470);
				services.objMgr.GetRyunen(6).setPosition(1100, 0);
			}

			services.objMgr.GetRyunen(6).setVisible(true);
			services.gameState.SetCanAdv(true);
			services.objMgr.GetUIText(ID_TEXT_T_AFTER_CLEAR).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_AC_EXPL1).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_AC_EXPL2).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_AC_EXPL3).setVisible(true);
			services.objMgr.GetRyunen(6).Action(manager, services.gameState);

			if (!services.objMgr.GetRyunen(6).getPositionFlg()) {
				services.objMgr.GetRyunen(6).SpawnAt(1100.0f);
			}
			break;

		case TutorialStepId::EndTutorial:
			if (manager.getOldTutorialStepType() != TutorialStepId::EndTutorial) {
				DEBUG_LOG("Current tutorial step : EndTutorial\n");
				manager.setOldTutorialStepType(TutorialStepId::EndTutorial);
				services.objMgr.GetPlayer().setPosition(740, 380);
				services.objMgr.GetRyunen(6).setVisible(false);
			}

			services.objMgr.GetPlayer().setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_END).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_E_EXPL1).setVisible(true);
			services.objMgr.GetUIText(ID_TEXT_T_E_EXPL2).setVisible(true);
			services.objMgr.GetUIShape(ID_SHAPE_T_END).setVisible(true);
			break;
		}
	}

private:
	inline static bool m_oneHit = false;  // 留年に一度でも当たったかどうかのフラグ

	// TutorialStepId(現在のステップ) の次のステップを返す関数
	static TutorialStepId NextStep(TutorialStepId step) {
		switch (step) {
		case TutorialStepId::Consept:
			return TutorialStepId::Move;
		case TutorialStepId::Move:
			return TutorialStepId::RyunenObject;
		case TutorialStepId::RyunenObject:
			return TutorialStepId::Life;
		case TutorialStepId::Life:
			return TutorialStepId::Bomb;
		case TutorialStepId::Bomb:
			return TutorialStepId::ClearCondition;
		case TutorialStepId::ClearCondition:
			return TutorialStepId::AfterCanAdv;
		case TutorialStepId::AfterCanAdv:
			return TutorialStepId::EndTutorial;
		default:
			return step;
		}
	}

	// TutorialStepId(現在のステップ) の前のステップを返す関数
	static TutorialStepId PrevStep(TutorialStepId step) {
		switch (step) {
		case TutorialStepId::Move:
			return TutorialStepId::Consept;
		case TutorialStepId::RyunenObject:
			return TutorialStepId::Move;
		case TutorialStepId::Life:
			return TutorialStepId::RyunenObject;
		case TutorialStepId::Bomb:
			return TutorialStepId::Life;
		case TutorialStepId::ClearCondition:
			return TutorialStepId::Bomb;
		case TutorialStepId::AfterCanAdv:
			return TutorialStepId::ClearCondition;
		case TutorialStepId::EndTutorial:
			return TutorialStepId::AfterCanAdv;
		default:
			return step;
		}
	}
};
