//=============================================================================
//
// モデル処理 [model.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "model.h"
#include "manager.h"
#include "player3D.h"
#include "scene.h"

//グローバル変数宣言
int g_nCntModel = 0;

//==============================================================================
// コンストラクタ
//==============================================================================
CModel::CModel(int nPriority, int nType)
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CModel::~CModel()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CModel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pTextName)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//Ｘファイルの読み込み
	D3DXLoadMeshFromX(pTextName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

	//位置
	m_pos = pos;

	//向き
	m_rot = rot;

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CModel::Uninit(void)
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
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}


//==============================================================================
// 更新処理
//==============================================================================
void CModel::Update(void)
{

}

//==============================================================================
// 描画処理
//==============================================================================
void CModel::Draw(void)
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

	//現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタ取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// 親のマトリックス
	D3DXMATRIX mtxParent;

	//親が存在する場合
	if (m_pParent != NULL)
	{
		//親のマトリックスを取得
		mtxParent = m_pParent->GetMtxWorld();
	}

	//親が存在しない場合
	else
	{
		//現在（最新）のマトリックスを取得する
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// 算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//保存したマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==============================================================================
// 生成
//==============================================================================
CModel * CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pTextName)
{
	//モデルを生成
	CModel *pModel = NULL;
	pModel = new CModel;

	//NULLチェック
	if (pModel != NULL)
	{
		pModel->Init(pos, rot, pTextName);
	}

	return pModel;
}

//=============================================================================
// Get関数　モーションで更新　位置
//=============================================================================
void CModel::MotionPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// Get関数　モーションで更新　向き
//=============================================================================
void CModel::MotionRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// テクスチャへのポインタ
//=============================================================================
void CModel::SetTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	//引数からテクスチャ情報を貰ってくる
	m_pTexture = pTexture;
}

//==============================================================================
// 親の設定
//==============================================================================
void CModel::SetParent(CModel * pModel)
{
	m_pParent = pModel;
}

//==============================================================================
// ワールドマトリックスの受け渡し
//==============================================================================
D3DXMATRIX CModel::GetMtxWorld(void)
{
	return m_mtxWorld;
}
