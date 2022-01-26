//=============================================================================
//
// チュートリアル画面処理 [tutorial.h]
// Author : taiki hayasaka
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "model.h"

//=============================================================================
// チュートリアルクラスの定義
//=============================================================================
class CTutorial : public CScene
{
public:
	CTutorial();								//コンストラクタ
	~CTutorial();								//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理

	static CTutorial *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//生成処理

private:
	CModel *m_apModel;				//モデルへのポインタ
};
#endif

