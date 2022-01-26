//=============================================================================
//
// ゲーム画面処理 [game.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _GAME_H_
#define _GAME_H_

//=============================================================================
//マクロ定義
//=============================================================================
#define TEXT_LENGTH (128)
#define DATA_LENGTH (64)
#define MAX_DATA (224)

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "debugproc.h"
#include "trafficLight.h"

//前方宣言
class CVertex;
class CObject3D;
class CScene3D;
class CCar;
class CTrafficLight;
class CSound;
class CPlayer3D;
class CPasserby;
class CDestination;

//=============================================================================
// ゲーム画面クラスの定義
//=============================================================================
class CGame : public CScene
{
public:
	CGame();									//コンストラクタ
	~CGame();									//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//初期化処理
	void Uninit(void);											//終了処理
	void Update(void);											//更新処理
	void Draw(void);											//描画処理

	void Switch(void);											//切り替え処理

	//アドレス保存
	static CVertex		*GetVertex(void) { return m_pVertex; }
	static CPasserby	*GetPasserby(int nCnt) { return m_pPasserby[nCnt]; }
	static CDestination		*GetDeliver(int nCnt) { return m_pDeliver[nCnt]; }

	//生成したいXファイルをしらせる
	static int GetObjectType3D(void);

	//生成したい頂点数をしらせる
	static int GetVecter(void);
				
	//時間Get
	static int GetTime(void);

	//オンオフの切り替え
	static bool GetSwitchScene3D(void);
	static bool GetSwitchCollision(void);

	//パーティクルの設定をテキストから読み込む関数
	void LoadParticle(void);

	//通行人の設定をテキストから読み込む関数
	void LoadPasserby(void);

	static void GameParticle(D3DXVECTOR3 pos, int nType);

	//新たに生成したオブジェクト3Dの数を記憶
	static int GetMaxObject3D(void);

	//生成処理
	static CGame *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);

	//信号機のポインタ
	static CTrafficLight *m_pTrafficLight[MAX_LIGHT_AREA*MAX_LIGHT_POINT];

	//信号機の情報取得
	static CTrafficLight *GetTrafficLight(int nLightNum) { return m_pTrafficLight[nLightNum]; }

	//車の情報取得
	static CCar				*GetCar(int nCnt) { return m_pCar[nCnt]; }

	//プレイヤーの情報所得
	static CPlayer3D		*GetPlayer3D(void) { return m_pPlayer3D; }

	//サウンドの情報取得
	static int				GetPlayBGM(void) { return m_nPlayBGM; }
	static float			GetVolumeGameBGM(void) { return m_fVolumeBGM; }

	//サウンド設定
	void ControlBGM(void);

private:
	LPD3DXFONT	m_pTimeFontGame;						//表示用フォント
	CScene3D	*m_pScene3D[256];						//可視化生成したアドレスを記憶
	CSound		*m_pSound;								//サウンドポインタ
	float		m_fOldVolumeBGM;						//変更前のBGM

	//静的メンバ変数
	static CVertex		*m_pVertex;						//アドレス保存用ポインタ
	static CPasserby	*m_pPasserby[256];				//アドレス保存用ポインタ
	static CDestination	*m_pDeliver[3];					//アドレス保存用ポインタ
	static CPlayer3D	*m_pPlayer3D;					//アドレス保存用ポインタ
	static CCar			*m_pCar[256];					//アドレス保存用ポインタ
	static int			m_nPlayBGM;						//再生させたい番号
	static float		m_fVolumeBGM;					//ボリューム

	//パーティクル用
	static int m_nGParicleMax[DATA_LENGTH];               //パーティクルの発生数
	int m_nGParicleText;								  //何個読み込んだかのカウント
	int m_nGCountText;									  //パーティクルの数
	static float m_GSizeX[DATA_LENGTH];                   //パーティクルのサイズ
	static float m_GSizeY[DATA_LENGTH];                   //パーティクルのサイズ
	static float m_fGSpeed[DATA_LENGTH];                  //パーティクルのスピード
	static D3DXCOLOR m_GParticlecol[DATA_LENGTH];         //パーティクルのカラー
	static int m_nGLife[DATA_LENGTH];                     //パーティクルの寿命
	static float m_fGGravity[DATA_LENGTH];                //パーティクルの重力

	//通行人用変数
	int m_poscount;
	int m_writecount;
	int m_nPasserbyText;									//何個読み込んだかのカウント
	D3DXVECTOR3 m_PasserbyPos[256];							//通行人の初期場所
	D3DXVECTOR3 m_PasserbyGoalPos[256];						//通行人のゴール場所
	D3DXVECTOR3 m_PasserbyRandPos[256];						//通行人のもう一つのゴール場所
	D3DXVECTOR3 m_startpos[256];
	D3DXVECTOR3 m_goalpos[256];
	D3DXVECTOR3 m_randpos[256];
};

#endif