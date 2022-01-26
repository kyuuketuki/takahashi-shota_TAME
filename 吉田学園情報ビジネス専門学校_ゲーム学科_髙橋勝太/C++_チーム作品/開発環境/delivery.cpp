//=============================================================================
//
// 荷物を届ける処理 [deliver.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "delivery.h"
#include "scene3D.h"
#include "player3D.h"
#include "passerby.h"
#include "game.h"
#include "manager.h"
#include "destination.h"

//静的メンバ変数宣言
LPDIRECT3DTEXTURE9	CDelivery::m_pTexture;
int					CDelivery::m_nNumber;

//==============================================================================
// コンストラクタ
//==============================================================================
CDelivery::CDelivery()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CDelivery::~CDelivery()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CDelivery::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/box.png ", &m_pTexture);

	//初期化処理
	CScene3D::Init(pos, fHight, fWidth);

	//テクスチャ情報
	SetTex();

	//メンバ変数の代入
	m_pos = pos;
	m_pos.y = 300.0f;
	m_nLife = 300;

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CDelivery::Uninit(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture = NULL;
	}

	//終了処理
	CScene3D::Uninit();
}

//==============================================================================
// 更新処理
//==============================================================================
void CDelivery::Update(void)
{
	//外部情報の取得
	D3DXVECTOR3 pP = CPlayer3D::GetPosition();			//プレイヤーの位置情報
	bool bUse = CPasserby::GetUse();					//当たり判定

	if (m_pos.y >= 130.0f)
	{
		m_move.y -= 0.2f;

		// 移動量の加算
		m_pos += m_move;

		//位置の更新
		CScene3D::SetPosition(m_pos);
	}

	//寿命がゼロの場合
	if (m_nLife <= 0)
	{
		//消す
		Uninit();
	}

	m_nLife--;					//寿命を減らす
}

//==============================================================================
// 描画処理
//==============================================================================
void CDelivery::Draw(void)
{
	//描画処理
	CScene3D::Draw();
}

//==============================================================================
// 生成処理
//==============================================================================
CDelivery * CDelivery::Create(D3DXVECTOR3 pos)
{
	//生成
	CDelivery *pDelivry = NULL;
	pDelivry = new CDelivery;

	//初期化呼び出し
	pDelivry->Init(pos, 60.0f, 60.0f);

	//テクスチャの受け渡し
	pDelivry->SetTexture(m_pTexture);

	return pDelivry;
}