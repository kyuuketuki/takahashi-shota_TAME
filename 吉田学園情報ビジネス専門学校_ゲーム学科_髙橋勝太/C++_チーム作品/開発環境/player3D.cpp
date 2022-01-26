//=============================================================================
//
// プレイヤー3D処理 [player3D.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "player3D.h"
#include "manager.h"
#include "model.h"
#include "camera.h"
#include "shadowS.h"
#include "vertex.h"
#include "game.h"
#include "trafficLight.h"
#include "car.h"
#include "effect.h"
#include "passerby.h"

//静的メンバ宣言
D3DXVECTOR3		CPlayer3D::m_pos;
D3DXVECTOR3		CPlayer3D::m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
int				CPlayer3D::m_nNumModel = 0;
int				CPlayer3D::m_MotionType = NULL;
bool			CPlayer3D::m_bViolation;
bool			CPlayer3D::m_bMoveUse;

//==============================================================================
// コンストラクタ
//==============================================================================
CPlayer3D::CPlayer3D(int nPriority, int nType)
{
	//メンバ変数初期化
	m_bMotionChange = false;
	m_nBrendFrame = 5;
	m_MotionChange = MOTIONTYPE_NEUTRAL;
	m_bMoveUse = false;
}

//==============================================================================
// デストラクタ
//==============================================================================
CPlayer3D::~CPlayer3D()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CPlayer3D::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//モーション読み込み
	LoadMotion();

	for (int nCntModel = 0; nCntModel < (int)m_nNumModel; nCntModel++)
	{
		//モデルを生成
		m_apModel[nCntModel] = CModel::Create(D3DXVECTOR3(m_aOffset[nCntModel][0], m_aOffset[nCntModel][1], m_aOffset[nCntModel][2]),
			D3DXVECTOR3(m_aOffset[nCntModel][3], m_aOffset[nCntModel][4], m_aOffset[nCntModel][5]), &m_pModelName[nCntModel][0]);

		m_apModel[nCntModel]->SetParent(m_apModel[m_aIndexParent[nCntModel]]);
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//マテリアル色の変更
	for (int nCntModel = 0; nCntModel < 13; nCntModel++)
	{
		D3DXMATERIAL *pMat;				//マテリアルへのポインタ

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)m_apModel[nCntModel]->GetBuff()->GetBufferPointer();

		//マテリアルの色変更
		pMat->MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	}

	//位置
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//向き
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CPlayer3D::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < (int)m_nNumModel; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{
			//モデルを生成
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
		}
	}

	//オブジェクトの破棄
	Release();
}

//==============================================================================
// 更新処理
//==============================================================================
void CPlayer3D::Update(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//ゲームパッドのポインタ宣言
	CGamepad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//当たり判定状態情報
	bool bUse = CPasserby::GetUse();

	if (bUse == true && m_nCnt == 0)
	{
		//メンバ変数初期化
		m_fCounterMotion = 0;					//フレームカウントをリセット
		m_Key = 0;								//フレームキーをリセット
		m_GetKey = 0;							//フレームキーをリセット

		//モーション変更
		m_MotionChange = MOTIONTYPE_FALLMOVE;
		m_nCnt++;								//カウントアップ
	}

	//転ぶモーションが始まった時カウントを増やす
	if (m_nCnt >= 1)
	{
		//カウントアップ
		m_nCnt++;
	}

	//カウントが150の時初期位置に戻してあげる
	if (m_nCnt == 150)
	{
		m_pos = D3DXVECTOR3(0.0f, 70.0f, 0.0f);		//プレイヤーの初期位置に戻す
		CPasserby::SetUse(false);					//フラグを初期化する

		m_fCounterMotion = 0;						//フレームカウントをリセット
		m_Key = 0;									//フレームキーをリセット
		m_GetKey = 0;								//フレームキーをリセット
		m_nNextKey = false;							//モーションリセット

		m_MotionChange = MOTIONTYPE_NEUTRAL;		//モーションをニュートラルに戻す
		m_nCnt = 0;									//カウントを初期化
	}

	//プレイヤー状態初期化
	if (pInputKeyboard->GetPress(DIK_8) == true)
	{
		//モーション変更
		m_MotionChange = MOTIONTYPE_NEUTRAL;
	}

	//旋回速度
	float fCamera = 0.03f;
	//float fCamera = 0.2f;

	//位置更新
	m_posold = m_pos;

	//モーションの更新
	UpdateMotion(m_MotionChange, false);

	//移動処理オンオフ
	if (m_bMoveUse == true)
	{
		// 移動量の加算
		m_pos += m_move;
	}

	//カメラ角度取得
	D3DXVECTOR3 RotationCamera = CManager::GetCamera()->GetRot();
	float RotCameraY = RotationCamera.y;

	// 角度修正
	if (RotCameraY > D3DX_PI) { RotCameraY -= D3DX_PI * 2; }
	if (RotCameraY < -D3DX_PI) { RotCameraY += D3DX_PI * 2; }

	m_rotDest.y = RotCameraY - D3DX_PI;

	if (bUse == false)
	{
		// Sキーのモデルの移動
		if (pInputKeyboard->GetPress(DIK_S) == true || pGamepad->IsButtonPush(CInput::DOWN) == true)
		{
			if (m_nMoveCnt < 20)
			{
				m_nMoveCnt++;
			}

			//モーションの変化
			m_MotionChange = MOTIONTYPE_MOVE;

			// 左上方向
			if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT) == true)
			{
				//カウントアップ
				m_nCounter++;

				//モーションの変化
				m_MotionChange = MOTIONTYPE_LEFTMOVE;

				//移動量計算
				m_move.x -= cosf(RotCameraY + D3DX_PI / 4) * 0.5f;
				m_move.z += sinf(RotCameraY + D3DX_PI / 4) * 0.5f;
			}

			// 右上方向
			else if (pInputKeyboard->GetPress(DIK_D) == true || pGamepad->IsButtonPush(CInput::RIGHT) == true)
			{
				//カウントアップ
				m_nCounter++;

				//モーションの変化
				m_MotionChange = MOTIONTYPE_RIGHTMOVE;

				m_move.x += cosf(RotCameraY - D3DX_PI / 4) * 0.5f;
				m_move.z -= sinf(RotCameraY - D3DX_PI / 4) * 0.5f;
			}
			else
			{
				// 下方向
				m_move.x -= sinf(RotCameraY) * 0.3f;
				m_move.z -= cosf(RotCameraY) * 0.3f;
			}
		}

		// Wキーのモデルの移動
		if (pInputKeyboard->GetPress(DIK_W) == true ||pGamepad->IsButtonPush(CInput::UP) == true)
		{
			//エフェクトをクリエイト
			CEffect3D::Create(D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43), 50.0f, 50.0f);

			if (m_nMoveCnt < 20)
			{
				//カウントアップ
				m_nMoveCnt++;
			}

			//モーションの変化
			m_MotionChange = MOTIONTYPE_MOVE;

			// 左下方向
			if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT) == true)
			{
				//カウントアップ
				m_nCounter++;

				//メンバ変数初期化
				m_fCounterMotion = 0;//フレームカウントをリセット
				m_Key = 0;//フレームキーをリセット
				m_GetKey = 0;//フレームキーをリセット

				//モーションの変化
				m_MotionChange = MOTIONTYPE_LEFTMOVE;

				//移動量計算
				m_move.x -= cosf(m_rot.y - D3DX_PI / 4) * 0.5f;
				m_move.z += sinf(m_rot.y - D3DX_PI / 4) * 0.5f;
				m_rotDest.y = RotCameraY - D3DX_PI / -2;
				fCamera = 0.04f;
			}

			// 右下方向
			else if (pInputKeyboard->GetPress(DIK_D) == true || pGamepad->IsButtonPush(CInput::RIGHT) == true)
			{
				//カウントアップ
				m_nCounter++;

				//メンバ変数初期化
				m_fCounterMotion = 0;//フレームカウントをリセット
				m_Key = 0;//フレームキーをリセット
				m_GetKey = 0;//フレームキーをリセット

				//モーションの変化
				m_MotionChange = MOTIONTYPE_RIGHTMOVE;

				//移動量計算
				m_move.x += cosf(m_rot.y + D3DX_PI / 4) * 0.5f;
				m_move.z -= sinf(m_rot.y + D3DX_PI / 4) * 0.5f;
				m_rotDest.y = RotCameraY - D3DX_PI / 2;
				fCamera = 0.04f;
			}

			//これ大事だから消さないでね(^▽^)/
			if (pInputKeyboard->GetTrigger(DIK_A) == true || pInputKeyboard->GetTrigger(DIK_D) == true ||
				 pGamepad->IsButtonUp(CInput::RIGHT) == true || pGamepad->IsButtonUp(CInput::LEFT) == true)
			{
			}

			else
			{// 下方向
				m_move.x -= sinf(m_rot.y) * 2.0f;
				m_move.z -= cosf(m_rot.y) * 2.0f;
			}
		}
		else
		{
			if (m_nMoveCnt > 10)
			{
				//カウントを減らしていく
				m_nMoveCnt--;
			}
		}

		// 移動モーションから待機モーションへ
		if (m_MotionChange == MOTIONTYPE_MOVE && m_nMoveCnt == 10)
		{
			//メンバ変数初期化
			m_fCounterMotion = 0;//フレームカウントをリセット
			m_Key = 0;//フレームキーをリセット
			m_GetKey = 0;//フレームキーをリセット
			m_MotionChange = MOTIONTYPE_NEUTRAL;//モーションをニュートラルにする
		}

		//右移動モーションの時 && カウントが1の時
		if (m_MotionChange == MOTIONTYPE_RIGHTMOVE && m_nCounter == 1)
		{
			//メンバ変数初期化
			m_fCounterMotion = 0;//フレームカウントをリセット
			m_Key = 0;//フレームキーをリセット
			m_GetKey = 0;//フレームキーをリセット
		}
		//左移動モーションの時 && カウントが1の時
		if (m_MotionChange == MOTIONTYPE_LEFTMOVE && m_nCounter == 1)
		{
			//メンバ変数初期化
			m_fCounterMotion = 0;//フレームカウントをリセット
			m_Key = 0;//フレームキーをリセット
			m_GetKey = 0;//フレームキーをリセット
		}
	}

	//各パーツの更新処理
	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->MotionPos(D3DXVECTOR3(m_aOffset[nCnt][0], m_aOffset[nCnt][1], m_aOffset[nCnt][2]));
		m_apModel[nCnt]->MotionRot(D3DXVECTOR3(m_aOffset[nCnt][3], m_aOffset[nCnt][4], m_aOffset[nCnt][5]));
	}

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
	m_rot.y += (m_rotDest.y - m_rot.y) * fCamera;

	// 現在の回転角の上限
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	// 加速後の減速処理
	m_move.x += (0.0f - m_move.x) * 0.05f;
	m_move.z += (0.0f - m_move.z) * 0.05f;

	// 回転角度が3.14以上、-3.14以下にならないように
	if (m_rot.y >= D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	//頂点オブジェクトの当たり判定
	CVertex *pVer;
	pVer = CGame::GetVertex();

	if (pVer != NULL)
	{
		pVer->CollisionVec(&m_pos, &m_posold, &m_move, m_MaxPlayer.x - 10.0f, m_MinPlayer.x + 10.0f, m_MaxPlayer.z - 10.0f, m_MinPlayer.z + 10.0f, m_MaxPlayer.y, m_MinPlayer.y);
	}
	
	//回数分
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//車の当たり判定
		CCar *pCar;
		pCar = CGame::GetCar(nCnt);
		pCar->CollisionVec(&m_pos, &m_posold, &m_move, m_MaxPlayer.x - 30.0f, m_MinPlayer.x + 30.0f, m_MaxPlayer.z - 30.0f, m_MinPlayer.z + 30.0f, m_MaxPlayer.y, m_MinPlayer.y);
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void CPlayer3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//ローカル
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

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
// 生成処理
//==============================================================================
CPlayer3D * CPlayer3D::Create()
{
	//プレイヤーを生成
	CPlayer3D *pPlayer3D = NULL;
	pPlayer3D = new CPlayer3D;

	//NULLチェック
	if (pPlayer3D != NULL)
	{
		pPlayer3D->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
	}

	return pPlayer3D;
}

//==============================================================================
// .txtフォルダの読み込み
//==============================================================================
void CPlayer3D::LoadMotion(void)
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

	// データの読み込み
	pFile = fopen("data/TEXT/motion_player00.txt", "r");

	if (pFile != NULL)
	{ //ファイル展開可能
		aText[0] = { NULL };
		aCharaSet[0] = { NULL };
		aPartsSet[0] = { NULL };
		aMotionSet[0] = { NULL };
		aKeySet[0] = { NULL };
		aKey[0] = { NULL };

		//ローカル変数宣言
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
									//一度モーションの全てを初期化
									for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
									{
										for (int nCntKey = 0; nCntKey < 10; nCntKey++)
										{
											m_nFrame[nCntMotion][nCntKey] = 40;

											for (int nCntPart = 0; nCntPart < 30; nCntPart++)
											{
												for (int nCntPosRot = 0; nCntPosRot < 6; nCntPosRot++)
												{
													//モーションの初期化
													m_Player3D[nCntMotion][nCntKey][nCntPart][nCntPosRot] = m_aOffset[nCntPart][nCntPosRot];
												}
											}
										}
									}

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
						 //値を初期化
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

							if (strcmp("KEYSET", pMotionSet) == 0)
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
												fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][0]);
												fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][1]);
												fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][2]);
											}
											else if (strcmp("ROT", pKey) == 0)
											{// 角度情報の読み込み
												fscanf(pFile, "%s", &aEqual[0]);
												fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][3]);
												fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][4]);
												fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][5]);
											}
										}
										//元のパーツ位置に移動したPOSの値を足す
										m_Player3D[nMotion][nKeySet][nKey][0] += m_aOffset[nKey][0];
										m_Player3D[nMotion][nKeySet][nKey][1] += m_aOffset[nKey][1];
										m_Player3D[nMotion][nKeySet][nKey][2] += m_aOffset[nKey][2];

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

								//キー分プラス
								nKeySet++;
							}
						}

						// キー数の設定
						m_NumKey[nMotion] = m_nNumKeyKeep;

						//モーションのカウントアップ
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
void CPlayer3D::UpdateMotion(int MOTIONTYPE, bool bUse)
{
	// キー数とループ情報の代入
	m_Key = m_GetKey;
	m_TotalKey = (m_nNumKeyKeep = m_NumKey[MOTIONTYPE]);
	m_bLoopKeep = m_Loop[MOTIONTYPE];

	m_MotionType = m_MotionChange;

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
		if (m_fCounterMotion < m_nFrame[MOTIONTYPE][m_Key])
		{// カウント上限
			if (bUse == false)
			{
				//モーションのカウントアップ
				m_fCounterMotion++;
			}
		}
	}

	// モデルパーツごとのモーションアニメーション
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_Key == m_TotalKey - 1 && m_bLoopKeep == true)
		{// ループして最初に戻るとき
			m_aOffset[nCnt][0] = m_Player3D[MOTIONTYPE][m_Key][nCnt][0] + (m_Player3D[MOTIONTYPE][0][nCnt][0] - m_Player3D[MOTIONTYPE][m_Key][nCnt][0]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][1] = m_Player3D[MOTIONTYPE][m_Key][nCnt][1] + (m_Player3D[MOTIONTYPE][0][nCnt][1] - m_Player3D[MOTIONTYPE][m_Key][nCnt][1]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][2] = m_Player3D[MOTIONTYPE][m_Key][nCnt][2] + (m_Player3D[MOTIONTYPE][0][nCnt][2] - m_Player3D[MOTIONTYPE][m_Key][nCnt][2]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][3] = m_Player3D[MOTIONTYPE][m_Key][nCnt][3] + (m_Player3D[MOTIONTYPE][0][nCnt][3] - m_Player3D[MOTIONTYPE][m_Key][nCnt][3]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][4] = m_Player3D[MOTIONTYPE][m_Key][nCnt][4] + (m_Player3D[MOTIONTYPE][0][nCnt][4] - m_Player3D[MOTIONTYPE][m_Key][nCnt][4]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][5] = m_Player3D[MOTIONTYPE][m_Key][nCnt][5] + (m_Player3D[MOTIONTYPE][0][nCnt][5] - m_Player3D[MOTIONTYPE][m_Key][nCnt][5]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
			m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
			m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
			m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
			m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
			m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
		}
		else if (m_bMotionChange == true)
		{// モーションブレンド
			m_aOffset[nCnt][0] = m_aOffsetMemo[nCnt][0] + (m_Player3D[m_MotionType][0][nCnt][0] - m_aOffsetMemo[nCnt][0]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][1] = m_aOffsetMemo[nCnt][1] + (m_Player3D[m_MotionType][0][nCnt][1] - m_aOffsetMemo[nCnt][1]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][2] = m_aOffsetMemo[nCnt][2] + (m_Player3D[m_MotionType][0][nCnt][2] - m_aOffsetMemo[nCnt][2]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][3] = m_aOffsetMemo[nCnt][3] + (m_Player3D[m_MotionType][0][nCnt][3] - m_aOffsetMemo[nCnt][3]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][4] = m_aOffsetMemo[nCnt][4] + (m_Player3D[m_MotionType][0][nCnt][4] - m_aOffsetMemo[nCnt][4]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][5] = m_aOffsetMemo[nCnt][5] + (m_Player3D[m_MotionType][0][nCnt][5] - m_aOffsetMemo[nCnt][5]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
			m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
			m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
			m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
			m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
			m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
		}
		else
		{// 通常時
			if (m_nNextKey == false)
			{
				m_aOffset[nCnt][0] = m_Player3D[MOTIONTYPE][m_Key][nCnt][0] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][0] - m_Player3D[MOTIONTYPE][m_Key][nCnt][0]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][1] = m_Player3D[MOTIONTYPE][m_Key][nCnt][1] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][1] - m_Player3D[MOTIONTYPE][m_Key][nCnt][1]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][2] = m_Player3D[MOTIONTYPE][m_Key][nCnt][2] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][2] - m_Player3D[MOTIONTYPE][m_Key][nCnt][2]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][3] = m_Player3D[MOTIONTYPE][m_Key][nCnt][3] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][3] - m_Player3D[MOTIONTYPE][m_Key][nCnt][3]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][4] = m_Player3D[MOTIONTYPE][m_Key][nCnt][4] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][4] - m_Player3D[MOTIONTYPE][m_Key][nCnt][4]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][5] = m_Player3D[MOTIONTYPE][m_Key][nCnt][5] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][5] - m_Player3D[MOTIONTYPE][m_Key][nCnt][5]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
				m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
				m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
				m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
				m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
				m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
			}
		}

		if (m_bMotionChange == false)
		{// モーション変化なし
			if (m_fCounterMotion == m_nFrame[MOTIONTYPE][m_Key])
			{// キーフレーム終了時
				if (m_Key != m_TotalKey - 2 || m_bLoopKeep == true)
				{// ジャンプ以外でキー終了時リセット
					if (m_nNextKey == false)
					{
						m_fCounterMotion = 0;
					}
				}

				if (m_Key == m_TotalKey - 1 && m_bLoopKeep == true)
				{// ループするとき
					m_Key = 0;
					m_GetKey = m_Key;
				}
				else if (m_Key == m_TotalKey - 2 && m_bLoopKeep == false)
				{// ループせず終了するとき
					m_fCounterMotion = m_nFrame[MOTIONTYPE][m_Key];
					m_Key++;
					m_GetKey = m_Key;
					m_nNextKey = true;
				}

				else if (m_Key < m_TotalKey && m_nNextKey != true)
				{// キーのカウント
					m_Key++;
					m_GetKey = m_Key;
				}
			}
			else if (m_MotionTypeOld != m_MotionType)
			{// モーション変化時	
				m_Key = 0;
				m_fCounterMotion = 0;
				m_bMotionChange = true;

				if (m_MotionTypeOld == MOTIONTYPE_NEUTRAL && m_MotionType == MOTIONTYPE_MOVE)
				{// 移動からニュートラル時
					m_nBrendFrame = 10;
				}
				if (m_MotionTypeOld == MOTIONTYPE_MOVE && m_MotionType == MOTIONTYPE_NEUTRAL)
				{// ニュートラルから移動時
					m_nBrendFrame = 100;
				}
				if (m_MotionTypeOld == MOTIONTYPE_LEFTMOVE && m_MotionType == MOTIONTYPE_RIGHTMOVE)
				{// ニュートラルから移動時
					m_nBrendFrame = 10;
				}
				if (m_MotionTypeOld == MOTIONTYPE_RIGHTMOVE && m_MotionType == MOTIONTYPE_LEFTMOVE)
				{// ニュートラルから移動時
					m_nBrendFrame = 10;
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
// Get関数
//==============================================================================
D3DXVECTOR3 CPlayer3D::GetPosition(void)
{
	return m_pos;
}

D3DXVECTOR3 CPlayer3D::GetRotation(void)
{
	return m_rot;
}

void CPlayer3D::SetMoveUse(bool bMoveUse)
{
	m_bMoveUse = bMoveUse;
}

