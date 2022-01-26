//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _MANAGER_H_
#define _MANAGER_H_

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "score.h"
#include "game.h"
#include "gamepad.h"
#include "camera.h"
#include "light.h"
#include "sceneX.h"
#include "texture.h"
#include "menu.h"
#include "title.h"
#include "result.h"
#include "tutorial.h"
#include "pause.h"
#include <stdio.h>

//前方宣言
class CLight;
class CCamera;

//=============================================================================
// マネージャークラスの定義
//=============================================================================
class CManager
{
public:
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_MENU,
		MODE_SELECT,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_GAME2,
		MODE_RESULT,
		MODE_PAUSE,
		MODE_MAX
	}MODE;

	CManager();								//コンストラクタ
	~CManager();							//デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);		//初期化処理
	static void SetMode(MODE mode);									//終了処理
	void Uninit(void);												//更新処理
	void Update(void);												//描画処理
	void Draw(void);
	
	//静的メンバ宣言
	static CInputKeyboard	*GetInputKeyboard(void);
	static CTexture			*GetTexture(void);
	static CRenderer		*GetRenderer(void);
	static CSound			*GetSound(void);
	static CCamera			*GetCamera(void);

	//モード情報
	static MODE GetMode(void) { return m_mode; }

	//ゲームパッドの情報取得
	static CGamepad *GetGamepad(void) { return m_pGamepad; }

	//ゲームパッドの情報書き換え
	static CGamepad *m_pGamePad;

	//遷移カウント
	static int GetNumber();

	//プレイヤーHP関係
	static int GetPlayerScore(void);
	static void SetPlayerScore(int nScore);

	// ポーズ状態の取得
	static bool GetPause();

	// タイマー表示
	HRESULT InitTime(void);
	void UninitTime(void);
	void UpdateTime(void);
	void DrawTime(void);

private:
	//静的メンバ変数
	static CRenderer		*m_pRenderer;
	static CTexture			*m_pTexture;
	static CInputKeyboard	*m_pInputKeyboard;
	static CInput			*m_pInput;
	static CSound			*m_pSound;
	static CScene			*m_pScene;
	static CScore			*m_pScore;
	static CNumber			*m_pNumber;
	static CGamepad			*m_pGamepad;
	static CGame			*m_pGame;
	static CPause			*m_pPause;
	static CTitle			*m_pTitle;
	static CMenu			*m_pMenu;
	static CTutorial		*m_pTutorial;
	static CResult			*m_pResult;
	static CCamera			*m_pCamera;
	static CLight			*m_pLight;
	static CSceneX			*m_pSceneX;
	static MODE				m_mode;

	static bool m_bEndGame;						// 終了時の死亡フラグ

	static int m_nPalyerScore;					// スコア管理
};

#endif
