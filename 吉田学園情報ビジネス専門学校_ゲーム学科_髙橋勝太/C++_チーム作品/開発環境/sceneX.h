//=============================================================================
//
// シーンX処理 [sceneX.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _SCENEX_H_
#define _SCENEX_H_
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// ポリゴンクラスの定義
//=============================================================================
class CSceneX : public CScene
{
public:
	CSceneX(int nPriority = 0, int nType = 0);						//コンストラクタ
	virtual ~CSceneX();					//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//初期化処理
	void Uninit(void);												//終了処理
	void Update(void);												//更新処理
	void Draw(void);												//描画処理

	void BindTextName(char *TextName);								//テクストへのポインタ

	void SetPosition(D3DXVECTOR3 pos);
	void SetRevolution(D3DXVECTOR3 rot);

	static CSceneX *Create(D3DXVECTOR3 pos, char *TextName);		//生成処理

	//Get関数
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRevolution(void) { return m_rot; }
																	
private:															
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