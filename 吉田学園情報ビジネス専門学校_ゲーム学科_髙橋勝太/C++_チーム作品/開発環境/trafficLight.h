//=============================================================================
//
// 信号機　処理 [TrafficLight.h]
// Author : katano hiroya
//=============================================================================

#ifndef _trafficLight_H_
#define _trafficLight_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#include "main.h"
#include "scene.h"
#include "sceneX.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_LIGHT_AREA	(2)		// AIガイドを設置するエリアの最大数
#define MAX_LIGHT_POINT (14)	// １エリアごとの最大設置数
#define MAX_NONE_LIGHT (41)	// １エリアごとの最大設置数

//==============================================================================
// モーションの種類
//==============================================================================
typedef enum
{
	LIGHTAREATYPE_NEUTRAL = 0,	// ニュートラル
	LIGHTAREATYPE_CITY,			// 街
	LIGHTAREATYPE_PARK,			// 公園
	LIGHTAREATYPE_BILL,			// ビル群
	LIGHTAREATYPE_MAX			// AIガイド設置エリアの最大数
} LIGHTAREATYPE;

typedef enum
{
	LIGHT_POS = 0,			// Pos
	LIGHT_ROT,				// Rot
	LIGHT_POS_OR_ROT_MAX	// PosかRotか
} LIGHT_POS_OR_ROT;

typedef struct
{
	D3DXVECTOR3	pos;
	D3DXVECTOR3	rot;
	bool		bSignal;
}TRAFFICLIGHT_DATA;

//=============================================================================
// 目的地クラスの定義
//=============================================================================
class CTrafficLight : public CSceneX
{
public:
	CTrafficLight(int nPriority = 5, int nType = 0);						//コンストラクタ
	virtual ~CTrafficLight();												//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSizeX, float fSizeY);			//初期化処理
	void Uninit(void);																	//終了処理
	void Update(void);																	//更新処理
	void Draw(void);																	//描画処理

	static CTrafficLight *Create(D3DXVECTOR3 pos, int nArea, int nPoint);		//初期生成処理
	static CTrafficLight *CreateNone(D3DXVECTOR3 pos, char *TextName);			//初期生成処理
	static CTrafficLight *CreateSave(D3DXVECTOR3 pos, char *TextName);			//新規生成処理

	void LoadTrafficLight(void);					//外部読み込み
	void LoadNoneTrafficLight(void);				//外部読み込み

	bool GetSignal(int nArea, int nPoint) { return m_TrafficLight[nArea][nPoint].bSignal; }
	D3DXVECTOR3	GetLightPos(int nArea, int nPoint) { return m_TrafficLight[nArea][nPoint].pos; }
	D3DXVECTOR3 GetNoneLightPos(int nPoint) { return m_NoneTrafficLight[nPoint].pos; }
	float GetRot(int nArea, int nPoint) { return m_TrafficLight[nArea][nPoint].rot.y; }
	float GetRotNone(int nPoint) { return m_NoneTrafficLight[nPoint].rot.y; }

	TRAFFICLIGHT_DATA		m_TrafficLight[MAX_LIGHT_AREA][MAX_LIGHT_POINT];			//各データの記憶場所
	TRAFFICLIGHT_DATA		m_NoneTrafficLight[MAX_NONE_LIGHT];							//各データの記憶場所

private:
	D3DXVECTOR3		m_pos;								//位置
	D3DXVECTOR3		m_rot;								//向き
	static char		*m_pTextName;						//テキスト名前

	int				m_NumArea[LIGHTAREATYPE_MAX];		//AIガイドの最大設置エリア数を記憶
	int				m_PointSet[LIGHTAREATYPE_MAX];		//最大ガイドポイント数を記憶
	int				m_nCntTime;


	DWORD				m_nNumMat;												//マトリックス
	LPD3DXMESH			m_pMesh;												//メッシュ
	LPD3DXBUFFER		m_pBuffMat;												//バッファ
	D3DXVECTOR3			m_vtx;													//頂点数
	LPDIRECT3DTEXTURE9	m_pTexture[MAX_LIGHT_AREA*MAX_LIGHT_POINT][2];			//テクスチャのポインタ

	int					m_nTypeX;					//タイプ
	int					m_nArea, m_nPoint;
	float				m_fRot;						//回転量

	//静的メンバ変数
	static D3DXMATRIX	m_mtxWorld;				//ワールドマトリックス
};

#endif
