//=============================================================================
//
// 免許処理 [License.h]
// Author : koseki yuto
//
//=============================================================================

#ifndef _LICENCE_H_
#define _LICENCE_H_

#include "main.h"
#include "scene2D.h"

//=============================================================================
// ライセンスクラスの定義
//=============================================================================
class CLicense : public CScene2D
{
public:
	CLicense(int nPriority = 3, int nType = 0);			//コンストラクタ
	~CLicense();										//デストラクタ

	typedef enum
	{
		LISENCETYPE_GOLD = 0,
		LISENCETYPE_NORMAL,
		LISENCETYPE_MAX,
	}LISENCETYPE;

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理

	static CLicense *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//生成処理
	static HRESULT Load(void);												//テクスチャ読み込み
	static void Unload(void);												//テクスチャ破棄

	static LISENCETYPE GetLisenceType(void) { return m_nLicenseType; }
	static void SetLisenceType(LISENCETYPE lisencetype);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;									// 用意するテクスチャの数
	static LISENCETYPE m_nLicenseType;										// 免許の位
};

#endif 
