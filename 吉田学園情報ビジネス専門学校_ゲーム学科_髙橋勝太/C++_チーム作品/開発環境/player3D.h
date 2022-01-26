//=============================================================================
//
// プレイヤー3D処理 [player3D.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _PLAYER3D_H_
#define _PLAYER3D_H_

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "scene.h"
#include "model.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MODEL_PARTS			(20)	// モデルパーツの最大数
#define TEXT_LENGTH			(128)	// テキスト読み込みの最大数
#define DATA_LENGTH			(64)	// データ読み込みの最大数
#define PLAYER_SPEED		(30)	// データ読み込みの最大数
#define MAX_PLAYER_PARTS	(30)	//使用できるパーツ数
#define MAX_MOTION          (20)    //モーションの最大数

#define	MAX_COUNT_VIOLATION	(120)	// 違反行為した際falseにするまでのフレーム数

//==============================================================================
// モーションの種類
//==============================================================================
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	// ニュートラルモーション
	MOTIONTYPE_MOVE,		// 移動モーション
	MOTIONTYPE_LEFTMOVE,	// 左移動モーション
	MOTIONTYPE_RIGHTMOVE,	// 右移動モーション
	MOTIONTYPE_FALLMOVE,	// 転ぶモーション
	MOTIONTYPE_MAX			// モーションの最大数
} MOTIONTYPE;

//=============================================================================
// プレイヤー3Dクラスの定義
//=============================================================================
class CPlayer3D : public CScene
{
public:
	CPlayer3D(int nPriority = 5, int nType = 0);								//コンストラクタ
	virtual ~CPlayer3D();														//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);					//初期化処理
	void Uninit(void);															//終了処理
	void Update(void);															//更新処理
	void Draw(void);															//描画処理

	static CPlayer3D *Create();													//生成処理
	void LoadMotion(void);														//外部読み込み
	void CPlayer3D::UpdateMotion(int MOTIONTYPE, bool bUse);                    //モーション更新用

	static D3DXVECTOR3	GetPosition(void);										//位置取得用
	static D3DXVECTOR3	GetRotation(void);										//向き取得用

	//移動処理の許可(オンオフ)
	static void SetMoveUse(bool bMoveUse);

	static bool GetViolation(void) { return m_bViolation; }
	static int GetMotionType(void) { return m_MotionType; }

private:
	//テキスト用
	CModel			*m_apModel[MAX_PLAYER_PARTS];				//モデルへのポインタ
	char			*m_pTextName[MAX_PLAYER_PARTS];				//名前保存
	int				m_aIndexParent[MAX_PLAYER_PARTS];			//モデルの親
	float			m_aOffset[MAX_PLAYER_PARTS][6];				//モデルの位置向きを設置
	float			m_aOffsetMemo[MAX_PLAYER_PARTS][6];			//保存用モデルの位置向きを設置

	D3DXMATRIX				m_mtxWorld;			//ワールドマトリックス
	LPDIRECT3DTEXTURE9		m_pTexture;			//テクスチャのポインタ
	LPD3DXMESH				m_pMesh;			//メッシュ
	LPD3DXBUFFER			m_pBuffMat;			//バッファ
	DWORD					m_nNumMat;			//マトリックス

	D3DXVECTOR3				m_move;				//移動量
	D3DXVECTOR3				m_MaxPlayer;		//最大値
	D3DXVECTOR3				m_MinPlayer;		//最小値
	D3DXVECTOR3				m_posold;			//位置フレーム前の位置情報
	D3DXVECTOR3             m_rotDest;          //向きの慣性
	D3DXVECTOR3				m_RotDest;			//角度計算用
	int						m_nNumVtx;			//頂点数
	float					m_fRot;				//回転

												//静的メンバ変数
	static D3DXVECTOR3		m_pos;				//位置
	static D3DXVECTOR3		m_rot;				//向き

												//モーション使用
	int						m_NumKey[MAX_MOTION];									//各モーションのフレーム数
	int						m_KeySet[MAX_MOTION];									//最大フレーム数を記憶
	int                     m_nMoveCnt;                                                 //モーション用カウント変数
	int						m_SetParent[MAX_PLAYER_PARTS];								//各パーツの親子関係
	int						m_nFrame[MAX_MOTION][10];								//各モーションのフレーム記憶
	int						m_nFrameTotal[MAX_MOTION];								//各モーションのフレーム記憶
	int						m_Key;														//現在のモーションをカウント
	float					m_Player3D[MAX_MOTION][10][MAX_PLAYER_PARTS][6];		//各モーションの位置向きを記憶場所
	int 					m_fCounterMotion;											//モーションのカウンター
	bool					m_Loop[MAX_MOTION];										//各モーションのループ判定
	static int 				    m_MotionType;												//モーションブレンド　現在
	int 					m_MotionTypeOld;											//モーションブレンド　過去

																						//モデル使用
	static int              m_nNumModel;                                                //モデルのパーツ数
	int                     m_nNumKeyKeep;                                              //モデルのキー保存用
	int                     m_nBrendCount;												//モーションブレンドカウント
	int                     m_nBrendFrame;                                              //ブレンドモーションフレーム
	char                    m_pModelName[MAX_PLAYER_PARTS][128];                        //Xファイル読み込み用	
	bool                    m_bLoopKeep;                                                //ループするかしないか保存用
	bool                    m_bMotionChange;                                            //モーション変化用
	D3DXVECTOR3             m_posMemo;                                                  //保存用

																						//パトカーが追跡処理するための変数
	static bool				m_bViolation;												//現在違反行為をしたか
	int						m_nCntViolation;											//違反行為判定の時間計測
	bool					m_bInterSectionTurn;										//交差点曲がったか
	int						m_nCntInterSectionTurn;										//曲がった後の判定用
																						//信号機の確認をするための変数
	float					m_fVecLightX;
	float					m_fVecLightZ;
	float					m_fDisLightZX;
	float					m_fAngleLight;

	bool                    m_nNextKey;
	int                     m_TotalKey;
	int                     m_GetKey;
	bool                    m_bMove;
	int                     m_MotionChange;
	int                     m_nCounter;
	int                     m_nCnt;

	static bool				m_bMoveUse;
};

#endif