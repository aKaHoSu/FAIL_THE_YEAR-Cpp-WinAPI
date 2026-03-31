/* =============================================================================
 * ファイル概要:
 * - 共通基底 ObjectData の処理を実装する。
 * ============================================================================= */
#include "ObjectData.h"

ObjectData::ObjectData()
	: stateData{ 0, 0, 0, false }
	, renderData{ 0, 0, nullptr, { L'\0' } }
	, transformData{ { 0, 0 }, { 0.0f, 0.0f }, { 0, 0 } }
	, collisionData{ { 0, 0, 0, 0 } }
	, id(stateData.id)
	, status(stateData.status)
	, actCnt(stateData.actCnt)
	, visible(stateData.visible)
	, size(transformData.size)
	, position(transformData.position)
	, collision(collisionData.hitbox)
	, speed(transformData.speed) {}

ObjectData::~ObjectData() {
	if (renderData.hBmp) {
		DeleteObject(renderData.hBmp);
		renderData.hBmp = nullptr;
	}
}

// オブジェクト識別IDを設定する
void ObjectData::setId(int16_t id) {
	this->id = id;
}

// ビットマップリソースを読み込み、描画用ハンドルを更新する
void ObjectData::setBmp(HINSTANCE hInst, int16_t bmpid) {
	// 既存のビットマップを解放してからロードする
	if (renderData.hBmp) {
		DeleteObject(renderData.hBmp);
		renderData.hBmp = nullptr;
	}
	renderData.bmpId = bmpid;
	renderData.hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(renderData.bmpId));
}

// 文字列リソースを読み込み、描画用テキストを更新する
void ObjectData::setText(HINSTANCE hInst, int16_t textId) {
	renderData.textId = textId;
	LoadStringW(hInst, renderData.textId, renderData.text, 256);
}

// オブジェクトの状態値を更新する
void ObjectData::setStatus(int16_t status) {
	this->status = status;
}

// オブジェクトの描画サイズを更新する
void ObjectData::setSize(int16_t width, int16_t height) {
	this->size.width = width;
	this->size.height = height;
}

// オブジェクトの基準座標を更新する
void ObjectData::setPosition(float xposition, float yposition) {
	this->position.x = xposition;
	this->position.y = yposition;
}

// 現在座標に差分を加算して移動する
void ObjectData::addPosition(int16_t xAdd, int16_t yAdd) {
	this->position.x += xAdd;
	this->position.y += yAdd;
}

// 1フレームあたりの移動速度を設定する
void ObjectData::setSpeed(int16_t xspeed, int16_t yspeed) {
	this->speed.x = xspeed;
	this->speed.y = yspeed;
}

// 行動カウンタ（状態遷移や演出に利用）を更新する
void ObjectData::setActCnt(int16_t actcnt) {
	this->actCnt = actcnt;
}

// 描画対象に含めるかどうかを切り替える
void ObjectData::setVisible(bool visible) {
	this->visible = visible;
}

// --- メンバ変数のゲッター
HBITMAP ObjectData::getHBitmap() const { return renderData.hBmp; }
const WCHAR* ObjectData::getText() const { return renderData.text; }
int16_t ObjectData::getId() const { return id; }
const int16_t ObjectData::getActCnt() const { return actCnt; }
int16_t ObjectData::getBmpNo() const { return renderData.bmpId; }
const OBJSIZE& ObjectData::getSize() const { return size; }
const OBJPOSITION& ObjectData::getPosition() const { return position; }
const int16_t ObjectData::getStatus() const { return status; }
bool ObjectData::isVisible() const { return visible; }
const OBJHITBOX& ObjectData::getCollision() const { return collision; }

