/* =============================================================================
 * ファイル概要:
 * - シーン種別定義と SceneManager を宣言。
 * ============================================================================= */
#pragma once

struct SceneServices;

enum class SceneId : int16_t {
	Blank = -1,			// なし
	Title,				// タイトル
	Menu,				// メニュー
	SelectDifficulty,	// 難易度選択
	Ready,				// スタート前のカウントダウンなど
	Playing,			// プレイ中
	Clear,				// ゲームクリア
	Fail,				// ゲームオーバー
	Result,				// リザルト画面
	AfterResult,		// ゲーム終了後のメニュー選択
	Ending,				// エンディング(最後のシーンとなる Thank you for playing表示)
	Exit,				// ゲーム終了

	Tutorial,			// チュートリアル

	EscNightmare,		// ナイトメア脱出後
};

enum class GameResult : int16_t {
	None = 0,
	Clear = static_cast<int16_t>(SceneId::Clear),
	Fail = static_cast<int16_t>(SceneId::Fail),
};


enum class TutorialStepId : int16_t {
	None = -1,			// なし
	Consept,			// コンセプト説明
	Move,				// 移動方法説明
	RyunenObject,		// 留年オブジェクト説明
	Life,				// ライフ説明
	Bomb,				// ボム説明
	ClearCondition,		// クリア条件説明
	AfterCanAdv,		// 進級確定後の説明
	EndTutorial,		// チュートリアル終了
};

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	void Init();
	void Update(
		SceneId sceneNow,
		const KeyManager& key,
		HINSTANCE hInst,
		HWND hWnd,
		SceneServices& services);
	void SceneLoop(HINSTANCE hInst, HWND hWnd, const KeyManager& key, SceneServices& services);
	void TutorialStepLoop(HINSTANCE hInst, HWND hWnd, const KeyManager& key, SceneServices& services);

	SceneId getCurrentSceneType() const { return currentScene; }
	void setCurrentSceneType(SceneId scene) { currentScene = scene; }
	const int16_t getCurrentScene() const { return static_cast<int16_t>(currentScene); }
	void setCurrentScene(int16_t scene) { currentScene = static_cast<SceneId>(scene); }

	SceneId getOldSceneType() const { return oldScene; }
	void setOldSceneType(SceneId scene) { oldScene = scene; }
	const int16_t getOldScene() const { return static_cast<int16_t>(oldScene); }
	void setOldScene(int16_t scene) { oldScene = static_cast<SceneId>(scene); }

	TutorialStepId getCurrentTutorialStepType() const { return currentTutorialStep; }
	void setCurrentTutorialStepType(TutorialStepId step) { currentTutorialStep = step; }
	const int16_t getCurrentTutorialStep() const { return static_cast<int16_t>(currentTutorialStep); }
	void setCurrentTutorialStep(int16_t step) { currentTutorialStep = static_cast<TutorialStepId>(step); }

	TutorialStepId getOldTutorialStepType() const { return oldTutorialStep; }
	void setOldTutorialStepType(TutorialStepId step) { oldTutorialStep = step; }
	const int16_t getOldTutorialStep() const { return static_cast<int16_t>(oldTutorialStep); }
	void setOldTutorialStep(int16_t step) { oldTutorialStep = static_cast<TutorialStepId>(step); }

	GameResult getResultType() const { return result; }
	void setResultType(GameResult val) { result = val; }
	const int16_t getResult() const { return static_cast<int16_t>(result); }
	void setResult(int16_t val) { result = static_cast<GameResult>(val); }

	bool IsPaused() const { return m_isPaused; }
	void SetIsPaused(bool value) { m_isPaused = value; }
	void TogglePaused() { m_isPaused = !m_isPaused; }


protected:
	SceneId currentScene;                 // 現在のシーン
	SceneId oldScene;                     // シーン更新時に一回だけ呼ぶ処理で使用
	TutorialStepId currentTutorialStep;   // 現在のチュートリアルステップ
	TutorialStepId oldTutorialStep;       // チュートリアル更新時に一回だけ呼ぶ処理で使用

	GameResult result;
	bool m_isPaused = false;              // ポーズ状態（Playing中のEscで切り替え）

	// 選択UIの基準座標。
	// 矢印はX固定でYのみ動かし、枠も同じYに追従させる。
	static constexpr int16_t cursorX = 310;
	static constexpr int16_t frameX = 350;
};




