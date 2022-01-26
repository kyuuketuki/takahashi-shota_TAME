//=============================================================================
//
// ランキング処理 [ranking.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _RANKING_H_
#define _RANKING_H_

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "number.h"
#include "scene.h"

//=============================================================================
// ランキングクラスの定義
//=============================================================================
class CRanking : public CScene
{
public:
	CRanking();								//コンストラクタ
	~CRanking();								//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//初期化処理
	void Uninit(void);												//終了処理
	void Update(void);												//更新処理
	void Draw(void);												//描画処理

																	//値変更
	static void RandRanking(void);

	//加算処理
	static void SetRanking(void);
	//最大数表示
	static void MaxScoreRanking(void);

	void LoadData(void);
	void SaveData(void);

	//生成処理
	static CRanking *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);

	static int GetRanking(void);

	void Ranking(int nScore);

private:
	//ランキング表示変数
	int RankingCount;									//保存する数
	static CNumber *m_apRankingNumber[5][8];			//桁数
	static int m_nRankingScore[5][8];					//スコアの値
	static int m_nNumber[5][8];							//比べる数の値
	static int m_nRankScore[5];							//スコアの値

	//スコアの最大数表示用変数
	static CNumber *m_apMaxScoreNumber[8];			    //桁数
	static int m_nMaxScore[8];					        //スコアの値
	static int m_nMaxScoreNumber[8];					//最大スコア数並び替え時に使用
	static int m_nMaxRankScore;							//スコアの値
};

#endif