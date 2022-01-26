//=============================================================================
//
// シーンX　AIガイド　処理 [AIguide.h]
// Author : katano hiroya(元ソース:takahashi shota
//
//=============================================================================

#ifndef _AIguide_H_
#define _AIguide_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#include "main.h"
#include "scene.h"
#include "sceneX.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_GUIDE_AREA	(4)			// AIガイドを設置するエリアの最大数
#define MAX_GUIDE_POINT (52)		// １エリアごとの最大ガイドポイント数

//==============================================================================
// エリアの種類
//==============================================================================
typedef enum
{
	GUIDEAREATYPE_NEUTRAL = 0,	// ニュートラル
	GUIDEAREATYPE_CITY,			// 街
	GUIDEAREATYPE_PARK,			// 公園
	GUIDEAREATYPE_BILL,			// ビル群
	GUIDEAREATYPE_MAX			// AIガイド設置エリアの最大数
} GUIDEAREATYPE;

//=============================================================================
// 目的地クラスの定義
//=============================================================================
//継承先描画クラス
class CAIguide : public CSceneX
{
public:
	CAIguide(int nPriority = 5, int nType = 0);						//コンストラクタ
	virtual ~CAIguide();											//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);			//初期化処理
	void Uninit(void);													//終了処理
	void Update(void);													//更新処理
	void Draw(void);													//描画処理

	static CAIguide *Create(D3DXVECTOR3 pos, char *TextName);			//生成処理
	static CAIguide *CreateSave(D3DXVECTOR3 pos, char *TextName);		//生成処理

	void LoadAIguide(void);												//外部読み込み
	static D3DXVECTOR3 *GetAIguidePos(int nArea, int nPoint);


	static D3DXVECTOR3		m_AIguide[MAX_GUIDE_AREA][MAX_GUIDE_POINT];			//各モーションの位置の記憶場所

private:
	D3DXVECTOR3		m_pos;				//位置
	D3DXVECTOR3		m_rot;				//向き
	static char		*m_pTextName;		//テキスト名前

	int						m_NumArea[GUIDEAREATYPE_MAX];						//AIガイドの最大設置エリア数を記憶
	int						m_PointSet[GUIDEAREATYPE_MAX];						//最大ガイドポイント数を記憶
	
};

#endif
