# 提出用作品 - FAIL THE YEAR

## 1. プロジェクト概要
- ゲームタイトル：FAIL THE YEAR ～留年回避ゲーム～
- ゲームジャンル：落下物回避ゲーム
- コンセプト：「留年を回避して進級する」  
↓ ゲームの紹介・デモプレイ動画

[![FAIL THE YEAR 紹介動画](https://img.youtube.com/vi/fpPF3-fYjk4/0.jpg)](https://www.youtube.com/watch?v=fpPF3-fYjk4)

--- 

## 2. ゲームの遊び方
- 操作方法：十字キー移動 ・ Shift 押しっぱで低速 ・ Z でボム
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
```
提出用作品-FAIL_THE_YEAR/
├── README.md
├── LICENSE.txt
├── FAIL_THE_YEAR.slnx
└── FAIL_THE_YEAR/
    ├── FAIL_THE_YEAR.vcxproj
    ├── FAIL_THE_YEAR.vcxproj.filters
    │
    ├── Image/                       # ビットマップ画像
    │   ├── Logo.bmp
    │   ├── Player.bmp
    │   ├── Ready.bmp
    │   ├── Start.bmp
    │   └── CanAdv.bmp
    │
    ├── Sound/                       # サウンドファイル
    │
    ├── docs/                        # ゲーム仕様ドキュメント  
    │   ├── game_mechanics.md            # ゲームルール・オブジェクト仕様  
    │   ├── game_flow.md                 # シーンごとの詳細な挙動・遷移条件  
    │   └── game_flowchart.md            # シーン遷移フローチャート（Mermaid）  
    │  
    ├── Resource.rc                  # リソーススクリプト（画像・文字列の埋め込み）
    ├── resource.h                   # リソース ID 定義    
    │
    ├── WinMain.cpp                  # エントリポイント・メインループ
    ├── WndProc.h / .cpp             # ウィンドウプロシージャ
    ├── EngineContext.h / .cpp       # プロジェクト全体のコンテキスト管理
    ├── Common.h / .cpp              # 定数定義・共有インスタンスの宣言
    │
    ├── Scene.h / .cpp               # SceneManager（シーン遷移管理）
    ├── SceneServices.h              # シーン間の依存関係集約
    ├── *SceneState.h群              # 各シーンのロジック  
    │   ├── Title# タイトル
    │   ├── MenuLikeSelectionState.h	 # メニュー
    │   ├── PlayingSceneState.h			 # プレイ中
    │   ├── TutorialSceneState.h		 # チュートリアル
    │   ├── ExitSceneState.h			 # 終了画面
    │   └── ProgressionSceneState.h		 # その他シーン
    │
    ├── GameState.h / .cpp           # ゲーム進行フラグ・数値の管理
    ├── ObjectManager.h / .cpp       # 全オブジェクトの一元管理
    ├── ObjectData.h / .cpp          # オブジェクト基底クラス
    │
    ├── Player.h / .cpp              # プレイヤーオブジェクト
    ├── Ryunen.h / .cpp              # 留年オブジェクト
    │
    ├── UI_Image.h / .cpp            # 画像 UI オブジェクト
    ├── UI_Text.h / .cpp             # テキスト UI オブジェクト
    ├── UI_Shape.h / .cpp            # 図形 UI オブジェクト
    │
    ├── Draw.h / .cpp                # 描画処理（GDI・ダブルバッファリング）
    ├── KeyManager.h / .cpp          # キー入力管理
    │
    ├── DebugCommandLine.h           # デバッグコマンドラインの状態管理
    ├── DebugLog.h                   # デバッグログ出力（Debug 構成のみ有効）
    └── ConsoleWindow.h / .cpp       # デバッグ用コンソールウィンドウ
```

---

## 7. 今後の課題点
- SE・BGMの実装
- プレイ中のリトライやタイトルに戻る機能の実装
