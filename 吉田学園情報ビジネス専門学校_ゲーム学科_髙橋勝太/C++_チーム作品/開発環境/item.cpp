//=============================================================================
//
// アイテム処理 [item.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "item.h"
#include "manager.h"
#include "life.h"
#include "animation.h"

// 静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CItem::m_pTexture[ITEM_TYPE] = {};

//==============================================================================
// コンストラクタ
//==============================================================================
CItem::CItem()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CItem::~CItem()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, float fmove, float fSizeX, float fSizeY, int nType)
{
	CScene2D::Init(pos, fSizeX, fSizeY);

	m_fMove = fmove;
	m_fAnimeX = 0.2f;			//テクスチャ分割
	m_fAnimeY = 0.25f;			//テクスチャ分割
	m_nX = 5;
	m_nY = 4;

	m_nType = nType;

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CItem::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// 更新処理
//==============================================================================
void CItem::Update(void)
{
	//サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	pos.y += m_fMove;

	//移動させる
	SetPosition(pos);

	//更新
	CScene2D::Update();

	//アニメーション更新関数
	Animasion();

	//下に行ったら消す
	if (pos.y >= 1500.0f)
	{
		Uninit();
	}

	//それ以外の場合当たり判定処理をする
	else
	{
		// 敵との当たり判定
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			// オブジェクト情報の取得
			CScene *pScene;
			pScene = GetScene(nCntScene, 2);

			// NULLチェック
			if (pScene != NULL)
			{
				// オブジェクトの種類の取得
				CScene::OBJTYPE objType = pScene->GetObjType();

				// オブジェクトの種類がEnemy場合
				if (objType == CScene::OBJTYPE_PLAYER)
				{
					// 位置の取得
					D3DXVECTOR3 posPlayer = pScene->GetPosition();

					// 当たり判定
					if (pos.x + 50 >= posPlayer.x - (PLAYER_SIZE - 40) && pos.x - 50 <= posPlayer.x + (PLAYER_SIZE - 40) &&
						pos.y - 50 <= posPlayer.y + (PLAYER_SIZE - 40) && pos.y + 50 >= posPlayer.y - (PLAYER_SIZE - 40))
					{
						if (m_nType == 0)
						{
							//バレット強化
							CPlayer::BulletLevel();
							CAnimasion::Create(pos, 100, 100, 5, 8);
						}

						if (m_nType == 1)
						{
							//最大HP以下の場合回復させる
							if (CManager::GetPlayerLife() <= (CManager::GetMaxPlayerLife() - 1))
							{
								//PLAYERのライフを読み込む
								int nLifePlayer = pScene->GetLife();

								//ライフ情報を受け渡す
								pScene->SetLife(nLifePlayer + 1);

								//回復アニメーション
								CAnimasion::Create(pos, 100, 100, 5, 7);

								//座標を計算
								float nX = ((CManager::GetPlayerLife()) * 80.0f) + 40.0f;

								//最新の左に生成する
								CLife::Create(D3DXVECTOR3(nX, 100.0f, 0.0f), 30.0f, 30.0f);						
							}
						}

						//サウンドSE
						pSound->Play(CSound::SOUND_LABEL_SE_RECOVER);

						// 弾の破棄
						Uninit();
					}
				}
			}
		}
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void CItem::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// アニメーションの更新
//==============================================================================
void CItem::Animasion(void)
{
	//アニメーションの更新条件
	if ((m_nCounterAnime % 8) == 0)
	{
		//アニメーションパターンの更新
		m_nPatternAnime = (m_nPatternAnime + 1);
	}

	//アニメーションの更新
	CScene2D::SetAnime(m_fAnimeX, m_fAnimeY, m_nX, m_nY, m_nPatternAnime);

	//カウンターを進める
	m_nCounterAnime++;

	//更新
	CScene2D::Update();

	//アニメーションを終了リセット
	if (m_nPatternAnime == m_nX * m_nY)
	{
		//カウンターのリセット
		m_nPatternAnime = 0;
	}
}

//==============================================================================
// 生成処理
//==============================================================================
CItem * CItem::Create(D3DXVECTOR3 pos, float fmove, float fSizeX, float fSizeY, int nType)
{
	// ローカル変数宣言
	CItem *pItem = NULL;
	pItem = new CItem;

	// 初期化
	if (pItem != NULL)
	{
		//アイテムの生成
		pItem->Init(pos, fmove, fSizeX, fSizeY, nType);
	}

	// テクスチャ割り当て
	pItem->BindTexture(m_pTexture[nType]);

	return pItem;
}

//==============================================================================
//ハートの形に生成
//==============================================================================
void CItem::Heart(D3DXVECTOR3 pos, float fmove, float fSizeX, float fSizeY, int nType)
{
	//アイテム生成
	CItem::Create(D3DXVECTOR3(pos.x, 0.0f, 0.0f), fmove, 50, 50, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 50.0f, -50.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 50.0f, -50.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 100.0f, -100.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 100.0f, -100.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 150.0f, -150.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 150.0f, -150.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 100.0f, -200.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 100.0f, -200.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 50.0f, -200.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 50.0f, -200.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x, -150.0f, 0.0f), 2.0f, 50, 50, 1);
}

void CItem::Heart2(D3DXVECTOR3 pos, float fmove, float fSizeX, float fSizeY, int nType, int nType2)
{
	//アイテム生成
	CItem::Create(D3DXVECTOR3(pos.x, 0.0f, 0.0f), fmove, 50, 50, nType);

	CItem::Create(D3DXVECTOR3(pos.x, -50.0f, 0.0f), fmove, fSizeX, fSizeY, nType2);
	CItem::Create(D3DXVECTOR3(pos.x - 50.0f, -50.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 50.0f, -50.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 50.0f, -100.0f, 0.0f), fmove, fSizeX, fSizeY, nType2);
	CItem::Create(D3DXVECTOR3(pos.x + 50.0f, -100.0f, 0.0f), fmove, fSizeX, fSizeY, nType2);
	CItem::Create(D3DXVECTOR3(pos.x - 100.0f, -100.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 100.0f, -100.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 150.0f, -150.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 150.0f, -150.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 100.0f, -200.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 100.0f, -200.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 50.0f, -200.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 50.0f, -200.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x, -150.0f, 0.0f), 2.0f, 50, 50, 1);
}

//==============================================================================
// テクスチャの読み込み
//==============================================================================
HRESULT CItem::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pipo-mapeffect012e.png", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pipo-mapeffect012h.png", &m_pTexture[1]);

	return S_OK;
}

//==============================================================================
// テクスチャの破棄
//==============================================================================
void CItem::Unload(void)
{
	for (int nCnt = 0; nCnt < ITEM_TYPE; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
