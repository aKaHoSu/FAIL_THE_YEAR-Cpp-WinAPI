/* =============================================================================
 * ファイル概要:
 * - シーン共通で利用する表示切り替え一括ヘルパー。
 * ============================================================================= */
#pragma once

#include <initializer_list>
#include "ObjectManager.h"

class UiVisibilityBatchHelper {
public:
    static void SetPlayerVisible(ObjectManager& objMgr, bool visible = true) {
        objMgr.getPlayer().setVisible(visible);
    }

    static void SetRyunenVisible(ObjectManager& objMgr, int index, bool visible = true) {
        objMgr.getRyunen(index).setVisible(visible);
    }

    static void SetRyunenRangeVisible(ObjectManager& objMgr, int begin, int endInclusive, bool visible) {
        for (int i = begin; i <= endInclusive; ++i) {
            objMgr.getRyunen(i).setVisible(visible);
        }
    }

    static void SetTextsVisible(ObjectManager& objMgr, std::initializer_list<int16_t> ids, bool visible = true) {
        for (const int16_t id : ids) {
            objMgr.getUIText(id).setVisible(visible);
        }
    }

    static void SetShapesVisible(ObjectManager& objMgr, std::initializer_list<int16_t> ids, bool visible = true) {
        for (const int16_t id : ids) {
            objMgr.getUIShape(id).setVisible(visible);
        }
    }

    static void SetImagesVisible(ObjectManager& objMgr, std::initializer_list<int16_t> ids, bool visible = true) {
        for (const int16_t id : ids) {
            objMgr.getUIImage(id).setVisible(visible);
        }
    }
};
