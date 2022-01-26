//=============================================================================
//
// テキスト生成オブジェクト処理 [textobject.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "textobject.h"
#include "object3D.h"

//グローバル変数宣言
CObject3D *CTextobject::m_pObject3D[MAX_OBJECT_3D] = {};		//生成したポインタ
D3DXVECTOR3 g_posObject3DText[256];								//読み込んだ位置情報
D3DXVECTOR3 g_rotObject3DText[256];								//読み込んだ位置情報
int g_nCntObject3DText;											//生成した数を記憶
int g_nTypeObject3DText[256];									//タイプ情報を記憶
int g_nMaxFileX;												//読み込んだ最大モデル数
int g_nCntName = 0;												//読み込んだURL数を記憶
char g_aNameObject3D[256][128];									//読み込んむURL情報

//==============================================================================
// 
//==============================================================================
CTextobject::CTextobject()
{
	g_posObject3DText[256] = {};
	g_rotObject3DText[256] = {};
	g_nCntObject3DText = 0;
	g_nTypeObject3DText[256] = {};
	g_aNameObject3D[256][128] = {};
	g_nCntName = 0;
}

//==============================================================================
// 
//==============================================================================
CTextobject::~CTextobject()
{
}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CTextobject::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	LoadObject3D();

	//読み込んだ回数分生成
	for (int nCount = 0; nCount < g_nCntObject3DText; nCount++)
	{
		m_pObject3D[nCount] = CObject3D::Create(g_posObject3DText[nCount], g_rotObject3DText[nCount], g_nTypeObject3DText[nCount]);
	}

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CTextobject::Uninit(void)
{

}

//==============================================================================
// 更新処理
//==============================================================================
void CTextobject::Update(void)
{

}

//==============================================================================
// 描画処理
//==============================================================================
void CTextobject::Draw(void)
{

}

//==============================================================================
// 生成した回数を伝える
//==============================================================================
int CTextobject::GetTextObject3D()
{
	return g_nCntObject3DText;
}

//==============================================================================
// 読み込むXファイル最大数
//==============================================================================
int CTextobject::GetMaxFileX()
{
	return g_nMaxFileX;
}

//==============================================================================
// テキストURLを伝える 回数
//==============================================================================
char * CTextobject::GetTextName(int nNumber)
{
	return g_aNameObject3D[nNumber];
}

//==============================================================================
// 生成処理
//==============================================================================
CTextobject * CTextobject::Create()
{
	// ポインタ変数の生成
	CTextobject *pTextobject;
	pTextobject = new CTextobject;

	// NULLチェック
	if (pTextobject != NULL)
	{
		pTextobject->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
	}

	return pTextobject;
}

//==============================================================================
//Xファイルの読み込み
//==============================================================================
void CTextobject::LoadObject3D(void)
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

	pFile = fopen("data/TEXT/object3Dget.txt", "r");

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
				if (strcmp("MODEL_FILENAME", pText) == 0)
				{
					// Xファイル名の読み込み
					fscanf(pFile, "%s", &aEqual[0]);
					fscanf(pFile, "%s", &g_aNameObject3D[g_nCntName][0]);
					g_nCntName++;
				}

				if (strcmp("MAX_MODEL", pText) == 0)
				{
					// Xファイル名の読み込み
					fscanf(pFile, "%s", &aEqual[0]);
					fscanf(pFile, "%d", &g_nMaxFileX);
				}

				else if (strcmp("OBJECTSET", aText) == 0)
				{
					pSetObjectEND[0] = { NULL };

					while (strcmp("END_OBJECTSET", pSetObjectEND) != 0)
					{
						fscanf(pFile, "%s", &pSetObjectEND[0]);

						if (strcmp("POSX", pSetObjectEND) == 0)
						{
							// モデルposX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObject3DText[g_nCntObject3DText].x);
						}

						if (strcmp("POSY", pSetObjectEND) == 0)
						{
							// モデルposY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObject3DText[g_nCntObject3DText].y);
						}

						if (strcmp("POSZ", pSetObjectEND) == 0)
						{
							// モデルposZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObject3DText[g_nCntObject3DText].z);
						}

						if (strcmp("ROTX", pSetObjectEND) == 0)
						{
							// モデルposX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_rotObject3DText[g_nCntObject3DText].x);
						}

						if (strcmp("ROTY", pSetObjectEND) == 0)
						{
							// モデルposY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_rotObject3DText[g_nCntObject3DText].y);
						}

						if (strcmp("ROTZ", pSetObjectEND) == 0)
						{
							// モデルposZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_rotObject3DText[g_nCntObject3DText].z);
						}

						if (strcmp("TYPE", pSetObjectEND) == 0)
						{
							// モデルposZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &g_nTypeObject3DText[g_nCntObject3DText]);
						}
					}
					g_nCntObject3DText++;
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

	//テキストを閉じる
	fclose(pFile);
}
