//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "pause.h"
#include "logo.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "animation.h"
#include "fade.h"

int nTimeGamePause = 0;				// 時間
int nTimeCounterPause = 0;			// 時間カウンター
int g_nPointerPauseX = 0;			// ポインター位置
bool g_bButtonDownPause = false;	// 連打防止

//==============================================================================
// マクロ定義
//==============================================================================
#define MENU_PAUSE_MAX (3)				// 選択できるモードの数
#define SCREEN_TRANSITION_SPEED (30)	// 画面遷移の速度

#define LOGO_RESTART_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, 350.0f, 0.0f))		// リスタートのロゴの位置
#define LOGO_MENU_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f))			// メニューののロゴの位置
#define LOGO_EXIT_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, 850.0f, 0.0f))			// 終了のロゴの位置	

//=============================================================================
// コンストラクタ
//=============================================================================
CPause::CPause(int nPriority, int nType) : CScene(nPriority, nType)
{
	// 初期化
	m_nFadeCntPause = 0;
	g_nPointerPauseX = 0;
	g_bButtonDownPause = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPause::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 背景
	CLogoPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, PAUSE01);

	// フレーム
	CLogoPause::Create(D3DXVECTOR3(950.0f, 350.0f, 0.0f), 250, 60, FRAME);

	// ロゴ
	CLogoPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 350.0f, 0.0f), 170, 40, PAUSE_RESTART);	// リスタート
	CLogoPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), 240, 40, PAUSE_MENU);		// メニュー
	CLogoPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 850.0f, 0.0f), 240, 40, PAUSE_EXIT);		// 終了(メニューに戻る)

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPause::Uninit(void)
{
	// オブジェクト破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPause::Update(void)
{
	// キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// ゲームパッドのポインタ宣言
	CGamepad *pGamepad;
	pGamepad = CManager::GetGamepad();

	// サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	// 決定を押してない場合
	if (g_bButtonDownPause == false)
	{
		// S ＆↓を押した場合
		if (pInputKeyboard->GetTrigger(DIK_S) == true || pGamepad->IsButtonDown(CInput::DOWN))
		{
			// タイプを変える
			g_nPointerPauseX++;

			// タイプが最大値より上になった時
			if (g_nPointerPauseX > MENU_PAUSE_MAX - 1)
			{
				// タイプを最小値にする
				g_nPointerPauseX = 0;
			}
		}

		// W＆↑を押した場合
		if (pInputKeyboard->GetTrigger(DIK_W) == true || pGamepad->IsButtonDown(CInput::UP))
		{
			// タイプを変える
			g_nPointerPauseX--;

			// タイプが最小値より下になった時
			if (g_nPointerPauseX < 0)
			{
				// 最大値にする
				g_nPointerPauseX = MENU_PAUSE_MAX - 1;
			}
		}

		// フレームが移動する場所設定 モードの設定
		if (g_nPointerPauseX == 0)
		{								
			m_mode = CManager::MODE_GAME;					// リスタート
			CLogo::TypeMove(LOGO_RESTART_POS, FRAME);		// 位置を変える
		}
		if (g_nPointerPauseX == 1)
		{
			m_mode = CManager::MODE_MENU;					// メニュー画面
			CLogo::TypeMove(LOGO_MENU_POS, FRAME);		// 位置を変える
		}
		if (g_nPointerPauseX == 2)
		{
			m_mode = CManager::MODE_TITLE;					// タイトル画面
			CLogo::TypeMove(LOGO_EXIT_POS, FRAME);			// 位置を変える
		}

		// 画面遷移	決定ボタンを押した場合
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) || pGamepad->IsButtonDown(CInput::B) == true)
		{
			// 連打防止
			g_bButtonDownPause = true;
		}
	}

	// 決定された場合
	else if (g_bButtonDownPause == true)
	{
		// フェードカウント加算
		m_nFadeCntPause++;

		// フェードまでの時間
		if ((m_nFadeCntPause % SCREEN_TRANSITION_SPEED) == 0)
		{
			// モード設定
			CFade::SetFade(CFade::FADE_OUT, (CManager::MODE)m_mode);		// シーン遷移
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPause::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CPause * CPause::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ポインタ変数の生成
	CPause *pPause;
	pPause = new CPause;

	// NULLチェック
	if (pPause != NULL)
	{
		pPause->Init(pos, fSizeX, fSizeY);
	}

	return pPause;
}