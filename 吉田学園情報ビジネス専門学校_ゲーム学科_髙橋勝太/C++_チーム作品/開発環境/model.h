//=============================================================================
//
// モデル処理 [model.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _MODEL_H_
#define _MODEL_H_

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// モデルクラスの定義
//=============================================================================
class CModel
{
public:
	CModel(int nPriority = 5, int nType = 0);										//コンストラクタ
	virtual ~CModel();																//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pTextName);				//初期化処理
	void Uninit(void);																//終了処理
	void Update(void);																//更新処理
	void Draw(void);																//描画処理

	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pTextName);		//生成処理

	//モーション更新
	void MotionPos(D3DXVECTOR3 pos);
	void MotionRot(D3DXVECTOR3 rot);

	void SetTexture(LPDIRECT3DTEXTURE9 pTexture);					//テクスチャ情報を貰ってくる関数
	void SetParent(CModel *pModel);									//親の関係
	D3DXMATRIX GetMtxWorld(void);									//ワールドマトリックスを受け渡すよう
	LPD3DXMESH GetMash(void) { return m_pMesh; }					//メッシュへのポインタ
	LPD3DXBUFFER GetBuff(void) { return m_pBuffMat; }				//バッファへのポインタ
	LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTexture; }		//テクスチャへのポインタ

private:
	LPDIRECT3DTEXTURE9	m_pTexture;		//テクスチャのポインタ
	LPD3DXMESH		m_pMesh;			//メッシュ
	LPD3DXBUFFER	m_pBuffMat;			//バッファ
	DWORD			m_nNumMat;			//マトリックス
	D3DXMATRIX		m_mtxWorld;			//ワールドマトリックス
	D3DXVECTOR3		m_pos;				//位置
	D3DXVECTOR3		m_rot;				//向き
	CModel			*m_pParent;			//モデルの親
	int				m_nNumVtx;			//頂点数
	float			m_fRot;				//回転量

};
#endif