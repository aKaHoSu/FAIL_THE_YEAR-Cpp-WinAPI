/* =============================================================================
 * ファイル概要:
 * - AfterResult シーンの入力制御と表示設定を記述。
 * ============================================================================= */
#pragma once

#include "Common.h"
#include "Scene.h"
#include "KeyManager.h"
#include "GameInitializer.h"
#include "SceneServices.h"
#include "SelectionUiHelper.h"
#include "UiVisibilityBatchHelper.h"
#include "DebugLog.h"

class AfterResultSceneState {
public:
    // AfterResult シーンの入力処理。
    // Retry / Title の選択、Nightmare 特殊挙動、Esc連打処理を扱う。
    static void HandleInput(
        SceneManager& manager,
        const KeyManager& key,
        HINSTANCE hInst,
        HWND hWnd,
        SceneServices& services)
    {
        // 選択カーソル（三角）と選択枠（四角）の基準座標
        constexpr int16_t kCursorX = 310;
        constexpr int16_t kFrameX = 350;

        // Nightmare 未クリア時 Esc 連打演出に使う定数
        const int16_t TIMER_MAX = 7;
        const int16_t ESC_MAX = 30;

        // Nightmare 未クリアの AfterResult かどうか（専用挙動の判定に使用）
        const bool isNightmareUncleared =
            services.gameState.CurrentDifficulty() == Difficulty::Nightmare &&
            !services.gameState.HasEverClearedNightmare();

        // 上下入力で選択位置を更新（Nightmare 未クリア時はグリッチ風に上下可能）
      if (isNightmareUncleared) {
            if (key.IsTriggered(KeyManager::Up)) {
                m_selection--;
            }
            if (key.IsTriggered(KeyManager::Down)) {
                m_selection++;
            }
        }
        else {
            SelectionUiHelper::MoveTwoChoiceSelection(key, m_selection);
        }

        // 現在の選択位置に応じてカーソルUIを移動
        switch (m_selection) {
        case 0:
         SelectionUiHelper::ApplyTwoChoiceCursor(services.objMgr, 0);
            break;
        case 1:
         SelectionUiHelper::ApplyTwoChoiceCursor(services.objMgr, 1);
            break;
        default:
            break;
        }

        // Enter 決定処理
        if (key.IsTriggered(KeyManager::Enter)) {
            if (isNightmareUncleared) {
                // Nightmare 未クリア時はカーソル位置に関係なくリトライ
                manager.setCurrentSceneType(SceneId::Ready);
                GameInitializer::ResetAllGameObjects(hInst, hWnd, services.gameState, services.objMgr);
            }
            else if (m_selection == 0) {
                // Retry
                manager.setCurrentSceneType(SceneId::Ready);
                GameInitializer::ResetAllGameObjects(hInst, hWnd, services.gameState, services.objMgr);
                m_selection = 0;
            }
            else if (m_selection == 1) {
                // Title
                manager.setCurrentSceneType(SceneId::Title);
                m_selection = 0;
            }
        }

        // Esc で Result に戻る（Nightmare 未クリア時を除く）
        if (key.IsTriggered(KeyManager::Esc)) {
            if (!isNightmareUncleared) {
                manager.setCurrentSceneType(SceneId::Result);
                m_selection = 0;
            }
        }

        // Nightmare 時の Esc 連打演出処理
        if (services.gameState.CurrentDifficulty() == Difficulty::Nightmare &&
            !services.gameState.HasEverClearedNightmare()) {
            if (key.IsTriggered(KeyManager::Esc)) {
                // Esc を押したらカウント増加し、減衰待機タイマーをリセット
                services.gameState.IncrementEscCnt();
                m_escapeTimer = TIMER_MAX;
                DEBUG_LOG("Triggered Esc key. Current EscCnt : %d\n", services.gameState.EscCnt());

                if (services.gameState.EscCnt() >= ESC_MAX) {
                    // しきい値到達で EscNightmare に遷移し、難易度を Easy に戻す
                    manager.setCurrentSceneType(SceneId::EscNightmare);
                    services.gameState.SetIsNightmare(false);
                    services.gameState.SetCurrentDifficulty(Difficulty::Easy);
                    services.gameState.SetEscCnt(0);
                }
            }
            else {
                // Esc 非入力時はタイマー経過後に EscCnt を徐々に減衰
                if (m_escapeTimer > 0) {
                    m_escapeTimer--;
                    DEBUG_LOG("Current value of timer : %d\n", m_escapeTimer);
                }
                else if (services.gameState.EscCnt() > 0) {
                    services.gameState.DecrementEscCnt();
                    m_escapeTimer = 10;
                    DEBUG_LOG("Decrement EscCnt. Current EscCnt : %d\n", services.gameState.EscCnt());
                }
            }
        }
    }

 // AfterResult シーンの表示設定。
    // 難易度条件に応じた文言と選択 UI を可視化する。
    static void SetupView(SceneManager& manager, SceneServices& services) {
        if (manager.getOldSceneType() != SceneId::AfterResult) {
            DEBUG_LOG("currentScene : AfterResult\n");
            manager.setOldSceneType(SceneId::AfterResult);
        }

      if (services.gameState.CurrentDifficulty() == Difficulty::Nightmare &&
            !services.gameState.HasEverClearedNightmare()) {
            // Nightmare 未クリア時は専用テキスト表示
         UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, { ID_TEXT_AFTER_RESULT_NIGHTMARE });
        }
        else {
            // 通常は Retry / Title の2択表示
           UiVisibilityBatchHelper::SetPlayerVisible(services.objMgr, true);
            UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, {
                ID_TEXT_AFTER_RESULT_RETRY,
                ID_TEXT_AFTER_RESULT_TITLE
            });
        }

     UiVisibilityBatchHelper::SetShapesVisible(services.objMgr, {
            ID_SHAPE_VEIL,
            ID_SHAPE_SELECTING_TRIANGLE,
            ID_SHAPE_SELECTING_RECTANGLE
        });
    }

private:
  // AfterResult シーン内の現在選択位置（0:Retry, 1:Title）
    inline static int m_selection = 0;
    // Esc 非入力時に escCnt を減衰させるまでの待機タイマー
    inline static int16_t m_escapeTimer = 0;
};
