//=============================================================================
//
// シーン2D処理 [scene.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "scene2d.h"
#include "renderer.h"
#include "manager.h"
#include "sceneX.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CScene2D::CScene2D(int nPriority, int nType) : CScene(nPriority, nType)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * 2,						// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuffPolygon,
		NULL)))
	{
		return E_FAIL;
	}

	//ポリゴンの位置
	m_pos = pos;

	//サイズを情報
	CScene2D::SetSize(fSizeX, fSizeY);
	
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//位置の設定
	//頂点の座標
	pVtx[0].pos.x = (m_pos.x - sinf(m_fAngle) * m_fLength);
	pVtx[0].pos.y = (m_pos.y + cosf(m_fAngle) * m_fLength);
	pVtx[0].pos.z = (m_pos.z = 0.0f);

	pVtx[1].pos.x = (m_pos.x - sinf(m_fAngle) * m_fLength);
	pVtx[1].pos.y = (m_pos.y - cosf(m_fAngle) * m_fLength);
	pVtx[1].pos.z = (m_pos.z = 0.0f);

	pVtx[2].pos.x = (m_pos.x + sinf(m_fAngle) * m_fLength);
	pVtx[2].pos.y = (m_pos.y + cosf(m_fAngle) * m_fLength);
	pVtx[2].pos.z = (m_pos.z = 0.0f);

	pVtx[3].pos.x = (m_pos.x + sinf(m_fAngle) * m_fLength);
	pVtx[3].pos.y = (m_pos.y - cosf(m_fAngle) * m_fLength);
	pVtx[3].pos.z = (m_pos.z = 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuffPolygon->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScene2D::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuffPolygon != NULL)
	{
		m_pVtxBuffPolygon->Release();
		m_pVtxBuffPolygon = NULL;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScene2D::Update(void)
{
	VERTEX_2D *pVtx;

	// 頂点情報の更新
	if (m_pVtxBuffPolygon != NULL)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

		//位置更新
		m_pos.x += m_move.x;
		m_pos.y += m_move.y;
		m_rot.z += m_move.z;

		//移動量の減衰
		m_move.x += (0 - m_move.x) * 0.2f;
		m_move.y += (0 - m_move.y) * 0.2f;
		m_move.z += (0 - m_move.z) * 0.5f;

		//頂点の座標
		pVtx[0].pos.x = (m_pos.x - sinf(m_fAngle + m_rot.z) * m_fLength);
		pVtx[0].pos.y = (m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength);
		pVtx[0].pos.z = (m_pos.z = 0.0f);

		pVtx[1].pos.x = (m_pos.x - sinf(m_fAngle - m_rot.z) * m_fLength);
		pVtx[1].pos.y = (m_pos.y - cosf(m_fAngle - m_rot.z) * m_fLength);
		pVtx[1].pos.z = (m_pos.z = 0.0f);

		pVtx[2].pos.x = (m_pos.x + sinf(m_fAngle - m_rot.z) * m_fLength);
		pVtx[2].pos.y = (m_pos.y + cosf(m_fAngle - m_rot.z) * m_fLength);
		pVtx[2].pos.z = (m_pos.z = 0.0f);

		pVtx[3].pos.x = (m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength);
		pVtx[3].pos.y = (m_pos.y - cosf(m_fAngle + m_rot.z) * m_fLength);
		pVtx[3].pos.z = (m_pos.z = 0.0f);

		// 頂点バッファをアンロックする
		m_pVtxBuffPolygon->Unlock();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	////アルファテスト
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, 0);
	
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画処理
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	////アルファテスト
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
}

//=============================================================================
// 更新された位置へのポインタ
//=============================================================================
void CScene2D::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// ポリゴンの回転設定
//=============================================================================
void CScene2D::SetRevolution(float nRot)
{
	m_rot.z = nRot;
}

//=============================================================================
// テクスチャへのポインタ
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// テクスチャ　アニメーション
//=============================================================================
void CScene2D::SetTex(float fPageX, float fPageY, int nAnime)
{
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f + (fPageX * nAnime), fPageY + (fPageY * nAnime));
	pVtx[1].tex = D3DXVECTOR2(0.0f + (fPageX * nAnime), (fPageY * nAnime));
	pVtx[2].tex = D3DXVECTOR2(fPageX + (fPageX * nAnime), fPageY + (fPageY * nAnime));
	pVtx[3].tex = D3DXVECTOR2(fPageX + (fPageX * nAnime), (fPageY * nAnime));

	// 頂点バッファをアンロックする
	m_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// アニメーションXY計算
//=============================================================================
void CScene2D::SetAnime(float fPageX, float fPageY, int nX, int nY, int nAnime)
{
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//Xの計算
	if ((nX - 1) == nAnime)
	{
		m_nCountX += nX;
		
	}

	m_nAnimeX = nAnime - m_nCountX;

	//Yの計算
	if (nX != 0)
	{
		if (nAnime % nX == 0)
		{
			m_nCountY++;
		}
	}

	m_nAnimeY = m_nCountY;

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f + (fPageX * m_nAnimeX), fPageY + (fPageY * m_nAnimeY));
	pVtx[1].tex = D3DXVECTOR2(0.0f + (fPageX * m_nAnimeX), 0.0f + (fPageY * m_nAnimeY));
	pVtx[2].tex = D3DXVECTOR2(fPageX + (fPageX * m_nAnimeX), fPageY + (fPageY * m_nAnimeY));
	pVtx[3].tex = D3DXVECTOR2(fPageX + (fPageX * m_nAnimeX), 0.0f + (fPageY * m_nAnimeY));

	// 頂点バッファをアンロックする
	m_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// 背景のアニメーション
//=============================================================================
void CScene2D::SetBg(float nAnime)
{
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, nAnime + 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, nAnime);
	pVtx[2].tex = D3DXVECTOR2(1.0f, nAnime + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, nAnime);

	// 頂点バッファをアンロックする
	m_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// ポリゴンの透明度(effect)
//=============================================================================
void CScene2D::SetEffect(D3DXCOLOR col,float nData)
{
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);

	// 頂点バッファをアンロックする
	m_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// ポリゴンのカラー設定
//=============================================================================
void CScene2D::SetCollar(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロックする
	m_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// ポリゴンのサイズ設定
//=============================================================================
void CScene2D::SetSize(float fSizeX, float fSizeY)
{
	// 対角線の設定
	m_fLength = sqrtf(fSizeX * fSizeX + fSizeY * fSizeY);
	m_fAngle = atan2f(fSizeX, fSizeY);

	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos.x = (m_pos.x - sinf(m_fAngle) * m_fLength);
	pVtx[0].pos.y = (m_pos.y + cosf(m_fAngle) * m_fLength);
	pVtx[0].pos.z = (m_pos.z = 0.0f);

	pVtx[1].pos.x = (m_pos.x - sinf(m_fAngle) * m_fLength);
	pVtx[1].pos.y = (m_pos.y - cosf(m_fAngle) * m_fLength);
	pVtx[1].pos.z = (m_pos.z = 0.0f);

	pVtx[2].pos.x = (m_pos.x + sinf(m_fAngle) * m_fLength);
	pVtx[2].pos.y = (m_pos.y + cosf(m_fAngle) * m_fLength);
	pVtx[2].pos.z = (m_pos.z = 0.0f);

	pVtx[3].pos.x = (m_pos.x + sinf(m_fAngle) * m_fLength);
	pVtx[3].pos.y = (m_pos.y - cosf(m_fAngle) * m_fLength);
	pVtx[3].pos.z = (m_pos.z = 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuffPolygon->Unlock();
}
