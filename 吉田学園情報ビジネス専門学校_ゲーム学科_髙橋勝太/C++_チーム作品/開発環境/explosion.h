//=============================================================================
//
// 爆発処理 [explosion.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
// 爆破クラスの定義
//=============================================================================
class CExplosion : public CScene2D
{
public:
	CExplosion();								//コンストラクタ
	virtual ~CExplosion();						//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);							//初期化処理
	void Uninit(void);																	//終了処理
	void Update(void);																	//更新処理
	void Draw(void);																	//描画処理

	static CExplosion *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//生成処理
	static HRESULT Load(void);															//テクスチャ読み込み
	static void Unload(void);															//テクスチャ破棄

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャのポインタ
	int m_nCounterAnime;					//アニメーションカウンター
	int m_nPatternAnime;					//アニメーションパターン
};

#endif