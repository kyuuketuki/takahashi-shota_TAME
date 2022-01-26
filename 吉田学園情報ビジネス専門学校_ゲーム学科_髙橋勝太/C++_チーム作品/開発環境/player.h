//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_

//==============================================================================
//インクルード
//==============================================================================
#include "main.h"
#include "scene2D.h"

//マクロ定義
#define PLAYER_SIZE (50)
#define PLAYER_TYPE (5)

//=============================================================================
// プレイヤークラスの定義
//=============================================================================
class CPlayer : public CScene2D
{
public:
	CPlayer(int nPriority = 2);								//コンストラクタ
	virtual ~CPlayer();										//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, int nLife, float fSizeX, float fSizeY);					//初期化処理
	void	Uninit(void);																	//終了処理
	void	Update(void);																	//更新処理
	void	Draw(void);																		//描画処理

	static CPlayer	*Create(D3DXVECTOR3 pos, int nLifefloat, float fSizeX, float fSizeY);	//生成処理
	static HRESULT	Load(void);					//テクスチャ読み込み
	static void		Unload(void);				//テクスチャ破棄

	void TypePlayer();							//タイプ別処理

	//プレイヤー強化
	static void BulletLevel(void);
	static void SetBulletLevel(int nData);

	//衝突判定＆無敵時間
	static bool GetBumped(void);
	static void SetBumped(bool bBumped);

	//Get関数
	static D3DXVECTOR3	GetPlayer(void);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[PLAYER_TYPE];	//テクスチャポインター
	D3DXVECTOR3					m_pos;						//位置
	D3DXVECTOR3					m_move;						//移動量
	int							m_nType;					//タイプテクスチャ識別用
	int							m_nSpeed;					//移動量
	bool						m_nBulletLevelUP;			//弾のレベル
	bool						m_nBullet;					//弾のレベル用
	bool						m_nBullet2;					//弾のレベル用
	bool						m_nBullet3;					//弾のレベル用

	//マルチターゲット用の変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;
	LPDIRECT3DTEXTURE9		m_pTextureMT[2];
	LPDIRECT3DSURFACE9		m_pRenderMT[2];
	LPDIRECT3DSURFACE9		m_pBuffMT[2];
	D3DVIEWPORT9			m_ViewportMT;
};

#endif