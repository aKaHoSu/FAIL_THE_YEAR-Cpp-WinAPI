/* =============================================================================
 * ファイル概要:
 * - EscNightmare シーンの表示設定と遷移処理を記述。
 * ============================================================================= */
#pragma once

#include "Common.h"
#include "Scene.h"
#include "SceneServices.h"
#include "UiVisibilityBatchHelper.h"
#include "DebugLog.h"

class EscNightmareSceneState {
public:
 // EscNightmare シーンの表示更新。
    // 専用メッセージを一定時間表示し、時間経過で Title へ戻す。
    static void SetupView(SceneManager& manager, SceneServices& services) {
        // シーン遷移直後に1回だけ初期化
        if (manager.getOldSceneType() != SceneId::EscNightmare) {
            DEBUG_LOG("currentScene : EscNightmare\n");
            manager.setOldSceneType(SceneId::EscNightmare);
            services.objMgr.GetUIText(ID_TEXT_ESC_NM).setActCnt(0);
            // Nightmare 脱出履歴を記録
            services.gameState.SetHasEverEscapedNightmare(true);
        }

        // 表示時間管理のためのカウンタを進める
        services.objMgr.GetUIText(ID_TEXT_ESC_NM).setActCnt(
            services.objMgr.GetUIText(ID_TEXT_ESC_NM).getActCnt() + 1);

        // 前半フレームのみテキストを表示
        if (services.objMgr.GetUIText(ID_TEXT_ESC_NM).getActCnt() <= 50) {
         UiVisibilityBatchHelper::SetTextsVisible(services.objMgr, { ID_TEXT_ESC_NM });
        }

        // 一定時間経過で Title へ自動遷移
        if (services.objMgr.GetUIText(ID_TEXT_ESC_NM).getActCnt() >= 100) {
            manager.setCurrentSceneType(SceneId::Title);
        }
    }
};
