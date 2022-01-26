//=============================================================================
//
// 目的地(配達先)　処理 [destination.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "destination.h"
#include "sceneX.h"
#include "player3D.h"
#include "score.h"
#include "delivery.h"
#include "game.h"

//生成された回数を記憶
D3DXVECTOR3 g_posDestination[3];				//位置を記憶　呼び出し用
int			g_CntDestination = 0;
int			g_CntDestination2 = 0;
int			g_nNumberDestination[3];			//位置ナンバーを記憶
int			g_TimeDestination[3];				//制限時間
float		g_fRangeDestination[3];				//制限時間
bool		CDestination::m_bMotion = false;	//モーション変化用
bool		CDestination::m_bUseDestination;

//=============================================================================
// コンストラクタ
//=============================================================================
CDestination::CDestination(int nPriority, int nType)
{
	//初期化
	m_bUseDestination = false;

	///ランダムにアイテムを生成
	int nRand = rand() % 40;
	m_TimeLimit = 60 + nRand;

	//タイムリミット
	m_MaxTimeLimit = m_TimeLimit;

	m_nRandomNumber = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CDestination::~CDestination()
{
	//グローバル変数の初期化
	g_CntDestination = 0;
	g_CntDestination2 = 0;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CDestination::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	CSceneX::Init(pos, fSizeX, fSizeY);

	//カウントリセット
	//配達先を3に固定する
	if (g_CntDestination2 == 3)
	{
		g_CntDestination2 = 0;
	}

	//生成された数を記憶
	m_nRandomNumber = g_CntDestination2;
	g_CntDestination2++;

	m_posdes = pos;

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void CDestination::Uninit(void)
{
	CSceneX::Uninit();

	//値の初期化
	g_CntDestination2 = NULL;

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CDestination::Update(void)
{
	//タイムカウンターが60になった場合
	if (m_nTimeCounter >= 60)
	{
		m_nTime++;

		//制限時間の更新
		m_TimeLimit -= 1;

		//タイムカウンターを0にする
		m_nTimeCounter = 0;
	}

	//位置情報を受け取る プレイヤー
	D3DXVECTOR3 p = CPlayer3D::GetPosition();

	//位置情報を受け取る
	D3DXVECTOR3 m_pos = CSceneX::GetPosition();
	D3DXVECTOR3 pP = CPlayer3D::GetPosition();

	////位置をプレイヤーの上に設定
	//p = D3DXVECTOR3(p.x, 50.0f, p.z);

	//距離を求める
	float f = (float)sqrt((m_pos.x - p.x) * (m_pos.x - p.x) + (m_pos.z - p.z) * (m_pos.z - p.z));

	//タイムカウンターを進める
	m_nTimeCounter++;
	
	//受け渡すよう
	g_TimeDestination[m_nRandomNumber] = m_TimeLimit;
	g_fRangeDestination[m_nRandomNumber] = f;

	//半径の大きさ
	float nRadius = 400.0f;

	//当たり判定
	if (m_pos.x + nRadius >= pP.x && m_pos.x - nRadius <= pP.x&&
		m_pos.z + nRadius >= pP.z && m_pos.z - nRadius <= pP.z)
	{
		//パーティクル発生
		CGame::GameParticle(D3DXVECTOR3(m_pos.x, m_pos.y + 200.0f, m_pos.z), 0);

		//位置を変える処理を許可する
		m_bUseDestination = true;

		//モーション変化用のフラグを取得
		m_bMotion = true;

		//スコア加算
		CScore::AddScore(300);

		//荷物を届ける処理
		CDelivery::Create(m_pos);
	}

	//時間切れの場合
	if (m_TimeLimit <= 0)
	{
		//位置を変える処理を許可する
		m_bUseDestination = true;
	}

	//常に3つ目的地を置く
	if (m_bUseDestination == true)
	{
		//位置をRandom関数で指定した位置に移動させる
		CSceneX::SetPosition(CDestination::RandomPos());

		//位置情報を書き換える
		g_posDestination[m_nRandomNumber] = CSceneX::GetPosition();

		//新たに設定
		m_TimeLimit = 60;

		//位置を変える条件をオフにする
		m_bUseDestination = false;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CDestination::Draw(void)
{
	CSceneX::Draw();
}

//=============================================================================
// ランダムな位置わたす
//=============================================================================
D3DXVECTOR3 CDestination::RandomPos(void)
{
	//ランダムな番号を入れる
	int nRand = rand() % 9;

	//無限ループ
	while (1)
	{
		//生成した位置を同じなら　位置ナンバーで判断
		if (g_nNumberDestination[0] == nRand || g_nNumberDestination[1] == nRand || g_nNumberDestination[2] == nRand)
		{
			//もう一度値を入れかえる
			nRand = rand() % 9;
		}

		//それ以外
		else
		{
			break;
		}
	}

	D3DXVECTOR3 r;			//記憶用

	switch (nRand)
	{
	case 0:
		r = D3DXVECTOR3(-4244.0f, 0.0f, -292.0f);
		break;
	case 1:
		r = D3DXVECTOR3(-12187.0f, 0.0f, 14467.0f);
		break;
	case 2:
		r = D3DXVECTOR3(-3563.0f, 0.0f, 22919.0f);
		break;
	case 3:
		r = D3DXVECTOR3(9969.0f, 0.0f, 27973.0f);
		break;
	case 4:
		r = D3DXVECTOR3(10303.0f, 0.0f, -7403.0f);
		break;
	case 5:
		r = D3DXVECTOR3(10320.0f, 0.0f, -21817.0f);
		break;
	case 6:
		r = D3DXVECTOR3(-26730.0f, 0.0f, -20450.0f);
		break;
	case 7:
		r = D3DXVECTOR3(-19828.0f, 0.0f, -2622.0f);
		break;
	case 8:
		r = D3DXVECTOR3(14091.0f, 0.0f, -3941.0f);
		break;
	}

	//位置ナンバーを入れる
	g_nNumberDestination[m_nRandomNumber] = nRand;
	m_nNumber = nRand;

	return r;
}

//=============================================================================
// 生成処理
//=============================================================================
CDestination * CDestination::Create(D3DXVECTOR3 pos, char * TextName, int RandNumber)
{
	// ローカル変数宣言
	CDestination *pDestination = NULL;
	pDestination = new CDestination;

	//テキスト情報をわたす
	pDestination->BindTextName(TextName);

	//位置ナンバーを入れる
	g_nNumberDestination[g_CntDestination] = RandNumber;

	//Get用に情報を入れる
	g_posDestination[g_CntDestination] = pos;

	//生成された順に進める
	g_CntDestination++;

	// 初期化
	if (pDestination != NULL)
	{
		pDestination->Init(pos, 0, 0);
	}

	return pDestination;
}

//=============================================================================
// Get関数　位置
//=============================================================================
D3DXVECTOR3 CDestination::GetPosition(int nCnt)
{
	return g_posDestination[nCnt];
}

//=============================================================================
// Get関数　制限時間
//=============================================================================
int CDestination::GetLimit(int nCnt)
{
	return g_TimeDestination[nCnt];
}

int CDestination::GetRange(int nCnt)
{
	return (int)g_fRangeDestination[nCnt];
}
