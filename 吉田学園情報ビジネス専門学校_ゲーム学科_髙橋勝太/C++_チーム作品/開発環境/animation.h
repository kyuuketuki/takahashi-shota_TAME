//=============================================================================
//
// アニメーション処理 [animasion.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _ANIMASION_H_
#define _ANIMASION_H_

#include "main.h"
#include "scene2D.h"

//マクロ定義
#define MAX_ANIME (3)

//=============================================================================
// 爆破クラスの定義
//=============================================================================

//継承先描画クラス
class CAnimasion : public CScene2D
{
public:
	CAnimasion(int nPriority = 0, int nType = 0);			//コンストラクタ
	virtual ~CAnimasion();									//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nAnime, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CAnimasion *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nAnime, int nType);
	static HRESULT Load(void);
	static void Unload(void);

	static void UninitType(int nUninitType);
	static void UninitTypeClear(void);

protected:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_ANIME];	//テクスチャのポインタ
	static int m_nUninitType;							//途中破棄に使用
	D3DXVECTOR3 m_pos;									//移動量
	int m_nType;										//タイプ　テクスチャ識別
	int m_nTypeBuulet;									//特殊移動時に使用
	int m_nCounterAnime;								//アニメーションカウンター
	int m_nPatternAnime;								//アニメーションパターン
	int m_fCriss;										//ループ判定
	float m_fAnimeX;									//画像分割X
	float m_fAnimeY;									//画像分割Y
	int m_nX;											//画像分割
	int m_nY;											//画像分割
	int m_nAnime;										//アニメーションスピード
	float m_fColorFa = 1.0f;							//アルファ値
};

//=============================================================================
// アニメーションクラスの定義(背景)
//=============================================================================
class CAnimasionBg : public CAnimasion
{
public:
	CAnimasionBg(int nPriority = 1, int nType = 0);		//コンストラクタ
	virtual ~CAnimasionBg();							//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nAnime, int nType);					   //初期化処理
	void Uninit(void);																					   //終了処理
	void Update(void);																					   //更新処理
	void Draw(void);																					   //描画処理

	static CAnimasionBg *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nAnime, int nType);	   //生成処理
	static void UninitTypeClear(void);																	   //途中破棄
};

#endif