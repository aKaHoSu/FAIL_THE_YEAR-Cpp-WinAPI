# FAIL THE YEAR - ゲームフロー（フローチャート版）

> 現行実装（`SceneManager` / 各 `*SceneState`）に基づく遷移を、Mermaid フローチャートで可視化したドキュメントです。

## メイン遷移

```mermaid
flowchart TD
    A[Blank] --> B[Title]

    B -->|Enter| C[Menu]
    B -->|Esc| X[Exit]
    X -->|500フレーム後| Z[Application End]

    C -->|Play| D[SelectDifficulty]
    C -->|Tutorial| T[Tutorial]
    C -->|Esc| B

    D -->|Easy: Enter| E[Ready]
    D -->|Hard: Enter| E
    D -->|Nightmare: Enter / hasEverCleared=true のみ| E
    D -->|Esc| C

    E --> F[Playing]

    F -->|ライフ0 かつ canAdv=false\nresultType=Fail\n全留年オフスクリーン後| R[Result]
    F -->|ライフ0 かつ canAdv=true\nresultType=Clear\n全留年オフスクリーン後| R

    R -->|Enter| I[AfterResult]

    I -->|Easy/Hard または Nightmare クリア済み: Retry 選択| E
    I -->|Easy/Hard または Nightmare クリア済み: Title 選択| B
    I -->|Easy/Hard または Nightmare クリア済み: Esc| R
    I -->|Nightmare 未クリア: Enter| E
    I -->|Nightmare 未クリア: Esc 連打 30回| J[EscNightmare]

    J -->|100フレーム後| B
```

## チュートリアル遷移

```mermaid
flowchart TD
    T0[Tutorial: None] --> T1[Consept]
    T1 -->|Enter| T2[Move]
    T2 -->|Enter| T3[RyunenObject]
    T3 -->|Enter| T4[Life]
    T4 -->|Enter| T5[Bomb]
    T5 -->|Enter| T6[ClearCondition]
    T6 -->|Enter| T7[AfterCanAdv]
    T7 -->|Enter| T8[EndTutorial]
    T8 -->|Enter| B[Title]

    T2 -->|Esc| T1
    T3 -->|Esc| T2
    T4 -->|Esc| T3
    T5 -->|Esc| T4
    T6 -->|Esc| T5
    T7 -->|Esc| T6
    T8 -->|Esc| T7

    T1 -->|Esc| C[Menu]
```

## 補足
- `SceneId::Clear`・`SceneId::Fail` は列挙体に存在するが、シーンとしての直接遷移は行われない。プレイ中の結果確定は `GameResult` 型で管理され、全留年オフスクリーン後に `Result` シーンへ遷移する。
- `SceneId::Ending` は列挙体に存在するが、現行遷移では未使用。
- `Exit` シーンはタイトルの Esc で呼び出され、ゲーム進行状況に応じたテキストを 100 フレーム表示した後にウィンドウを閉じる（即時終了ではない）。
