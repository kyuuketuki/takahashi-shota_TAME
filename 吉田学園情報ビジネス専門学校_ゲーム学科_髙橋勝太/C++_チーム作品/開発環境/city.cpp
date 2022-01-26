//=============================================================================
//
// マップ処理 [city.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "city.h"
#include "manager.h"
#include "player3D.h"

//静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CCity::m_pTexture[54] = {};
char *CCity::m_pTextName;

//=============================================================================
// コンストラクタ
//=============================================================================
CCity::CCity(int nPriority, int nType)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CCity::~CCity()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCity::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//model読み込み
	D3DXLoadMeshFromX(m_pTextName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

	int nNumVtx;			//頂点数
	DWORD sizeFVF;			//頂点フォーマットのサイズ
	D3DXMATERIAL *pMat;		//マテリアルへのポインタ

	//頂点数を取得
	nNumVtx = m_pMesh->GetNumVertices();
	
	m_nNumVtx = nNumVtx;

	//マテリアルデータへのポインタ取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//位置
	m_pos = pos;

	//向き
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCity::Uninit(void)
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
	for (int nCnt = 0; nCnt < 54; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CCity::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CCity::Draw(void)
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

		if (m_pTexture != NULL)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, m_pTexture[nCntMat]);
		}

		else
		{
			//テクスチャの設定
			pDevice->SetTexture(0, NULL);
		}

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//保存したマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// Set位置
//=============================================================================
void CCity::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// Set向き
//=============================================================================
void CCity::SetRevolution(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// 生成処理
//=============================================================================
CCity *CCity::Create(D3DXVECTOR3 pos, char *TextName)
{
	//シーンXを生成
	CCity *pCity = NULL;
	pCity = new CCity;

	m_pTextName = TextName;

	//NULLチェック
	if (pCity != NULL)
	{
		pCity->Init(pos, 0.0f, 0.0f);
	}

	return pCity;
}

//==============================================================================
//テクスチャ設定
//==============================================================================
void CCity::SetTexture(int nNumber, LPDIRECT3DTEXTURE9 pTexture)
{
	//引数からテクスチャ情報を貰ってくる
	m_pTexture[nNumber] = pTexture;
}
