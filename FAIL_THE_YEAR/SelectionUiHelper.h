/* =============================================================================
 * ファイル概要:
 * - メニュー系シーンで共通利用する選択カーソル操作ヘルパー。
 * ============================================================================= */
#pragma once

#include "Common.h"
#include "KeyManager.h"
#include "ObjectManager.h"

class SelectionUiHelper {
public:
    // 選択カーソルUIの既定基準座標
    static constexpr int16_t kDefaultCursorX = 310;
    static constexpr int16_t kDefaultFrameX = 350;
    static constexpr int16_t kDefaultTopCursorY = 300;
    static constexpr int16_t kDefaultBottomCursorY = 400;

    // 2択メニュー用の選択移動（0/1のみを行き来する）
    static void MoveTwoChoiceSelection(const KeyManager& key, int& selection) {
        if (key.IsTriggered(KeyManager::Up) && selection == 1) {
            selection = 0;
        }
        if (key.IsTriggered(KeyManager::Down) && selection == 0) {
            selection = 1;
        }
    }

    // 2択メニュー用のカーソル表示位置を反映する
    static void ApplyTwoChoiceCursor(
        ObjectManager& objMgr,
        int selection,
        int16_t cursorX = kDefaultCursorX,
        int16_t frameX = kDefaultFrameX,
        int16_t topCursorY = kDefaultTopCursorY,
        int16_t bottomCursorY = kDefaultBottomCursorY)
    {
        const int16_t cursorY = (selection == 0) ? topCursorY : bottomCursorY;
        const int16_t frameY = static_cast<int16_t>(cursorY - 32);

        objMgr.GetUIShape(ID_SHAPE_SELECTING_TRIANGLE).setPosition(cursorX, cursorY);
        objMgr.GetUIShape(ID_SHAPE_SELECTING_RECTANGLE).setPosition(frameX, frameY);
    }
};
