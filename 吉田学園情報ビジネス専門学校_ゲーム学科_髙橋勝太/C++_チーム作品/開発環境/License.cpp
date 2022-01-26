//=============================================================================
//
// 免許処理 [License.cpp]
// Author : koseki yuto
//
//=============================================================================

#include "manager.h"
#include "game.h"
#include "player3D.h"
#include "License.h"

// 静的メンバ変数
LPDIRECT3DTEXTURE9 CLicense::m_pTexture = NULL;
CLicense::LISENCETYPE CLicense::m_nLicenseType = LISENCETYPE_GOLD;

//=============================================================================
// コンストラクタ
//=============================================================================
CLicense::CLicense(int nPriority, int nType) : CScene2D(nPriority, nType)
{
	//m_nLicenseType = LISENCETYPE_GOLD;
}

//=============================================================================
// デストラクタ
//=============================================================================
CLicense::~CLicense()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CLicense *CLicense::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ローカル変数宣言
	CLicense *pLicense = NULL;
	pLicense = new CLicense;

	// 初期化
	if (pLicense != NULL)
	{
		pLicense->Init(pos, fSizeX, fSizeY);
	}

	// テクスチャ割り当て
	pLicense->BindTexture(m_pTexture);

	return pLicense;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLicense::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// CScene2D初期化
	CScene2D::Init(pos, fSizeX, fSizeY);

	//ランキング使用処理
	if (m_nLicenseType == LISENCETYPE_NORMAL)
	{
		CScene2D::SetTex(1.0f, 0.5f, 0);
	}
	if (m_nLicenseType == LISENCETYPE_GOLD)
	{
		CScene2D::SetTex(1.0f, 0.5f, 1);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLicense::Uninit(void)
{
	// CScene2D終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLicense::Update(void)
{
	CPlayer3D *pPlayer3D = CGame::GetPlayer3D();

	if (pPlayer3D->GetMotionType() == MOTIONTYPE_FALLMOVE)
	{
		CScene2D::SetTex(1.0f, 0.5f, 0);

		m_nLicenseType = LISENCETYPE_NORMAL;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CLicense::Draw(void)
{
	// CScene2D描画
	CScene2D::Draw();
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CLicense::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/License.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//テクスチャアンロード
//=============================================================================
void CLicense::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ライセンスタイプ
//=============================================================================
void CLicense::SetLisenceType(LISENCETYPE lisencetype)
{
	//引数からタイプを持ってくる
	m_nLicenseType = lisencetype;
}

