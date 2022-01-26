//=============================================================================
//
// シーンX　ポイント　処理 [pointerX.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _POINTER_H_
#define _POINTER_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#include "main.h"
#include "scene.h"
#include "sceneX.h"

//=============================================================================
// シーンX　ポイントクラスの定義
//=============================================================================
class CPointer : public CSceneX
{
public:
	CPointer(int nPriority = 5, int nType = 0);							//コンストラクタ
	virtual ~CPointer();												//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);			//初期化処理
	void Uninit(void);													//終了処理
	void Update(void);													//更新処理
	void Draw(void);													//描画処理

	static CPointer *Create(D3DXVECTOR3 pos, char *TextName);			//生成処理

	static void SetPointNumber(int nNumber);		//生成処理

private:
	D3DXVECTOR3		m_pos;				//位置
	D3DXVECTOR3		m_rot;				//向き
	//static char		*m_pTextName;		//テキスト名前
};

#endif