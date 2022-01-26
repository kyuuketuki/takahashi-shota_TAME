//=============================================================================
//
// アニメーション処理 [animasion.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "animation.h"
#include "manager.h"
#include "logo.h"

// 静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CAnimasion::m_pTexture[MAX_ANIME] = {};
int CAnimasion::m_nUninitType = NULL;

//グローバル変数宣言
int g_nUninitType;

//==============================================================================
// コンストラクタ
//==============================================================================
CAnimasion::CAnimasion(int nPriority, int nType) : CScene2D(nPriority, nType)
{
	//初期化
	m_nAnime = 0;
	m_nPatternAnime = 0;
	g_nUninitType = -1;
}

//==============================================================================
// デストラクタ
//==============================================================================
CAnimasion::~CAnimasion()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CAnimasion::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nAnime, int nType)
{
	CScene2D::Init(pos, fSizeX, fSizeY);

	m_pos = pos;
	m_nType = nType;

	//アニメーションの初期化
	CScene2D::SetTex(1, 0, 0);

	// メンバ変数に代入
	m_nAnime = nAnime;

	// 画像の分割数設定
	//パトランプ
	if (nType == 0)
	{
		m_fAnimeX = 0.2f;
		m_fAnimeY = 0.5f;
		m_nX = 5;
		m_nY = 2;
		m_fCriss = 1;
	}

	//タイトル背景
	if (nType == 1)
	{
		m_fAnimeX = 0.25f;
		m_fAnimeY = 0.33333333f;
		m_nX = 4;
		m_nY = 3;
		m_fCriss = 1;
	}

	//ゲーム開始カウントダウン
	if (nType == 2)
	{
		m_fAnimeX = 0.16666666f;
		m_fAnimeY = 1.0f;
		m_nX = 6;
		m_nY = 1;
		m_fCriss = 0;
	}

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CAnimasion::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// 更新処理
//==============================================================================
void CAnimasion::Update(void)
{
	// 途中変更Uninit
	if (m_nType == g_nUninitType)
	{
		m_fCriss = 0;
		m_nAnime = 1;
	}

	// アニメーションの更新条件
	if ((m_nCounterAnime % m_nAnime) == 0)
	{
		// アニメーションパターンの更新
		m_nPatternAnime = (m_nPatternAnime + 1);

		// アニメーションの更新
		CScene2D::SetAnime(m_fAnimeX, m_fAnimeY, m_nX, m_nY, m_nPatternAnime);
	}

	// カウンターを進める
	m_nCounterAnime++;

	if (m_nType == 2)
	{
		//D3DX_PI/ m_nAnime;
		CScene2D::SetRevolution((D3DX_PI / m_nAnime) * m_nCounterAnime * 2);
	}

	// 更新
	CScene2D::Update();

	// アニメーションを終了リセット
	if (m_nPatternAnime == m_nX * m_nY)
	{
		// カウンターのリセット
		m_nPatternAnime = 0;

		// ループさせないとき
		if (m_fCriss == 0)
		{
			// 終了される
			Uninit();

			if (m_nType == 2)
			{
				CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 160, 80, START);
			}
		}
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void CAnimasion::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//// アルファテスト
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, 0);

	// 描画処理
	CScene2D::Draw();

	//// アルファテスト
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
}

//==============================================================================
// 生成処理
//==============================================================================
CAnimasion * CAnimasion::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nAnime, int nType)
{
	//　アニメーションを生成
	CAnimasion *pAnimasion = NULL;
	pAnimasion = new CAnimasion;

	// NULLチェック
	if (pAnimasion != NULL)
	{
		pAnimasion->Init(pos, fSizeX, fSizeY, nAnime, nType);
	}

	//　テクスチャを入れる
	pAnimasion->BindTexture(m_pTexture[nType]);

	return pAnimasion;
}

//==============================================================================
// テクスチャの読み込み
//==============================================================================
HRESULT CAnimasion::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ptranp.png", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg_move.png", &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/countdown.jpg", &m_pTexture[2]);

	return S_OK;
}

//==============================================================================
// テクスチャの破棄
//==============================================================================
void CAnimasion::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_ANIME; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//==============================================================================
// 破棄するタイプ
//==============================================================================
void CAnimasion::UninitType(int nUninitType)
{
	g_nUninitType = nUninitType;
}

//==============================================================================
// 廃棄したグローバルを初期化
//==============================================================================
void CAnimasion::UninitTypeClear(void)
{
	g_nUninitType = 0;
}

//=============================================================================
//
// アニメーション背景処理 [animasionbg.cpp]
// Author : takahashi shota
//
//=============================================================================

//==============================================================================
// コンストラクタ
//==============================================================================
CAnimasionBg::CAnimasionBg(int nPriority, int nType) : CAnimasion(nPriority, nType)
{
	m_nAnime = 0;
	m_nPatternAnime = 0;
}

//==============================================================================
// デストラクタ
//==============================================================================
CAnimasionBg::~CAnimasionBg()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CAnimasionBg::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nAnime, int nType)
{
	CScene2D::Init(pos, fSizeX, fSizeY);

	// メンバ変数に代入
	m_pos = pos;
	m_nType = nType;

	// アニメーションの初期化
	CScene2D::SetTex(1, 0, 0);

	// アニメーションスピード
	m_nAnime = nAnime;

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CAnimasionBg::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// 更新処理
//==============================================================================
void CAnimasionBg::Update(void)
{
	if (m_fColorFa <= 1.0f)
	{
		m_fColorFa += 0.005f;
		CScene2D::SetCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fColorFa));
	}

	// アニメーションの更新条件
	if ((m_nCounterAnime % m_nAnime) == 0)
	{
		// アニメーションパターンの更新
		m_nPatternAnime = (m_nPatternAnime + 1);

		// アニメーションの更新
		CScene2D::SetAnime(m_fAnimeX, m_fAnimeY, m_nX, m_nY, m_nPatternAnime);
	}

	// カウンターを進める
	m_nCounterAnime++;

	// 更新
	CScene2D::Update();

	// アニメーションを終了リセット
	if (m_nPatternAnime == m_nX * m_nY)
	{
		// カウンターのリセット
		m_nPatternAnime = 0;

		// ループさせないとき
		if (m_fCriss == 0)
		{
			// 終了される
			Uninit();
		}
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void CAnimasionBg::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// 生成処理
//==============================================================================
CAnimasionBg * CAnimasionBg::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nAnime, int nType)
{
	// アニメーション背景を生成
	CAnimasionBg *pAnimasionBg = NULL;
	pAnimasionBg = new CAnimasionBg;

	// NULLチェック
	if (pAnimasionBg != NULL)
	{
		pAnimasionBg->Init(pos, fSizeX, fSizeY, nAnime, nType);
	}

	// テクスチャを入れる
	pAnimasionBg->BindTexture(m_pTexture[nType]);

	return pAnimasionBg;
}

//==============================================================================
// 途中破棄処理
//==============================================================================
void CAnimasionBg::UninitTypeClear(void)
{
	for (int nCnt = 0; nCnt < MAX_ANIME; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
