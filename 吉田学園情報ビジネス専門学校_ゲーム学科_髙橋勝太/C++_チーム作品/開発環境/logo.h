//=============================================================================
//
// ロゴ処理 [logo.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _LOGO_H_
#define _LOGO_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// ロゴクラスの定義
//=============================================================================
//継承先描画クラス
class CLogo : public CScene2D
{
public:
	CLogo(int nPriority = 0, int nType = 0);			//コンストラクタ
	~CLogo();											//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType);			//初期化処理
	void Uninit(void);																//終了処理
	void Update(void);																//更新処理
	void Draw(void);																//描画処理

	static CLogo *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType);	//生成処理
	static HRESULT Load(void);														//テクスチャ読み込み
	static void Unload(void);														//テクスチャ破棄

	//特殊関数
	static void TypeUninit(int nType);												//途中破棄
	static void TypeMove(D3DXVECTOR3 pos, int nTypeMove);							//途中移動
	static void TypeChangeCollar(D3DXCOLOR col, int nTypeCollar);					//途中色変化

protected:
	static LPDIRECT3DTEXTURE9 m_apTextureLogo[MAX_UI];		// 用意するテクスチャの数

	CScene2D *m_apScene2D[3];								// シーン2Dへのポインタ
	D3DXVECTOR3		m_pos;									// 位置情報
	float			m_nCntAnime;							// テクスチャをずらす
	int				m_nType;								//タイプでテクスチャを管理している
	float			m_fSizeX;								//サイズX
	float			m_fSizeY;								//サイズY
	int				m_nCountSize;							//大きさを変える
};

//=============================================================================
// ロゴクラスの定義(Pause)
//=============================================================================
class CLogoPause : public CLogo
{
public:
	CLogoPause(int nPriority = 0, int nType = 2);				//コンストラクタ
	~CLogoPause();												//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType);				//初期化処理
	void Uninit(void);																	//終了処理
	void Update(void);																	//更新処理
	void Draw(void);																	//描画処理

	static CLogoPause *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType);	//生成処理
	static void Unload(void);															//テクスチャ読み込み
	static void TypeUninit(int nType);													//途中破棄
	static void TypeMove(D3DXVECTOR3 pos, int nTypeMove);								//途中移動
	static void TypeChangeCollar(D3DXCOLOR col, int nTypeCollar);						//途中色変化
};

#endif