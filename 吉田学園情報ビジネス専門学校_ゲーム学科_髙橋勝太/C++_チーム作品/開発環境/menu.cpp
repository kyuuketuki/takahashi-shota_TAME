//=============================================================================
//
// メニュー処理 [menu.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "menu.h"
#include "input.h"
#include "manager.h"
#include "scene2D.h"
#include "logo.h"
#include "animation.h"
#include "fade.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MENU_MODE_MAX (3)				// 選択できるモードの数
#define SCREEN_TRANSITION_SPEED (30)	// 画面遷移の速度
#define LOGO_CENTER (770)				// 画面の中心（X軸）

#define LOGO_GAMESTART_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, LOGO_CENTER - 170.0f, 0.0f))	// ゲームスタートのロゴの位置
#define LOGO_TUTORIAL_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, LOGO_CENTER, 0.0f))			// チュウトリアルののロゴの位置
#define LOGO_END_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, LOGO_CENTER + 170.0f, 0.0f))		// クイットのロゴの位置	

//==============================================================================
// コンストラクタ
//==============================================================================
CMenu::CMenu()
{
	m_nFadeCnt = 0;
	m_nPointerMenuX = 0;
	m_bButtonDownMenu = false;
}

//==============================================================================
// コンストラクタ
//==============================================================================
CMenu::~CMenu()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
HRESULT CMenu::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// 動く背景
	CAnimasion::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 3, 1);

	// パトランプ
	CAnimasion::Create(D3DXVECTOR3(810.0f, 50.0f, 0.0f), 100, 80, 5, 0);

	// タイトルロゴ
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), 800, 220, TITLE_LOGO);

	//背景フレーム
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 770, 0.0f), 340, 260, MENU_FLAME);

	CLogo::Create(LOGO_GAMESTART_POS, 210, 40, TITLE_UI_GAMESTART);	// ゲームスタート

	CLogo::Create(LOGO_TUTORIAL_POS, 230, 40, TITLE_UI_TUTORIAL);	// チュートリアル

	CLogo::Create(LOGO_END_POS, 230, 40, TITLE_UI_END);				// 終了

	CLogo::Create(LOGO_GAMESTART_POS, 280, 70, FRAME);				// 移動フレーム

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CMenu::Uninit(void)
{
	// オブジェクト破棄
	Release();
}

//==============================================================================
// 更新処理
//==============================================================================
void CMenu::Update(void)
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

	// 決定を押してない場合
	if (m_bButtonDownMenu == false)
	{
		//S＆↓を押した場合
		if (pInputKeyboard->GetTrigger(DIK_S) == true || pGamepad->IsButtonDown(CInput::DOWN))
		{
			// タイプを変える
			m_nPointerMenuX++;

			// タイプが最大値より上になった時
			if (m_nPointerMenuX > MENU_MODE_MAX - 1)
			{
				// タイプを最小値にする
				m_nPointerMenuX = 0;
			}

			//サウンドSE
			pSound->Play(CSound::SOUND_LABEL_SE_MIGRATION);
		}

		//W＆↑を押した場合
		if (pInputKeyboard->GetTrigger(DIK_W) == true || pGamepad->IsButtonDown(CInput::UP))
		{
			// タイプを変える
			m_nPointerMenuX--;

			// タイプが最小値より下になった時
			if (m_nPointerMenuX < 0)
			{
				// 最大値にする
				m_nPointerMenuX = MENU_MODE_MAX - 1;
			}

			//サウンドSE
			pSound->Play(CSound::SOUND_LABEL_SE_MIGRATION);
		}

		// フレームが移動する場所設定 モードの設定
		if (m_nPointerMenuX == 0)
		{
			m_mode = CManager::MODE_GAME;
			CLogo::TypeMove(LOGO_GAMESTART_POS, FRAME);		//位置を変える
		}

		if (m_nPointerMenuX == 1)
		{
			m_mode = CManager::MODE_TUTORIAL;
			CLogo::TypeMove(LOGO_TUTORIAL_POS, FRAME);		//位置を変える
		}

		if (m_nPointerMenuX == 2)
		{
			m_mode = CManager::MODE_TITLE;
			CLogo::TypeMove(LOGO_END_POS, FRAME);			//位置を変える
		}

		//画面遷移	決定ボタンを押した場合
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) || pGamepad->IsButtonDown(CInput::B) == true)
		{
			//連打防止
			m_bButtonDownMenu = true;

			//サウンドSE
			pSound->Play(CSound::SOUND_LABEL_SE_DECISION);
		}
	}

	// 決定された場合
	else if (m_bButtonDownMenu == true)
	{
		// フェードカウント加算
		m_nFadeCnt++;

		//フェードまでの時間
		if ((m_nFadeCnt % SCREEN_TRANSITION_SPEED) == 0)
		{
			//モード設定
			CFade::SetFade(CFade::FADE_OUT, (CManager::MODE)m_mode);		// シーン遷移
		}
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void CMenu::Draw(void)
{

}

//==============================================================================
// 生成処理
//==============================================================================
CMenu * CMenu::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ポインタ変数の生成
	CMenu *pMenu;
	pMenu = new CMenu;

	// NULLチェック
	if (pMenu != NULL)
	{
		pMenu->Init(pos, fSizeX, fSizeY);
	}

	return pMenu;
}