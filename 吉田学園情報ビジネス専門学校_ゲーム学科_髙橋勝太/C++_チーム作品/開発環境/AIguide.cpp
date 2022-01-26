//=============================================================================
//
// シーンX　AIガイド　処理 [AIguide.cpp]
// Author : katano hiroya(元ソース:takahashi shota
//
//=============================================================================
#include "AIguide.h"
#include "sceneX.h"
#include "game.h"
#include "car.h"
#include "input.h"
#include "manager.h"

//静的メンバ変数宣言
D3DXVECTOR3 CAIguide::m_AIguide[MAX_GUIDE_AREA][MAX_GUIDE_POINT] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CAIguide::CAIguide(int nPriority, int nType)
{
	LoadAIguide();
}

//=============================================================================
// デストラクタ
//=============================================================================
CAIguide::~CAIguide()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CAIguide::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void CAIguide::Uninit(void)
{
	
}

//=============================================================================
// 更新処理
//=============================================================================
void CAIguide::Update(void)
{
	
}

//=============================================================================
// 描画処理
//=============================================================================
void CAIguide::Draw(void)
{
	
}

//=============================================================================
// 生成処理
//=============================================================================
CAIguide * CAIguide::Create(D3DXVECTOR3 pos, char * TextName)
{
	CAIguide *pAIguide = NULL;
	for (int nCntArea = 0; nCntArea < MAX_GUIDE_AREA; nCntArea++)
	{
		for (int nCntPoint = 0; nCntPoint < MAX_GUIDE_POINT; nCntPoint++)
		{
			// ローカル変数宣言
			pAIguide = new CAIguide;

			pAIguide->BindTextName(TextName);

			// 初期化
			if (pAIguide != NULL)
			{
				pAIguide->Init(m_AIguide[nCntArea][nCntPoint], 0, 0);
			}

		}
	}
	return pAIguide;
}

//=============================================================================
// 生成処理
//=============================================================================
CAIguide * CAIguide::CreateSave(D3DXVECTOR3 pos, char * TextName)
{
	// ローカル変数宣言
	CAIguide *pAIguide = NULL;
	pAIguide = new CAIguide;

	pAIguide->BindTextName(TextName);

	// 初期化
	if (pAIguide != NULL)
	{
		pAIguide->Init(pos, 0, 0);
	}

	return pAIguide;
}

//==============================================================================
// .txtフォルダの読み込み
//==============================================================================
void CAIguide::LoadAIguide(void)
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
	pFile = fopen("data/TEXT/LoadAIguide.txt", "r");

	if (pFile != NULL)
	{
		//ファイル展開可能
		aText[0] = { NULL };
		aAreaSet[0] = { NULL };
		aPointSet[0] = { NULL };
		aPoint[0] = { NULL };
		int nIdxParts = 0;			// パーツ数
		int nArea = 0;			// モーションの数
		int nCntFile = 0;			// Xデータの数
		int nAs = 0;

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

				else if (strcmp("GUIDESET", pText) == 0)
				{
					// 変数の初期化
					aAreaSet[0] = { NULL };

					// ローカル変数宣言
					int nArea = 0;

					while (strcmp("END_GUIDESET", pAreaSet) != 0)
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

											fscanf(pFile, "%f", &m_AIguide[nArea][nPoint].x);
											fscanf(pFile, "%f", &m_AIguide[nArea][nPoint].y);
											fscanf(pFile, "%f", &m_AIguide[nArea][nPoint].z);
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
D3DXVECTOR3 *CAIguide::GetAIguidePos(int nArea, int nPoint)
{
	return &m_AIguide[nArea][nPoint];
}

