//=============================================================================
//
// アイテム処理 [item.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _ITEM_H_
#define _ITEM_H_

//==============================================================================
//インクルード
//==============================================================================
#include "main.h"
#include "player.h"
#include "scene2D.h"

//マクロ定義
#define ITEM_SIZE (70)
#define ITEM_TYPE (2)

//=============================================================================
// アイテムクラスの定義
//=============================================================================
class CItem : public CScene2D
{
public:
	CItem();								//コンストラクタ
	virtual ~CItem();						//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fmove, float fSizeX, float fSizeY, int nType);	//初期化処理
	void Uninit(void);																	//終了処理
	void Update(void);																	//更新処理
	void Draw(void);																	//描画処理

	void Animasion(void);		//アニメション処理

	//生成処理
	static CItem *Create(D3DXVECTOR3 pos, float fmove, float fSizeX, float fSizeY, int nType);
	static void Heart(D3DXVECTOR3 pos, float fmove, float fSizeX, float fSizeY, int nType);
	static void Heart2(D3DXVECTOR3 pos, float fmove, float fSizeX, float fSizeY, int nType, int nType2);

	//テクスチャ関連
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[ITEM_TYPE];			//テクスチャへのポインタ

	D3DXVECTOR3					m_pos;							//位置
	int							m_nType;						//敵の種類をタイプで管理
	float						m_fMove;						//移動スピード
	
	//アニメーション関連
	int							m_nCounterAnime;				//アニメーションカウンター
	int							m_nPatternAnime;				//アニメーションパターン
	float						m_fAnimeX;						//サイズ
	float						m_fAnimeY;						//サイズ
	int							m_nX;							//サイズ
	int							m_nY;							//サイズ
};															

#endif