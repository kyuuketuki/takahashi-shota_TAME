//=============================================================================
//
// 機体選択画面処理 [select.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _SELECT_H_
#define _SELECT_H_

//==============================================================================
//インクルード
//==============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// 機体選択画面
//=============================================================================
class CSelect : public CScene
{
public:
	CSelect();								//コンストラクタ
	~CSelect();								//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理

	static CSelect *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//生成処理
};
#endif
