/* =============================================================================
 * ファイル概要:
 * - 定数の定義。
 * - ヘッダーファイルの集約。
 * - 共有インスタンスの extern 宣言。
 * ============================================================================= */
#pragma once

 /* --- ヘッダーファイルのインクルード --- */
#include <windows.h>
#include <cstdint>

/* --- 定数の定義 --- */
// ウィンドウのサイズ
constexpr int16_t WINDOW_W      = 1200;
constexpr int16_t WINDOW_H      = 700;

// 地面のy座標
constexpr int16_t FIX            = 597;

// 右側の有効x座標
constexpr int16_t RIGHT_FIX      = 1126;

// オブジェクトを点滅させる時の速度
constexpr int16_t BLINK_INTERVAL = 50;

// プレイヤーダメージ後の無敵時間
constexpr int16_t DMG_CT         = 100;

// ボム使用後のクールダウンタイム
constexpr int16_t BOMB_CT        = 2000;

// 各オブジェクトの最大数
constexpr int16_t MAX_UI_IMAGE   = 30;
constexpr int16_t MAX_UI_TEXT    = 70;
constexpr int16_t MAX_UI_SHAPE   = 30;
constexpr int16_t MAX_RYUNEN     = 72;

// 各オブジェクトのID（Draw.cpp でヒットボックス座標補正の分岐に使用）
constexpr int16_t ID_PLAYER = 0;
constexpr int16_t ID_RYUNEN = 1;

// 画像を使用するUIオブジェクトのID
enum class ImageId : int16_t {
	Begin = 0,
	Logo,
	Ready,
	Start,
	CanAdv,
};

constexpr int16_t ID_BEGIN_IMAGE            = static_cast<int16_t>(ImageId::Begin);
constexpr int16_t ID_IMAGE_LOGO             = static_cast<int16_t>(ImageId::Logo);
constexpr int16_t ID_IMAGE_READY            = static_cast<int16_t>(ImageId::Ready);
constexpr int16_t ID_IMAGE_START            = static_cast<int16_t>(ImageId::Start);
constexpr int16_t ID_IMAGE_CANADV           = static_cast<int16_t>(ImageId::CanAdv);

// テキストを使用するUIオブジェクトのID
enum class TextId : int16_t {
	Begin = 0,
	Title,
	MenuPlay,
	MenuTutorial,
	MenuBack,
	SelDiff,
	SelDiffBack,
	SelDiffNormal,
	SelDiffHard,

	Life,

	ResultClear,
	ResultFail,
	ResultEvadeRyunen,
	ResultEvadeShinkyu,
	ResultNext,
	AfterResultRetry,
	AfterResultTitle,
	BestEvadeCnt,

	T_Title,
	T_Consept1,
	T_Consept2,
	T_Move,
	T_M_Expl1,
	T_M_Expl2,
	T_M_Expl3,
	T_Ryunen,
	T_R_Expl1,
	T_R_Expl2,
	T_R_Expl3,
	T_Life,
	T_L_Expl1,
	T_L_Expl2,
	T_L_Expl3,
	T_L_Expl4,
	T_L_Expl5,
	T_Bomb,
	T_B_Expl1,
	T_B_Expl2,
	T_B_Expl3,
	T_Clear,
	T_C_Expl1,
	T_C_Expl2,
	T_C_Expl3,
	T_C_Expl4,
	T_AfterClear,
	T_AC_Expl1,
	T_AC_Expl2,
	T_AC_Expl3,
	T_End,
	T_E_Expl1,
	T_E_Expl2,
	T_Back,
	T_Next,
	T_BackTitle,

	SelDiffNightmare,
	AfterResultNightmare,
	EscNm,

	E_NEW_GUEST,
	E_THANKS,
	E_NM_ESC,
	E_NM_CLEAR,

	PAUSE,
};
constexpr int16_t ID_BEGIN_TEXT                     = static_cast<int16_t>(TextId::Begin);
constexpr int16_t ID_TEXT_TITLE                     = static_cast<int16_t>(TextId::Title);
constexpr int16_t ID_TEXT_MENU_PLAY                 = static_cast<int16_t>(TextId::MenuPlay);
constexpr int16_t ID_TEXT_MENU_TUTORIAL             = static_cast<int16_t>(TextId::MenuTutorial);
constexpr int16_t ID_TEXT_MENU_BACK                 = static_cast<int16_t>(TextId::MenuBack);
constexpr int16_t ID_TEXT_SELDIFF                   = static_cast<int16_t>(TextId::SelDiff);
constexpr int16_t ID_TEXT_SELDIFF_BACK              = static_cast<int16_t>(TextId::SelDiffBack);
constexpr int16_t ID_TEXT_SELDIFF_NORMAL            = static_cast<int16_t>(TextId::SelDiffNormal);
constexpr int16_t ID_TEXT_SELDIFF_HARD              = static_cast<int16_t>(TextId::SelDiffHard);
constexpr int16_t ID_TEXT_LIFE  					= static_cast<int16_t>(TextId::Life);
constexpr int16_t ID_TEXT_RESULT_CLEAR              = static_cast<int16_t>(TextId::ResultClear);
constexpr int16_t ID_TEXT_RESULT_FAIL               = static_cast<int16_t>(TextId::ResultFail);
constexpr int16_t ID_TEXT_RESULT_EVADE_RYUNEN       = static_cast<int16_t>(TextId::ResultEvadeRyunen);
constexpr int16_t ID_TEXT_RESULT_EVADE_SHINKYU      = static_cast<int16_t>(TextId::ResultEvadeShinkyu);
constexpr int16_t ID_TEXT_RESULT_NEXT               = static_cast<int16_t>(TextId::ResultNext);
constexpr int16_t ID_TEXT_AFTER_RESULT_RETRY        = static_cast<int16_t>(TextId::AfterResultRetry);
constexpr int16_t ID_TEXT_AFTER_RESULT_TITLE        = static_cast<int16_t>(TextId::AfterResultTitle);
constexpr int16_t ID_TEXT_BEST_EVADE_CNT            = static_cast<int16_t>(TextId::BestEvadeCnt);
constexpr int16_t ID_TEXT_T_TITLE                   = static_cast<int16_t>(TextId::T_Title);
constexpr int16_t ID_TEXT_T_CONSEPT1                = static_cast<int16_t>(TextId::T_Consept1);
constexpr int16_t ID_TEXT_T_CONSEPT2                = static_cast<int16_t>(TextId::T_Consept2);
constexpr int16_t ID_TEXT_T_MOVE                    = static_cast<int16_t>(TextId::T_Move);
constexpr int16_t ID_TEXT_T_M_EXPL1                 = static_cast<int16_t>(TextId::T_M_Expl1);
constexpr int16_t ID_TEXT_T_M_EXPL2                 = static_cast<int16_t>(TextId::T_M_Expl2);
constexpr int16_t ID_TEXT_T_M_EXPL3                 = static_cast<int16_t>(TextId::T_M_Expl3);
constexpr int16_t ID_TEXT_T_RYUNEN                  = static_cast<int16_t>(TextId::T_Ryunen);
constexpr int16_t ID_TEXT_T_R_EXPL1                 = static_cast<int16_t>(TextId::T_R_Expl1);
constexpr int16_t ID_TEXT_T_R_EXPL2                 = static_cast<int16_t>(TextId::T_R_Expl2);
constexpr int16_t ID_TEXT_T_R_EXPL3                 = static_cast<int16_t>(TextId::T_R_Expl3);
constexpr int16_t ID_TEXT_T_LIFE                    = static_cast<int16_t>(TextId::T_Life);
constexpr int16_t ID_TEXT_T_L_EXPL1                 = static_cast<int16_t>(TextId::T_L_Expl1);
constexpr int16_t ID_TEXT_T_L_EXPL2                 = static_cast<int16_t>(TextId::T_L_Expl2);
constexpr int16_t ID_TEXT_T_L_EXPL3                 = static_cast<int16_t>(TextId::T_L_Expl3);
constexpr int16_t ID_TEXT_T_L_EXPL4                 = static_cast<int16_t>(TextId::T_L_Expl4);
constexpr int16_t ID_TEXT_T_L_EXPL5                 = static_cast<int16_t>(TextId::T_L_Expl5);
constexpr int16_t ID_TEXT_T_BOMB                    = static_cast<int16_t>(TextId::T_Bomb);
constexpr int16_t ID_TEXT_T_B_EXPL1                 = static_cast<int16_t>(TextId::T_B_Expl1);
constexpr int16_t ID_TEXT_T_B_EXPL2                 = static_cast<int16_t>(TextId::T_B_Expl2);
constexpr int16_t ID_TEXT_T_B_EXPL3                 = static_cast<int16_t>(TextId::T_B_Expl3);
constexpr int16_t ID_TEXT_T_CLEAR                   = static_cast<int16_t>(TextId::T_Clear);
constexpr int16_t ID_TEXT_T_C_EXPL1                 = static_cast<int16_t>(TextId::T_C_Expl1);
constexpr int16_t ID_TEXT_T_C_EXPL2                 = static_cast<int16_t>(TextId::T_C_Expl2);
constexpr int16_t ID_TEXT_T_C_EXPL3                 = static_cast<int16_t>(TextId::T_C_Expl3);
constexpr int16_t ID_TEXT_T_C_EXPL4                 = static_cast<int16_t>(TextId::T_C_Expl4);
constexpr int16_t ID_TEXT_T_AFTER_CLEAR             = static_cast<int16_t>(TextId::T_AfterClear);
constexpr int16_t ID_TEXT_T_AC_EXPL1                = static_cast<int16_t>(TextId::T_AC_Expl1);
constexpr int16_t ID_TEXT_T_AC_EXPL2                = static_cast<int16_t>(TextId::T_AC_Expl2);
constexpr int16_t ID_TEXT_T_AC_EXPL3                = static_cast<int16_t>(TextId::T_AC_Expl3);
constexpr int16_t ID_TEXT_T_END                     = static_cast<int16_t>(TextId::T_End);
constexpr int16_t ID_TEXT_T_E_EXPL1                 = static_cast<int16_t>(TextId::T_E_Expl1);
constexpr int16_t ID_TEXT_T_E_EXPL2                 = static_cast<int16_t>(TextId::T_E_Expl2);
constexpr int16_t ID_TEXT_T_BACK                    = static_cast<int16_t>(TextId::T_Back);
constexpr int16_t ID_TEXT_T_NEXT                    = static_cast<int16_t>(TextId::T_Next);
constexpr int16_t ID_TEXT_T_BACK_TITLE              = static_cast<int16_t>(TextId::T_BackTitle);
constexpr int16_t ID_TEXT_SELDIFF_NIGHTMARE         = static_cast<int16_t>(TextId::SelDiffNightmare);
constexpr int16_t ID_TEXT_AFTER_RESULT_NIGHTMARE    = static_cast<int16_t>(TextId::AfterResultNightmare);
constexpr int16_t ID_TEXT_ESC_NM                    = static_cast<int16_t>(TextId::EscNm);
constexpr int16_t ID_TEXT_E_NEW_GUEST               = static_cast<int16_t>(TextId::E_NEW_GUEST);
constexpr int16_t ID_TEXT_E_THANKS                  = static_cast<int16_t>(TextId::E_THANKS);
constexpr int16_t ID_TEXT_E_NM_ESC                  = static_cast<int16_t>(TextId::E_NM_ESC);
constexpr int16_t ID_TEXT_E_NM_CLEAR                = static_cast<int16_t>(TextId::E_NM_CLEAR);
constexpr int16_t ID_TEXT_PAUSE						= static_cast<int16_t>(TextId::PAUSE);

// 図形を使用するUIオブジェクトのID
enum class ShapeId : int16_t {
	Begin = 0,
	SelectingTriangle,
	SelectingRectangle,
	FrameSelDiff,
	Veil,

	T_Title,
	T_Move,
	T_RyunenBomb,
	T_Life,
	T_Clear,
	T_End,
};
constexpr int16_t ID_BEGIN_SHAPE                = static_cast<int16_t>(ShapeId::Begin);
constexpr int16_t ID_SHAPE_SELECTING_TRIANGLE   = static_cast<int16_t>(ShapeId::SelectingTriangle);
constexpr int16_t ID_SHAPE_SELECTING_RECTANGLE  = static_cast<int16_t>(ShapeId::SelectingRectangle);
constexpr int16_t ID_SHAPE_FRAME_SELDIFF        = static_cast<int16_t>(ShapeId::FrameSelDiff);
constexpr int16_t ID_SHAPE_VEIL                 = static_cast<int16_t>(ShapeId::Veil);
constexpr int16_t ID_SHAPE_T_TITLE              = static_cast<int16_t>(ShapeId::T_Title);
constexpr int16_t ID_SHAPE_T_MOVE               = static_cast<int16_t>(ShapeId::T_Move);
constexpr int16_t ID_SHAPE_T_RYUNEN_BOMB        = static_cast<int16_t>(ShapeId::T_RyunenBomb);
constexpr int16_t ID_SHAPE_T_LIFE               = static_cast<int16_t>(ShapeId::T_Life);
constexpr int16_t ID_SHAPE_T_CLEAR              = static_cast<int16_t>(ShapeId::T_Clear);
constexpr int16_t ID_SHAPE_T_END                = static_cast<int16_t>(ShapeId::T_End);

// 各オブジェクトのサイズ
constexpr int16_t PLAYER_WIDTH  = 56;
constexpr int16_t PLAYER_HEIGHT = 62;

constexpr int16_t RYUNEN_WIDTH  = 41;
constexpr int16_t RYUNEN_HEIGHT = 80;

constexpr int16_t LOGO_WIDTH    = 700;
constexpr int16_t LOGO_HEIGHT   = 400;

// --- 難易度
// 難易度識別子（currentDiff に格納する値）
enum class Difficulty : int16_t {
	Easy = 0,
	Hard = 1,
	Nightmare = 2,
};

// 難易度ごとの回避目標数
namespace EvadeGoal {
	constexpr int16_t Easy = 150;
	constexpr int16_t Hard = 450;
	constexpr int16_t Nightmare = 700;
};

// 左右（汎用）
constexpr int16_t LEFT  = -1;
constexpr int16_t RIGHT = 1;

/* --- プロジェクト内の型の前方宣言 --- */
// 実体が必要な翻訳単位は、それぞれの .cpp/.h 内で個別に該当ヘッダを include してください。
class Player;
class Ryunen;
class UI_Image;
class UI_Text;
class UI_Shape;
class ObjectData;
class SceneManager;
class KeyManager;
class GameState;
class ObjectManager;
class EngineContext;
struct GameContext;


/* --- 外部変数の宣言(extern) --- */
extern GameContext g_gameContext;	 // ゲーム全体の共有コンテキスト
extern EngineContext& g_engine;		 // ゲーム全体コンテキスト内のエンジン関連
extern GameState& g_gameState;		 // ゲーム全体コンテキスト内のゲーム進行状態
extern ObjectManager& g_objMgr;		 // ゲーム全体コンテキスト内のゲームオブジェクト管理


