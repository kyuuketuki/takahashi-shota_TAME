//=============================================================================
//
// スコア処理 [score.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "score.h"
#include"number.h"
#include "manager.h"

//グローバル変数宣言
int CScore::m_nScore = NULL;				//スコアへの静的メンバ変数
CNumber		*CScore::m_apNumber[8] = {};	//ナンバークラスへのポインタ

//=============================================================================
// コンストラクタ
//=============================================================================
CScore::CScore()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		//番号を生成
		m_apNumber[nCnt] = CNumber::Create(D3DXVECTOR3(pos.x - (nCnt * 70), pos.y, 0.0f), fSizeX, fSizeY);
	}
	
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScore::Uninit(void)
{
	CManager::SetPlayerScore(m_nScore);

	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		// 頂点バッファの開放
		if (m_apNumber[nCnt] != NULL)
		{
			m_apNumber[nCnt]->Uninit();
			delete m_apNumber[nCnt];
			m_apNumber[nCnt] = NULL;
		}
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScore::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CScore::Draw(void)
{
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		m_apNumber[nCnt]->Draw();
	}
}

//=============================================================================
// スコアの生成
//=============================================================================
CScore * CScore::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// スコアの生成
	CScore *pScore = NULL;
	pScore = new CScore;

	// 初期化
	if (pScore != NULL)
	{
		pScore->Init(pos, fSizeX, fSizeY);
	}

	return pScore;
}

//=============================================================================
// スコアを桁順に並び変える　スコアに値を入れる
//=============================================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;

	int nNumber[8];

	//桁数を計算
	nNumber[0] = m_nScore % 100000000 / 10000000;
	nNumber[1] = m_nScore % 10000000 / 1000000;
	nNumber[2] = m_nScore % 1000000 / 100000;
	nNumber[3] = m_nScore % 100000 / 10000;
	nNumber[4] = m_nScore % 10000 / 1000;
	nNumber[5] = m_nScore % 1000 / 100;
	nNumber[6] = m_nScore % 100 / 10;
	nNumber[7] = m_nScore % 10 / 1;

	//桁の合わせてずらす
	m_apNumber[0]->SetNumber(nNumber[7]);
	m_apNumber[1]->SetNumber(nNumber[6]);
	m_apNumber[2]->SetNumber(nNumber[5]);
	m_apNumber[3]->SetNumber(nNumber[4]);
	m_apNumber[4]->SetNumber(nNumber[3]);
	m_apNumber[5]->SetNumber(nNumber[2]);
	m_apNumber[6]->SetNumber(nNumber[1]);
	m_apNumber[7]->SetNumber(nNumber[0]);
}

//=============================================================================
// スコアを加える
//=============================================================================
void CScore::AddScore(int nValue)
{
	m_nScore += nValue;

	int nNumber[8];

	//桁数を計算
	nNumber[0] =m_nScore % 100000000 / 10000000;
	nNumber[1] =m_nScore % 10000000 / 1000000;
	nNumber[2] =m_nScore % 1000000 / 100000;
	nNumber[3] =m_nScore % 100000 / 10000;
	nNumber[4] =m_nScore % 10000 / 1000;
	nNumber[5] =m_nScore % 1000 / 100;
	nNumber[6] =m_nScore % 100 / 10;
	nNumber[7] =m_nScore % 10 / 1;

	//桁の合わせてずらす
	m_apNumber[0]->SetNumber(nNumber[7]);
	m_apNumber[1]->SetNumber(nNumber[6]);
	m_apNumber[2]->SetNumber(nNumber[5]);
	m_apNumber[3]->SetNumber(nNumber[4]);
	m_apNumber[4]->SetNumber(nNumber[3]);
	m_apNumber[5]->SetNumber(nNumber[2]);
	m_apNumber[6]->SetNumber(nNumber[1]);
	m_apNumber[7]->SetNumber(nNumber[0]);
}

//=============================================================================
// Get関数
//=============================================================================
int CScore::GetScore(void)
{
	return m_nScore;
}