//=============================================================================
//
// 頂点当たり判定処理処理 [vertex.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "vertex.h"
#include "manager.h"
#include "scene3D.h"
#include "visualization.h"

//グローバル変数宣言
D3DXVECTOR3		g_posObjectVector[1024][4];				//テキストデータの位置情報
int				g_nCntVertex[1024];						//生成番号記憶
int				g_nDisplayVertex = 0;					//生成番号を表示用
int				g_nDisplayVertex2 = 0;					//ベクトル番号の表示用
int				g_FailCntVecter;						//生成カウント
int				g_nMaxFail;								//テキストで読み込んだ回数
float			g_fAtan2Vecter;							//角度の表示用
bool			g_bObjectVector[1024];					//テキストデータの可視化オンオフ状態

//=============================================================================
// コンストラクタ
//=============================================================================
CVertex::CVertex(int nPriority, int nType)
{
	//変数の初期化
	g_nCntVertex[256] = {};
	g_nDisplayVertex = 0;
	g_nDisplayVertex2 = 0;
	g_fAtan2Vecter = 0;
	g_nMaxFail = 0;
	g_FailCntVecter = 0;

	for (int nCount = 0; nCount < 256; nCount++)
	{
		g_bObjectVector[nCount] = false;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CVertex::~CVertex()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CVertex::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CVertex::Uninit(void)
{
	//カウントリセット
	g_FailCntVecter = 0;

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CVertex::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CVertex::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CVertex * CVertex::Create(D3DXVECTOR3 pos, D3DXVECTOR3 fMaxSizeX, D3DXVECTOR3 fMinSizeX, D3DXVECTOR3 fMaxSizeY, D3DXVECTOR3 fMinSizeY)
{
	//モデルを生成
	CVertex *pVertex = NULL;
	pVertex = new CVertex;

	D3DXVECTOR3 pos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点当たり判定の生成
	pVertex->Init(pos2, 0.0f, 0.0f);
	pVertex->CVertex::LoadObject();

	return pVertex;
}

//=============================================================================
// 変数を減らす
//=============================================================================
void CVertex::Release(void)
{
	//生成番号を減らす
	g_FailCntVecter -= 2;
}

//==============================================================================
// 外積を利用した当たり判定
//==============================================================================
bool CVertex::CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	// カメラ情報の取得
	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 rot = pCamera->GetRot();

	bool bLand = false;

	//計算用
	D3DXVECTOR3 posold;
	D3DXVECTOR3 g_Vec[MAX_OBJECT_VEC][4];
	D3DXVECTOR3 g_oldVec[MAX_OBJECT_VEC][4];
	float PlayerVec[MAX_OBJECT_VEC][4];
	float oldPlayerVec[MAX_OBJECT_VEC][4];

	D3DXVECTOR3 pos = *pPos;			// 判定対象の位置
	posold = *pPosOld;					// 判定対象の位置

	//読み込んだ座標をもとに当たり判定を作る
	for (int nCnt = 0; nCnt < g_FailCntVecter + 1; nCnt++)
	{
		m_aPos[nCnt][0] = g_posObjectVector[nCnt][0];
		m_aPos[nCnt][1] = g_posObjectVector[nCnt][1];
		m_aPos[nCnt][2] = g_posObjectVector[nCnt][2];
		m_aPos[nCnt][3] = g_posObjectVector[nCnt][3];

		//並び方をただす
		//90ずれている場合
		if (m_aPos[nCnt][0].x > m_aPos[nCnt][2].x && m_aPos[nCnt][0].z < m_aPos[nCnt][2].z)
		{
			D3DXVECTOR3 vecold;
			vecold = m_aPos[nCnt][0];

			m_aPos[nCnt][0] = m_aPos[nCnt][3];
			m_aPos[nCnt][3] = m_aPos[nCnt][2];
			m_aPos[nCnt][2] = m_aPos[nCnt][1];
			m_aPos[nCnt][1] = vecold;
		}
		//180ずれている場合
		else if (m_aPos[nCnt][0].x < m_aPos[nCnt][2].x && m_aPos[nCnt][0].z < m_aPos[nCnt][2].z)
		{
			D3DXVECTOR3 vecold;
			vecold = m_aPos[nCnt][0];

			D3DXVECTOR3 vecold2;
			vecold2 = m_aPos[nCnt][1];

			m_aPos[nCnt][0] = m_aPos[nCnt][2];
			m_aPos[nCnt][2] = vecold;
			m_aPos[nCnt][1] = m_aPos[nCnt][3];
			m_aPos[nCnt][3] = vecold2;
		}
		//270ずれている場合
		else if (m_aPos[nCnt][0].x < m_aPos[nCnt][2].x && m_aPos[nCnt][0].z > m_aPos[nCnt][2].z)
		{
			D3DXVECTOR3 vecold;
			vecold = m_aPos[nCnt][0];

			m_aPos[nCnt][0] = m_aPos[nCnt][1];
			m_aPos[nCnt][1] = m_aPos[nCnt][2];
			m_aPos[nCnt][2] = m_aPos[nCnt][3];
			m_aPos[nCnt][3] = vecold;
		}

		//外積の式　ベクトル
		m_Vec[nCnt][0] = m_aPos[nCnt][1] - m_aPos[nCnt][0];
		m_Vec[nCnt][1] = m_aPos[nCnt][2] - m_aPos[nCnt][1];
		m_Vec[nCnt][2] = m_aPos[nCnt][3] - m_aPos[nCnt][2];
		m_Vec[nCnt][3] = m_aPos[nCnt][0] - m_aPos[nCnt][3];

		//四頂点分
		for (int nCount = 0; nCount < 4; nCount++)
		{
			//外積の式2
			if (nCount == 0)
			{
				g_Vec[nCnt][nCount] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPos[nCnt][nCount];
			}
			else if (nCount == 1)
			{
				g_Vec[nCnt][nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPos[nCnt][nCount];
			}
			else if (nCount == 2)
			{
				g_Vec[nCnt][nCount] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPos[nCnt][nCount];
			}
			else if (nCount == 3)
			{
				g_Vec[nCnt][nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPos[nCnt][nCount];
			}

			//外積計算
			PlayerVec[nCnt][nCount] = (m_Vec[nCnt][nCount].z * g_Vec[nCnt][nCount].x) - (m_Vec[nCnt][nCount].x * g_Vec[nCnt][nCount].z);
		}

		//if (pPos->y <= m_pos.y + m_MaxStone.y - fHeightMin && pPos->y > m_pos.y - m_MinStone.y - fHeightMax)
		{
			if (PlayerVec[nCnt][0] > 0.0f && PlayerVec[nCnt][1] > 0.0f && PlayerVec[nCnt][2] > 0.0f && PlayerVec[nCnt][3] > 0.0f)
			{
				for (int nCount = 0; nCount < 4; nCount++)
				{
					//外積の式2
					if (nCount == 0)
					{
						g_oldVec[nCnt][nCount] = posold + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPos[nCnt][nCount];
					}
					else if (nCount == 1)
					{
						g_oldVec[nCnt][nCount] = posold + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPos[nCnt][nCount];
					}
					else if (nCount == 2)
					{
						g_oldVec[nCnt][nCount] = posold + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPos[nCnt][nCount];
					}
					else if (nCount == 3)
					{
						g_oldVec[nCnt][nCount] = posold + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPos[nCnt][nCount];
					}

					oldPlayerVec[nCnt][nCount] = (m_Vec[nCnt][nCount].z * g_oldVec[nCnt][nCount].x) - (m_Vec[nCnt][nCount].x * g_oldVec[nCnt][nCount].z);
				}

				//左右の当たり判定
				if (PlayerVec[nCnt][1] > 0 && PlayerVec[nCnt][3] > 0)
				{
					//表示用
					g_nDisplayVertex = g_nCntVertex[nCnt];

					//左
					if (oldPlayerVec[nCnt][0] <= 0 && PlayerVec[nCnt][0] >= 0 && PlayerVec[nCnt][2] >= 0)
					{
						g_nDisplayVertex2 = 0;

						//2頂点差を計算
						D3DXVECTOR3 a = m_aPos[nCnt][0] - m_aPos[nCnt][1];

						//角度を計算
						double nAtan2 = atan2(a.z, a.x);

						if (nAtan2 > 0.0f)						//斜辺の場合
						{
							pPos->z = pPosOld->z;				//1フレーム前の位置に戻す
							pPos->x = pPosOld->x;
							pMove->z = 0.0f;					//移動量をゼロにする
							pMove->x = sinf((float)nAtan2) * 5;
						}

						else if (nAtan2 <= 0.0f)				//斜辺の場合 または角度が0のとき
						{
							pPos->z = pPosOld->z;				//1フレーム前の位置に戻す
							pPos->x = pPosOld->x;
							pMove->z = 0.0f;					//移動量をゼロにする
							pMove->x = sinf((float)-nAtan2) * 5;
						}

						//表示用
						g_fAtan2Vecter = (float)nAtan2;
					}

					//右
					else if (oldPlayerVec[nCnt][2] <= 0 && PlayerVec[nCnt][2] >= 0 && PlayerVec[nCnt][1] >= 0)
					{
						//表示用
						g_nDisplayVertex2 = 2;

						//2頂点差を計算
						D3DXVECTOR3 a = m_aPos[nCnt][2] - m_aPos[nCnt][3];

						//角度を計算
						double nAtan2 = atan2(a.z, a.x);

						if (nAtan2 > 0.0f)							//斜辺の場合
						{
							pPos->z = pPosOld->z;					//1フレーム前の位置に戻す
							pPos->x = pPosOld->x;
							pMove->z = 0.0f;						//移動量をゼロにする
							pMove->x = sinf((float)-nAtan2) * 5;
						}

						else if (nAtan2 <= 0.0f)					//斜辺の場合 または角度が0のとき	
						{
							pPos->z = pPosOld->z;					//1フレーム前の位置に戻す
							pPos->x = pPosOld->x;
							pMove->z = 0.0f;						//移動量をゼロにする
							pMove->x = sinf((float)nAtan2) * 5;
						}

						//表示用
						g_fAtan2Vecter = (float)nAtan2;
					}
				}

				//前後の当たり判定
				if (PlayerVec[nCnt][0] > 0 && PlayerVec[nCnt][2] > 0)
				{
					//表示用
					g_nDisplayVertex = g_nCntVertex[nCnt];

					//後
					if (oldPlayerVec[nCnt][1] <= 0 && PlayerVec[nCnt][1] >= 0)
					{
						//表示用
						g_nDisplayVertex2 = 1;

						//2頂点差を計算
						D3DXVECTOR3 a = m_aPos[nCnt][2] - m_aPos[nCnt][1];

						//角度を計算
						double nAtan2 = atan2(a.z, a.x);

						if (nAtan2 > 0.0f)							//斜辺の場合
						{
							pPos->z = pPosOld->z;					//1フレーム前の位置に戻す
							pPos->x = pPosOld->x;
							pMove->x = 0.0f;						//移動量をゼロにする
							pMove->z = cosf((float)nAtan2) * 5;
						}

						else if (nAtan2 <= 0.0f)					//斜辺の場合 または角度が0のとき
						{
							pPos->z = pPosOld->z;					//1フレーム前の位置に戻す
							pPos->x = pPosOld->x;
							pMove->x = 0.0f;						//移動量をゼロにする
							pMove->z = cosf((float)-nAtan2) * 5;
						}

						//表示用
						g_fAtan2Vecter = (float)nAtan2;
					}

					//前
					if (oldPlayerVec[nCnt][3] <= 0 && PlayerVec[nCnt][3] >= 0)
					{
						//表示用
						g_nDisplayVertex2 = 3;

						//2頂点差を計算
						D3DXVECTOR3 a = m_aPos[nCnt][0] - m_aPos[nCnt][3];

						//角度を計算
						double nAtan2 = atan2(a.z, a.x);

						if (nAtan2 > 0.0f)							//斜辺の場合
						{
							pPos->z = pPosOld->z;					//1フレーム前の位置に戻す
							pPos->x = pPosOld->x;
							pMove->x = 0.0f;						//移動量をゼロにする
							pMove->z = cosf((float)nAtan2) * 5;
						}

						else if (nAtan2 <= 0.0f)					//斜辺の場合	 または角度が0のとき			
						{
							pPos->z = pPosOld->z;					//1フレーム前の位置に戻す
							pPos->x = pPosOld->x;
							pMove->x = 0.0f;						//移動量をゼロにする
							pMove->z = cosf((float)-nAtan2) * 5;
						}

						//表示用
						g_fAtan2Vecter = (float)nAtan2;
					}
				}
			}
		}
	}

	//テキストで生成した数だけ
	//読み込んだ座標をもとに当たり判定を作る
	for (int nCnt = 0; nCnt < g_nMaxFail + 1; nCnt++)
	{
		//可視化
		if (g_bObjectVector[nCnt] == false)
		{
			CVisualization::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_posObjectVector[nCnt][0], g_posObjectVector[nCnt][1], g_posObjectVector[nCnt][3], g_posObjectVector[nCnt][2], false, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.3f));
			g_bObjectVector[nCnt] = true;
		}
	}

	return bLand;
	//return false;
}

//==============================================================================
//Xファイルの読み込み
//==============================================================================
void CVertex::LoadObject(void)
{
	FILE *pFile = NULL;
	char aEqual[2] = { NULL };					// 「=」読み取り用変数
	bool bComment = false;						// コメントアウトするか
	char aText[TEXT_LENGTH] = { NULL };			// テキスト読み取り用変数
	char aSetObject[DATA_LENGTH] = { NULL };	// テキスト読み取り用変数
	char aSetObjectEND[DATA_LENGTH] = { NULL };	// テキスト読み取り用変数

												// strcmp読み取り用ポインタ
	char *pText = &aText[0];
	char *pSetObject = &aSetObject[0];
	char *pSetObjectEND = &aSetObjectEND[0];

	pFile = fopen("data/TEXT/vectorget.txt", "r");

	aText[0] = { NULL };
	pSetObject[0] = { NULL };
	pSetObjectEND[0] = { NULL };

	while (strcmp("END_SCRIPT", pText) != 0)
	{
		aText[0] = { NULL };

		if (bComment == false)
		{
			// コメントアウトしていない
			fscanf(pFile, "%s", &aText[0]);

			if (aText[0] == '#')
			{
				// 文字列の先頭が「#」ならばコメントアウトへ
				bComment = true;
			}

			else
			{
				if (strcmp("VECTORSET", aText) == 0)
				{
					pSetObjectEND[0] = { NULL };

					g_FailCntVecter++;

					while (strcmp("END_VECTORSET", pSetObjectEND) != 0)
					{
						fscanf(pFile, "%s", &pSetObjectEND[0]);

						if (strcmp("Vec1POSX", pSetObjectEND) == 0)
						{
							// モデルposX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][0].x);
							g_nCntVertex[g_FailCntVecter] = g_FailCntVecter + 1;
						}

						if (strcmp("Vec1POSY", pSetObjectEND) == 0)
						{
							// モデルposY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][0].y);
						}

						if (strcmp("Vec1POSZ", pSetObjectEND) == 0)
						{
							// モデルposZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][0].z);
						}

						if (strcmp("Vec2POSX", pSetObjectEND) == 0)
						{
							// モデルposX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][1].x);
						}

						if (strcmp("Vec2POSY", pSetObjectEND) == 0)
						{
							// モデルposY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][1].y);
						}

						if (strcmp("Vec2POSZ", pSetObjectEND) == 0)
						{
							// モデルposZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][1].z);
						}

						if (strcmp("Vec3POSX", pSetObjectEND) == 0)
						{
							// モデルposX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][2].x);
						}

						if (strcmp("Vec3POSY", pSetObjectEND) == 0)
						{
							// モデルposY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][2].y);
						}

						if (strcmp("Vec3POSZ", pSetObjectEND) == 0)
						{
							// モデルposZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][2].z);
						}

						if (strcmp("Vec4POSX", pSetObjectEND) == 0)
						{
							// モデルposX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][3].x);
						}

						if (strcmp("Vec4POSY", pSetObjectEND) == 0)
						{
							// モデルposY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][3].y);
						}

						if (strcmp("Vec4POSZ", pSetObjectEND) == 0)
						{
							// モデルposZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][3].z);
						}
					}
				}
			}
		}
		else if (bComment == true)
		{
			// コメントアウト処理
			// ローカル変数宣言
			char a = NULL;
			char b = NULL;
			fscanf(pFile, "%c", &a);
			while (a != '\n' && b != '\n')
			{
				fscanf(pFile, "%s", &aText[0]);
				fscanf(pFile, "%c", &b);
			}
			bComment = false;
		}
	}

	//テクストで生成する数を記憶
	g_nMaxFail = g_FailCntVecter;

	//テキストを閉じる
	fclose(pFile);
}

//==============================================================================
// 当たっている生成番号
//==============================================================================
int CVertex::GetNumber(void)
{
	return g_nDisplayVertex - 1;
}

//==============================================================================
// 当たっているベクトル番号
//==============================================================================
int CVertex::GetNumber2(void)
{
	return g_nDisplayVertex2;
}

//==============================================================================
// 当たっている斜面の角度
//==============================================================================
float CVertex::GetAtan2(void)
{
	return g_fAtan2Vecter;
}
