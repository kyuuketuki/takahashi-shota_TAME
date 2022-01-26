//=============================================================================
//
// シーン3D処理[Visualization.cpp]
// Author; takahashi syota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "visualization.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "game.h"

//グローバル変数宣言
D3DXVECTOR3 CVisualization::size[4];
D3DXCOLOR	CVisualization::m_col;
bool		CVisualization::m_bLife;

//=============================================================================
// コンストラクタ
//=============================================================================
CVisualization::CVisualization()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CVisualization::~CVisualization()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CVisualization::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/noname.png", &m_pTexture);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	//ポリゴン3Dのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(size[0].x, 100.0f, size[0].z);
	pVtx[1].pos = D3DXVECTOR3(size[1].x, 100.0f, size[1].z);
	pVtx[2].pos = D3DXVECTOR3(size[2].x, 100.0f, size[2].z);
	pVtx[3].pos = D3DXVECTOR3(size[3].x, 100.0f, size[3].z);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	m_nLife = 10;

	return S_OK;
}

//=============================================================================
//ポリゴン3Dの終了処理
//=============================================================================
void CVisualization::Uninit(void)
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	//頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
//ポリゴン3Dの更新処理
//=============================================================================
void CVisualization::Update(void)
{
	if (m_bLife == true)
	{
		m_nLife--;

		if (m_nLife <= 0)
		{
			Uninit();
		}
	}
}

//=============================================================================
//ポリゴン3Dの描画処理
//=============================================================================
void CVisualization::Draw(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//可視化スイッチ
	if (CGame::GetSwitchScene3D() == false)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CVisualization * CVisualization::Create(D3DXVECTOR3 pos, D3DXVECTOR3 fMaxSizeX, D3DXVECTOR3 fMinSizeX, D3DXVECTOR3 fMaxSizeY, D3DXVECTOR3 fMinSizeY, bool bLife, D3DXCOLOR col)
{
	//シーン3D生成
	CVisualization *pVisualization = NULL;
	pVisualization = new CVisualization;

	//NULLチェック
	if (pVisualization == NULL)
	{
		//静的メンバ変数に代入
		size[0] = fMaxSizeX;
		size[1] = fMinSizeX;
		size[2] = fMaxSizeY;
		size[3] = fMinSizeY;

		m_bLife = bLife;
		m_col = col;

		//初期化呼び出し
		pVisualization->Init(pos, 30.0f, 30.0f);
	}

	return pVisualization;
}
