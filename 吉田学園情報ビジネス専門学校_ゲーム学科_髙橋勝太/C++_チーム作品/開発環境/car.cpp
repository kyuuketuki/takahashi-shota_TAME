//=============================================================================
//
// 車処理 [car.cpp]
// Author : Hiroya Katano
//
//=============================================================================
#include "car.h"
#include "manager.h"
#include "player3D.h"
#include "input.h"
#include "camera.h"
#include "AIguide.h"
#include "trafficLight.h"
#include "player3D.h"
#include "scene3D.h"

//静的メンバ変数
D3DXVECTOR3		CCar::m_rotStatic = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3		CCar::m_moveStatic = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXMATRIX      CCar::m_mtxWorld;
bool			CCar::m_bPlayCar;

//グローバル変数
int			g_nTypeObjectX = -1;
int			g_nCarNumber = 0;

//==============================================================================
// コンストラクタ
//==============================================================================
CCar::CCar(int nPriority, int nType)
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CCar::~CCar()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CCar::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//Xファイルの読み込み
	if (m_CarType == CARTYPE_NEUTRAL)
	{
		D3DXLoadMeshFromX("data/MODEL/New-Traffic-light.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);
	}
	if (m_CarType == CARTYPE_NORMAL)
	{
		D3DXLoadMeshFromX("data/MODEL/car001.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);
	}
	if (m_CarType == CARTYPE_SUPER)
	{
		D3DXLoadMeshFromX("data/MODEL/car002.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);
	}
	if (m_CarType == CARTYPE_TRUCK)
	{
		D3DXLoadMeshFromX("data/MODEL/truck_blank001.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);
	}
	if (m_CarType == CARTYPE_POLICE)
	{
		D3DXLoadMeshFromX("data/MODEL/PoliceCar.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);
	}

	//位置
	m_pos = pos;
	m_pos.y = -20.0f;			//微調整

	//向き
	m_rot = m_rotStatic;
	m_move = m_moveStatic;

	int nNumVtx;				//頂点数
	BYTE *pVtxBuff;				//頂点バッファへのポインタ
	DWORD sizeFVF;				//頂点フォーマットのサイズ
	D3DXMATERIAL *pMat;			// マテリアルへのポインタ

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//テクスチャの割り当て
	if (m_CarType == CARTYPE_SUPER)
	{
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Lamborginhi_base_phong_baseColor.jpeg", &m_pTexture[m_CarType][0]);
	}

	//テクスチャ情報
	if (m_CarType == CARTYPE_TRUCK)
	{
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/GEN4DR_INTERIOR_dif.png", &m_pTexture[m_CarType][0]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/GENERIC_BADGES_DIF.png", &m_pTexture[m_CarType][1]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Numberplates_dif.png", &m_pTexture[m_CarType][2]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UCB_BOTTOM_DIF.pn", &m_pTexture[m_CarType][3]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UCB_GLASS_CLEAN_dif.png", &m_pTexture[m_CarType][4]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/krmlgtbdy85_headlights_dif.png", &m_pTexture[m_CarType][6]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tier_1o.png", &m_pTexture[m_CarType][7]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wheel_truck_stamp_spec.png", &m_pTexture[m_CarType][8]);
	}

	// 頂点数を取得
	nNumVtx = m_pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//頂点バッファのロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//頂点位置情報
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		m_vtx = *(D3DXVECTOR3*)pVtxBuff;//頂点座標の代入

		if (m_vtx.x > m_MaxSize.x)
		{
			m_MaxSize.x = m_vtx.x;
		}

		if (m_vtx.x < m_MinSize.x)
		{
			m_MinSize.x = m_vtx.x;
		}

		if (m_vtx.y > m_MaxSize.y)
		{
			m_MaxSize.y = m_vtx.y;
		}

		if (m_vtx.y < m_MinSize.y)
		{
			m_MinSize.y = m_vtx.y;
		}

		if (m_vtx.z > m_MaxSize.z)
		{
			m_MaxSize.z = m_vtx.z;
		}

		if (m_vtx.z < m_MinSize.z)
		{
			m_MinSize.z = m_vtx.z;
		}

		pVtxBuff += sizeFVF;	//頂点フォーマットのサイズ分ポインタを進める
	}

	//値を代入
	m_posMaxBase = m_MaxSize;
	m_posMinBase = m_MinSize;

	//アンロック
	m_pMesh->UnlockVertexBuffer();

	//初期化
	m_bHoming = false;
	m_fDestAngle = 0.0f;
	m_bPlayCar = false;

	//目的地ガイド
	for (int nCntArea = 0; nCntArea < MAX_GUIDE_AREA; nCntArea++)
	{
		for (int nCntGuide = 0; nCntGuide < MAX_GUIDE_POINT; nCntGuide++)
		{
			m_AIguide[nCntArea][nCntGuide] = CAIguide::GetAIguidePos(nCntArea, nCntGuide);
		}
	}
	for (int nCntArea = 0; nCntArea < MAX_LIGHT_AREA; nCntArea++)
	{
		for (int nCntLight = 0; nCntLight < MAX_LIGHT_POINT; nCntLight++)
		{
			m_Light[nCntArea][nCntLight] = &CGame::GetTrafficLight(nCntArea*nCntLight + nCntLight)->m_TrafficLight[nCntArea][nCntLight].pos;
		}
	}

	//初期化
	m_nNowGuide = 0;
	m_nNowLight = 0;
	m_bSignalRed = false;
	m_bPlayerVio = false;
	m_bHearCar = false;
	m_bHearSiren = false;

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CCar::Uninit(void)
{
	//メッシュの開放
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//マテリアルの開放
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//テクスチャの破棄
	for (int n1 = 0; n1 < 5; n1++)
	{
		for (int n2 = 0; n2 < 64; n2++)
		{
			if (m_pTexture[n1][n2] != NULL)
			{
				m_pTexture[n1][n2]->Release();
				m_pTexture[n1][n2] = NULL;
			}
		}
	}

	//オブジェクトの破棄
	Release();
}

//==============================================================================
// 更新処理
//==============================================================================
void CCar::Update(void)
{
	//カメラ角度取得
	D3DXVECTOR3 RotationCamera = CManager::GetCamera()->GetRot();
	float RotCameraY = RotationCamera.y;

	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	float fCarSpeed;

	//タイプ別に更新処理
	switch (m_CarType)
	{
	case CARTYPE_SUPER:
		fCarSpeed = AUTO_SUPERCAR_SPD;
		break;
	case CARTYPE_TRUCK:
		fCarSpeed = AUTO_NEUTORALCAR_SPD;
		break;
	}

	if (pInputKeyboard->GetTrigger(DIK_LSHIFT) == true && m_nTypeX == 0)
	{
		// LSHIFTを押した時にプレイヤー操作可能にする(※生成されている車両を1台にする必要がある)
		m_bPlayCar = !m_bPlayCar;
		m_move = D3DXVECTOR3(0.f, 0.f, 0.f);
	}

	// 角度修正
	if (RotCameraY > D3DX_PI) { RotCameraY -= D3DX_PI * 2; }
	if (RotCameraY < -D3DX_PI) { RotCameraY += D3DX_PI * 2; }

	// 位置を移動量分ずらす
	m_pos += m_move;

	// 相対的な向きの計算関数
	CalculationCar();

	//角度計算
	if (m_rot.y - D3DX_PI / 4 < m_fAngleLight && m_rot.y > m_fAngleLight&&m_fDisLightZX < 3000.f)
	{
		m_bSignalRed = CGame::GetTrafficLight(m_nTypeX*m_nNowLight + m_nNowLight)->GetSignal(m_nTypeX, m_nNowLight);
	}
	else
	{
		m_bSignalRed = false;
	}

	if (m_fDisLightZX > 2500.f)
	{
		// 車⇔信号機間の距離が2000以下の時
		if (m_nNowLight == MAX_LIGHT_POINT - 1)
		{
			// 配列内最後の信号であった時、配列内最初の信号へ
			m_nNowLight = 0;
		}
		else if (m_nNowLight != MAX_LIGHT_POINT - 1)
		{
			// 配列内の次の信号へ
			m_nNowLight += 1;
		}
	}

	if (m_bPlayCar == false)
	{
		if (m_bSignalRed == true && m_fDisGuideZX < SIGNAL_FOV && m_fDisGuideZX > 50.f)
		{
			// 赤信号かつ車と直近ガイドの距離がSIGNAL_FOV以内の時
			m_move += D3DXVECTOR3((0.f - m_move.x) * 0.01f, 0.f, (0.f - m_move.z) * 0.01f);
		}
		else
		{
			if (m_fDisGuideZX > 50.f && m_bHoming == false)
			{
				// 車⇔ガイド間の距離が50より遠く、ガイドへの追従がOFFの時
				m_bHoming = true;
			}
			else if (m_fDisGuideZX <= 100.f)
			{
				// 車⇔ガイド間の距離が100以下の時
				if (m_nNowGuide == MAX_GUIDE_POINT - 1)
				{
					// 配列内最後のガイドであった時、配列内最初のガイドへ
					m_nNowGuide = 0;
				}
				else if (m_nNowGuide != MAX_GUIDE_POINT - 1)
				{
					// 配列内の次のガイドへ
					m_nNowGuide += 1;
				}
			}

			if (m_bHoming == true)
			{
				// ガイドへの追従がONの時
				if (m_rot.y > m_fAngleGuide)
				{
					// 車の向きが目的の向きより大きい時
					if (m_rot.y >= 0)
					{
						// 目的の向きが0以下 かつ 車の向きが0以上の時
						m_fRot = m_rot.y - m_fAngleGuide;
						m_fRot *= -1;
					}
					else
					{
						// 目的の向きが0より小さい かつ 車の向きが0より小さい時
						m_fRot = m_fAngleGuide - m_rot.y;
					}
				}
				else if (m_rot.y <= m_fAngleGuide)
				{
					// 目的の向きが車の向き以上の時
					m_fRot = m_fAngleGuide - m_rot.y;
				}

				// 回転量が正負どちらかのπを超えた時
				if (m_fRot > D3DX_PI) { m_fRot -= D3DX_PI * 2; }
				if (m_fRot < -D3DX_PI) { m_fRot += D3DX_PI * 2; }

				// 回転量の調整
				m_fRot = m_fRot * 0.1f;

				// 車の向きを回転量分ずらす
				m_rot.y += m_fRot;

				// 車の向きが正負どちらかのπを超えている場合範囲内に修正
				if (m_rot.y > D3DX_PI) { m_rot.y = m_rot.y - D3DX_PI * 2; }
				if (m_rot.y < -D3DX_PI) { m_rot.y = m_rot.y + D3DX_PI * 2; }

				// 車の向きに合わせて移動量を取得
				m_move = D3DXVECTOR3(sinf(m_rot.y) * fCarSpeed, 0.f, cosf(m_rot.y) * fCarSpeed);

				if (m_nTypeX == 1)
				{
					printf("chaos");
				}
			}
		}

		if (m_CarType == CARTYPE_POLICE)
		{
			if (m_rot.y > D3DX_PI * 31 / 32 && m_fAnglePlayer < 0.f)
			{
				m_fAnglePlayer += D3DX_PI * 2;
			}
			else if (m_rot.y < -D3DX_PI * 31 / 32 && m_fAnglePlayer > 0.f)
			{
				m_fAnglePlayer -= D3DX_PI * 2;
			}

			if (m_rot.y - D3DX_PI / 4 < m_fAnglePlayer && m_rot.y + D3DX_PI / 4 > m_fAnglePlayer && m_fDisPlayerZX < 3000.f)
			{
				if (CPlayer3D::GetViolation() == true)
				{
					m_bPlayerVio = true;
				}

				if (m_bPlayerVio == true)
				{
					m_rot.y = m_fAnglePlayer;
					m_move = D3DXVECTOR3(sinf(m_rot.y) * fCarSpeed, 0.f, cosf(m_rot.y) * fCarSpeed);
				}
			}

			else
			{
				if (m_bPlayerVio == true && m_fDisPlayerZX > 100000.f)
				{
					m_bPlayerVio = false;
					float fTemporaryDistance = 9999999.f;	// 仮の値(次に入ると考えられる値より大きい値に)

					for (int nCntGuide = 0; nCntGuide < MAX_GUIDE_POINT; nCntGuide++)
					{
						m_fVecGuideX = m_AIguide[m_nTypeX][nCntGuide]->x - m_pos.x;								// ガイドと車のX座標の差
						m_fVecGuideZ = m_AIguide[m_nTypeX][nCntGuide]->z - m_pos.z;								// ガイドと車のZ座標の差
						m_fDisGuideZX = sqrtf((m_fVecGuideX * m_fVecGuideX) + (m_fVecGuideZ * m_fVecGuideZ));	// ガイドと車の距離
						m_fAngleGuide = atan2f(m_fVecGuideX, m_fVecGuideZ);										// ガイドに対する車の角度
						if (m_fDisGuideZX < fTemporaryDistance)
						{
							fTemporaryDistance = m_fDisGuideZX;
							m_nNowGuide = nCntGuide;
						}
					}

					if (m_nNowGuide > 0)
					{
						m_fVecGuideX = m_AIguide[m_nTypeX][m_nNowGuide - 1]->x - m_pos.x;	// ガイドと車のX座標の差
						m_fVecGuideZ = m_AIguide[m_nTypeX][m_nNowGuide - 1]->z - m_pos.z;	// ガイドと車のZ座標の差
						m_fAngleGuide = atan2f(m_fVecGuideX, m_fVecGuideZ);					// ガイドに対する車の角度

						m_move = D3DXVECTOR3(0.f, 0.f, 0.f);
						m_pos = D3DXVECTOR3(m_AIguide[m_nTypeX][m_nNowGuide - 1]->x, 0.f, m_AIguide[m_nTypeX][m_nNowGuide - 1]->z);
						m_rot.y = m_fAngleGuide;
						m_bSignalRed = false;
					}

					else if (m_nNowGuide == 0)
					{
						m_fVecGuideX = m_AIguide[m_nTypeX][MAX_GUIDE_POINT - 1]->x - m_pos.x;	// ガイドと車のX座標の差
						m_fVecGuideZ = m_AIguide[m_nTypeX][MAX_GUIDE_POINT - 1]->z - m_pos.z;	// ガイドと車のZ座標の差
						m_fAngleGuide = atan2f(m_fVecGuideX, m_fVecGuideZ);					// ガイドに対する車の角度

						m_move = D3DXVECTOR3(0.f, 0.f, 0.f);
						m_pos = D3DXVECTOR3(m_AIguide[m_nTypeX][MAX_GUIDE_POINT - 1]->x, 0.f, m_AIguide[m_nTypeX][MAX_GUIDE_POINT - 1]->z);
						m_rot.y = m_fAngleGuide;
						m_bSignalRed = false;
					}
				}
			}
		}
	}

	else if (m_bPlayCar == true)
	{
		m_rot.y = RotCameraY;

		float fMoveCar = PLAY_CAR_SPD;
		if (pInputKeyboard->GetPress(DIK_COLON) == true)
		{
			fMoveCar = PLAY_CAR_SPDDOWN;
		}

		//移動処理
		//←を押したとき
		if (pInputKeyboard->GetPress(DIK_LEFT) == true)
		{
			//向き
			m_rot = D3DXVECTOR3(0.0f, RotCameraY - (D3DX_PI / 2), 0.0f);

			if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_DOWN) == true)
			{
				m_move += D3DXVECTOR3(-cosf(RotCameraY) * fMoveCar, 0.f, sinf(RotCameraY) * fMoveCar) * 0.71f;
			}
			else
			{
				m_move += D3DXVECTOR3(-cosf(RotCameraY) * fMoveCar, 0.f, sinf(RotCameraY) * fMoveCar);
			}
		}

		//→を押したとき
		if (pInputKeyboard->GetPress(DIK_RIGHT) == true)
		{
			//向き
			m_rot = D3DXVECTOR3(0.0f, RotCameraY + (D3DX_PI / 2), 0.0f);

			if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_DOWN) == true)
			{
				m_move += D3DXVECTOR3(cosf(RotCameraY) * fMoveCar, 0.f, -sinf(RotCameraY) * fMoveCar) * 0.71f;
			}
			else
			{
				m_move += D3DXVECTOR3(cosf(RotCameraY) * fMoveCar, 0.f, -sinf(RotCameraY) * fMoveCar);
			}
		}

		//↑を押したとき
		if (pInputKeyboard->GetPress(DIK_UP) == true)
		{
			//向き
			m_rot = D3DXVECTOR3(0.0f, RotCameraY, 0.0f);

			if (pInputKeyboard->GetPress(DIK_LEFT) == true || pInputKeyboard->GetPress(DIK_RIGHT) == true)
			{
				m_move += D3DXVECTOR3(sinf(RotCameraY) * fMoveCar, 0.f, cosf(RotCameraY) * fMoveCar) * 0.71f;
			}
			else
			{
				m_move += D3DXVECTOR3(sinf(RotCameraY) * fMoveCar, 0.f, cosf(RotCameraY) * fMoveCar);
			}
		}

		//↓を押したとき
		if (pInputKeyboard->GetPress(DIK_DOWN) == true)
		{
			//向き
			m_rot = D3DXVECTOR3(0.0f, RotCameraY - D3DX_PI, 0.0f);

			if (pInputKeyboard->GetPress(DIK_LEFT) == true || pInputKeyboard->GetPress(DIK_RIGHT) == true)
			{
				m_move += D3DXVECTOR3(-sinf(RotCameraY) * fMoveCar, 0.f, -cosf(RotCameraY) * fMoveCar) * 0.71f;
			}
			else
			{
				m_move += D3DXVECTOR3(-sinf(RotCameraY) * fMoveCar, 0.f, -cosf(RotCameraY) * fMoveCar);
			}
		}
		m_move += D3DXVECTOR3((0.f - m_move.x) * 0.1f, 0.f, (0.f - m_move.z) * 0.1f);
	}

	//サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	if (m_fDisPlayerZX < 3000.f && m_bHearCar == false)
	{
		m_bHearCar = true;
		pSound->Play(CSound::SOUND_LABEL_SE_CAR_RUNNING);
	}
	else if (m_fDisPlayerZX > 3000.f && m_bHearCar == true)
	{
		m_bHearCar = false;
		pSound->Stop(CSound::SOUND_LABEL_SE_CAR_RUNNING);
	}

	if (m_bPlayerVio == true && m_bHearSiren == false)
	{
		if (m_bHearCar == true)
		{
			m_bHearCar = false;
			pSound->Stop(CSound::SOUND_LABEL_SE_CAR_RUNNING);
		}
		m_bHearSiren = true;
		pSound->Play(CSound::SOUND_LABEL_SE_POLICE_SIREN);
	}
	else if (m_bPlayerVio == false && m_bHearSiren == true)
	{
		m_bHearSiren = false;
		pSound->Stop(CSound::SOUND_LABEL_SE_POLICE_SIREN);
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void CCar::Draw(void)
{
	//ローカル
	D3DXMATRIX mtxRot, mtxTrans, revmat;	//計算用マトリックス
	D3DMATERIAL9 matDef;					//現在のマテリアル保存
	D3DXMATERIAL *pMat;						//マテリアルへのポインタ

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

	if (m_MaxSize.x < 0.0f)
	{
		m_MaxSize.x *= -1.0f;
	}
	if (m_MaxSize.y < 0.0f)
	{
		m_MaxSize.y *= -1.0f;
	}
	if (m_MaxSize.z < 0.0f)
	{
		m_MaxSize.z *= -1.0f;
	}

	if (m_MinSize.x < 0.0f)
	{
		m_MinSize.x *= -1.0f;
	}
	if (m_MinSize.y < 0.0f)
	{
		m_MinSize.y *= -1.0f;
	}
	if (m_MinSize.z < 0.0f)
	{
		m_MinSize.z *= -1.0f;
	}

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
		if (m_pTexture[m_CarType][nCntMat] != NULL)
		{
			pDevice->SetTexture(0, m_pTexture[m_CarType][nCntMat]);
		}

		else
		{
			pDevice->SetTexture(0, NULL);
		}

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//保存したマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==============================================================================
// 生成処理
//==============================================================================
CCar * CCar::Create(D3DXVECTOR3 pos, int nType, CARTYPE carType)
{
	//モデルを生成
	CCar *pCar = NULL;
	pCar = new CCar;

	//NULLチェック
	if (pCar != NULL)
	{
		pCar->m_nTypeX = nType;

		pCar->m_CarType = carType;

		pCar->Init(pos, 0.0f, 0.0f);
	}

	return pCar;
}

//==============================================================================
// 外積を利用した当たり判定
//==============================================================================
bool CCar::CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	// カメラ情報の取得
	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 rot = pCamera->GetRot();

	bool bLand = false;

	D3DXVECTOR3		g_Vec[4];
	D3DXVECTOR3		g_oldVec[4];
	D3DXVECTOR3		posold;
	float			PlayerVec[4];
	float			oldPlayerVec[4];

	D3DXVECTOR3 pos = *pPos;			// 判定対象の位置
	posold = *pPosOld;					// 判定対象の位置

	//aPosの値設定
	m_aPos[0] = D3DXVECTOR3(m_pos.x + m_MinSize.x, 0.0f, m_pos.z + m_MinSize.z);
	m_aPos[1] = D3DXVECTOR3(m_pos.x + m_MinSize.x, 0.0f, m_pos.z + m_MaxSize.z);
	m_aPos[2] = D3DXVECTOR3(m_pos.x + m_MaxSize.x, 0.0f, m_pos.z + m_MaxSize.z);
	m_aPos[3] = D3DXVECTOR3(m_pos.x + m_MaxSize.x, 0.0f, m_pos.z + m_MinSize.z);

	// 対角線の設定
	float fLength[4];
	fLength[0] = sqrtf(m_MinSize.x * m_MinSize.x + m_MinSize.z * m_MinSize.z);
	fLength[1] = sqrtf(m_MinSize.x * m_MinSize.x + m_MaxSize.z * m_MaxSize.z);
	fLength[2] = sqrtf(m_MaxSize.x * m_MaxSize.x + m_MaxSize.z * m_MaxSize.z);
	fLength[3] = sqrtf(m_MaxSize.x * m_MaxSize.x + m_MinSize.z * m_MinSize.z);

	float fAngle[4];
	fAngle[0] = atan2f(m_MinSize.x, m_MinSize.z);
	fAngle[1] = atan2f(m_MinSize.x, m_MaxSize.z);
	fAngle[2] = atan2f(m_MaxSize.x, m_MaxSize.z);
	fAngle[3] = atan2f(m_MaxSize.x, m_MinSize.z);

	float fRot = m_rot.y;

	//頂点の座標
	m_aPos[0].x = (m_pos.x + sinf(fAngle[0] + fRot) * fLength[0]);
	m_aPos[0].z = (m_pos.z + cosf(fAngle[0] + fRot) * fLength[0]);

	m_aPos[1].x = (m_pos.x + sinf(fAngle[1] - fRot) * fLength[1]);
	m_aPos[1].z = (m_pos.z - cosf(fAngle[1] - fRot) * fLength[1]);

	m_aPos[2].x = (m_pos.x - sinf(fAngle[2] + fRot) * fLength[2]);
	m_aPos[2].z = (m_pos.z - cosf(fAngle[2] + fRot) * fLength[2]);

	m_aPos[3].x = (m_pos.x - sinf(fAngle[3] - fRot) * fLength[3]);
	m_aPos[3].z = (m_pos.z + cosf(fAngle[3] - fRot) * fLength[3]);

	//並び方をただす
	//90ずれている場合
	if (m_aPos[0].x > m_aPos[2].x && m_aPos[0].z < m_aPos[2].z)
	{
		D3DXVECTOR3 vecold;
		vecold = m_aPos[0];

		m_aPos[0] = m_aPos[3];
		m_aPos[3] = m_aPos[2];
		m_aPos[2] = m_aPos[1];
		m_aPos[1] = vecold;
	}

	//180ずれている場合
	else if (m_aPos[0].x < m_aPos[2].x && m_aPos[0].z < m_aPos[2].z)
	{
		D3DXVECTOR3 vecold;
		vecold = m_aPos[0];

		D3DXVECTOR3 vecold2;
		vecold2 = m_aPos[1];

		m_aPos[0] = m_aPos[2];
		m_aPos[2] = vecold;
		m_aPos[1] = m_aPos[3];
		m_aPos[3] = vecold2;
	}

	//270ずれている場合
	else if (m_aPos[0].x < m_aPos[2].x && m_aPos[0].z > m_aPos[2].z)
	{
		D3DXVECTOR3 vecold;
		vecold = m_aPos[0];

		m_aPos[0] = m_aPos[1];
		m_aPos[1] = m_aPos[2];
		m_aPos[2] = m_aPos[3];
		m_aPos[3] = vecold;
	}

	//外積の式
	m_Vec[0] = m_aPos[1] - m_aPos[0];
	m_Vec[1] = m_aPos[2] - m_aPos[1];
	m_Vec[2] = m_aPos[3] - m_aPos[2];
	m_Vec[3] = m_aPos[0] - m_aPos[3];

	for (int nCount = 0; nCount < 4; nCount++)
	{
		//外積の式2
		if (nCount == 0)
		{
			g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPos[nCount];
		}
		else if (nCount == 1)
		{
			g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPos[nCount];
		}
		else if (nCount == 2)
		{
			g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPos[nCount];
		}
		else if (nCount == 3)
		{
			g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPos[nCount];
		}

		PlayerVec[nCount] = (m_Vec[nCount].z * g_Vec[nCount].x) - (m_Vec[nCount].x * g_Vec[nCount].z);
	}

	//上下の当たり判定
	//if (pPos->y <= m_pos.y + m_MaxSize.y - fHeightMin && pPos->y > m_pos.y - m_MinSize.y - fHeightMax)
	{
		if (PlayerVec[0] > 0.0f && PlayerVec[1] > 0.0f && PlayerVec[2] > 0.0f && PlayerVec[3] > 0.0f)
		{
			for (int nCount = 0; nCount < 4; nCount++)
			{
				//外積の式2
				if (nCount == 0)
				{
					g_oldVec[nCount] = posold + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPos[nCount];
				}
				else if (nCount == 1)
				{
					g_oldVec[nCount] = posold + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPos[nCount];
				}
				else if (nCount == 2)
				{
					g_oldVec[nCount] = posold + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPos[nCount];
				}
				else if (nCount == 3)
				{
					g_oldVec[nCount] = posold + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPos[nCount];
				}

				oldPlayerVec[nCount] = (m_Vec[nCount].z * g_oldVec[nCount].x) - (m_Vec[nCount].x * g_oldVec[nCount].z);
			}

			//左右の当たり判定
			if (PlayerVec[1] > 0 && PlayerVec[3] > 0)
			{
				//左
				if (oldPlayerVec[0] < 0 && PlayerVec[0] > 0)
				{
					//2頂点差を計算
					D3DXVECTOR3 a = m_aPos[0] - m_aPos[1];

					//角度を計算
					double nAtan2 = atan2(a.z, a.x);

					if (nAtan2 > 0.0f)						//斜辺の場合
					{
						pPos->z = pPosOld->z;				//1フレーム前の位置に戻す
						pPos->x = pPosOld->x;
						pMove->z = 0.0f;					//移動量をゼロにする
						pMove->x = sinf((float)nAtan2) * 35;
					}

					else if (nAtan2 <= 0.0f)				//斜辺の場合　垂直の場合	
					{
						pPos->z = pPosOld->z;				//1フレーム前の位置に戻す
						pPos->x = pPosOld->x;
						pMove->z = 0.0f;					//移動量をゼロにする
						pMove->x = sinf((float)-nAtan2) * 35;
					}
					bLand = true;
				}

				//右
				else if (oldPlayerVec[2] < 0 && PlayerVec[2] > 0)
				{
					//2頂点差を計算
					D3DXVECTOR3 a = m_aPos[3] - m_aPos[2];

					//角度を計算
					double nAtan2 = atan2(a.z, a.x);

					if (nAtan2 > 0.0f)							//斜辺の場合		
					{
						pPos->z = pPosOld->z;					//1フレーム前の位置に戻す
						pPos->x = pPosOld->x;
						pMove->z = 0.0f;						//移動量をゼロにする
						pMove->x = sinf((float)-nAtan2) * 35;
					}

					else if (nAtan2 <= 0.0f)					//斜辺の場合　垂直の場合	
					{
						pPos->z = pPosOld->z;					//1フレーム前の位置に戻す
						pPos->x = pPosOld->x;
						pMove->z = 0.0f;						//移動量をゼロにする
						pMove->x = sinf((float)nAtan2) * 35;
					}
					bLand = true;
				}
			}

			//前後の当たり判定
			if (PlayerVec[0] > 0 && PlayerVec[2] > 0)
			{
				//後
				if (oldPlayerVec[1] < 0 && PlayerVec[1] > 0)
				{
					//2頂点差を計算
					D3DXVECTOR3 a = m_aPos[2] - m_aPos[1];

					//角度を計算
					double nAtan2 = atan2(a.z, a.x);
					//float nAtan2 = m_rot.y;

					if (nAtan2 > 0.0f)							//斜辺の場合
					{
						pPos->z = pPosOld->z;					//1フレーム前の位置に戻す
						pPos->x = pPosOld->x;
						pMove->x = 0.0f;						//移動量をゼロにする
						pMove->z = cosf((float)nAtan2) * 35;
					}

					else if (nAtan2 <= 0.0f)					//斜辺の場合　垂直の場合	
					{
						pPos->z = pPosOld->z;					//1フレーム前の位置に戻す
						pPos->x = pPosOld->x;
						pMove->x = 0.0f;						//移動量をゼロにする
						pMove->z = cosf((float)-nAtan2) * 35;
					}
					bLand = true;
				}

				//前
				if (oldPlayerVec[3] < 0 && PlayerVec[3] > 0)
				{
					//2頂点差を計算
					D3DXVECTOR3 a = m_aPos[0] - m_aPos[3];

					//角度を計算
					double nAtan2 = atan2(a.z, a.x);

					if (nAtan2 > 0.0f)							//斜辺の場合
					{
						pPos->z = pPosOld->z;					//1フレーム前の位置に戻す
						pPos->x = pPosOld->x;
						pMove->x = 0.0f;						//移動量をゼロにする
						pMove->z = cosf((float)-nAtan2) * 35;
					}

					else if (nAtan2 <= 0.0f)					//斜辺の場合　垂直の場合	
					{
						pPos->z = pPosOld->z;					//1フレーム前の位置に戻す
						pPos->x = pPosOld->x;
						pMove->x = 0.0f;						//移動量をゼロにする
						pMove->z = cosf((float)nAtan2) * 35;
					}
					bLand = true;
				}
			}
		}
	}

	//SE
	if (bLand == true)
	{
		CSound *pSound = CManager::GetSound();

		pSound->Play(CSound::SOUND_LABEL_SE_CAR_HORN);
	}

	return bLand;
}

//==============================================================================
// Getワールドマトリックス
//==============================================================================
D3DXMATRIX CCar::GetMtxWorld(void)
{
	return m_mtxWorld;
}

//==============================================================================
// 各種計算
//==============================================================================
void CCar::CalculationCar(void)
{
	m_fVecGuideX = m_AIguide[m_nTypeX][m_nNowGuide]->x - m_pos.x;							// ガイドと車のX座標の差
	m_fVecGuideZ = m_AIguide[m_nTypeX][m_nNowGuide]->z - m_pos.z;							// ガイドと車のZ座標の差
	m_fDisGuideZX = sqrtf((m_fVecGuideX * m_fVecGuideX) + (m_fVecGuideZ * m_fVecGuideZ));	// ガイドと車の距離
	m_fAngleGuide = atan2f(m_fVecGuideX, m_fVecGuideZ);										// ガイドに対する車の角度


	m_fVecLightX = m_Light[m_nTypeX][m_nNowLight]->x - m_pos.x;								// 信号機と車のX座標の差
	m_fVecLightZ = m_Light[m_nTypeX][m_nNowLight]->z - m_pos.z;								// 信号機と車のZ座標の差
	m_fDisLightZX = sqrtf((m_fVecLightX * m_fVecLightX) + (m_fVecLightZ * m_fVecLightZ));	// 信号機と車の距離
	m_fAngleLight = atan2f(m_fVecLightX, m_fVecLightZ);										// 信号機に対する車の角度

	m_fVecPlayerX = CPlayer3D::GetPosition().x - m_pos.x;										// playerと車のX座標の差
	m_fVecPlayerZ = CPlayer3D::GetPosition().z - m_pos.z;										// playerと車のZ座標の差
	m_fDisPlayerZX = sqrtf((m_fVecPlayerX * m_fVecPlayerX) + (m_fVecPlayerZ * m_fVecPlayerZ));	// playerと車の距離
	m_fAnglePlayer = atan2f(m_fVecPlayerX, m_fVecPlayerZ);										// playerに対する車の角度
}