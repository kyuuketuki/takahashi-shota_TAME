//=============================================================================
//
// 信号機　処理 [trafficLight.cpp]
// Author : katano hiroya
//
//=============================================================================
#include "trafficLight.h"
#include "sceneX.h"
#include "game.h"
#include "car.h"
#include "input.h"
#include "manager.h"

//TRAFFICLIGHT_DATA CTrafficLight::m_TrafficLight[MAX_LIGHT_AREA][MAX_LIGHT_POINT] = {};
D3DXMATRIX      CTrafficLight::m_mtxWorld;

//=============================================================================
// コンストラクタ
//=============================================================================
CTrafficLight::CTrafficLight(int nPriority, int nType)
{
	LoadTrafficLight();
	LoadNoneTrafficLight();
}

//=============================================================================
// デストラクタ
//=============================================================================
CTrafficLight::~CTrafficLight()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTrafficLight::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSizeX, float fSizeY)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXLoadMeshFromX("data/MODEL/New-Traffic-light02.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

	if ((m_TrafficLight[m_nArea][m_nPoint].rot.y > D3DX_PI * 1 / 4
		&& m_TrafficLight[m_nArea][m_nPoint].rot.y < D3DX_PI * 3 / 4)
		|| (m_TrafficLight[m_nArea][m_nPoint].rot.y < -D3DX_PI * 1 / 8
			&& m_TrafficLight[m_nArea][m_nPoint].rot.y > -D3DX_PI * 3 / 8))
	{
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/City_Props02.png", &m_pTexture[m_nArea*m_nPoint + m_nPoint][0]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/City_Props01.png", &m_pTexture[m_nArea*m_nPoint + m_nPoint][1]);

		m_TrafficLight[m_nArea][m_nPoint].bSignal = false;
	}
	else
	{
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/City_Props02.png", &m_pTexture[m_nArea*m_nPoint + m_nPoint][0]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/City_Props01.png", &m_pTexture[m_nArea*m_nPoint + m_nPoint][1]);

		m_TrafficLight[m_nArea][m_nPoint].bSignal = true;
	}


	CSceneX::Init(pos, fSizeX, fSizeY);

	m_pos = pos;

	m_rot = rot;

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void CTrafficLight::Uninit(void)
{
	//メッシュの開放
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//テクスチャの破棄
	for (int n1 = 0; n1 < 28; n1++)
	{
		for (int n2 = 0; n2 < 2; n2++)
		{
			if (m_pTexture[n1][n2] != NULL)
			{
				m_pTexture[n1][n2]->Release();
				m_pTexture[n1][n2] = NULL;
			}
		}
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTrafficLight::Update(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//TABを押したとき
	if (m_nCntTime % 1200 == 0)
	{
		m_TrafficLight[m_nArea][m_nPoint].bSignal = !m_TrafficLight[m_nArea][m_nPoint].bSignal;

		//for (int nCntLight = 0; nCntLight < MAX_NONE_LIGHT; nCntLight++)
		//{
		//	m_NoneTrafficLight[nCntLight].bSignal = !m_NoneTrafficLight[nCntLight].bSignal;
		//}
	}

	m_nCntTime++;

	//CSceneX::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CTrafficLight::Draw(void)
{
	//ローカル
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存
	D3DXMATERIAL *pMat;				//マテリアルへのポインタ

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きの反転
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置の反転
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタ取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		if (m_TrafficLight[m_nArea][m_nPoint].bSignal == true)
		{
			pDevice->SetTexture(0, m_pTexture[m_nArea*m_nPoint + m_nPoint][0]);
		}
		else if (m_TrafficLight[m_nArea][m_nPoint].bSignal == false)
		{
			pDevice->SetTexture(0, m_pTexture[m_nArea*m_nPoint + m_nPoint][1]);
		}

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//保存したマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	//CSceneX::Draw();
}

//=============================================================================
// 生成処理
//=============================================================================
CTrafficLight * CTrafficLight::Create(D3DXVECTOR3 pos, int nArea, int nPoint)
{
	CTrafficLight *pTrafficLight = NULL;

	// ローカル変数宣言
	pTrafficLight = new CTrafficLight;

	// 初期化
	if (pTrafficLight != NULL)
	{
		pTrafficLight->m_nArea = nArea;

		pTrafficLight->m_nPoint = nPoint;

		pTrafficLight->Init(pTrafficLight->m_TrafficLight[nArea][nPoint].pos, pTrafficLight->m_TrafficLight[nArea][nPoint].rot, 0, 0);
	}

	return pTrafficLight;
}

//=============================================================================
// 生成処理
//=============================================================================
CTrafficLight * CTrafficLight::CreateNone(D3DXVECTOR3 pos, char * TextName)
{
	CTrafficLight *pTrafficLight = NULL;
	for (int nCntLight = 0; nCntLight < MAX_NONE_LIGHT; nCntLight++)
	{
		// ローカル変数宣言
		pTrafficLight = new CTrafficLight;

		pTrafficLight->BindTextName(TextName);

		// 初期化
		if (pTrafficLight != NULL)
		{
			pTrafficLight->Init(pTrafficLight->m_NoneTrafficLight[nCntLight].pos, pTrafficLight->m_NoneTrafficLight[nCntLight].rot, 0, 0);
			pTrafficLight->SetRevolution(pTrafficLight->m_NoneTrafficLight[nCntLight].rot);
		}
	}
	return pTrafficLight;
}

//=============================================================================
// 生成処理
//=============================================================================
CTrafficLight * CTrafficLight::CreateSave(D3DXVECTOR3 pos, char * TextName)
{
	// ローカル変数宣言
	CTrafficLight *pTrafficLight = NULL;
	pTrafficLight = new CTrafficLight;

	pTrafficLight->BindTextName(TextName);

	// 初期化
	if (pTrafficLight != NULL)
	{
		pTrafficLight->Init(pos, D3DXVECTOR3(0, 0, 0), 0, 0);
	}

	return pTrafficLight;
}

//==============================================================================
// .txtフォルダの読み込み
//==============================================================================
void CTrafficLight::LoadTrafficLight(void)
{
	// ローカル変数宣言
	FILE *pFile = NULL;
	char aEqual[2] = { NULL };					//「=」読み取り用変数
	bool bComment = false;						// コメントアウトするか
	char aText[TEXT_LENGTH] = { NULL };			// テキスト読み取り用変数
	char aAreaSet[DATA_LENGTH] = { NULL };		// モーション読み取り用変数
	char aPointSet[DATA_LENGTH] = { NULL };		// キー設定情報読み取り用変数
	char aPoint[DATA_LENGTH] = { NULL };		// キー情報読み取り用変数

												// strcmp読み取り用ポインタ
	char *pText = &aText[0];
	char *pAreaSet = &aAreaSet[0];
	char *pPointSet = &aPointSet[0];
	char *pPoint = &aPoint[0];

	// データの読み込み
	pFile = fopen("data/TEXT/LoadTrafficLight.txt", "r");

	if (pFile != NULL)
	{
		//ファイル展開可能
		aText[0] = { NULL };
		aAreaSet[0] = { NULL };
		aPointSet[0] = { NULL };
		aPoint[0] = { NULL };

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

				else if (strcmp("LIGHTSET", pText) == 0)
				{
					// 変数の初期化
					aAreaSet[0] = { NULL };

					// ローカル変数宣言
					int nArea = 0;

					while (strcmp("END_LIGHTSET", pAreaSet) != 0)
					{
						// ガイドの読み込み
						fscanf(pFile, "%s", &aAreaSet[0]);

						if (strcmp("NUM_AREA", pAreaSet) == 0)
						{
							// ガイドエリア数の読み込み
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_NumArea);
						}

						else if (strcmp("AREASET", pAreaSet) == 0)
						{
							// ガイドエリアの読み込み
							// ローカル変数宣言
							int nPoint = 0;

							m_PointSet[nPoint] = nPoint;

							// 変数の初期化
							aPointSet[0] = { NULL };

							while (strcmp("END_AREASET", pPointSet) != 0)
							{
								// ガイドポイント情報項目の読み込み
								fscanf(pFile, "%s", &aPointSet[0]);

								if (aPointSet[0] == '#')
								{
									// コメントアウト
									// ローカル変数宣言
									char c = NULL;
									char d = NULL;

									fscanf(pFile, "%c", &c);

									while (c != '\n' && d != '\n')
									{
										fscanf(pFile, "%s", &aPoint[0]);
										fscanf(pFile, "%c", &d);
									}
								}

								if (strcmp("POINT", pPointSet) == 0)
								{
									// ガイドポイント情報の読み込み
									aPoint[0] = { NULL };
									while (strcmp("END_POINT", pPoint) != 0)
									{
										// 位置の読み込み
										fscanf(pFile, "%s", &aPoint[0]);

										if (strcmp("POS", pPoint) == 0)
										{
											// 位置情報の読み込み
											fscanf(pFile, "%s", &aEqual[0]);

											fscanf(pFile, "%f", &m_TrafficLight[nArea][nPoint].pos.x);
											fscanf(pFile, "%f", &m_TrafficLight[nArea][nPoint].pos.y);
											fscanf(pFile, "%f", &m_TrafficLight[nArea][nPoint].pos.z);
										}
										else if (strcmp("ROT", pPoint) == 0)
										{
											// 位置情報の読み込み
											fscanf(pFile, "%s", &aEqual[0]);

											fscanf(pFile, "%f", &m_TrafficLight[nArea][nPoint].rot.x);
											fscanf(pFile, "%f", &m_TrafficLight[nArea][nPoint].rot.y);
											fscanf(pFile, "%f", &m_TrafficLight[nArea][nPoint].rot.z);
										}
									}
									nPoint++;
								}
							}
							nArea++;
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
	}
	fclose(pFile);
}

//==============================================================================
// .txtフォルダの読み込み
//==============================================================================
void CTrafficLight::LoadNoneTrafficLight(void)
{
	// ローカル変数宣言
	FILE *pFile = NULL;
	char aEqual[2] = { NULL };					//「=」読み取り用変数
	bool bComment = false;						// コメントアウトするか
	char aText[TEXT_LENGTH] = { NULL };			// テキスト読み取り用変数
	char aAreaSet[DATA_LENGTH] = { NULL };		// モーション読み取り用変数
	char aPointSet[DATA_LENGTH] = { NULL };		// キー設定情報読み取り用変数
	char aPoint[DATA_LENGTH] = { NULL };		// キー情報読み取り用変数

												// strcmp読み取り用ポインタ
	char *pText = &aText[0];
	char *pAreaSet = &aAreaSet[0];
	char *pPointSet = &aPointSet[0];
	char *pPoint = &aPoint[0];

	// データの読み込み
	pFile = fopen("data/TEXT/LoadNoneTrafficLight.txt", "r");

	if (pFile != NULL)
	{
		//ファイル展開可能
		aText[0] = { NULL };
		aAreaSet[0] = { NULL };
		aPointSet[0] = { NULL };
		aPoint[0] = { NULL };

		while (strcmp("END_SCRIPT", pText) != 0)
		{
			aText[0] = { NULL };
			if (bComment == false)
			{// コメントアウトしていない
				fscanf(pFile, "%s", &aText[0]);

				if (aText[0] == '#')
				{// 文字列の先頭が「#」ならばコメントアウトへ
					bComment = true;
				}

				else if (strcmp("LIGHTSET", pText) == 0)
				{
					// 変数の初期化
					aAreaSet[0] = { NULL };

					// ローカル変数宣言
					int nArea = 0;

					while (strcmp("END_LIGHTSET", pAreaSet) != 0)
					{
						// ガイドの読み込み
						fscanf(pFile, "%s", &aAreaSet[0]);

						if (strcmp("NUM_AREA", pAreaSet) == 0)
						{
							// ガイドエリア数の読み込み
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_NumArea);
						}

						else if (strcmp("AREASET", pAreaSet) == 0)
						{
							// ガイドエリアの読み込み
							// ローカル変数宣言
							int nPoint = 0;

							m_PointSet[nPoint] = nPoint;

							// 変数の初期化
							aPointSet[0] = { NULL };

							while (strcmp("END_AREASET", pPointSet) != 0)
							{
								// ガイドポイント情報項目の読み込み
								fscanf(pFile, "%s", &aPointSet[0]);

								if (aPointSet[0] == '#')
								{
									// コメントアウト
									// ローカル変数宣言
									char c = NULL;
									char d = NULL;

									fscanf(pFile, "%c", &c);

									while (c != '\n' && d != '\n')
									{
										fscanf(pFile, "%s", &aPoint[0]);
										fscanf(pFile, "%c", &d);
									}
								}

								if (strcmp("POINT", pPointSet) == 0)
								{
									// ガイドポイント情報の読み込み
									aPoint[0] = { NULL };
									while (strcmp("END_POINT", pPoint) != 0)
									{
										// 位置の読み込み
										fscanf(pFile, "%s", &aPoint[0]);

										if (strcmp("POS", pPoint) == 0)
										{
											// 位置情報の読み込み
											fscanf(pFile, "%s", &aEqual[0]);

											fscanf(pFile, "%f", &m_NoneTrafficLight[nPoint].pos.x);
											fscanf(pFile, "%f", &m_NoneTrafficLight[nPoint].pos.y);
											fscanf(pFile, "%f", &m_NoneTrafficLight[nPoint].pos.z);
										}
										else if (strcmp("ROT", pPoint) == 0)
										{
											// 位置情報の読み込み
											fscanf(pFile, "%s", &aEqual[0]);

											fscanf(pFile, "%f", &m_NoneTrafficLight[nPoint].rot.x);
											fscanf(pFile, "%f", &m_NoneTrafficLight[nPoint].rot.y);
											fscanf(pFile, "%f", &m_NoneTrafficLight[nPoint].rot.z);
										}
									}
									nPoint++;
								}
							}
							nArea++;
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
	}
	fclose(pFile);
}