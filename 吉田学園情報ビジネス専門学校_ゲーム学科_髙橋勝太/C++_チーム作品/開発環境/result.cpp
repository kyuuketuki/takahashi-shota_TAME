//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "score.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "animation.h"
#include "result.h"
#include "logo.h"
#include "fade.h"
#include "ranking.h"
#include "number.h"
#include "License.h"

//静的メンバ変数宣言
CScore *CResult::m_pScore[5];

int nTimeGameResult = 0;			//時間
int nTimeCounterResult = 0;			//時間カウンター
int g_nPointerResultX = 0;			//現在の位置を記憶
bool g_bButtonDownResult = false;	//連打防止用

//=============================================================================
// コンストラクタ
//=============================================================================
CResult::CResult()
{
	//初期化
	g_nPointerResultX = 0;
	g_bButtonDownResult = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//背景ロゴ
	CLogo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, RANKING_BG);

	//説明フレーム
	CLogo::Create(D3DXVECTOR3(1550.0f, 150.0f, 0.0f), 270, 100, RANKING_FREAM);

	//ランキングロゴ
	CLogo::Create(D3DXVECTOR3(950.0f, 100.0f, 0.0f), 300, 60, RANKING_LOGO);

	//プレイヤースコアロゴ
	CLogo::Create(D3DXVECTOR3(350.0f, 200.0f, 0.0f), 250, 40, MAX_SCORE);

	// 順位ロゴ
	CLogo::Create(D3DXVECTOR3(500.0f, 500.0f, 0.0f), 90, 70, RANKING_FIRST);
	CLogo::Create(D3DXVECTOR3(500.0f, 620.0f, 0.0f), 90, 70, RANKING_SECOND);
	CLogo::Create(D3DXVECTOR3(500.0f, 740.0f, 0.0f), 90, 70, RANKING_THIRD);
	CLogo::Create(D3DXVECTOR3(500.0f, 860.0f, 0.0f), 70, 50, RANKING_FOUR);
	CLogo::Create(D3DXVECTOR3(500.0f, 980.0f, 0.0f), 70, 50, RANKING_FIVE);

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		//数字のフレーム
		CLogo::Create(D3DXVECTOR3(1100.0f, 500.0f + (nCnt * 120.0f), 0.0f), 350, 70, RESULT_SCORE);
	}

	//×ロゴ
	CLogo::Create(D3DXVECTOR3(700.0f, 350.0f, 0.0f), 50, 50, RANKING_ADD);

	//=ロゴ
	CLogo::Create(D3DXVECTOR3(1150.0f, 350.0f, 0.0f), 60, 50, RANKING_EQUAL);

	//説明ロゴ
	CLogo::Create(D3DXVECTOR3(1550.0f, 150.0f, 0.0f), 250, 100, RANKING_TUTORIAL);

	//プレイヤースコア
	CScore::Create(D3DXVECTOR3(600.0f, 350.0f, 0.0f), 30.0f, 40.0f);
	//ゲーム時のスコアを呼び出す
	CScore::SetScore(CScore::GetScore());

	//ランキングデータの管理　読み込み＆並び替え
	CRanking::Create(D3DXVECTOR3(1350.0f, 500.0f, 0.0f), 100.0f, 100.0f);

	//免許システム
	CLicense::Create(D3DXVECTOR3(900.0f, 350.0f, 0.0f), 100, 70);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	// オブジェクト破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
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
	if (g_bButtonDownResult == false)
	{
		//画面遷移	決定ボタンを押した場合
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) || pGamepad->IsButtonDown(CInput::B) == true)
		{
			//連打防止
			g_bButtonDownResult = true;

			//モード設定
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_MENU);		// シーン遷移
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CResult * CResult::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// リザルト画面の生成
	CResult *pResult;
	pResult = new CResult;

	// NULLチェック
	if (pResult != NULL)
	{
		pResult->Init(pos, fSizeX, fSizeY);
	}

	return pResult;
}
