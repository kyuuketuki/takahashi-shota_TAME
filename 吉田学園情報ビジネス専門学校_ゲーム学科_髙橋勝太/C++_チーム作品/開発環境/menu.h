//=============================================================================
//
// メニュー処理 [menu.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _MENU_H_
#define _MENU_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// タイトルクラスの定義
//=============================================================================
//継承先描画クラス
class CMenu : public CScene
{
public:
	CMenu();								//コンストラクタ
	~CMenu();								//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);			//初期化処理
	void Uninit(void);													//終了処理
	void Update(void);													//更新処理
	void Draw(void);													//描画処理

	static CMenu *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//生成処理

private:
	int m_nFadeCnt;				// フェードカウント
	int  m_mode;				// 画面遷移用
	int  m_nPointerMenuX;		// タイプ
	bool m_bButtonDownMenu;		// 連打対策用
};
#endif
