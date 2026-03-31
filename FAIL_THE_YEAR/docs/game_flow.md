# FAIL THE YEAR - ゲーム仕様（現行実装ベース）

> このドキュメントは、現行コード（`SceneManager` / 各 `*SceneState`）に基づく実際の遷移と挙動をまとめたものです。  

## 0. 起動時
- 初期シーンは `Blank`。
- `SceneLoop` 内で `Blank -> Title` に即時遷移。

---

## 1. タイトル画面（`Title`）
- 表示:
  - ロゴ画像
  - テキスト（点滅表示）
    > Enterキーで続ける / Escキーでゲームを終了  
- 入力:
  - `Enter` -> `Menu`
  - `Esc` -> `Exit`（`Exit` シーン経由でゲーム終了）
- 初期化:
  - 難易度を `Easy` にリセット（NMのベールを解除）

---

## 2. メニュー画面（`Menu`）
- 表示:
  - テキスト（選択肢）
    > ゲームを遊ぶ（`Play`）  
    > チュートリアル（`Tutorial`）  
    > ⇦  Escキーでタイトルに戻る  
  - 図形（選択中の項目を指す）
    - 三角形
    - 矩形
- 入力:
  - 十字キーの上下で選択
  - `Enter` で確定
    - ゲームを遊ぶ（`Play`） -> `SelectDifficulty`
    - チュートリアル（`Tutorial`） -> `Tutorial`
  - `Esc` -> `Title`

---

## 3. 難易度選択（`SelectDifficulty`）
- 表示:
    - テキスト
      > かんたんめ（`Easy`）
      > むずかしめ（`Hard`）  
      > N（`Nightmare`）（`hasEverCleared == true` のときのみ表示/選択可）  
    - 図形（選択中の項目を指す）
      - 三角形
      - 矩形
    - 難易度選択枠
- 入力:
  - 十字キーの上下で `Easy / Hard` 選択
  - 十字キーの左右で `Nightmare` 選択（`hasEverCleared == true` のみ）
  - `Enter` で確定 -> 難易度を確定し `Ready` へ
  - `Esc` -> `Menu`

---

## 4. プレイ開始前（`Ready`）
- 表示:
  - `READY` 画像が左スライド → 一定時間後に `START` 画像表示
  - プレイヤー
- 処理:
  - 難易度ごとの初期値設定（遷移直後のみ）

    | 難易度 | ライフ | 回避目標数（evadeGoal） | ボムCT |
    |---|---|---|---|
    | Easy | 5 | 150 | 3000 |
    | Hard | 3 | 450 | 3000 |
    | Nightmare | 1 | 700 | 2000 |

- 遷移:
  - `START` 画像表示終了後に `Playing` へ自動遷移

---

## 5. プレイ中（`Playing`）
- 表示:
  - プレイヤー
  - 留年/進級テキスト
    > 留年  
    > 進級  
  - UI（進級確定カットイン画像など）
- 入力:
  - 矢印キーで移動
  - `Shift(Press)` で低速移動
  - `Z` でボム
- 進行:
  - 留年オブジェクトのスポーン/落下/衝突判定
  - 回避数（`evadeCnt`）が目標（`evadeGoal`）到達で進級確定（`canAdv = true`）
    - 全留年の状態を `Landed` に変更し、スポーンを一時停止
    - プレイヤーのライフを 1 にリセット
    - 進級確定カットインアニメーション終了後、スポーン再開
- 結果判定:
  - ライフ `<= 0` かつ `resultType == None` の時点で結果を確定:
    - `canAdv == false` -> `resultType = Fail`
    - `canAdv == true` -> `resultType = Clear`
  - 全留年オブジェクトが画面外に出た時点で `Result` へ遷移

---

## 6. リザルト（`Result`）
- 表示:
  - 結果テキスト
    > ゲームクリア or 留年確定  
  - 回避数関連テキスト
    > 回避した留年の数：%d  
    > 追い込んだ数　　：%d（クリア時のみ）  
    > ベスト回避記録：%d（ベスト記録更新時のみ）  
  - 次画面案内テキスト（点滅）
    > - Enterキーで続ける -  
- 処理（遷移直後のみ）:
  - `resultType == Clear` のとき:
    - `evadeCnt > bestEvadeCnt` なら `bestEvadeCnt` を更新
    - `hasEverCleared = true` に設定
    - Nightmare 難易度の場合、`hasEverClearedNightmare = true` に設定
  - `resultType == Fail` のとき:
    - 回避数テキストの表示位置を下にずらす（追い込んだ数を表示しないため）
  - `isFirstPlay = false` に設定（結果に関わらず）
- 入力:
  - `Enter` -> `AfterResult`

---

## 7. リザルト後メニュー（`AfterResult`）

### 通常（`Easy` / `Hard`、または Nightmare クリア済み）
- 表示:
  - テキスト（選択肢）
    > は じ め か ら  
    > タイトルに戻る  
  - 図形（選択中の項目を指す）
    - 三角形
    - 矩形
- 入力:
  - 十字キーの上下で選択
  - `Enter` で確定
    - `Retry` -> `Ready`
    - `Title` -> `Title`
  - `Esc` -> `Result`

### `Nightmare` かつ未クリア（`hasEverClearedNightmare == false`）
- 表示:
  - 専用テキスト
    > 逃避など許されない逃避など許されない逃避など許されない  
  - フェード演出（`escCnt` に応じてアルファ値が増加）
  - 文字が震える演出（`escCnt` に応じて震えの強さが変化）
- 入力:
  - `Enter` -> `Ready`（再挑戦）
  - `Esc` 連打で `escCnt` 増加（連打を止めると自動減少）
    - `escCnt >= 30` で `EscNightmare` へ遷移し、難易度を `Easy` にリセット

---

## 8. Nightmare離脱演出（`EscNightmare`）
- 処理（遷移直後のみ）:
  - `hasEverEscapedNightmare = true` に設定
- 表示:
  - 専用テキスト（50フレーム間表示）
    > おかえりなさい。  
- 遷移:
  - 100フレーム後に `Title` へ自動遷移

---

## 9. Exit シーン（`Exit`）
- 処理:
  - ゲームの進行状況に応じたテキストを表示（優先順位順）:

    | 条件 | 表示テキスト |
    |---|---|
    | NMをクリア（`hasEverClearedNightmare == true`） | ...君、本当に人間？ |
    | NMを脱出（`hasEverEscapedNightmare == true`） | 災難だったね |
    | 一回以上プレイ（`isFirstPlay == false`） | 遊んでくれてありがとう |
    | 上記すべて該当なし | 次は遊んでね |

  - 100フレーム後にウィンドウを閉じる（`WM_DESTROY`をSendMessageで送信）

---

## 10. チュートリアル（`Tutorial`）
- ステップ:
  1. `Consept`
     > チュートリアル  
     > 留年の危機が迫ってる彼→  
     > 留年を回避して、棒人間くんを進級させよう!  
  2. `Move`
     > 移動方法  
     > 十字キーで移動できるよ  
     > Shiftキーを押している間は  
     > 低速で移動するよ  
  3. `RyunenObject`
     > 留年について  
     > これに当たってしまうと  
     > 棒人間くんのSUN値が減ってしまいます  
     > 灰色になった留年は当たっても大丈夫  
  4. `Life`
     > SUN値について  
     > SUN値がゼロになると  
     > 留年が確定してしまう  
     > SUN値は表示されないので注意!  
     > SUN値が残り一つになると  
     > 棒人間くんは震え上がるぞ!  
  5. `Bomb`
     > ボムについて  
     > 足元に魔法陣が出てるときは  
     > Zキーでボムが発動できるぞ!  
     > ボム発動後はしばらく使えなくなるので注意  
  6. `ClearCondition`
     > クリア条件について  
     > 留年を一定数回避すると、  
     > 進級確定カットインが走るよ  
     > カットインは見てからのお楽しみ...  
     > 見せてくれよ、進級確定の景色...  
  7. `AfterCanAdv`
     > その後は、進級が降ってきます  
     > この進級に当たれば、晴れて進級となります  
     > さっさと進級してもいいし、  
     > 自らを追い込んで成績をもっとよくしてもいい  
  8. `EndTutorial`
     > チュートリアルは以上!  
     > がんばって進級してね  
     > おねがいね  
- 共通表示（下部ナビゲーション）:
  > ⇦  Escキーで戻る  
  > Enterキーで次へ ⇨  
  > Enterキーでタイトルへ ⇨（`EndTutorial` 時のみ）  
- 入力:
  - `Enter` で次ステップ
  - `Esc` で前ステップ
  - 最初のステップ（`Consept`）で `Esc` -> `Menu`
  - `EndTutorial` で `Enter` -> `Title`（ステップを `None` に戻す）

---

## 11. デバッグコマンド（全シーン共通事項）
- デバッグ機能は、ビルド構成が `Debug` のときに有効化  
- 実行画面で'/'キーを押すと、コマンドラインが表示される
- コマンド入力中に `Backspace` で末尾一文字を削除
- コマンド入力後は `Enter` で確定。対応するデバッグ機能が実行される
- コマンドライン表示中に `Esc` で、コマンド入力をキャンセル（入力内容は破棄される）
- コマンドライン表示中は、`Enter`や`Esc`によるシーン遷移は行われない
- デバッグ機能は、コマンドラインに以下のコマンドを入力することで実行可能:

  | コマンド | 効果 |
  |---|---|
  | `godmode` | ゴッドモード（無敵）を切り替え |
  | `hitbox` | ヒットボックス表示を切り替え |
  | `firstplay` | 初回プレイフラグを切り替え |
  | `clearflag` | クリアフラグを切り替え |
  | `help` | 有効コマンドを一覧表示 |

--- 

## 補足
- `ベスト回避記録` は、Playing / Ready / EscNightmare 以外のシーンで、かつ Nightmare 未クリア時の AfterResult 以外の場合に表示される（`bestEvadeCnt != 0` が前提）。
- `SceneId::Clear`・`SceneId::Fail`・`SceneId::Ending` は列挙体に定義されているが、シーンとして直接遷移されることはない（`Clear`/`Fail` は `GameResult` 型として内部的に使用）。
- Nightmare 時は背景色が `RGB(30, 30, 30)`（暗色）になる。
- デバッグ機能としてゴッドモード（無敵）・ヒットボックス表示がある（コンソールから操作）。
