//=============================================================================
//
// ライフ処理 [bg.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _LIFE_H_
#define _LIFE_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// ライフ管理クラスの定義
//=============================================================================
class CLife : public CScene2D
{
public:
	CLife(int nPriority = 5);								//コンストラクタ
	~CLife();												//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理

	static CLife *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//生成処理
	static HRESULT Load(void);												//テクスチャ読み込み
	static void Unload(void);												//テクスチャ破棄

	//回復
	static void Recovery(int nLife);

private:
	static LPDIRECT3DTEXTURE9 m_apTextureLife[1];		// 用意するテクスチャの数
	CScene2D *m_apScene2D[1];							// シーン2Dへのポインタ
	D3DXVECTOR3 m_pos;									// 位置情報
	float m_nCntAnime;									// テクスチャをずらす
	int m_nCount;										// カウンター用
};

#endif