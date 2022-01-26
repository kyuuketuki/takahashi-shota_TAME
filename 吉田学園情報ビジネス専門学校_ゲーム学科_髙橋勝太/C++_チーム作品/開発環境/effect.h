//==============================================================================
//
// エフェクト3D処理[effect3D.h]
// Author; takahashi syota
//
//==============================================================================

#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

//==============================================================================
// マクロ定義
//==============================================================================
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//==============================================================================
//インクルード読み込み
//==============================================================================
#include "main.h"
#include "Scene.h"

//==============================================================================
//エフェクト3Dクラスの定義
//==============================================================================
class CEffect3D : public CScene
{
public:
	CEffect3D(); //コンストラクタ
	~CEffect3D();//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHight);      //初期化処理
	void Uninit(void);												//終了処理
	void Update(void);												//更新処理
	void Draw(void);												//描画処理

	static CEffect3D *Create(D3DXVECTOR3 pos, float fWidth, float fHight);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);							//テクスチャへのポインタ
	void SetPosition(D3DXVECTOR3 pos);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;  //頂点情報
	LPDIRECT3DTEXTURE9 m_pTexture;		 //テクスチャ情報
	D3DXMATRIX m_mtxWorld;               //ワールドマトリックス
	D3DXVECTOR3 m_pos;                   //位置情報
	D3DXVECTOR3 m_pos2;                  //位置情報
	D3DXVECTOR3 m_move;                  //移動情報
	D3DXVECTOR3 m_Rot;                   //回転情報
	D3DXCOLOR m_col;                     //カラー設定
	int m_nLife;                         //可視化の寿命
	float m_fAngle;                      //移動の角度保存用
	float m_fWidth;                      //エフェクトの横幅
	float m_fHight;                      //エフェクトの縦幅
	float m_alpha;
};
#endif

