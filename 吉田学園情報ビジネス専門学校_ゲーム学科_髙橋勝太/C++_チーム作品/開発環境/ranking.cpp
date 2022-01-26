//=============================================================================
//
// ランキング処理 [ranking.cpp]
// Author : Miura　Yudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "ranking.h"
#include "number.h"
#include "manager.h"
#include "logo.h"
#include "License.h"

//グローバル変数宣言
int CRanking::m_nRankingScore[5][8] = {};				//スコアへの静的メンバ変数
CNumber		*CRanking::m_apRankingNumber[5][8] = {};	//ナンバークラスへのポインタ
int CRanking::m_nNumber[5][8] = {};
int CRanking::m_nRankScore[5] = {};
CNumber		*CRanking::m_apMaxScoreNumber[8] = {};
int CRanking::m_nMaxScoreNumber[8] = {};
int CRanking::m_nMaxScore[8] = {};
int CRanking::m_nMaxRankScore;

//=============================================================================
// コンストラクタ
//=============================================================================
CRanking::CRanking()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRanking::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	for (int nCount = 0; nCount < 5; nCount++)
	{
		for (int nCnt = 0; nCnt < 8; nCnt++)
		{
			//番号を生成
			m_apRankingNumber[nCount][nCnt] = CNumber::Create(D3DXVECTOR3(pos.x - (nCnt * 70), pos.y + (nCount * 120.0f), 0.0f), 30, 40);
		}
	}

	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		//番号を生成
		m_apMaxScoreNumber[nCnt] = CNumber::Create(D3DXVECTOR3(1800.0f - (nCnt * 70), 350.0f, 0.0f), 30, 40);
	}

	//テキストから読み込み
	LoadData();

	//免許システムの処理
	if (CLicense::GetLisenceType() == CLicense::LISENCETYPE_GOLD)
	{
		//免許がゴールドの場合スコアが４倍になる
		m_nMaxRankScore = CScore::GetScore() * 4;
	}
	else if (CLicense::GetLisenceType() == CLicense::LISENCETYPE_NORMAL)
	{
		//免許が普通の場合スコアが２倍になる
		m_nMaxRankScore = CScore::GetScore() * 2;
	}

	//ランキング並び替え
	Ranking(m_nMaxRankScore);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	for (int nCount = 0; nCount < 5; nCount++)
	{
		for (int nCnt = 0; nCnt < 8; nCnt++)
		{
			// 頂点バッファの開放
			if (m_apRankingNumber[nCount][nCnt] != NULL)
			{
				m_apRankingNumber[nCount][nCnt]->Uninit();
				delete m_apRankingNumber[nCount][nCnt];
				m_apRankingNumber[nCount][nCnt] = NULL;
			}
		}
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CRanking::Update(void)
{
	//カウントアップ
	RankingCount++;

	//カウントが150になったら
	if (RankingCount <= 150)
	{
		for (int nCount = 0; nCount < 5; nCount++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				m_nRankingScore[nCount][nCnt] = rand() % 9 + 1;
			}
		}

		for (int nCnt = 0; nCnt < 8; nCnt++)
		{
			m_nMaxScore[nCnt] = rand() % 9 + 1;
		}

		//ランダム数セット
		RandRanking();
	}
	else
	{
		//ランキングセット
		SetRanking();
		MaxScoreRanking();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CRanking::Draw(void)
{
	//ランキング表示の描画
	for (int nCount = 0; nCount < 5; nCount++)
	{
		for (int nCnt = 0; nCnt < 8; nCnt++)
		{
			m_apRankingNumber[nCount][nCnt]->Draw();
		}
	}
	//計算したときのプレイヤーのスコア
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		m_apMaxScoreNumber[nCnt]->Draw();
	}
}

//=============================================================================
// スコアの生成
//=============================================================================
CRanking * CRanking::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ローカル変数宣言
	CRanking *pRanking = NULL;
	pRanking = new CRanking;

	// 初期化
	if (pRanking != NULL)
	{
		pRanking->Init(pos, fSizeX, fSizeY);
	}

	return pRanking;
}

//=============================================================================
// スコアを桁順に並び変える　スコアに値を入れる
//=============================================================================
void CRanking::RandRanking(void)
{
	//下のランキング５個用
	for (int nCount = 0; nCount < 5; nCount++)
	{
		//桁数を計算
		m_nNumber[nCount][0] = m_nRankingScore[nCount][7] % 10 / 1;
		m_nNumber[nCount][1] = m_nRankingScore[nCount][6] % 10 / 1;
		m_nNumber[nCount][2] = m_nRankingScore[nCount][5] % 10 / 1;
		m_nNumber[nCount][3] = m_nRankingScore[nCount][4] % 10 / 1;
		m_nNumber[nCount][4] = m_nRankingScore[nCount][3] % 10 / 1;
		m_nNumber[nCount][5] = m_nRankingScore[nCount][2] % 10 / 1;
		m_nNumber[nCount][6] = m_nRankingScore[nCount][1] % 10 / 1;
		m_nNumber[nCount][7] = m_nRankingScore[nCount][0] % 10 / 1;
	}
	//下のランキング５個用
	for (int nCount = 0; nCount < 5; nCount++)
	{
		//桁の合わせてずらす
		m_apRankingNumber[nCount][0]->SetNumber(m_nNumber[nCount][0]);
		m_apRankingNumber[nCount][1]->SetNumber(m_nNumber[nCount][1]);
		m_apRankingNumber[nCount][2]->SetNumber(m_nNumber[nCount][2]);
		m_apRankingNumber[nCount][3]->SetNumber(m_nNumber[nCount][3]);
		m_apRankingNumber[nCount][4]->SetNumber(m_nNumber[nCount][4]);
		m_apRankingNumber[nCount][5]->SetNumber(m_nNumber[nCount][5]);
		m_apRankingNumber[nCount][6]->SetNumber(m_nNumber[nCount][6]);
		m_apRankingNumber[nCount][7]->SetNumber(m_nNumber[nCount][7]);
	}

	//上の最大スコア用
	//桁数を計算
	m_nMaxScoreNumber[0] = m_nMaxScore[7] % 10 / 1;
	m_nMaxScoreNumber[1] = m_nMaxScore[6] % 10 / 1;
	m_nMaxScoreNumber[2] = m_nMaxScore[5] % 10 / 1;
	m_nMaxScoreNumber[3] = m_nMaxScore[4] % 10 / 1;
	m_nMaxScoreNumber[4] = m_nMaxScore[3] % 10 / 1;
	m_nMaxScoreNumber[5] = m_nMaxScore[2] % 10 / 1;
	m_nMaxScoreNumber[6] = m_nMaxScore[1] % 10 / 1;
	m_nMaxScoreNumber[7] = m_nMaxScore[0] % 10 / 1;

	//上の最大スコア用
	//桁の合わせてずらす
	m_apMaxScoreNumber[0]->SetNumber(m_nMaxScoreNumber[0]);
	m_apMaxScoreNumber[1]->SetNumber(m_nMaxScoreNumber[1]);
	m_apMaxScoreNumber[2]->SetNumber(m_nMaxScoreNumber[2]);
	m_apMaxScoreNumber[3]->SetNumber(m_nMaxScoreNumber[3]);
	m_apMaxScoreNumber[4]->SetNumber(m_nMaxScoreNumber[4]);
	m_apMaxScoreNumber[5]->SetNumber(m_nMaxScoreNumber[5]);
	m_apMaxScoreNumber[6]->SetNumber(m_nMaxScoreNumber[6]);
	m_apMaxScoreNumber[7]->SetNumber(m_nMaxScoreNumber[7]);
}
//=============================================================================
// スコアを加える
//=============================================================================
void CRanking::SetRanking(void)
{
	for (int nCount = 0; nCount < 5; nCount++)
	{
		//桁数を計算
		m_nNumber[nCount][0] = m_nRankScore[nCount] % 100000000 / 10000000;
		m_nNumber[nCount][1] = m_nRankScore[nCount] % 10000000 / 1000000;
		m_nNumber[nCount][2] = m_nRankScore[nCount] % 1000000 / 100000;
		m_nNumber[nCount][3] = m_nRankScore[nCount] % 100000 / 10000;
		m_nNumber[nCount][4] = m_nRankScore[nCount] % 10000 / 1000;
		m_nNumber[nCount][5] = m_nRankScore[nCount] % 1000 / 100;
		m_nNumber[nCount][6] = m_nRankScore[nCount] % 100 / 10;
		m_nNumber[nCount][7] = m_nRankScore[nCount] % 10 / 1;
	}

	for (int nCount = 0; nCount < 5; nCount++)
	{
		//桁の合わせてずらす
		m_apRankingNumber[nCount][0]->SetNumber(m_nNumber[nCount][7]);
		m_apRankingNumber[nCount][1]->SetNumber(m_nNumber[nCount][6]);
		m_apRankingNumber[nCount][2]->SetNumber(m_nNumber[nCount][5]);
		m_apRankingNumber[nCount][3]->SetNumber(m_nNumber[nCount][4]);
		m_apRankingNumber[nCount][4]->SetNumber(m_nNumber[nCount][3]);
		m_apRankingNumber[nCount][5]->SetNumber(m_nNumber[nCount][2]);
		m_apRankingNumber[nCount][6]->SetNumber(m_nNumber[nCount][1]);
		m_apRankingNumber[nCount][7]->SetNumber(m_nNumber[nCount][0]);
	}
}
//=============================================================================
// 最大スコアを表示
//=============================================================================
void CRanking::MaxScoreRanking(void)
{
	//桁数を計算
	m_nMaxScoreNumber[0] = m_nMaxRankScore % 100000000 / 10000000;
	m_nMaxScoreNumber[1] = m_nMaxRankScore % 10000000 / 1000000;
	m_nMaxScoreNumber[2] = m_nMaxRankScore % 1000000 / 100000;
	m_nMaxScoreNumber[3] = m_nMaxRankScore % 100000 / 10000;
	m_nMaxScoreNumber[4] = m_nMaxRankScore % 10000 / 1000;
	m_nMaxScoreNumber[5] = m_nMaxRankScore % 1000 / 100;
	m_nMaxScoreNumber[6] = m_nMaxRankScore % 100 / 10;
	m_nMaxScoreNumber[7] = m_nMaxRankScore % 10 / 1;

	//桁の合わせてずらす
	m_apMaxScoreNumber[0]->SetNumber(m_nMaxScoreNumber[7]);
	m_apMaxScoreNumber[1]->SetNumber(m_nMaxScoreNumber[6]);
	m_apMaxScoreNumber[2]->SetNumber(m_nMaxScoreNumber[5]);
	m_apMaxScoreNumber[3]->SetNumber(m_nMaxScoreNumber[4]);
	m_apMaxScoreNumber[4]->SetNumber(m_nMaxScoreNumber[3]);
	m_apMaxScoreNumber[5]->SetNumber(m_nMaxScoreNumber[2]);
	m_apMaxScoreNumber[6]->SetNumber(m_nMaxScoreNumber[1]);
	m_apMaxScoreNumber[7]->SetNumber(m_nMaxScoreNumber[0]);
}
//==============================================================================
// ファイルの読み込み
//==============================================================================
void CRanking::LoadData(void)
{
	// ローカル変数宣言
	FILE *pFile;

	// データの読み込み
	pFile = fopen("data/TEXT/ranking.txt", "r");
	if (pFile != NULL)
	{ //ファイル展開可能
		for (int nCount = 0; nCount < 5; nCount++)
		{
			fscanf(pFile, "%d", &m_nRankScore[nCount]);
		}
		fclose(pFile);
	}
	else
	{ // ファイル展開不可
	}
}
//==============================================================================
// ランキングの並び替え
//==============================================================================
void CRanking::Ranking(int nScore)
{
	// ローカル変数宣言
	int nSubData;

	// ランキング外の除外
	if (m_nRankScore[5 - 1] <= nScore)
	{ // スコアがランキング内の場合
		m_nRankScore[5 - 1] = nScore;
	}

	// 計算処理
	for (int nCnt = 0; nCnt < 5 - 1; nCnt++)
	{
		for (int nCnt2 = nCnt + 1; nCnt2 < 5; nCnt2++)
		{
			if (m_nRankScore[nCnt] < m_nRankScore[nCnt2])
			{
				nSubData = m_nRankScore[nCnt];
				m_nRankScore[nCnt] = m_nRankScore[nCnt2];
				m_nRankScore[nCnt2] = nSubData;
			}
		}
	}

	//ランキングを保存
	SaveData();
}
//==============================================================================
// データ保存処理
//==============================================================================
void CRanking::SaveData(void)
{
	// ローカル変数宣言
	FILE *pFile;

	// ファイルの展開
	pFile = fopen("data/TEXT/ranking.txt", "w");
	if (pFile != NULL)
	{ // ファイル展開可能
		for (int nCount = 0; nCount < 5; nCount++)
		{
			fprintf(pFile, "%d\n", m_nRankScore[nCount]);
		}
		fclose(pFile);
	}
	else
	{ // ファイル展開不可
	}
}
//=============================================================================
// Get関数
//=============================================================================
int CRanking::GetRanking(void)
{
	return S_OK;
}