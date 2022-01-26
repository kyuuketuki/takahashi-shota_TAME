//==============================================================================
//
// シーン3D処理[Scene3D.h]
// Author; takahashi syota
//
//==============================================================================

#ifndef _SCENE3D_H_
#define _SCENE3D_H_
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//==============================================================================
//インクルード読み込み
//==============================================================================
#include "main.h"
#include "Scene.h"

//==============================================================================
//シーン3Dクラスの定義
//==============================================================================
class CScene3D : public CScene
{
public:
	CScene3D();                                                             //コンストラクタ
	~CScene3D();                                                            //デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);              //初期化処理
	void Uninit(void);												        //終了処理
	void Update(void);												        //更新処理
	void Draw(void);												        //描画処理

	static CScene3D *Create(D3DXVECTOR3 pos, float fHight, float fWidth);	//生成処理
	void SetTexture(LPDIRECT3DTEXTURE9 pTexture);                           //テクスチャ設定
	void SetPosition(D3DXVECTOR3 pos);                                      //位置情報の受け渡し
	void SetRotation(D3DXVECTOR3 rot);                                      //角度情報の受け渡し
	void SetCol(D3DXCOLOR col);                                             //カラー変更関数
	void SetAnim(float tex, int nCnt, float tx, float ty);                  //アニメーション処理
	void SetTex(void);                                                      //テクスチャ設定

	D3DXVECTOR3 GetPosition(void) { return m_pos; }		                	//位置情報を伝える

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		                                //頂点情報
	LPDIRECT3DTEXTURE9 m_pTexture;			                                //テクスチャ情報
	D3DXMATRIX m_mtxWorld;					                                //ワールドマトリックス
	D3DXVECTOR3 m_pos;						                                //位置情報
	D3DXVECTOR3 m_Rot;						                                //回転情報
	D3DXCOLOR m_col;						                                //カラー設定
	int m_nAnimeX;
	int m_nCountY;
	int m_nAnimeY;
	int m_nCountX;
};
#endif
