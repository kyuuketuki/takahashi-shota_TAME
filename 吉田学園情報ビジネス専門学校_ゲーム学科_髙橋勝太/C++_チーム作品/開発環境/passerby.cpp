//=============================================================================
//
// 通行人処理 [passerby.cpp]
// Author : Miura　Yudai
//
//=============================================================================
#include "passerby.h"
#include "manager.h"
#include "model.h"
#include "camera.h"
#include "shadowS.h"
#include "scene3D.h"
#include "player3D.h"
#include "destination.h"

//静的メンバ変数
D3DXVECTOR3 CPasserby::m_posHoming;
bool CPasserby::m_bUse = false;

//==============================================================================
// コンストラクタ
//==============================================================================
CPasserby::CPasserby(int nPriority, int nType)
{
	//メンバ変数初期化
	m_bUse = false;
	m_bMotionChange = false;
	m_nBrendFrame = 5;
}

//==============================================================================
// デストラクタ
//==============================================================================
CPasserby::~CPasserby()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CPasserby::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//モーション読み込み
	LoadMotion(m_passerbytype);

	for (int nCntModel = 0; nCntModel < (int)m_nNumModel; nCntModel++)
	{
		//モデルを生成
		m_apModel[nCntModel] = CModel::Create(D3DXVECTOR3(m_aOffset[nCntModel][0], m_aOffset[nCntModel][1], m_aOffset[nCntModel][2]),
			D3DXVECTOR3(m_aOffset[nCntModel][3], m_aOffset[nCntModel][4], m_aOffset[nCntModel][5]), &m_pModelName[nCntModel][0]);

		m_apModel[nCntModel]->SetParent(m_apModel[m_aIndexParent[nCntModel]]);
	
		D3DXMATERIAL *pMat;				//マテリアルへのポインタ

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)m_apModel[nCntModel]->GetBuff()->GetBufferPointer();

		if (m_passerbytype == PASSERBY_TYPE_CUSTOMER)
		{
			//マテリアルの色変更
			pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		}

		else
		{
			//マテリアルの色変更
			pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ローカル変数宣言
	int nNumVtx;			// 頂点数
	DWORD sizeFVF;			// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;			// 頂点バッファへのポインタ

	// 頂点数を取得
	nNumVtx = m_apModel[0]->GetMash()->GetNumVertices();

	// 頂点フォーマットのサイズの取得
	sizeFVF = D3DXGetFVFVertexSize(m_apModel[0]->GetMash()->GetFVF());

	// 頂点バッファをロック
	m_apModel[0]->GetMash()->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// 頂点座標の比較と更新
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

													// 各座標の最大値の比較
		if (m_MaxPasserby.x < vtx.x)
		{
			m_MaxPasserby.x = vtx.x;
		}
		if (m_MaxPasserby.y < vtx.y)
		{
			m_MaxPasserby.y = vtx.y;
		}
		if (m_MaxPasserby.z < vtx.z)
		{
			m_MaxPasserby.z = vtx.z;
		}

		// 各座標の最小値の比較
		if (m_MinPasserby.x > vtx.x)
		{
			m_MinPasserby.x = vtx.x;
		}
		if (m_MinPasserby.y > vtx.y)
		{
			m_MinPasserby.y = vtx.y;
		}
		if (m_MinPasserby.z > vtx.z)
		{
			m_MinPasserby.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	// 頂点フォーマットのサイズ分ポインタを進める
	}

	// 頂点バッファをアンロック
	m_apModel[0]->GetMash()->UnlockVertexBuffer();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CPasserby::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < (int)m_nNumModel; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{
			//モデルを生成
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = NULL;
		}
	}

	//オブジェクトの破棄
	Release();
}

//==============================================================================
// 更新処理
//==============================================================================
void CPasserby::Update(void)
{
	//通行人の設定
	PasserbyMove();

	//プレイヤーの位置情報を獲得
	D3DXVECTOR3 pP = CPlayer3D::GetPosition();

	//当たり判定の大きさ
	float fCollision = 150.0f;

	if (m_bUse == false)
	{
		//当たり判定 お客さん以外
		if (m_passerbytype != PASSERBY_TYPE_CUSTOMER)
		{
			if (m_pos.x + fCollision >= pP.x && m_pos.x - fCollision <= pP.x&&
				m_pos.z + fCollision >= pP.z && m_pos.z - fCollision <= pP.z)
			{
				//フラグ処理
				m_bUse = true;

				//パーティクル発生
				CGame::GameParticle(D3DXVECTOR3(m_pos.x, m_pos.y + 200.0f, m_pos.z), 1);
			}
		}
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void CPasserby::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//ローカル
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	//計算用マトリックス

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

	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->Draw();
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void CPasserby::PasserbyMove(void)
{
	//ローカル変数宣言
	float fAngle;       //目的地までの角度
	D3DXVECTOR3 vec;    //目的地までのベクトル

	//値代入
	m_nRamdom[0] = m_randpos;
	m_nRamdom[1] = m_startpos;

	//当たり判定ゾーン
	m_aPos[m_nPasserbyCount][0] = D3DXVECTOR3(m_pos.x + m_MinPasserby.x, 0.0f, m_pos.z + m_MinPasserby.z);
	m_aPos[m_nPasserbyCount][1] = D3DXVECTOR3(m_pos.x + m_MinPasserby.x, 0.0f, m_pos.z + m_MaxPasserby.z);
	m_aPos[m_nPasserbyCount][2] = D3DXVECTOR3(m_pos.x + m_MaxPasserby.x, 0.0f, m_pos.z + m_MaxPasserby.z);
	m_aPos[m_nPasserbyCount][3] = D3DXVECTOR3(m_pos.x + m_MaxPasserby.x, 0.0f, m_pos.z + m_MinPasserby.z);

	// 移動量の加算
	m_pos += m_move;
	m_posHoming = m_pos;

	//モーションの更新
	UpdateMotion(m_MotionType);

	if (m_passerbytype != PASSERBY_TYPE_CUSTOMER)
	{
		if (m_bGoal == false)
		{
			//モーションの変化
			m_MotionType = MOTIONTYPE_PASSERBY_MOVE;

			vec = m_goalpos - m_pos;		            //現在地から目的地へのベクトル
			fAngle = atan2f(vec.x, vec.z);				//ベクトルの角度を求める
			m_move.x = sinf(fAngle) * 7.5f;				//移動量を求める
			m_move.z = cosf(fAngle) * 7.5f;				//移動量を求める
			m_rotDest.y = fAngle + D3DX_PI;				//目的地に合わせて角度を変える

														//目的地についた時の処理
			if (m_pos.x >= m_goalpos.x - 10.0f && m_pos.x <= m_goalpos.x + 10.0f &&
				m_pos.z >= m_goalpos.z - 10.0f && m_pos.z <= m_goalpos.z + 10.0f)
			{
				m_bGoal = true;
				m_nRand = rand() % 2 + 0;//ランダムの目的地設定
			}
		}
		else if (m_bGoal == true)
		{
			//モーションの変化
			m_MotionType = MOTIONTYPE_PASSERBY_MOVE;

			vec = m_nRamdom[m_nRand] - m_pos;		    //現在地から目的地へのベクトル
			fAngle = atan2f(vec.x, vec.z);		//ベクトルの角度を求める
			m_move.x = sinf(fAngle) * 7.5f;	//移動量を求める
			m_move.z = cosf(fAngle) * 7.5f;	//移動量を求める
			m_rotDest.y = fAngle + D3DX_PI;		//目的地に合わせて角度を変える

												//目的地についた時の処理
			if (m_pos.x >= m_nRamdom[m_nRand].x - 10.0f && m_pos.x <= m_nRamdom[m_nRand].x + 10.0f &&
				m_pos.z >= m_nRamdom[m_nRand].z - 10.0f && m_pos.z <= m_nRamdom[m_nRand].z + 10.0f)
			{
				m_bGoal = false;
			}
		}
	}

	//各パーツの更新処理
	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->MotionPos(D3DXVECTOR3(m_aOffset[nCnt][0], m_aOffset[nCnt][1], m_aOffset[nCnt][2]));
		m_apModel[nCnt]->MotionRot(D3DXVECTOR3(m_aOffset[nCnt][3], m_aOffset[nCnt][4], m_aOffset[nCnt][5]));
	}

	// 加速後の減速処理
	m_move.x += (0.0f - m_move.x) * 0.2f;
	m_move.z += (0.0f - m_move.z) * 0.2f;

	// 目的の回転角の上限
	if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rotDest.y += D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rotDest.y -= D3DX_PI * 2.0f;
	}

	// 向きの更新
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.1f;

	// 現在の回転角の上限
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
}

//==============================================================================
// お客様の動き
//==============================================================================
void CPasserby::CustomerMove(void)
{
	//フラグを目的地から持ってくる
	bool bUse = CDestination::GetMotion();

	if (bUse == false)
	{
		//モーションの変化
		m_MotionType = MOTIONTYPE_PASSERBY_NEUTRAL;
	}
	else
	{
		//モーションの変化
		m_MotionType = MOTIONTYPE_PASSERBY_MOVE;
	}

	//モーションの更新
	UpdateMotion(m_MotionType);

	//各パーツの更新処理
	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->MotionPos(D3DXVECTOR3(m_aOffset[nCnt][0], m_aOffset[nCnt][1], m_aOffset[nCnt][2]));
		m_apModel[nCnt]->MotionRot(D3DXVECTOR3(m_aOffset[nCnt][3], m_aOffset[nCnt][4], m_aOffset[nCnt][5]));
	}
}

//==============================================================================
// 生成処理
//==============================================================================
CPasserby * CPasserby::Create(D3DXVECTOR3 pos, D3DXVECTOR3 goalpos, D3DXVECTOR3 randpos, PASSERBY_TYPE passerbytype)
{
	//プレイヤーを生成
	CPasserby *pPasserby = NULL;
	pPasserby = new CPasserby;

	//NULLチェック
	if (pPasserby != NULL)
	{
		//引数から値を代入(通行人のタイプ)
		pPasserby->m_passerbytype = passerbytype;

		//位置情報を取得
		pPasserby->m_pos = pos;
		pPasserby->m_pos.y = -20.0f;		//微調整
		pPasserby->m_startpos = pos;
		pPasserby->m_goalpos = goalpos;
		pPasserby->m_randpos = randpos;

		//初期化処理
		pPasserby->Init(pos, 0.0f, 0.0f);
	}

	return pPasserby;
}

//==============================================================================
// .txtフォルダの読み込み
//==============================================================================
void CPasserby::LoadMotion(PASSERBY_TYPE passerbytype)
{
	// ローカル変数宣言
	FILE *pFile = NULL;
	char aEqual[2] = { NULL };					// 「=」読み取り用変数
	bool bComment = false;						// コメントアウトするか
	char aText[TEXT_LENGTH] = { NULL };			// テキスト読み取り用変数
	char aCharaSet[DATA_LENGTH] = { NULL };		// キャラデータ読み取り用変数
	char aPartsSet[DATA_LENGTH] = { NULL };		// パーツデータ読み取り用変数
	char aMotionSet[DATA_LENGTH] = { NULL };	// モーション読み取り用変数
	char aKeySet[DATA_LENGTH] = { NULL };		// キー設定情報読み取り用変数
	char aKey[DATA_LENGTH] = { NULL };			// キー情報読み取り用変数

												// strcmp読み取り用ポインタ
	char *pText = &aText[0];
	char *pCharaSet = &aCharaSet[0];
	char *pPartsSet = &aPartsSet[0];
	char *pMotionSet = &aMotionSet[0];
	char *pKeySet = &aKeySet[0];
	char *pKey = &aKey[0];

	switch (passerbytype)
	{
	case PASSERBY_TYPE_MEN:
		// データの読み込み
		pFile = fopen("data/TEXT/motion_passerby00.txt", "r");
		break;
	case PASSERBY_TYPE_CAT:
		// データの読み込み
		pFile = fopen("data/TEXT/motion_cat00.txt", "r");
		break;
	case PASSERBY_TYPE_CUSTOMER:
		// データの読み込み
		pFile = fopen("data/TEXT/motion_customer00.txt", "r");
		break;
	default:
		break;
	}

	if (pFile != NULL)
	{ //ファイル展開可能
		aText[0] = { NULL };
		aCharaSet[0] = { NULL };
		aPartsSet[0] = { NULL };
		aMotionSet[0] = { NULL };
		aKeySet[0] = { NULL };
		aKey[0] = { NULL };
		int nIdxParts = 0;			// パーツ数
		int nMotion = 0;			// モーションの数
		int nCntFile = 0;			// Xデータの数
		int nAs = 0;

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
				else
				{// 通常時
					if (strcmp("NUM_MODEL", pText) == 0)
					{// モデルの数
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%d", &m_nNumModel);
					}
					else if (strcmp("MODEL_FILENAME", pText) == 0)
					{// Xファイル名の読み込み
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%s", &m_pModelName[nCntFile][0]);
						nCntFile++;
					}
					else if (strcmp("CHARACTERSET", pText) == 0)
					{// キャラの情報
						while (strcmp("END_CHARACTERSET", pCharaSet) != 0)
						{// キャラ情報の読み取り
							fscanf(pFile, "%s", &aCharaSet[0]);
							if (strcmp("NUM_PARTS", pCharaSet) == 0)
							{

							}
							else if (strcmp("PARTSSET", pCharaSet) == 0)
							{// パーツ情報
								aPartsSet[0] = { NULL };

								while (strcmp("END_PARTSSET", pPartsSet) != 0)
								{
									fscanf(pFile, "%s", &aPartsSet[0]);
									if (strcmp("INDEX", pPartsSet) == 0)
									{// パーツ情報の読み取り
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &nIdxParts);
									}
									else if (strcmp("PARENT", pPartsSet) == 0)
									{// パーツの親情報
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &m_aIndexParent[nIdxParts]);
									}
									else if (strcmp("POS", pPartsSet) == 0)
									{// パーツの位置情報
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][0]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][1]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][2]);
									}
									else if (strcmp("ROT", pPartsSet) == 0)
									{// パーツの角度情報
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][3]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][4]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][5]);
									}
								}
							}
						}
					}
					else if (strcmp("MOTIONSET", pText) == 0)
					{
						// 変数の初期化
						aMotionSet[0] = { NULL };

						// ローカル変数宣言
						int nLoop = 0;
						int nKeySet = 0;

						while (strcmp("END_MOTIONSET", pMotionSet) != 0)
						{// モーションの読み込み
							fscanf(pFile, "%s", &aMotionSet[0]);
							if (strcmp("LOOP", pMotionSet) == 0)
							{// ループするか
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%d", &nLoop);
								if (nLoop == 1)
								{// １の場合ループする
									m_bLoopKeep = true;
									m_Loop[nMotion] = true;
								}
							}
							else if (strcmp("NUM_KEY", pMotionSet) == 0)
							{// キー数の読み込み
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%d", &m_nNumKeyKeep);
							}
							else if (strcmp("KEYSET", pMotionSet) == 0)
							{// キーの読み込み
							 // ローカル変数宣言
								int nKey = 0;
								// 変数の初期化
								aKeySet[0] = { NULL };

								while (strcmp("END_KEYSET", pKeySet) != 0)
								{// キー情報項目の読み込み
									fscanf(pFile, "%s", &aKeySet[0]);

									if (aKeySet[0] == '#')
									{// コメントアウト
									 // ローカル変数宣言
										char c = NULL;
										char d = NULL;
										fscanf(pFile, "%c", &c);
										while (c != '\n' && d != '\n')
										{
											fscanf(pFile, "%s", &aKey[0]);
											fscanf(pFile, "%c", &d);
										}
									}

									if (strcmp("KEY", pKeySet) == 0)
									{// キー情報の読み込み
										aKey[0] = { NULL };
										while (strcmp("END_KEY", pKey) != 0)
										{// 位置・角度の読み込み
											fscanf(pFile, "%s", &aKey[0]);
											if (strcmp("POS", pKey) == 0)
											{// 位置情報の読み込み
												fscanf(pFile, "%s", &aEqual[0]);
												// 位置情報の読み込み
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][0]);
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][1]);
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][2]);
												m_Passerby[nMotion][nKeySet][nKey][0] += m_aOffset[nKey][0];
												m_Passerby[nMotion][nKeySet][nKey][1] += m_aOffset[nKey][1];
												m_Passerby[nMotion][nKeySet][nKey][2] += m_aOffset[nKey][2];
											}
											else if (strcmp("ROT", pKey) == 0)
											{// 角度情報の読み込み
												fscanf(pFile, "%s", &aEqual[0]);
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][3]);
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][4]);
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][5]);
											}
										}
										nKey++;
									}
									else if (strcmp("FRAME", pKeySet) == 0)
									{// キーフレーム数の読み込み
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &m_nFrame[nMotion][nKeySet]);
										m_nFrameTotal[nMotion] += m_nFrame[nMotion][nKeySet];
										if (m_Loop[nMotion] == false && nKeySet == (m_NumKey[nMotion] - 1))
										{
											m_nFrameTotal[nMotion] -= m_nFrame[nMotion][nKeySet];
										}
									}
								}
								nKeySet++;
							}
						}
						m_NumKey[nMotion] = nKeySet;	// キー数の設定
						nMotion++;
					}
				}
			}
			else if (bComment == true)
			{// コメントアウト処理
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
		fclose(pFile);
	}
	else
	{ // ファイル展開不可
	}
}

//==============================================================================
// モーションの更新
//==============================================================================
void CPasserby::UpdateMotion(MOTIONTYPE_PASSERBY passerby)
{
	// キー数とループ情報の代入
	m_nNumKeyKeep = m_NumKey[passerby];
	m_bLoopKeep = m_Loop[passerby];

	// モーションカウンターの加算
	if (m_bMotionChange == true)
	{// モーションブレンド実行時
		if (m_fCounterMotion < m_nBrendFrame)
		{// カウント上限
			m_fCounterMotion++;
		}
	}
	else
	{// 通常時
		if (m_fCounterMotion < m_nFrame[passerby][m_Key])
		{// カウント上限
			m_fCounterMotion++;
		}
	}

	// モデルパーツごとのモーションアニメーション
	for (int nCnt = 0; nCnt < MAX_PASSERBY_PARTS; nCnt++)
	{
		if (m_Key == m_nNumKeyKeep - 1 && m_bLoopKeep == true)
		{// ループして最初に戻るとき
			m_aOffset[nCnt][0] = m_Passerby[passerby][m_Key][nCnt][0] + (m_Passerby[passerby][0][nCnt][0] - m_Passerby[passerby][m_Key][nCnt][0]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][1] = m_Passerby[passerby][m_Key][nCnt][1] + (m_Passerby[passerby][0][nCnt][1] - m_Passerby[passerby][m_Key][nCnt][1]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][2] = m_Passerby[passerby][m_Key][nCnt][2] + (m_Passerby[passerby][0][nCnt][2] - m_Passerby[passerby][m_Key][nCnt][2]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][3] = m_Passerby[passerby][m_Key][nCnt][3] + (m_Passerby[passerby][0][nCnt][3] - m_Passerby[passerby][m_Key][nCnt][3]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][4] = m_Passerby[passerby][m_Key][nCnt][4] + (m_Passerby[passerby][0][nCnt][4] - m_Passerby[passerby][m_Key][nCnt][4]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][5] = m_Passerby[passerby][m_Key][nCnt][5] + (m_Passerby[passerby][0][nCnt][5] - m_Passerby[passerby][m_Key][nCnt][5]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
			m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
			m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
			m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
			m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
			m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
		}
		else if (m_bMotionChange == true)
		{// モーションブレンド
			m_aOffset[nCnt][0] = m_aOffsetMemo[nCnt][0] + (m_Passerby[m_MotionType][0][nCnt][0] - m_aOffsetMemo[nCnt][0]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][1] = m_aOffsetMemo[nCnt][1] + (m_Passerby[m_MotionType][0][nCnt][1] - m_aOffsetMemo[nCnt][1]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][2] = m_aOffsetMemo[nCnt][2] + (m_Passerby[m_MotionType][0][nCnt][2] - m_aOffsetMemo[nCnt][2]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][3] = m_aOffsetMemo[nCnt][3] + (m_Passerby[m_MotionType][0][nCnt][3] - m_aOffsetMemo[nCnt][3]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][4] = m_aOffsetMemo[nCnt][4] + (m_Passerby[m_MotionType][0][nCnt][4] - m_aOffsetMemo[nCnt][4]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][5] = m_aOffsetMemo[nCnt][5] + (m_Passerby[m_MotionType][0][nCnt][5] - m_aOffsetMemo[nCnt][5]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
			m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
			m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
			m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
			m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
			m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
		}
		else
		{// 通常時
			m_aOffset[nCnt][0] = m_Passerby[passerby][m_Key][nCnt][0] + (m_Passerby[passerby][m_Key + 1][nCnt][0] - m_Passerby[passerby][m_Key][nCnt][0]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][1] = m_Passerby[passerby][m_Key][nCnt][1] + (m_Passerby[passerby][m_Key + 1][nCnt][1] - m_Passerby[passerby][m_Key][nCnt][1]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][2] = m_Passerby[passerby][m_Key][nCnt][2] + (m_Passerby[passerby][m_Key + 1][nCnt][2] - m_Passerby[passerby][m_Key][nCnt][2]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][3] = m_Passerby[passerby][m_Key][nCnt][3] + (m_Passerby[passerby][m_Key + 1][nCnt][3] - m_Passerby[passerby][m_Key][nCnt][3]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][4] = m_Passerby[passerby][m_Key][nCnt][4] + (m_Passerby[passerby][m_Key + 1][nCnt][4] - m_Passerby[passerby][m_Key][nCnt][4]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][5] = m_Passerby[passerby][m_Key][nCnt][5] + (m_Passerby[passerby][m_Key + 1][nCnt][5] - m_Passerby[passerby][m_Key][nCnt][5]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
			m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
			m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
			m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
			m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
			m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
		}

		if (m_bMotionChange == false)
		{// モーション変化なし
			if (m_fCounterMotion == m_nFrame[passerby][m_Key])
			{// キーフレーム終了時
			 //if (m_MotionType != MOTIONTYPE_JUMP || m_Key != m_nNumKeyKeep - 2)
				{// ジャンプ以外でキー終了時リセット
					m_fCounterMotion = 0;
				}

				if (m_Key == m_nNumKeyKeep - 1 && m_bLoopKeep == true)
				{// ループするとき
					m_Key = 0;
				}
				else if (m_Key == m_nNumKeyKeep - 2 && m_bLoopKeep == false)
				{// ループせず終了するとき
				 //if (m_MotionTypeOld != MOTIONTYPE_JUMP)
					{// 直前がジャンプ以外の時
						m_MotionType = MOTIONTYPE_PASSERBY_NEUTRAL;	// ニュートラルへ移行
						m_fCounterMotion = 0;
						break;
					}
				}
				else if (m_Key < m_nNumKeyKeep)
				{// キーのカウント
					m_Key++;
				}
			}
			else if (m_MotionTypeOld != m_MotionType)
			{// モーション変化時	
				m_Key = 0;
				m_fCounterMotion = 0;
				m_bMotionChange = true;

				if (m_MotionTypeOld == MOTIONTYPE_NEUTRAL && m_MotionType == MOTIONTYPE_MOVE)
				{// 移動からニュートラル時
					m_nBrendFrame = 20;
				}
				else if (m_MotionTypeOld == MOTIONTYPE_MOVE && m_MotionType == MOTIONTYPE_NEUTRAL)
				{// ニュートラルから移動時
					m_nBrendFrame = 40;
				}
				break;
			}
		}
		else if (m_bMotionChange == true)
		{// モーションブレンド時のカウントリセット
			if (m_fCounterMotion == m_nBrendFrame || m_MotionTypeOld != m_MotionType)
			{
				m_Key = 0;
				m_fCounterMotion = 0;
				m_bMotionChange = false;
			}
		}
	}

	// 直前のモーション状態の保存
	m_MotionTypeOld = m_MotionType;
}
//==============================================================================
// モーションの切り替わりフラグ
//==============================================================================
void CPasserby::SetUse(bool bUse)
{
	//引数から代入
	m_bUse = bUse;
}
