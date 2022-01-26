//=============================================================================
//
// チュートリアル画面処理 [tutorial.cpp]
// Author : Taiki hayasaka
//
//=============================================================================
#include "tutorial.h"
#include "input.h"
#include "manager.h"
#include "scene2D.h"
#include "logo.h"
#include "animation.h"
#include "gamepad.h"
#include "fade.h"

//グローバル変数宣言
int		g_nCountTutrial = 0;				//ボタンの押した数
bool	g_bUITutorial = false;				//UI出現
bool	g_bButtonDownTutorial = false;		//連打防止用

//==============================================================================
// コンストラクタ
//==============================================================================
CTutorial::CTutorial()
{
	//初期化
	g_nCountTutrial = 0;
	g_bUITutorial = false;
	g_bButtonDownTutorial = false;
}

//==============================================================================
// デストラクタ
//==============================================================================
CTutorial::~CTutorial()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CTutorial::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//ルール説明
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, RULE);

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CTutorial::Uninit(void)
{
	// オブジェクト破棄
	Release();
}

//==============================================================================
// 更新処理
//==============================================================================
void CTutorial::Update(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//ゲームパッドのポインタ宣言
	CGamepad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//連打防止
	if (g_bButtonDownTutorial == false)
	{
		//画面遷移
		if (pGamepad->IsButtonDown(CInput::A) == true || pGamepad->IsButtonDown(CInput::B) == true || pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{
			//ボタンを押した数
			g_nCountTutrial++;

			if (g_nCountTutrial == 1 && g_bUITutorial == false)
			{
				//操作説明
				CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, CONTROLS_BG);
				g_bUITutorial = true;
			}

			if (g_nCountTutrial == 2)
			{
				//ボタン操作をロックする
				g_bButtonDownTutorial = true;

				// メニュー画面に遷移
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_MENU);	// シーン遷移
			}
		}
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void CTutorial::Draw(void)
{

}

//==============================================================================
// 生成処理
//==============================================================================
CTutorial * CTutorial::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ポインタ変数の生成
	CTutorial *pTutorial;
	pTutorial = new CTutorial;

	// NULLチェック
	if (pTutorial != NULL)
	{
		pTutorial->Init(pos, fSizeX, fSizeY);
	}

	return pTutorial;
}
