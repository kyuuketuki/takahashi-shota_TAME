//=============================================================================
//
// ポーズ処理 [pause.h]
// Author : taiki hayasaka
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

// インクルード読み込み
#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// ポーズ処理クラスの定義
//=============================================================================
class CPause : public CScene
{
public:
	CPause(int nPriority = 0, int nType = 2);							// コンストラクタ								
	~CPause();															// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);			// 初期化処理
	void Uninit(void);													// 終了処理
	void Update(void);													// 更新処理
	void Draw(void);													// 描画処理
																		   
	static CPause *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	// 生成処理

private:
	int  m_nFadeCntPause;		// フェードカウント
	int  m_mode;				// 画面遷移用
};

#endif