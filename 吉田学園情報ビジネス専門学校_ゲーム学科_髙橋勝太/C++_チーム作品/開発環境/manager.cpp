//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "sound.h"
#include "score.h"
#include "number.h"
#include "gamepad.h"
#include "fade.h"
#include "texture.h"
#include "menu.h"
#include "title.h"
#include "animation.h"
#include "logo.h"
#include "city.h"
#include "map.h"
#include "result.h"
#include "License.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CRenderer		*CManager::m_pRenderer = NULL;
CInputKeyboard	*CManager::m_pInputKeyboard = NULL;
CInput			*CManager::m_pInput = NULL;
CSound			*CManager::m_pSound = NULL;
CScene			*CManager::m_pScene = NULL;
CScore			*CManager::m_pScore = NULL;
CNumber			*CManager::m_pNumber = NULL;
CGamepad		*CManager::m_pGamepad = NULL;
CGame			*CManager::m_pGame = NULL;
CTitle			*CManager::m_pTitle = NULL;
CMenu			*CManager::m_pMenu = NULL;
CTutorial		*CManager::m_pTutorial = NULL;
CResult			*CManager::m_pResult = NULL;
CCamera			*CManager::m_pCamera = NULL;
CLight			*CManager::m_pLight = NULL;
CSceneX			*CManager::m_pSceneX = NULL;
CTexture		*CManager::m_pTexture = NULL;

//初期の画面
CManager::MODE	CManager::m_mode = MODE_TITLE;

//スコアデータを記憶保存
int CManager::m_nPalyerScore;

//グローバル変数宣言
int		nManagerTime;
int     nManagercount;
bool	g_bManager = false;
bool	CManager::m_bEndGame = false;		//経過時間を表示させる
bool	g_bPause = false;					//ポーズ状態

//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//レンダラー作成
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;
		m_pRenderer->Init(hWnd, bWindow);
	}

	//キーボード生成
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);

	//ゲームパッド生成
	m_pGamepad = new CGamepad;
	m_pGamepad->Init(hInstance, hWnd);

	//サウンド生成
	m_pSound = new CSound;
	m_pSound->Init(hWnd);

	//カメラ生成
	m_pCamera = new CCamera;
	m_pCamera->Init();

	//ライト生成
	m_pLight = new CLight;
	m_pLight->Init();

	//番号のテクスチャ読み込み
	CNumber::Load();

	//テクスチャクラスを生成
	m_pTexture = new CTexture;
	m_pTexture->Load();

	// アニメーション画像ロード
	CAnimasion::Load();

	//免許所
	CLicense::Load();

	//ロゴのテクスチャの読み込み
	CLogo::Load();

	//モード設定
	SetMode(m_mode);

	return S_OK;
}

//==============================================================================
// モード選択
//==============================================================================
void CManager::SetMode(MODE mode)
{
	//終了処理
	CScene::ReleaseAll();

	//フェードをクリエイト
	CFade::Create(m_mode);

	//画面遷移時に初期化
	g_bPause = false;

	switch (mode)
	{
	case MODE_TITLE:

		//タイトル画面の初期化処理
		m_pTitle->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

		break;

	case MODE_TUTORIAL:

		// チュートリアル画面の初期化処理
		m_pTutorial->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		break;

	case MODE_MENU:

		//タイトル画面の初期化処理
		m_pMenu->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		break;

	case MODE_GAME:

		//ゲーム画面の初期化処理
		m_pGame->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		m_pPause->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		break;

	case MODE_RESULT:

		//ゲーム画面の初期化処理
		m_pResult->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		break;
	}

	m_mode = mode;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	//オブジェクトクラスの破棄
	CScene::ReleaseAll();

	// テクスチャの開放
	CNumber::Unload();
	CLogo::Unload();
	CAnimasion::Unload();
	CLicense::Unload();

	//テクスチャクラスの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = NULL;
	}

	//入力処理の破棄
	if (m_pInput != NULL)
	{
		m_pInput->Uninit();
		delete m_pInput;
		m_pInput = NULL;
	}

	//サウンドの破棄
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	//レンダリングクラスの破棄
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	//ゲームパッドのポインタ宣言
	CGamepad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//カメラの更新
	m_pCamera->Update();

	//キーボードの更新処理
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	//ゲームパッドの更新処理
	if (m_pGamepad != NULL)
	{
		m_pGamepad->Update();
	}

	//レンダラーの更新
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	//ゲーム画面時のみポーズ
	if (CManager::m_mode == MODE_GAME)
	{
		//ポーズの切り替えオン
		if (g_bPause == false)
		{
			if (m_pInputKeyboard->GetTrigger(DIK_P) == true || pGamepad->IsButtonDown(CInput::P))
			{
				g_bPause = true;
			}
		}

		//ポーズの切り替えオフ
		else if (g_bPause == true)
		{
			if (m_pInputKeyboard->GetTrigger(DIK_P) == true || pGamepad->IsButtonDown(CInput::P))
			{
				g_bPause = false;
			}
		}
	}

	//デバック用
#ifdef _DEBUG
	//自動ステージスキップ
	if (m_pInputKeyboard->GetTrigger(DIK_9) == true)
	{
		if (g_bManager == false)
		{
			g_bManager = true;
		}

		else if (g_bManager == true)
		{
			g_bManager = false;
		}
	}

	if (g_bManager == true)
	{
		nManagerTime++;

		if (nManagerTime >= 60)
		{
			nManagerTime = 0;
			nManagercount++;
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_GAME);	// シーン遷移
		}
	}

	//ステージスキップ
	if (m_pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		CFade::SetFade(CFade::FADE_OUT, CManager::MODE_GAME);	// シーン遷移
	}

	if (m_pInputKeyboard->GetTrigger(DIK_0) == true)
	{
		CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);	// シーン遷移
	}

	if (m_pInputKeyboard->GetTrigger(DIK_2) == true)
	{
		CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);	// シーン遷移
	}
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
// キーボードのGet関数
//=============================================================================
CInputKeyboard * CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
// レンダラーのGet関数
//=============================================================================
CRenderer * CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
// サウンドのGet関数
//=============================================================================
CSound * CManager::GetSound(void)
{
	return m_pSound;
}

//=============================================================================
// カメラのGet関数
//=============================================================================
CCamera * CManager::GetCamera(void)
{
	return m_pCamera;
}

//=============================================================================
// テクスチャのGet関数
//=============================================================================
CTexture * CManager::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
// 遷移カウント
//=============================================================================
int CManager::GetNumber()
{
	return nManagercount;
}

//=============================================================================
// Getプレイヤースコア
//=============================================================================
int CManager::GetPlayerScore(void)
{
	return m_nPalyerScore;
}

//=============================================================================
// Setプレイヤースコア
//=============================================================================
void CManager::SetPlayerScore(int nScore)
{
	m_nPalyerScore = nScore;
}

//=============================================================================
// ポーズの取得
//=============================================================================
bool CManager::GetPause()
{
	return g_bPause;
}
