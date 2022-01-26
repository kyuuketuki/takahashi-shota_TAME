//=============================================================================
//
// テキスト生成オブジェクト処理 [textobject.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _TEXT_H_
#define _TEXT_H_

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "debugproc.h"
#include "object3D.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXT_LENGTH			(128)	// テキスト読み込みの最大数
#define DATA_LENGTH			(64)	// データ読み込みの最大数

//=============================================================================
// テキストオブジェクト3Dクラスの定義
//=============================================================================
class CTextobject : public CScene
{
public:
	CTextobject();									//コンストラクタ
	~CTextobject();									//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//初期化処理
	void Uninit(void);											//終了処理
	void Update(void);											//更新処理
	void Draw(void);											//描画処理

	//アドレスを記憶
	static CObject3D *GetObject(int nCnt) { return m_pObject3D[nCnt]; }

	static int GetTextObject3D();							//生成カウント伝える
	static int GetMaxFileX();								//生成カウント伝える
	static char *GetTextName(int nNumber);					//生成カウント伝える

	//生成処理
	static CTextobject *Create();

	void CTextobject::LoadObject3D(void);			//テキスト読み込み

private:
	//静的メンバ変数
	static CObject3D	*m_pObject3D[MAX_OBJECT_3D];	//オブジェクト3Dのアドレスポインタ
};

#endif