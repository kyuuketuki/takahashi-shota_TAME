//=============================================================================
//
// ナンバー処理 [number.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "number.h"
#include "manager.h"
#include "scene2D.h"

// 静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CNumber::CNumber()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
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
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	// 対角線の設定
	m_fLength = sqrtf(fSizeX * fSizeX + fSizeY * fSizeY);
	m_fAngle = atan2f(fSizeX, fSizeY);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//位置の設定
	//頂点の座標
	pVtx[0].pos.x = (pos.x - sinf(m_fAngle) * m_fLength);
	pVtx[0].pos.y = (pos.y + cosf(m_fAngle) * m_fLength);
	pVtx[0].pos.z = (pos.z = 0.0f);

	pVtx[1].pos.x = (pos.x - sinf(m_fAngle) * m_fLength);
	pVtx[1].pos.y = (pos.y - cosf(m_fAngle) * m_fLength);
	pVtx[1].pos.z = (pos.z = 0.0f);

	pVtx[2].pos.x = (pos.x + sinf(m_fAngle) * m_fLength);
	pVtx[2].pos.y = (pos.y + cosf(m_fAngle) * m_fLength);
	pVtx[2].pos.z = (pos.z = 0.0f);

	pVtx[3].pos.x = (pos.x + sinf(m_fAngle) * m_fLength);
	pVtx[3].pos.y = (pos.y - cosf(m_fAngle) * m_fLength);
	pVtx[3].pos.z = (pos.z = 0.0f);

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

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CNumber::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CNumber::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CNumber::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);
	
	// ポリゴンの描画処理
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 番号に合わせてテクスチャをずらす
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.1f*nNumber, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f*nNumber, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f*(nNumber + 1), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f*(nNumber + 1), 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 番号ポリゴンの生成
//=============================================================================
CNumber * CNumber::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ローカル変数宣言
	CNumber *pNumber = NULL;
	pNumber = new CNumber;

	// 初期化
	if (pNumber != NULL)
	{
		pNumber->Init(pos, fSizeX, fSizeY);
	}

	return pNumber;
}

//=============================================================================
// テクスチャを読み込む
//=============================================================================
HRESULT CNumber::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.jpg", &m_pTexture);
	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CNumber::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}