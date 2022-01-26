//=============================================================================
//
// シーン3D処理[Scene3D.cpp]
// Author; takahashi syota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "scene3D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//=============================================================================
//コンストラクタ
//=============================================================================
CScene3D::CScene3D()
{

}

//=============================================================================
//デストラクタ
//=============================================================================
CScene3D::~CScene3D()
{

}

//=============================================================================
//ポリゴン3Dの初期化処理
//=============================================================================
HRESULT CScene3D::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

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

	//カラー情報設定
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y + fHight, m_pos.z);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y + fHight, m_pos.z);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y - fHight, m_pos.z);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y - fHight, m_pos.z);

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

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//位置情報の代入
	m_pos = pos;

	return S_OK;
}

//=============================================================================
//ポリゴン3Dの終了処理
//=============================================================================
void CScene3D::Uninit(void)
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
void CScene3D::Update(void)
{

}

//=============================================================================
//ポリゴン3Dの描画処理
//=============================================================================
void CScene3D::Draw(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//ビューマトリックス取得用
	D3DXMATRIX mtxView;

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);//逆行列を求める
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

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
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
//ポリゴン3Dのクリエイト処理
//=============================================================================
CScene3D * CScene3D::Create(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//シーン3D生成
	CScene3D *pScene3D = NULL;
	pScene3D = new CScene3D;

	//初期化呼び出し
	pScene3D->Init(pos, fHight, fWidth);

	return pScene3D;
}
//=============================================================================
//ポリゴン3Dのテクスチャ処理
//=============================================================================
void CScene3D::SetTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	//テクスチャを引数から代入
	m_pTexture = pTexture;
}

//=============================================================================
//ポリゴン3Dの位置情報の受け渡し
//=============================================================================
void CScene3D::SetPosition(D3DXVECTOR3 pos)
{
	//位置情報を引数から代入
	m_pos = pos;
}

//=============================================================================
//ポリゴン3Dの角度情報の受け渡し
//=============================================================================
void CScene3D::SetRotation(D3DXVECTOR3 rot)
{
	//位置情報を引数から代入
	m_Rot = rot;
}

//=============================================================================
//ポリゴン3Dのカラー情報の受け渡し
//=============================================================================
void CScene3D::SetCol(D3DXCOLOR col)
{
	//ポリゴン3Dのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// アニメーション
//=============================================================================
void CScene3D::SetAnim(float tex, int nCnt, float tx, float ty)
{
	//構造体のポインタ変数
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(tex * nCnt, tx);
	pVtx[1].tex = D3DXVECTOR2(tex + (tex * nCnt), tx);
	pVtx[2].tex = D3DXVECTOR2(tex * nCnt, ty);
	pVtx[3].tex = D3DXVECTOR2(tex + (tex * nCnt), ty);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// テクスチャ設定
//=============================================================================
void CScene3D::SetTex(void)
{
	//構造体のポインタ変数
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
