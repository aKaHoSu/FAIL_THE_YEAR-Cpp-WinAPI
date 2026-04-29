/* =============================================================================
 * ファイル概要:
 * - Playing シーンの勝敗・進級確定・演出進行・遷移判定を管理する。
 * ============================================================================= */
#pragma once

#include "Common.h"
#include "Scene.h"
#include "SceneServices.h"

class PlayingOutcomeSystem {
public:
    // ライフが尽きたタイミングでリザルトを確定する
    static void EvaluateGameResult(SceneManager& manager, SceneServices& services) {
        if (services.objMgr.getPlayer().getLife() <= 0 &&
            manager.getCurrentSceneType() == SceneId::Playing &&
            manager.getResultType() == GameResult::None) {
            if (services.gameState.CanAdv()) {
               // 進級確定後に被弾した場合はクリア扱い
                manager.setResultType(GameResult::Clear);
            }
            else {
                // 進級未確定でライフ枯渇した場合は留年確定
                manager.setResultType(GameResult::Fail);
              // 留年確定時は表示位置を調整（追い込み数テキスト非表示対応）
                services.objMgr.getUIText(ID_TEXT_RESULT_EVADE_RYUNEN).addPosition(0, 45);
            }
          // 勝敗確定後はスポーン停止し、経過カウンタをリセット
            services.gameState.SetCanSpawn(false);
            services.gameState.SetGameCnt(0);
        }
    }

    // 回避目標達成時に進級確定状態へ移行する
    static void EvaluateCanAdv(SceneManager& manager, SceneServices& services) {
        if (services.gameState.EvadeCnt() >= services.gameState.EvadeGoal() &&
            !services.gameState.CanAdv() &&
            manager.getCurrentSceneType() == SceneId::Playing) {
          // 画面上の留年を着地状態にし、進級確定演出へ移行
            for (int i = 0; i < MAX_RYUNEN; ++i) {
                services.objMgr.getRyunen(i).setStatusType(RyunenStatus::Landed);
            }
            services.gameState.SetCanSpawn(false);
            services.gameState.SetRyunenCount(5);
            services.gameState.SetGameCnt(0);
            services.gameState.SetCanAdv(true);
         // 進級確定後はライフを1に固定
            services.objMgr.getPlayer().setLife(1);
        }
    }

    // 進級確定時のカットイン演出を1フレーム分更新する
    static void UpdateCanAdvCutIn(SceneManager& manager, SceneServices& services) {
        if (services.gameState.CanAdv() &&
            !services.gameState.CanSpawn() &&
            manager.getCurrentSceneType() == SceneId::Playing) {
            auto& img = services.objMgr.getUIImage(ID_IMAGE_CANADV);
            img.setVisible(true);
            img.setActCnt(img.getActCnt() + 1);
            // フレーム帯ごとに速度を変えてカットインを制御
            if (img.getActCnt() <= 11) {
                img.setPosition(img.getPosition().x - 80.0f, img.getPosition().y - 2.3f);
            }
            else if (img.getActCnt() <= 70) {
                img.setPosition(img.getPosition().x - 1.2f, img.getPosition().y - 0.2f);
            }
            else if (img.getActCnt() <= 120) {
                img.setPosition(img.getPosition().x - 80.0f, img.getPosition().y - 2.3f);
            }
            else {
               // カットイン完了後に再スポーンを許可
                services.gameState.SetCanSpawn(true);
            }
        }
    }

    // 全留年が画面外かどうかを確認する
    static bool CheckAllOffScreen(SceneServices& services) {
        for (int i = 0; i < MAX_RYUNEN; ++i) {
            if (services.objMgr.getRyunen(i).isVisible()) {
                // 1体でも画面内扱いが残っていれば遷移不可
                if (services.objMgr.getRyunen(i).getStatusType() != RyunenStatus::Offscreen) {
                    return false;
                }
            }
        }
        return true;
    }
};
