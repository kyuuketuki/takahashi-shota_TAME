//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : Miura　Yudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "particle.h"
#include "manager.h"
#include "scene3D.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CParticle::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CParticle::CParticle(int nPriority, int nType)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos, float fX, float fY, float fSpeed, D3DXCOLOR col, int nLife, float fGravity)
{
	//位置情報の受け渡し
	m_pos = pos;

	//関数呼び出し
	SetParticle(pos, fX, fY, fSpeed, col, nLife, fGravity);

	//初期化処理
	CScene3D::Init(pos, m_SizeY, m_SizeX);

	//テクスチャ情報
	SetTex();

	//計算処理
	m_fAngle = (float)(rand() % 628 - 314) / D3DX_PI - 100.0f;//爆発の範囲
	m_move.x = cosf(m_fAngle / D3DX_PI) * m_fSpeed;//x軸の弾の速度
	m_move.y = cosf(m_fAngle * D3DX_PI) * m_fSpeed;//y軸の弾の速度
	m_move.z = sinf(m_fAngle) * m_fSpeed;          //z軸の弾の速度

												   //情報受け渡し
	m_nLifeGravity = m_nLife;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CParticle::Update(void)
{
	//位置更新
	m_pos += m_move;

	//カラー変更
	SetCol(m_col);

	//アルファ値を下げていく
	m_col.a -= 0.01f;

	//ライフの半分まで来たら重力をつける
	if (m_nLife <= (m_nLifeGravity / 1.25f))
	{
		//重力発生
		m_move.y -= m_fGravity;
	}

	//ライフ処理
	m_nLife--;
	if (m_nLife == 0)//寿命が0になったら
	{
		//消去処理
		Uninit();
	}

	//位置更新の関数呼び出し
	SetPosition(m_pos);
}

//=============================================================================
// 描画処理
//=============================================================================
void CParticle::Draw(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//レンダーステートの設定(加算合成）
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//描画処理
	CScene3D::Draw();

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//レンダーステートの設定(元に戻す）
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 生成処理
//=============================================================================
CParticle * CParticle::Create(D3DXVECTOR3 pos, float fX, float fY, float fSpeed, D3DXCOLOR col, int nLife, float fGravity)
{
	//モデルを生成
	CParticle *pParticle = NULL;
	pParticle = new CParticle;

	//NULLチェック
	if (pParticle != NULL)
	{
		//オブジェクトの初期化処理
		pParticle->Init(pos, fX, fY, fSpeed, col, nLife, fGravity);

		//テクスチャ処理
		pParticle->SetTexture(m_pTexture);
	}

	return pParticle;
}
//=============================================================================
// テクスチャロード処理
//=============================================================================
void CParticle::Load(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture);
}

//=============================================================================
// 代入用関数処理
//=============================================================================
void CParticle::SetParticle(D3DXVECTOR3 pos, float fX, float fY, float fSpeed, D3DXCOLOR col, int nLife, float fGravity)
{
	//位置情報
	m_pos = pos;

	//Xのサイズ情報
	m_SizeX = fX;

	//Yのサイズ情報
	m_SizeY = fY;

	//移動量情報
	m_fSpeed = fSpeed;

	//カラー情報
	m_col = col;

	//寿命
	m_nLife = nLife;

	//重力
	m_fGravity = fGravity;
}
