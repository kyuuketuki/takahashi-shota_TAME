//=============================================================================
//
// ポリゴン描画処理 [polygon.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _POLYGON_FADE_H_
#define _POLYGON_FADE_H_

#include "main.h"
#include "scene.h"

//==============================================================================
// ポリゴンクラス
//==============================================================================
class CPolygon_Fade
{
public:
	CPolygon_Fade();
	~CPolygon_Fade();
	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPolygon_Fade *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);

	void SetPosition(D3DXVECTOR3 pos);
	void SetTex(int nCutX, int nCutY, int nPatternX, int nPatternY, float fTexX, float fTexY);
	void SetCol(D3DXCOLOR col);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// バッファへのポインタ
	D3DXVECTOR3 m_pos;							// 位置
	float m_fLength;							// 対角線の長さ
	float m_fAngle;								// 対角線の角度
};

#endif