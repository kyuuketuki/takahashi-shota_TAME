//=============================================================================
//
// 機体選択画面処理 [select.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "bg.h"
#include "input.h"
#include "manager.h"
#include "scene2D.h"
#include "logo.h"
#include "animation.h"
#include "polygon.h"
#include "select.h"
#include "fade.h"

//グローバル
int g_nPointerSelectY = 0;
int g_nOldPinterY = 0;
bool g_bUseKeyDown = false;
bool g_bButtonDownSelect = false;

//==============================================================================
// コンストラクタ
//==============================================================================
CSelect::CSelect()
{
	//初期化
	g_nPointerSelectY = 0;
	g_nOldPinterY = 0;
	g_bUseKeyDown = false;
	g_bButtonDownSelect = false;
}

//==============================================================================
// デストラクタ
//==============================================================================
CSelect::~CSelect()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CSelect::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//背景を表示
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	//フレームを生成
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), 130, 130, FRAME);

	//機体ロゴを生成
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 800.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), 100, 100, PLAYER04);			//3
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 400.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), 100, 100, PLAYER05);			//4
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), 100, 100, PLAYER01);						//0
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 400.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), 100, 100, PLAYER02);			//1
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 800.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), 100, 100, PLAYER03);			//2

	//ステータス表示
	CLogo::Create(D3DXVECTOR3(300.0f, 500.0f, 0.0f), 200, 100, SELECT_PLAYER01_STATUS);
	CLogo::Create(D3DXVECTOR3(1200.0f, 700.0f, 0.0f), 600, 300, SELECT_PLAYER01_STATUS2);

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CSelect::Uninit(void)
{
	// オブジェクト破棄
	Release();
}

//==============================================================================
// 更新処理
//==============================================================================
void CSelect::Update(void)
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

	//カーソル移動
	//D＆→を押した場合
	if (pInputKeyboard->GetTrigger(DIK_D) == true || pGamepad->IsButtonDown(CInput::RIGHT))
	{
		g_bUseKeyDown = true;

		if (g_nPointerSelectY == 0)
		{
			g_nPointerSelectY = 1;

			//ロゴの移動
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 + 400.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		else if (g_nPointerSelectY == 1)
		{
			g_nPointerSelectY = 2;

			//ロゴの移動
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 + 800.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		else if (g_nPointerSelectY == 2)
		{
			g_nPointerSelectY = 3;

			//ロゴの移動
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 - 800.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		else if (g_nPointerSelectY == 3)
		{
			g_nPointerSelectY = 4;

			//ロゴの移動
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 - 400.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		else if (g_nPointerSelectY == 4)
		{
			g_nPointerSelectY = 0;

			//ロゴの移動
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		//サウンドSE
		pSound->Play(CSound::SOUND_LABEL_SE_MIGRATION);
	}

	//カーソル移動
	//A＆←を押した場合
	if (pInputKeyboard->GetTrigger(DIK_A) == true || pGamepad->IsButtonDown(CInput::LEFT))
	{
		g_bUseKeyDown = true;

		if (g_nPointerSelectY == 0)
		{
			g_nPointerSelectY = 4;

			//ロゴの移動
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 - 400.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		else if (g_nPointerSelectY == 1)
		{
			g_nPointerSelectY = 0;
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		else if (g_nPointerSelectY == 2)
		{
			g_nPointerSelectY = 1;

			//ロゴの移動
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 + 400.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		else if (g_nPointerSelectY == 3)
		{
			g_nPointerSelectY = 2;

			//ロゴの移動
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 + 800.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		else if (g_nPointerSelectY == 4)
		{
			g_nPointerSelectY = 3;

			//ロゴの移動
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 - 800.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		//サウンドSE
		pSound->Play(CSound::SOUND_LABEL_SE_MIGRATION);
	}

	//ステータス設定
	if (g_bUseKeyDown == true)
	{
		//表示されているステータスを消す
		if (g_nOldPinterY == 0)
		{
			CLogo::TypeUninit(SELECT_PLAYER01_STATUS);
			CLogo::TypeUninit(SELECT_PLAYER01_STATUS2);
		}

		if (g_nOldPinterY == 1)
		{
			CLogo::TypeUninit(SELECT_PLAYER02_STATUS);
			CLogo::TypeUninit(SELECT_PLAYER02_STATUS2);
		}

		if (g_nOldPinterY == 2)
		{
			CLogo::TypeUninit(SELECT_PLAYER03_STATUS);
			CLogo::TypeUninit(SELECT_PLAYER04_STATUS);
		}

		if (g_nOldPinterY == 3)
		{
			CLogo::TypeUninit(SELECT_PLAYER03_STATUS2);
			CLogo::TypeUninit(SELECT_PLAYER04_STATUS2);
		}

		if (g_nOldPinterY == 4)
		{
			CLogo::TypeUninit(SELECT_PLAYER05_STATUS);
			CLogo::TypeUninit(SELECT_PLAYER05_STATUS2);
		}

		//前のポインターを記憶
		g_nOldPinterY = g_nPointerSelectY;
		
		//ステータスを表示
		if (g_nPointerSelectY == 0)
		{
			CLogo::Create(D3DXVECTOR3(300.0f, 500.0f, 0.0f), 200, 100, SELECT_PLAYER01_STATUS);
			CLogo::Create(D3DXVECTOR3(1200.0f, 700.0f, 0.0f), 600, 300, SELECT_PLAYER01_STATUS2);
		}

		if (g_nPointerSelectY == 1)
		{
			CLogo::Create(D3DXVECTOR3(300.0f, 500.0f, 0.0f), 200, 100, SELECT_PLAYER02_STATUS);
			CLogo::Create(D3DXVECTOR3(1200.0f, 700.0f, 0.0f), 600, 300, SELECT_PLAYER02_STATUS2);
		}

		if (g_nPointerSelectY == 2)
		{
			CLogo::Create(D3DXVECTOR3(300.0f, 500.0f, 0.0f), 200, 100, SELECT_PLAYER03_STATUS);
			CLogo::Create(D3DXVECTOR3(1200.0f, 700.0f, 0.0f), 600, 300, SELECT_PLAYER03_STATUS2);
		}

		if (g_nPointerSelectY == 3)
		{
			CLogo::Create(D3DXVECTOR3(300.0f, 500.0f, 0.0f), 200, 100, SELECT_PLAYER04_STATUS);
			CLogo::Create(D3DXVECTOR3(1200.0f, 700.0f, 0.0f), 600, 300, SELECT_PLAYER04_STATUS2);
		}

		if (g_nPointerSelectY == 4)
		{
			CLogo::Create(D3DXVECTOR3(300.0f, 500.0f, 0.0f), 200, 100, SELECT_PLAYER05_STATUS);
			CLogo::Create(D3DXVECTOR3(1200.0f, 700.0f, 0.0f), 600, 300, SELECT_PLAYER05_STATUS2);
		}

		g_bUseKeyDown = false;
	}

	//プレイヤーのタイプ
	CManager::SetPlayerType(g_nPointerSelectY);

	//初めて押したとき
	if (g_bButtonDownSelect == false)
	{
		//画面遷移
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) || pGamepad->IsButtonDown(CInput::B) == true)
		{
			//モード設定
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_GAME, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// シーン遷移

			//連打防止
			g_bButtonDownSelect = true;

			//サウンドSE
			pSound->Play(CSound::SOUND_LABEL_SE_DECISION);
		}
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void CSelect::Draw(void)
{

}

//==============================================================================
// 生成処理
//==============================================================================
CSelect * CSelect::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// 機体選択画面の生成
	CSelect *pSelect;
	pSelect = new CSelect;

	// NULLチェック
	if (pSelect != NULL)
	{
		pSelect->Init(pos, fSizeX, fSizeY);
	}

	return pSelect;
}
