//=============================================================================
//
// 番号処理 [number.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _NUMBER_H_
#define _NUMBER_H_

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"

//=============================================================================
// 番号クラスの定義
//=============================================================================
class CNumber
{
public:
	CNumber();								//コンストラクタ
	~CNumber();								//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);			//初期化処理
	void Uninit(void);													//終了処理
	void Update(void);													//更新処理
	void Draw(void);													//描画処理
	void SetNumber(int nNumber);
	
	static CNumber *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//生成処理
																			
	static HRESULT Load(void);												//テクスチャ読み込み
	static void Unload(void);												//テクスチャ破棄

private:
	static LPDIRECT3DTEXTURE9	m_pTexture;				//用意するテクスチャの数
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;				//バッファ
	int							m_nNumber;				//記憶用
	float						m_fLength;				//サイズ
	float						m_fAngle;				//対角線計算用
};								

#endif