# 提出用作品 - FAIL THE YEAR

## 1. プロジェクト概要
- ゲームタイトル：FAIL THE YEAR ～留年回避ゲーム～
- ゲームジャンル：落下物回避ゲーム
- コンセプト：「留年を回避して進級する」  
↓ ゲームの紹介・デモプレイ動画

[![FAIL THE YEAR 紹介動画](https://img.youtube.com/vi/fpPF3-fYjk4/0.jpg)](https://www.youtube.com/watch?v=fpPF3-fYjk4)

--- 

## 2. ゲームの遊び方
- 操作方法：十字キー / WASD 移動 ・ Shift 押しっぱで低速 ・ Z / P でボム
- 上から降ってくる「留年」を一定数回避すると進級できる

--- 

## 3. 使用技術・開発環境
- 言語：C++
- API：Windows API（GDI による描画）
- IDE：Visual Studio 2026
- OS：Windows 11
- ウィンドウサイズ：1200 × 700 px
- フレームレート：およそ64fps（15ms毎に更新）

---  

## 4. 重視した点・工夫した点

### ・ ゲームエンジン・外部ライブラリ不使用
Unity ・ Unreal Engine などのゲームエンジンや、 DxLib などのゲーム向けライブラリを使用せず、  Windows API（GDI）のみを用いて制作しました。

### ・ 責務分離を意識
処理を役割ごとに独立したクラスへ分離し、各クラスの責任範囲を明確にすることを意識して制作しました。

--- 

## 5. ビルド・実行方法
- 動作環境：Windows11
- 開発環境
	- Visual Studio 2026
	- Windows SDK（Visual Studio インストール時に同梱）
- ビルド・実行手順
	1. `FAIL_THE_YEAR.slnx` を Visual Studio 2026 で開く
	2. プラットフォームを `x64` に設定する
	4. 構成を `Debug` か `Release` に設定する
	5. **ビルド → ソリューションのビルド**
	6. `x64`ディレクトリ内にある `Debug` あるいは `Release` ディレクトリを開き、中にある `FAIL_THE_YEAR.exe` を実行する
- Debug 構成について  
	`Debug` 構成で実行すると、 `/` キーによるデバッグコマンドラインが有効になります。
	詳細は [`FAIL_THE_YEAR/docs/game_mechanics.md`](FAIL_THE_YEAR/docs/game_mechanics.md) を参照してください。
- 備考  
画像リソースは実行ファイルに組み込み済みのため、`.exe` 単体で動作します。追加ファイルの配置は不要です。

--- 

## 6. ディレクトリ構成
※ `Debug/`・`Release/`・`x64/` はビルド生成物のため省略しています。
```
提出用作品-FAIL_THE_YEAR/
├── README.md
├── LICENSE.txt
├── FAIL_THE_YEAR.slnx
└── FAIL_THE_YEAR/
    ├── AfterResultSceneState.h
    ├── Common.cpp
    ├── Common.h
    ├── ConsoleWindow.cpp
    ├── ConsoleWindow.h
    ├── DebugCommandLine.h
    ├── DebugLog.h
    ├── Draw.cpp
    ├── Draw.h
    ├── EngineContext.cpp
    ├── EngineContext.h
    ├── EscNightmareSceneState.h
    ├── ExitSceneState.h
    ├── FAIL_THE_YEAR.vcxproj
    ├── FAIL_THE_YEAR.vcxproj.filters
    ├── FAIL_THE_YEAR.vcxproj.user
    ├── GameInitializer.cpp
    ├── GameInitializer.h
    ├── GameState.cpp
    ├── GameState.h
    ├── Hitbox.h
    ├── Image/                         # ビットマップ画像
    │   ├── CanAdv.bmp
    │   ├── Logo.bmp
    │   ├── Player.bmp
    │   ├── Ready.bmp
    │   └── Start.bmp
    ├── KeyManager.cpp
    ├── KeyManager.h
    ├── MenuLikeSelectionState.h
    ├── ObjectData.cpp
    ├── ObjectData.h
    ├── ObjectManager.cpp
    ├── ObjectManager.h
    ├── Player.cpp
    ├── Player.h
    ├── PlayingOutcomeSystem.h
    ├── PlayingSceneLifecycle.h
    ├── PlayingSpawnAndEntitySystem.h
    ├── Position.h
    ├── Resource.aps
    ├── Resource.rc
    ├── resource.h
    ├── Ryunen.cpp
    ├── Ryunen.h
    ├── Scene.cpp
    ├── Scene.h
    ├── SceneServices.h
    ├── SelectionUiHelper.h
    ├── Size.h
    ├── Speed.h
    ├── UI_Image.cpp
    ├── UI_Image.h
    ├── UI_Shape.cpp
    ├── UI_Shape.h
    ├── UI_Text.cpp
    ├── UI_Text.h
    ├── UiVisibilityBatchHelper.h
    ├── Utils.h
    ├── WinMain.cpp
    ├── WndProc.cpp
    ├── WndProc.h
    ├── シーン別処理ヘッダー群
    │   ├── AfterResultSceneState.h
    │   ├── EscNightmareSceneState.h
    │   ├── ExitSceneState.h
    │   ├── MenuSceneState.h
    │   ├── PlayingSceneState.h
    │   ├── ProgressionSceneState.h
    │   ├── ReadySceneState.h
    │   ├── ResultSceneState.h
    │   ├── SelectDifficultySceneState.h
    │   ├── TitleSceneState.h
    │   └── TutorialSceneState.h
    ├── docs/                          # ゲーム仕様ドキュメント
    │   ├── game_flow.md
    │   ├── game_flowchart.md
    │   └── game_mechanics.md
    └── sound/                         # サウンドファイル
        ├── Bomb.mp3
        ├── Enter.mp3
        ├── Landed.mp3
        ├── MoveCursor.mp3
        └── Spawn.mp3
```

---

## 7. 今後の課題点
- SE・BGMの実装
- プレイ中のリトライやタイトルに戻る機能の実装
