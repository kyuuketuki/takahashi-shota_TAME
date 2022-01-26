//=============================================================================
//
//フェード処理[fade.cpp]
// Author; takahashi syota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "fade.h"
#include "Renderer.h"
#include "manager.h"
#include "input.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
CFade::FADE CFade::m_fade = FADE_IN;							//状態をノーマル
CManager::MODE CFade::m_modenext = CManager::MODE_TITLE;		//初期モード

//=============================================================================
//コンストラクタ
//=============================================================================
CFade::CFade(int nPriority, int nType) : CScene(nPriority, nType)
{

}

//=============================================================================
//デストラクタ
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
//フェードの設定・処理関数
//=============================================================================
CFade * CFade::Create(CManager::MODE modenext)
{
	//インスタンス生成
	CFade *pfade = NULL;
	pfade = new CFade;

	//初期化呼び出し
	pfade->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH);

	//遷移先を知らせる
	m_modenext = modenext;

	return pfade;
}
//=============================================================================
//フェードの初期化処理
//=============================================================================
HRESULT CFade::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	m_fade = FADE_IN;//フェードの状態
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);//フェード色

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//位置情報の代入
	m_pos = D3DXVECTOR3(pos);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0, fHight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(fWidth, fHight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fWidth, 0, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//フェードの終了処理
//=============================================================================
void CFade::Uninit(void)
{
	// オブジェクト破棄
	Release();
}

//=============================================================================
//フェードの更新処理
//=============================================================================
void CFade::Update(void)
{
	//構造体のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//フェードインの時
	if (m_fade == FADE_IN)
	{
		m_col.a -= 0.01f;
		if (m_col.a <= 0.0f)
		{
			m_col.a = 0.0f;
			m_fade = FADE_NONE;
		}
	}
	//フェードアウトの時
	if (m_fade == FADE_OUT)
	{
		m_col.a += 0.01f;
		if (m_col.a >= 1.0f)
		{
			m_col.a = 1.0f;
			m_fade = FADE_IN;
			CManager::SetMode(m_modenext);
		}
	}

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
//フェードの描画処理
//=============================================================================
void CFade::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
}

//=============================================================================
//フェードのセット関数
//=============================================================================
void CFade::SetFade(FADE fade, CManager::MODE modeNext)
{
	m_fade = fade;
	m_modenext = modeNext;
}
