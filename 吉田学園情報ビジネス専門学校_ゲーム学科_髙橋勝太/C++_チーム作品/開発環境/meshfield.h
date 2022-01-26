//=============================================================================
//
//メッシュフィールド処理[meshfield.h]
// Author; takahashi syota
//
//=============================================================================

#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXT_LENGTH			(128)	// テキスト読み込みの最大数
#define DATA_LENGTH			(64)	// データ読み込みの最大数

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "Scene.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define WIDTHNUM	(30)		// 列数(幅)
#define DEPTHNUM	(30)		// 行数(奥行)
#define WIDTH		(100.0f)	// 幅の大きさ
#define DEPTH		(100.0f)	// 奥行の大きさ
#define FIELD_MAX	(1)			// フィールドの最大数

//=============================================================================
//メッシュフィールドクラスの定義
//=============================================================================
class CMeshfield : public CScene
{
public:
	CMeshfield();															//コンストラクタ
	~CMeshfield();															//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos,float fHight, float fWidth);				//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理
	
	static CMeshfield *Create();	//生成処理

	//位置情報伝える
	D3DXVECTOR3 GetPosition(void) { return m_pos; }

	void CMeshfield::LoadObject(void);			//テキストの読み込み

	//サイズ情報を伝える　プレイヤーの移動制限に使用
	static float GetMeshFildX(void);
	static float GetMeshFildZ(void);

private:
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff = NULL;			// バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff = NULL;			// インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9			m_pTexture = NULL;			// テクスチャへのポインタ
	D3DXVECTOR3					m_pos;						// 位置
	D3DXVECTOR3					m_rot;						// 向き
	D3DXMATRIX					m_mtxWorld;					// ワールドマトリックス
	int							m_nAllPoint;				// 総頂点数
	int							m_nPolygon;					// ポリゴン数
	int							m_nIdxPoint;				// インデックスバッファの必要な確保数
	int							m_nWidth;					// 横幅の頂点数
	int							m_nDepth;					// 奥行の頂点数
	float						m_fWidth;					// 横幅の端
	float						m_fDepth;					// 奥行の端
	char						aTextureName[128];			// テクスチャ読み込み

	//静的メンバ変数
	static float				m_fX;						//サイズを記憶X
	static float				m_fZ;						//サイズを記憶Z
};
#endif
