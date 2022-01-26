//=============================================================================
//
// プレイヤー3D処理 [player3D.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _MAP_H_
#define _MAP_H_

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "scene.h"
#include "model.h"
#include "city.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_MAP_TEXTURE			(54)	// 読み込むテクスチャの最大数
#define MAX_MAP_OPBJECT			(29)	// 読み込むオブジェクトの最大数

//=============================================================================
// ポリゴンクラスの定義
//=============================================================================
class CMap : public CScene
{
public:
	CMap(int nPriority = 5, int nType = 0);										//コンストラクタ
	virtual ~CMap();															//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);					//初期化処理
	void Uninit(void);															//終了処理
	void Update(void);															//更新処理
	void Draw(void);															//描画処理

	static CMap *Create();														//生成処理

	//テクスチャ関連
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_MAP_TEXTURE];		//テクスチャへのポインタ

	LPD3DXMESH				m_pMesh;			//メッシュ
	LPD3DXBUFFER			m_pBuffMat;			//バッファ

	//テキスト用
	CCity			*m_apMap[MAX_MAP_OPBJECT];					//オブジェクトへのポインタ
	char			*m_pTextName[MAX_MAP_OPBJECT];				//名前保存
};

#endif