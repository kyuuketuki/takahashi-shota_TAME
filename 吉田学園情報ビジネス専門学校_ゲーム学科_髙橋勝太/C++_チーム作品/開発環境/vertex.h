//=============================================================================
//
// 頂点当たり判定処理 [vertex.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _VERTEX_H_
#define _VERTEX_H_
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "scene.h"
#include "player3D.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_OBJECT_VEC (1024)			//当たり判定の最大数

//=============================================================================
// ポリゴンクラスの定義
//=============================================================================

//継承先描画クラス
class CVertex : public CScene
{
public:
	CVertex(int nPriority = 0, int nType = 0);								//コンストラクタ
	virtual ~CVertex();														//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理

	//生成処理
	static CVertex *Create(D3DXVECTOR3 pos, D3DXVECTOR3 fMaxSizeX, D3DXVECTOR3 fMinSizeX, D3DXVECTOR3 fMaxSizeY, D3DXVECTOR3 fMinSizeY);
	
	//消去処理
	static void Release(void);

	//当たり判定
	bool CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);

	void CVertex::LoadObject(void);			//テキストの読み込み

	//表示　受け渡し用
	static int GetNumber(void);
	static int GetNumber2(void);
	static float GetAtan2(void);

private:
	LPD3DXMESH		m_pMesh;							//メッシュ
	LPD3DXBUFFER	m_pBuffMat;							//バッファ
	DWORD			m_nNumMat;							//マトリックス
	
	D3DXVECTOR3		m_pos;								//位置
	D3DXVECTOR3		m_CollisionVecPos[4];				//当たり判定ベクトル
	D3DXVECTOR3		m_move;								//位置
	D3DXVECTOR3		m_rot;								//位置
	D3DXVECTOR3     m_vtx;								//頂点数
	D3DXVECTOR3		m_MaxStone;							//最大値
	D3DXVECTOR3		m_MinStone;							//最小値
	D3DXVECTOR3		m_aPos[MAX_OBJECT_VEC][4];			//位置情報計算後の保存場所
	D3DXVECTOR3		m_Vec[MAX_OBJECT_VEC][4];			//ベクトル情報計算後の保存場所
	int				m_nNumVtx;							//頂点数
	float			m_fRot;								//回転量
	float			m_fLength;							//対角線の長さ
	float			m_fAngle;							//対角線の角度
	float			m_fLength2;							//対角線の長さ

	//静的メンバ変数
	static D3DXMATRIX		m_mtxWorld;					//ワールドマトリックス
	static D3DXVECTOR3		m_rotStatic;				//回転量
	static D3DXVECTOR3		m_moveStatic;				//移動量
	static int				m_nType;					//タイプ
	static char				*m_pTextName;				//テキスト名
};

#endif