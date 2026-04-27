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
		UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, { ID_TEXT_T_BACK });
		if (manager.getCurrentTutorialStepType() != TutorialStepId::EndTutorial) {
			UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, { ID_TEXT_T_NEXT });
		}
		else {
			UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, { ID_TEXT_T_BACK_TITLE });
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
			UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, {
				ID_TEXT_T_TITLE,
				ID_TEXT_T_CONSEPT1,
				ID_TEXT_T_CONSEPT2 });
			UiVisibilityBatchHelper::SetShapesVisible(services.objMgr, { ID_SHAPE_T_TITLE });
			UiVisibilityBatchHelper::SetPlayerVisible(services.objMgr);
			services.objMgr.GetPlayer().Action(key, manager, services.objMgr, services.gameState);
			break;

		case TutorialStepId::Move:
			if (manager.getOldTutorialStepType() != TutorialStepId::Move) {
				DEBUG_LOG("Current tutorial step : Move\n");
				manager.setOldTutorialStepType(TutorialStepId::Move);
				services.objMgr.GetRyunen(0).setVisible(false);
			}
			UiVisibilityBatchHelper::SetPlayerVisible(services.objMgr);
			UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, {
				ID_TEXT_T_MOVE,
				ID_TEXT_T_M_EXPL1,
				ID_TEXT_T_M_EXPL2,
				ID_TEXT_T_M_EXPL3 });
			UiVisibilityBatchHelper::SetShapesVisible(services.objMgr, { ID_SHAPE_T_MOVE });
			services.objMgr.GetPlayer().Action(key, manager, services.objMgr, services.gameState);
			break;

		case TutorialStepId::RyunenObject:
			if (manager.getOldTutorialStepType() != TutorialStepId::RyunenObject) {
				DEBUG_LOG("Current tutorial step : RyunenObject\n");
				manager.setOldTutorialStepType(TutorialStepId::RyunenObject);
				services.objMgr.GetPlayer().setPosition(900, 310);
				m_oneHit = false;
			}
			UiVisibilityBatchHelper::SetPlayerVisible(services.objMgr);
			UiVisibilityBatchHelper::SetRyunenVisible(services.objMgr, 0);
			UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, {
				ID_TEXT_T_RYUNEN,
				ID_TEXT_T_R_EXPL1,
				ID_TEXT_T_R_EXPL2,
				ID_TEXT_T_R_EXPL3 });
			UiVisibilityBatchHelper::SetShapesVisible(services.objMgr, { ID_SHAPE_T_RYUNEN_BOMB });
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

			UiVisibilityBatchHelper::SetPlayerVisible(services.objMgr);

			if ((!m_oneHit) || (m_oneHit && services.objMgr.GetRyunen(0).getStatusType() != RyunenStatus::Offscreen))
				UiVisibilityBatchHelper::SetRyunenVisible(services.objMgr, 0);

			UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, {
				   ID_TEXT_T_LIFE,
				   ID_TEXT_T_L_EXPL1,
				   ID_TEXT_T_L_EXPL2,
				   ID_TEXT_T_L_EXPL3,
				   ID_TEXT_T_L_EXPL4,
				   ID_TEXT_T_L_EXPL5 });
			UiVisibilityBatchHelper::SetShapesVisible(services.objMgr, { ID_SHAPE_T_LIFE });

			if (!services.objMgr.GetRyunen(0).getPositionFlg()) {
				services.objMgr.GetRyunen(0).SpawnAt(952.0f);
			}
			if (services.objMgr.checkHitBetweenPlayerAndRyunen(services.gameState))
				m_oneHit = true;

			if (m_oneHit) {
				services.objMgr.GetPlayer().Action(key, manager, services.objMgr, services.gameState);

				if (services.objMgr.GetPlayer().getLife() > 1)
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

			UiVisibilityBatchHelper::SetPlayerVisible(services.objMgr);

			UiVisibilityBatchHelper::SetRyunenRangeVisible(services.objMgr, 1, 5, true);
			UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, {
				ID_TEXT_T_BOMB,
				ID_TEXT_T_B_EXPL1,
				ID_TEXT_T_B_EXPL2,
				ID_TEXT_T_B_EXPL3 });
			UiVisibilityBatchHelper::SetShapesVisible(services.objMgr, { ID_SHAPE_T_RYUNEN_BOMB });
			services.objMgr.GetPlayer().Action(key, manager, services.objMgr, services.gameState);

			for (int i = 1; i < 6; i++) {
				services.objMgr.GetRyunen(i).Action(manager, services.gameState);
			}

			if (key.IsTriggered(KeyManager::Z) || key.IsTriggered(KeyManager::P)) {
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

			UiVisibilityBatchHelper::SetPlayerVisible(services.objMgr);
			UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, {
				ID_TEXT_T_CLEAR,
				ID_TEXT_T_C_EXPL1,
				ID_TEXT_T_C_EXPL2,
				ID_TEXT_T_C_EXPL3,
				ID_TEXT_T_C_EXPL4 });
			UiVisibilityBatchHelper::SetShapesVisible(services.objMgr, { ID_SHAPE_T_CLEAR });
			break;

		case TutorialStepId::AfterCanAdv:
			if (manager.getOldTutorialStepType() != TutorialStepId::AfterCanAdv) {
				DEBUG_LOG("Current tutorial step : AfterCanAdv\n");
				manager.setOldTutorialStepType(TutorialStepId::AfterCanAdv);
				services.objMgr.GetPlayer().setPosition(790, 470);
				services.objMgr.GetRyunen(6).setPosition(1100, 0);
			}

			UiVisibilityBatchHelper::SetRyunenVisible(services.objMgr, 6);
			services.gameState.SetCanAdv(true);
			UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, {
				  ID_TEXT_T_AFTER_CLEAR,
				  ID_TEXT_T_AC_EXPL1,
				  ID_TEXT_T_AC_EXPL2,
				  ID_TEXT_T_AC_EXPL3 });
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

			UiVisibilityBatchHelper::SetPlayerVisible(services.objMgr);
			UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, {
				ID_TEXT_T_END,
				ID_TEXT_T_E_EXPL1,
				ID_TEXT_T_E_EXPL2 });
			UiVisibilityBatchHelper::SetShapesVisible(services.objMgr, { ID_SHAPE_T_END });
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
