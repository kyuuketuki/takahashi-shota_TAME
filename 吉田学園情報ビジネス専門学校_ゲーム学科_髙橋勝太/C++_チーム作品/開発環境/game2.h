//=============================================================================
//
// ゲーム画面2処理 [game2.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _GAME2_H_
#define _GAME2_H_

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "player.h"

//=============================================================================
// ゲーム画面クラスの定義
//=============================================================================
class CGame2 : public CScene
{
public:
	CGame2();									//コンストラクタ
	~CGame2();									//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//初期化処理
	void Uninit(void);											//終了処理
	void Update(void);											//更新処理
	void Draw(void);											//描画処理

	//攻撃管理
	void Attack(void);											
	void Attack2(void);											
	void Attack3(void);											

	//時間Get
	static int GetTime(void);

	//生成処理
	static CGame2 *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);

private:
	static CPlayer				*m_pPlayer;			//プレイヤーへのポインタ	ホーミング時に使用
	LPD3DXFONT					m_pTimeFontGame;	// 表示用フォント
};

#endif