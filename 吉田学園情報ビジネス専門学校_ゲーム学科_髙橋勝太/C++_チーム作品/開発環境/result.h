//=============================================================================
//
// リザルト画面処理 [game2.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _RESULT_H_
#define _RESULT_H_

//==============================================================================
//インクルード
//==============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "number.h"

//=============================================================================
// リザルト画面クラスの定義
//=============================================================================
class CResult : public CScene
{
public:
	CResult();									//コンストラクタ
	~CResult();									//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//初期化処理
	void Uninit(void);												//終了処理
	void Update(void);												//更新処理
	void Draw(void);												//描画処理

	static CResult *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//生成処理

private:
	LPD3DXFONT			m_pTimeFontGame;		// 表示用フォント
	CNumber				*m_apNumber[5][8];		//桁数
	int					m_nRankintData[5];		//ランキングデータ保管
	
	//静的メンバ変数
	static CScore		*m_pScore[5];					//アドレス保存用ポインタ
};

#endif