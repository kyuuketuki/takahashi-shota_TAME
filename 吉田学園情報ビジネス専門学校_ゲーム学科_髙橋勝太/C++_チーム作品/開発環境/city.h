//=============================================================================
//
// マップ処理 [city.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _CITY_H_
#define _CITY_H_
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// ポリゴンクラスの定義
//=============================================================================
class CCity
{
public:
	CCity(int nPriority = 0, int nType = 0);						//コンストラクタ
	virtual ~CCity();												//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//初期化処理
	void Uninit(void);												//終了処理
	void Update(void);												//更新処理
	void Draw(void);												//描画処理

	void SetPosition(D3DXVECTOR3 pos);
	void SetRevolution(D3DXVECTOR3 rot);

	static CCity *Create(D3DXVECTOR3 pos, char *TextName);		//生成処理

	//テクスチャ関連
	static HRESULT Load(void);
	static void Unload(void);

	void SetTexture(int nNumber, LPDIRECT3DTEXTURE9 pTexture);//テクスチャ情報を貰ってくる関数

	//Get関数
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRevolution(void) { return m_rot; }
	LPD3DXMESH GetMash(void) { return m_pMesh; }
	LPD3DXBUFFER GetBuff(void) { return m_pBuffMat; }

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[54];		//テクスチャへのポインタ

	LPD3DXMESH		m_pMesh;			//メッシュ
	LPD3DXBUFFER	m_pBuffMat;			//バッファ
	DWORD			m_nNumMat;			//マトリックス
	D3DXMATRIX		m_mtxWorld;			//ワールドマトリックス
	D3DXVECTOR3		m_pos;				//位置
	D3DXVECTOR3		m_rot;				//向き
	int				m_nNumVtx;			//頂点数
	float			m_fRot;				//回転量

	//静的メンバ変数
	static char		*m_pTextName;		//Xファイル読み込み
};

#endif