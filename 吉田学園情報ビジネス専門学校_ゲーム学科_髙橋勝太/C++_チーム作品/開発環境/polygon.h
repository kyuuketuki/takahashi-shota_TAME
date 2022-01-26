//=============================================================================
//
// バレットポリゴン処理 [polygon.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _POLYGON_H_
#define _POLYGON_H_

//==============================================================================
//インクルード
//==============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
// 図形弾クラスの定義
//=============================================================================
class CPolygon : public CScene2D
{
public:
	CPolygon();								//コンストラクタ
	virtual ~CPolygon();						//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fSizeX, float fSizeY, float fmove, float fRot, float fRotMove, int nMaxSize, int MinSize, int nType, int nTypeNumber);				//初期化処理
	void Uninit(void);																																																	//終了処理
	void Update(void);																																																	//更新処理
	void Draw(void);																																																	//描画処理

	//Create関数																																																		  //生成処理
	static CPolygon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fSizeX, float fSizeY, float fmove, float fRot, float fRotMove, int nMaxSize, int MinSize, int nType, int nTypeNumber);	//テクスチャ読み込み
	static void PlayerBullet(int nDamage);																																												//テクスチャ破棄
	static void UninitBullet(void);

private:
	static bool	m_bUsePolygon;				//敵の種類をタイプで管理

	D3DXVECTOR3 m_move;							//移動量
	D3DXCOLOR   m_col;							//カラー情報
	int			m_nLife;						//ライフ
	int			m_nType;						//タイプ　弾
	int			m_CirclePattern;				//特殊回転用
	int			m_nMaxSize;						//サイズ
	int			m_nMinSize;						//サイズ
	float		m_fmove;						//移動量
	float		m_fRot;							//回転角度
	float		m_fSizeX;						//Xの大きさ
	float		m_fSizeY;						//Yの大きさ
	float		m_fRotMove;						//回転量
	bool		m_bUse;							//使用しているか

	//ダメージ量
	static int	m_nDamage;
	static int	m_nTypeNumber;					//タイプ
};

#endif