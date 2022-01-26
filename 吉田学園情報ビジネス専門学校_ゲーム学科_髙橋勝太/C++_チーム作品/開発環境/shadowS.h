//=============================================================================
//
// ステンシルシャドウ処理 [shodowS.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _SHADOWS_H_
#define _SHADOWS_H_

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "sceneX.h"

//=============================================================================
// ポリゴンクラスの定義
//=============================================================================
class CShadowS : public CSceneX
{
public:
	CShadowS(int nPriority = 5, int nType = 0);						//コンストラクタ
	virtual ~CShadowS();											//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//初期化処理
	void Uninit(void);												//終了処理
	void Update(void);												//更新処理
	void Draw(void);												//描画処理

	static CShadowS *Create();										//生成処理

private:
	//シャドウのモデルメンバ変数
	LPD3DXMESH		m_pMesh;			//メッシュ
	LPD3DXBUFFER	m_pBuffMat;			//バッファ
	DWORD			m_nNumMat;			//マトリックス
	D3DXMATRIX		m_mtxWorld;			//ワールドマトリックス
	D3DXVECTOR3		m_pos;				//位置
	D3DXVECTOR3		m_rot;				//向き
	int				m_nNumVtx;			//頂点数
	float			m_fRot;				//回転量

	//ポリゴン2Dのメンバ変数
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff = NULL;				//頂点情報
	LPDIRECT3DTEXTURE9			m_pTexture = NULL;				//プレイヤーテクスチャへのポインタ
	D3DXVECTOR3					m_pospolygon;					//ポリゴンの初期位置
	D3DXVECTOR3					m_rotpolygon;					//ポリゴンの回転値
};

#endif