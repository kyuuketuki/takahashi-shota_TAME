//=============================================================================
//
// シーン2D処理 [scene2d.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _SCENE2D_H_
#define _SCENE2D_H_
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// シーン2Dクラスの定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
	D3DXVECTOR3 g_rotPolygon;
} VERTEX_2D;

//継承先描画クラス
class CScene2D : public CScene
{
public:
	CScene2D(int nPriority = 3, int nType = 0);								//コンストラクタ
	virtual ~CScene2D();													//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos , float fSizeX, float fSizeY);				//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);							//テクスチャへのポインタ

	//Set関数
	void SetPosition(D3DXVECTOR3 pos);										//移動更新
	void SetRevolution(float nRot);											//回転更新
	void SetSize(float fSizeX, float fSizeY);								//サイズ更新
	void SetBg(float nAnime);												//背景移動
	void SetCollar(D3DXCOLOR col);											//カラー変更
	void SetEffect(D3DXCOLOR col, float nData);								//アルファ値減少処理
	void SetTex(float fPageX, float fPageY,int nAnime);						//アニメーション更新
	void SetAnime(float fPageX, float fPageY, int nX, int nY, int nAnime);	//アニメーション更新　複数

	//Get関数
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRevolution(void) { return m_rot; }

private:
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffPolygon = NULL;

	//ポリゴンの情報
	LPDIRECT3DTEXTURE9			m_pTexture = NULL;		// プレイヤーテクスチャへのポインタ
	D3DXVECTOR3					m_pos;					//ポリゴンの初期位置
	D3DXVECTOR3					m_rot;					//ポリゴンの回転値
	D3DXVECTOR3					m_move;					//ポリゴンの移動量
	float						m_fLength;				//対角線の長さ
	float						m_fAngle;				//対角線の角度
	int							m_nAnime;				//アニメーションスピード
	int m_nAnimeX;										//サイズ
	int m_nAnimeY;										//サイズ
	int m_nCountX;										//計算用
	int m_nCountY;										//計算用
};														

#endif