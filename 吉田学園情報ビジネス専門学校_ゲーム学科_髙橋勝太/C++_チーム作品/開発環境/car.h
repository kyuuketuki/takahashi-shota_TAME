//=============================================================================
//
// 車処理 [car.h]
// Author : Hiroya Katano
//
//=============================================================================

#ifndef _CAR_H_
#define _CAR_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#include "main.h"
#include "scene.h"
#include "player3D.h"
#include "AIguide.h"
#include "trafficLight.h"
#include "model.h"

#define MAX_FOV		(3000.f)
#define SIGNAL_FOV	(1500.f)
#define AUTO_NEUTORALCAR_SPD	(10.f)
#define AUTO_SUPERCAR_SPD		(20.f)
#define PLAY_CAR_SPD		(20.f)
#define PLAY_CAR_SPDDOWN		(2.f)

enum CARAREATYPE
{
	CARAREATYPE_NEUTRAL = 0,	// ニュートラル
	CARAREATYPE_CITY,			// 街
	CARAREATYPE_PARK,			// 公園
	CARAREATYPE_BILL,			// ビル群
	CARAREATYPE_MAX				// 車両巡回ルートの最大数
};

enum CARTYPE
{
	CARTYPE_NEUTRAL = 0,	// ニュートラル
	CARTYPE_NORMAL,			// 普通
	CARTYPE_SUPER,			// スーパーカー
	CARTYPE_TRUCK,			// トラック
	CARTYPE_POLICE,			// パトカー
	CARTYPE_MAX				// 車両の種類の最大数
};

//=============================================================================
// ポリゴンクラスの定義
//=============================================================================
//継承先描画クラス
class CCar : public CScene
{
public:
	CCar(int nPriority = 0, int nType = 0);									//コンストラクタ
	virtual ~CCar();														//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理

	D3DXVECTOR3	GetPosition(void) { return m_pos; }					//位置取得用
	D3DXVECTOR3	GetRotation(void) { return m_rot; }					//向き取得用

	static bool GetPlayCar(void) { return m_bPlayCar; }

	static CCar *Create(D3DXVECTOR3 pos, int nType, CARTYPE carType);			//生成処理
	bool CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);

	D3DXMATRIX GetMtxWorld(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; }

	void CalculationCar(void);

	int GetNowGuide(int NumCar)
	{
		if (m_nTypeX == NumCar) { return m_nNowGuide; }
		else { return 0; }
	}
	int
		GetNowLight(int NumCar)
	{
		if (m_nTypeX == NumCar) { return m_nNowLight; }
		else { return 0; }
	}

	float GetNowGuideAngle(int NumCar)
	{
		if (m_nTypeX == NumCar) { return m_fAngleGuide; }
		else { return 0; }
	}

private:
	DWORD				m_nNumMat;					//マトリックス
	LPD3DXMESH			m_pMesh;					//メッシュ
	LPD3DXBUFFER		m_pBuffMat;					//バッファ
	D3DXVECTOR3			m_pos;						//位置
	D3DXVECTOR3			m_move;						//移動量
	D3DXVECTOR3			m_rot;						//回転
	D3DXVECTOR3			m_vtx;						//頂点数
	LPDIRECT3DTEXTURE9	m_pTexture[5][64];			//テクスチャのポインタ
	int					m_nTypeX;					//タイプ
	float				m_fRot;						//回転量

	//静的メンバ変数
	static D3DXMATRIX		m_mtxWorld;				//ワールドマトリックス

	//当たり判定用
	D3DXVECTOR3		m_MaxSize;				//最大値
	D3DXVECTOR3		m_MinSize;				//最小値
	D3DXVECTOR3		m_aPos[4];				//四頂点(当たり判定)
	D3DXVECTOR3		m_Vec[4];				//ベクトル保存
	D3DXVECTOR3		m_posMaxBase;			//最大値
	D3DXVECTOR3		m_posMinBase;			//最小値

	//静的メンバ変数
	static D3DXVECTOR3	m_rotStatic;		//回転量
	static D3DXVECTOR3	m_moveStatic;		//移動量

	//ガイド用
	int m_nNowGuide;
	int m_nNowLight;

	float m_fDestAngle;

	float m_fVecGuideX;
	float m_fVecGuideZ;
	float m_fDisGuideZX;
	float m_fAngleGuide;

	float m_fVecLightX;
	float m_fVecLightZ;
	float m_fDisLightZX;
	float m_fAngleLight;

	float m_fVecPlayerX;
	float m_fVecPlayerZ;
	float m_fDisPlayerZX;
	float m_fAnglePlayer;

	bool m_bHoming;

	bool m_bSignalRed;
	bool m_bPlayerVio;
	bool m_bHearCar;
	bool m_bHearSiren;

	CARTYPE m_CarType;

	D3DXVECTOR3		*m_AIguide[MAX_GUIDE_AREA][MAX_GUIDE_POINT];		//各ガイドの位置を記憶
	D3DXVECTOR3		*m_Light[MAX_LIGHT_AREA][MAX_LIGHT_POINT];			//各信号機の位置を記憶

	//静的メンバ変数
	static bool m_bPlayCar;
};

#endif