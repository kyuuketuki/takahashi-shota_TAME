//==============================================================================
//
// シーン3D処理[Visualization.h]
// Author; takahashi syota
//
//==============================================================================

#ifndef _VISUALIZATION_H_
#define _VISUALIZATION_H_
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//==============================================================================
//インクルード読み込み
//==============================================================================
#include "main.h"
#include "Scene.h"

//==============================================================================
//シーン3Dクラスの定義
//==============================================================================
class CVisualization : public CScene
{
public:
	CVisualization();			//コンストラクタ
	~CVisualization();			//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);      //初期化処理
	void Uninit(void);												//終了処理
	void Update(void);												//更新処理
	void Draw(void);												//描画処理

																	//生成処理
	static CVisualization *Create(D3DXVECTOR3 pos, D3DXVECTOR3 fMaxSizeX, D3DXVECTOR3 fMinSizeX, D3DXVECTOR3 fMaxSizeY, D3DXVECTOR3 fMinSizeY, bool bLife, D3DXCOLOR col);

	//Get関数
	D3DXVECTOR3 GetPosition(void) { return m_pos; }			//位置情報を伝える

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点情報
	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャ情報
	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス
	D3DXVECTOR3 m_pos;						//位置情報
	D3DXVECTOR3 m_Rot;						//回転情報
	int			m_nLife;

	//静的メンバ変数
	static D3DXVECTOR3	size[4];				//4頂点を記憶
	static D3DXCOLOR	m_col;					//色情報
	static bool			m_bLife;				//4頂点を記憶
};
#endif
