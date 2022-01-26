//=============================================================================
//
// 通行人処理 [passerby.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _PASSERBY_H_
#define _PASSERBY_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#include "main.h"
#include "scene.h"
#include "model.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MODEL_PARTS		(20)	// モデルパーツの最大数
#define TEXT_LENGTH		(128)	// テキスト読み込みの最大数
#define DATA_LENGTH		(64)	// データ読み込みの最大数

#define MAX_PASSERBY_PARTS (13)

//==============================================================================
// モーションの種類
//==============================================================================
typedef enum
{
	MOTIONTYPE_PASSERBY_NEUTRAL = 0,// ニュートラルモーション
	MOTIONTYPE_PASSERBY_MOVE,		// 移動モーション
	MOTIONTYPE_PASSERBY_LEFTMOVE,	// 左移動モーション
	MOTIONTYPE_PASSERBY_RIGHTMOVE,	// 右移動モーション
	MOTIONTYPE_PASSERBY_JUMP,		// ジャンプモーション
	MOTIONTYPE_PASSERBY_MAX			// モーションの最大数
} MOTIONTYPE_PASSERBY;

//==============================================================================
// 歩行者の種類
//==============================================================================
typedef enum
{
	PASSERBY_TYPE_MEN = 0,		// 男の人
	PASSERBY_TYPE_CAT,	        // 猫
	PASSERBY_TYPE_CUSTOMER,     // お客様
	PASSERBY_TYPE_MAX			// モーションの最大数
} PASSERBY_TYPE;

//=============================================================================
// 通行人クラスの定義
//=============================================================================
class CPasserby : public CScene
{
public:
	CPasserby(int nPriority = 5, int nType = 0);								//コンストラクタ
	virtual ~CPasserby();														//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);					//初期化処理
	void Uninit(void);															//終了処理
	void Update(void);															//更新処理
	void Draw(void);															//描画処理

	static CPasserby *Create(D3DXVECTOR3 pos, D3DXVECTOR3 goalpos, D3DXVECTOR3 randpos, PASSERBY_TYPE passerbytype);						//生成処理
	void LoadMotion(PASSERBY_TYPE passerbytype);														//外部読み込み
	void UpdateMotion(MOTIONTYPE_PASSERBY passerby);
	void PasserbyMove(void);										//通行人の動き
	void CustomerMove(void);													//お客様の動き
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	static bool GetUse(void) { return m_bUse; }
	static void SetUse(bool bUse);

private:
	CModel			*m_apModel[MAX_PASSERBY_PARTS];					//モデルへのポインタ
	char			*m_pTextName[MAX_PASSERBY_PARTS];				//名前保存
	int				m_aIndexParent[MAX_PASSERBY_PARTS];				//モデルの親
	float			m_aOffset[MAX_PASSERBY_PARTS][6];				//モデルの位置向きを設置
	float			m_aOffsetMemo[MAX_PASSERBY_PARTS][6];			//保存用モデルの位置向きを設置

	//ワールドマトリックス関連
	LPDIRECT3DTEXTURE9	m_pTexture;				//テクスチャのポインタ
	LPD3DXMESH				m_pMesh;			//メッシュ
	LPD3DXBUFFER			m_pBuffMat;			//バッファ
	DWORD					m_nNumMat;			//マトリックス
	D3DXMATRIX				m_mtxWorld;			//ワールドマトリックス
	D3DXVECTOR3				m_pos;				//位置
	D3DXVECTOR3				m_poskeep;          //位置を記憶する変数
	D3DXVECTOR3				m_rot;				//向き
	D3DXVECTOR3             m_rotDest;          //向きの慣性
	D3DXVECTOR3				m_RotDest;			//回転値
	D3DXVECTOR3				m_move;				//移動量
	D3DXVECTOR3				m_MaxPasserby;		//最大値
	D3DXVECTOR3				m_MinPasserby;		//最小値
	D3DXVECTOR3				m_posold;			//位置情報
	D3DXVECTOR3				m_posold2;			//位置情報
	int						nCntPlayer;			//プレイヤーの数
	int						m_nNumVtx;			//頂点数
	float					m_fRot;				//回転
	
	
	//静的メンバ変数
	static D3DXVECTOR3   m_posHoming;           //可視化を追尾する

	//モーション使用
	bool					m_Loop[MOTIONTYPE_PASSERBY_MAX];
	int						m_NumKey[MOTIONTYPE_PASSERBY_MAX];
	int						m_SetParent[MAX_PASSERBY_PARTS];
	float					m_Passerby[MOTIONTYPE_PASSERBY_MAX][10][MAX_PASSERBY_PARTS][6];		//各モーションの位置向きを記憶場所
	int						m_nFrame[MOTIONTYPE_PASSERBY_MAX][10];								//各モーションのフレーム記憶
	int						m_nFrameTotal[MOTIONTYPE_PASSERBY_MAX];								//各モーションのフレーム記憶
	int						m_Key;																//現在のモーションをカウント
	float					m_fCounterMotion;													//モーションのカウンター
	int						m_KeySet[MOTIONTYPE_PASSERBY_MAX];									//最大フレーム数を記憶
	MOTIONTYPE_PASSERBY		m_MotionType;														//モーションタイプ
	MOTIONTYPE_PASSERBY		m_MotionTypeOld;													//前回のモーションタイプ

	//カウント用
	int						m_nRand;													//位置情報に使用用
	int                     m_nMoveCnt;                                                 //モーション用カウント変数
	int                     m_nNumModel;                                                //モデルのパーツ数
	int                     m_nNumKeyKeep;                                              //モデルのキー保存用
	int                     m_nBrendFrame;                                              //ブレンドモーションフレーム
	int						m_nPasserbyCount;                                           //歩行者の別々の動き変数
	char                    m_pModelName[MAX_PASSERBY_PARTS][128];                      //Xファイル読み込み用
	bool                    m_bGoal;													//目的地判定用
	bool                    m_bMotionChange;                                            //モーション変化用
	bool                    m_bLoopKeep;                                                //ループするかしないか保存用
	bool                    m_fUseScene3D;												//可視化オンオフ
	D3DXVECTOR3             m_posMemo;                                                  //保存用
	D3DXVECTOR3             m_nRamdom[2];                                               //ランダムを可視化する
	D3DXVECTOR3          	m_aPos[10][4];                                              //当たり判定の外積用
	D3DXVECTOR3				m_startpos;													//スタート地点
	D3DXVECTOR3				m_goalpos;													//向かう場所
	D3DXVECTOR3				m_randpos;													//生成場所
	PASSERBY_TYPE			m_passerbytype;                                             //歩行者の種類
	
	//静的メンバ変数
	static bool             m_bUse;														//使用しているかどうか
};

#endif
