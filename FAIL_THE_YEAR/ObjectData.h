/* =============================================================================
 * ファイル概要:
 * - 全オブジェクト共通基底クラス ObjectData を宣言する。
 * ============================================================================= */
#pragma once
#include <windows.h>
#include <stdint.h>
#include "Position.h"
#include "Size.h"
#include "Hitbox.h"
#include "Speed.h"

// updateDrawState の引数型に使用する前方宣言
class SceneManager;
class GameState;

class ObjectData {

public:
	ObjectData();
	virtual ~ObjectData();

	// オブジェクトの基本的なプロパティのセッター
	void setId(int16_t id);
	void setBmp(HINSTANCE hInst, int16_t bmpid);
	void setText(HINSTANCE hInst, int16_t textId);
	void setStatus(int16_t status);
	void setSize(int16_t width, int16_t height);
	void setPosition(float xposition, float yposition);
	void addPosition(int16_t xAdd, int16_t yAdd);
	void setSpeed(int16_t xspeed, int16_t yspeed);
	void setActCnt(int16_t actcnt);

	// 描画や外部からの利用のためのゲッター
	HBITMAP getHBitmap() const;
	const WCHAR* getText() const;
	int16_t getId() const;
	const int16_t getActCnt() const;
	int16_t getBmpNo() const;
	const OBJSIZE& getSize() const;
	const OBJPOSITION& getPosition() const;
	const int16_t getStatus() const;

	// 描画処理を制御するためのフラグ
	void setVisible(bool visible);
	bool isVisible() const;

	// ヒットボックスへ読み取り専用アクセス
	const OBJHITBOX& getCollision() const;

	// ステータスを任意の enum 型として取得する
	template<typename TStatus>
	TStatus getStatusAs() const {
		return static_cast<TStatus>(getStatus());
	}

	// ステータスを任意の enum 型で設定する
	template<typename TStatus>
	void setStatusAs(TStatus next) {
		setStatus(static_cast<int16_t>(next));
	}

protected:
	struct ObjectStateData {
		int16_t id;
		int16_t status;
		int16_t actCnt;
		bool visible;
	};

	struct RenderData {
		int16_t bmpId;
		int16_t textId;
		HBITMAP hBmp;
		WCHAR text[256];
	};

	struct TransformData {
		OBJSIZE size;
		OBJPOSITION position;
		OBJSPEED speed;
	};

	struct CollisionData {
		OBJHITBOX hitbox;
	};

	// 派生クラスで描画状態を更新するための仮想関数。
	// ゲームオブジェクト系のサブクラスはこれをオーバーライドする。
	virtual void updateDrawState(
		const SceneManager& sceneManager,
		const GameState& gameState) {}

	void clearSourceText() {
		for (int i = 0; i < 256; ++i) {
			renderData.text[i] = L'\0';
		}
	}
	void setSourceText(const WCHAR* value) {
		wcscpy_s(renderData.text, 256, value ? value : L"");
	}
	const WCHAR* sourceText() const { return renderData.text; }

	ObjectStateData stateData;   // 状態関連（ID/状態/行動カウント/可視）
	RenderData      renderData;  // 描画リソース情報（画像/テキスト）
	TransformData   transformData;// 変換情報（サイズ/座標/速度）
	CollisionData   collisionData;// 当たり判定情報

	// 既存コード互換のための参照メンバ
	int16_t&		id;
	int16_t&		status;
	int16_t&		actCnt;
	bool&			visible;
	OBJSIZE&		size;
	OBJPOSITION&	position;
	OBJHITBOX&		collision;
	OBJSPEED&		speed;

};


