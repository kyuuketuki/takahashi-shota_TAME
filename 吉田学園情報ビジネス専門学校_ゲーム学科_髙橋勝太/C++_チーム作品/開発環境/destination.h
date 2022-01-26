//=============================================================================
//
// シーンX　目的地　処理 [destination.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _DESTINATION_H_
#define _DESTINATION_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#include "main.h"
#include "scene.h"
#include "sceneX.h"

//=============================================================================
// 目的地クラスの定義
//=============================================================================
//継承先描画クラス
class CDestination : public CSceneX
{
public:
	CDestination(int nPriority = 5, int nType = 0);						//コンストラクタ
	virtual ~CDestination();											//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);			//初期化処理
	void Uninit(void);													//終了処理
	void Update(void);													//更新処理
	void Draw(void);													//描画処理

	D3DXVECTOR3 RandomPos(void);														//ランダムな位置を設定

	static CDestination *Create(D3DXVECTOR3 pos, char *TextName, int RandNumber);		//生成処理
	static D3DXVECTOR3	GetPosition(int nCnt);											//位置取得用
	static int	GetLimit(int nCnt);														//制限時間を取得用
	static int	GetRange(int nCnt);														//制限時間を取得用
	static bool GetMotion(void) {return m_bMotion; }                                    //モーション変化用フラグ
	static bool GetUse(void) { return m_bUseDestination; }                              //モーション変化用フラグ

private:
	D3DXVECTOR3		m_pos;					//位置
	D3DXVECTOR3		m_rot;					//向き
	static char		*m_pTextName;			//テキスト名前
	int				m_nNumber;
	int				m_nRandomNumber;
	int				m_nRandomNumber2;
	static bool		m_bUseDestination;
	int				m_TimeLimit;
	int				m_MaxTimeLimit;
	int				m_nTime;
	int				m_nTimeCounter;
	static bool		m_bMotion;				//モーション変化用フラグ
	D3DXVECTOR3		m_posdes;
};

#endif