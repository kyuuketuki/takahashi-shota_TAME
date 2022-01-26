//=============================================================================
//
// エフェクト3D処理[effect3D.cpp]
// Author; miura yudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "effect.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "player3D.h"

//=============================================================================
//コンストラクタ
//=============================================================================
CEffect3D::CEffect3D()
{
	//メンバ変数初期化
	m_alpha = 1.0f;
}

//=============================================================================
//デストラクタ
//=============================================================================
CEffect3D::~CEffect3D()
{
}

//=============================================================================
//エフェクトの初期化処理
//=============================================================================
HRESULT CEffect3D::Init(D3DXVECTOR3 pos, float fWidth, float fHight)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/smoke.png", &m_pTexture);

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

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//位置情報の代入
	m_pos = pos;
	m_nLife = 15 + rand() % 25;	//寿命をランダム

	return S_OK;
}

//=============================================================================
//エフェクトの終了処理
//=============================================================================
void CEffect3D::Uninit(void)
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
//エフェクトの更新処理
//=============================================================================
void CEffect3D::Update(void)
{
	//ポリゴン3Dのポインタ
	VERTEX_3D *pVtx;

	//カラー情報設定
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha);
	m_alpha -= 0.05f;

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//計算処理
	m_fAngle = (float)(rand() % 628 - 314) / 100.0f;	//爆発の範囲
	m_move.x = sinf(m_fAngle) * 2.5f;					//x軸の弾の速度
	m_move.y = 3.5f;									//y軸の弾の速度
	m_move.z = cosf(m_fAngle) * 2.5f;					//z軸の弾の速度

	//移動量の更新
	m_pos += m_move;

	//寿命チェック
	m_nLife--;

	if (m_nLife == 0)//寿命が0になったら
	{
		Uninit();//使用しない
	}
}

//=============================================================================
//エフェクトの描画処理
//=============================================================================
void CEffect3D::Draw(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//ビューマトリックス取得用
	D3DXMATRIX mtxView;

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//レンダーステートの設定(加算合成）
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける(ビルボード)
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);	//逆行列を求める
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//レンダーステートの設定(元に戻す）
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
//エフェクトの作成処理
//=============================================================================
CEffect3D * CEffect3D::Create(D3DXVECTOR3 pos, float fWidth, float fHight)
{
	//エフェクト3D生成d
	CEffect3D *pEffect = NULL;
	pEffect = new CEffect3D;

	//NULLチェック
	if (pEffect != NULL)
	{
		//初期化呼び出し
		pEffect->Init(pos, fWidth, fHight);
	}

	return pEffect;
}

//=============================================================================
//エフェクトのテクスチャ処理
//=============================================================================
void CEffect3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	//引数からの受け渡し
	m_pTexture = pTexture;
}
//=============================================================================
//エフェクトの位置処理
//=============================================================================
void CEffect3D::SetPosition(D3DXVECTOR3 pos)
{
	//引数からの受け渡し
	m_pos = pos;
}
