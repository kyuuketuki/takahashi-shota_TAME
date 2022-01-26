//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "game.h"
#include "score.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "vertex.h"
#include "player3D.h"
#include "meshfield.h"
#include "scene3D.h"
#include "debugproc.h"
#include "AIguide.h"
#include "car.h"
#include "trafficLight.h"
#include "pointerX.h"
#include "destination.h"
#include "logo.h"
#include "city.h"
#include "map.h"
#include "passerby.h"
#include "License.h"
#include "delivery.h"
#include "vertex.h"
#include "debugproc.h"
#include "animation.h"
#include "particle.h"

//=============================================================================
//マクロ定義
//=============================================================================
//リスト座標
#define DESTINATION_LIST1 (100)
#define DESTINATION_LIST2 (60)
#define DESTINATION_LIST3 (120)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
//アドレス保存静的メンバ変数宣言
CPasserby		*CGame::m_pPasserby[256];										//受け渡し用　通行人
CCar			*CGame::m_pCar[256];											//受け渡し用　車
CTrafficLight	*CGame::m_pTrafficLight[MAX_LIGHT_AREA*MAX_LIGHT_POINT] = {};	//受け渡し用　トラッキングナビ
CPlayer3D		*CGame::m_pPlayer3D;											//受け渡し用　プレイヤー
CDestination	*CGame::m_pDeliver[3];											//受け渡し用　配達先
CVertex			*CGame::m_pVertex = NULL;										//受け渡し用　頂点当たり判定

//グローバル変数宣言
D3DXVECTOR3		g_posfile[256][4];					//テキストに書き出す情報　頂点当たり判定用　pos
D3DXVECTOR3		g_posfileObject3D[256];				//テキストに書き出す情報　オブジェクト3D用　pos
D3DXVECTOR3		g_rotfileObject3D[256];				//テキストに書き出す情報　オブジェクト3D用　rot
int				g_nTypefileObject3D[256];			//テキストに書き出す情報　オブジェクト3D用　type
int				g_FileCnt = 0;						//頂点当たり判定生成カウント　TEXT用
int				g_FileCntObject3D = 0;				//オブジェクト3D生成カウント　TEXT用
int				g_nPointerGameY = 0;				//UI位置情報
int				g_nObject3DType = 0;				//オブジェクト3Dタイプ
int				g_VecCnt = 0;						//頂点当たり判定生成カウント
bool			g_bMigrate = false;					//画面遷移用
bool			g_bDeathFlagGame = false;			//死亡フラグ解除用
bool			g_bSwitchScene3D = true;			//可視化切り替えスイッチ
bool			g_bSwitchCollision = false;			//可視化切り替えスイッチ

//時間
int				nTimeGame = 0;						//時間
int				nTimeCounter = 0;					//時間カウンター
int				nTimeLimitGame = 120;				//遷移時間制限

//BGM
int				CGame::m_nPlayBGM;
float			CGame::m_fVolumeBGM;

int CGame::m_nGParicleMax[DATA_LENGTH] = {};             //パーティクルの発生数
float CGame::m_GSizeX[DATA_LENGTH] = {};
float CGame::m_GSizeY[DATA_LENGTH] = {};
float CGame::m_fGSpeed[DATA_LENGTH] = {};
D3DXCOLOR CGame::m_GParticlecol[DATA_LENGTH] = {};
int CGame::m_nGLife[DATA_LENGTH] = {};
float CGame::m_fGGravity[DATA_LENGTH] = {};

//==============================================================================
// コンストラクタ
//==============================================================================
CGame::CGame()
{
	//初期化
	nTimeGame = 0;					//時間
	nTimeCounter = 0;				//時間カウンター
	nTimeLimitGame = 120;			//タイムリミット
	g_bDeathFlagGame = false;		//死亡フラグ解除用

	//その他
	m_nPlayBGM = 1;
	m_fVolumeBGM = 1.0f;
}

//==============================================================================
// デストラクタ
//==============================================================================
CGame::~CGame()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//ゲーム起動時ライセンス初期化
	CLicense::SetLisenceType(CLicense::LISENCETYPE_GOLD);

	//テキストから読み込み関数
	LoadParticle();//パーティクル
	LoadPasserby();//通行人

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//サウンドを取得
	m_pSound = CManager::GetSound();

	//サウンドBGM
	m_pSound->Play(CSound::SOUND_LABEL_BGM_GAME);

	// フォントオブジェクトの生成
	D3DXCreateFont(pDevice, 36, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pTimeFontGame);

	//メッシュフィールド
	CMeshfield::Create();

	//パーティクルのテクスチャロード
	CParticle::Load();

	//スコア
	CScore::Create(D3DXVECTOR3(1800.0f, 50.0f, 0.0f), 30.0f, 40.0f);

	//ゲーム起動時スコア初期化
	CScore::SetScore(0);

	//頂点当たり判定の生成
	D3DXVECTOR3 pos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pVertex = CVertex::Create(pos2, pos2, pos2, pos2, pos2);

	//マップの生成
	CMap::Create();

	//3Dプレイヤーの生成
	m_pPlayer3D = CPlayer3D::Create();

	//通行人の生成
	for (int nCnt = 0; nCnt < m_nPasserbyText; nCnt++)
	{
		CPasserby::Create(m_PasserbyPos[nCnt], m_PasserbyGoalPos[nCnt], m_PasserbyRandPos[nCnt], PASSERBY_TYPE_MEN);//通行人
	}

	//お客様の生成
	CPasserby::Create(D3DXVECTOR3(-4495.0f, 70.0f, -55.0f + 150.0f), m_PasserbyGoalPos[0], m_PasserbyRandPos[0], PASSERBY_TYPE_CUSTOMER);//お客様

	//目的地
	char *TextName = "data/MODEL/goal.x";
	m_pDeliver[0] = CDestination::Create(D3DXVECTOR3(-4244.0f, 0.0f, -292.0f), TextName, 0);
	m_pDeliver[1] = CDestination::Create(D3DXVECTOR3(-12187.0f, 0.0f, 14467.0f), TextName, 1);
	m_pDeliver[2] = CDestination::Create(D3DXVECTOR3(-3563.0f, 0.0f, 22919.0f), TextName, 2);

	//目的地ポインター
	char *TextName2 = "data/MODEL/point.x";
	CPointer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), TextName2);

	//車両用ガイドの生成(可視化した時の生成物：現在はゴールと同様なのでご注意を)
	char *TextName3 = "data/MODEL/goal.x";
	CAIguide::Create(D3DXVECTOR3(0.f, 0.f, 0.f), TextName3);

	//信号機の生成(生成物の状態：生成している2台のトラックの通行箇所のみの設置)
	for (int nCntArea = 0; nCntArea < MAX_LIGHT_AREA; nCntArea++)
	{
		for (int nCntPoint = 0; nCntPoint < MAX_LIGHT_POINT; nCntPoint++)
		{
			m_pTrafficLight[nCntArea*nCntPoint + nCntPoint] = CTrafficLight::Create(D3DXVECTOR3(0.f, 0.f, 0.f), nCntArea, nCntPoint);
		}
	}

	//車両の生成(現在の生成物：トラック＋スーパーカー)
	m_pCar[0] = CCar::Create(D3DXVECTOR3(-2025.39f, 0.0f, -1746.39f), 0, CARTYPE_TRUCK);
	m_pCar[1] = CCar::Create(D3DXVECTOR3(5739.1f, 0.0f, -9422.22f), 1, CARTYPE_SUPER);

	//UI生成
	{
		//ナビ背景
		CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 750.0f, SCREEN_HEIGHT / 2 + 330.0f, 0.0f), 160, 120, LIST);

		//ナビフレーム
		CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 770.0f, SCREEN_HEIGHT / 2 + 295.0f, 0.0f), 100, 30, GAMEFRAME);

		//ルール説明
		CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 680.0f, SCREEN_HEIGHT / 2 + 300.0f, 0.0f), 260, 100, GAMR_UI01);
	}

	//免許書
	CLicense::Create(D3DXVECTOR3(SCREEN_WIDTH - 200, 200, 0.0f), 150, 90);

	// カウント
	CAnimasion::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 150, 150, 60, 2);

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CGame::Uninit(void)
{
	//サウンド設置
	m_pSound->Stop(CSound::SOUND_LABEL_BGM_GAME);
	m_nPlayBGM = 0;

	/*for (int nCnt = 0; nCnt < 256; nCnt++)
	{
		if (m_pPasserby[nCnt] != NULL)
		{
			m_pPasserby[nCnt]->Uninit();
			m_pPasserby[nCnt] = NULL;
		}
	}*/

	// オブジェクト破棄
	Release();
}

//==============================================================================
// 更新処理
//==============================================================================
void CGame::Update(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//ゲームパッドのポインタ宣言
	CGamepad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//スイッチ処理(ツール)
	Switch();

	//デバック用
#ifdef _DEBUG
	//画面遷移
	if (pInputKeyboard->GetTrigger(DIK_M) == true)
	{
		//モード設定
		CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);	// シーン遷移
	}
#endif

	//画面遷移
	if (nTimeLimitGame <= 0)
	{
		//モード設定
		CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);	// シーン遷移
	}

	//カウンター時間
	if (nTimeCounter >= 60)
	{
		nTimeLimitGame--;
		nTimeCounter = 0;	//カウンター
	}

	//カウンター進める
	nTimeCounter++;

	//UI更新処理
	{
		//フレームをずらす
		if (pInputKeyboard->GetTrigger(DIK_H) == true || pGamepad->IsButtonDown(CInput::A) == true)
		{
			if (g_nPointerGameY == 0)
			{
				CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 + 770.0f, SCREEN_HEIGHT / 2 + 295.0f + DESTINATION_LIST2, 0.0f), GAMEFRAME);//1
				g_nPointerGameY = 1;
			}

			else if (g_nPointerGameY == 1)
			{
				CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 + 770.0f, SCREEN_HEIGHT / 2 + 295.0f + DESTINATION_LIST3, 0.0f), GAMEFRAME);//2
				g_nPointerGameY = 2;
			}

			else if (g_nPointerGameY == 2)
			{
				CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 + 770.0f, SCREEN_HEIGHT / 2 + 295.0f, 0.0f), GAMEFRAME);//0
				g_nPointerGameY = 0;
			}

			//ナビを切り替える
			CPointer::SetPointNumber(g_nPointerGameY);
		}
	}

	//BGM更新処理
	ControlBGM();
}

//==============================================================================
// 描画処理
//==============================================================================
void CGame::Draw(void)
{
	// ローカル変数宣言
	RECT rect = { 0,30,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	//画面に表示させる
	sprintf(&aStr[0], "残り時間 : %d\n", nTimeLimitGame);

	// テキストの描画　残り時間の表示
	m_pTimeFontGame->DrawText(NULL, &aStr[0], -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//==============================================================================
// 切り替え処理
//==============================================================================
void CGame::Switch(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//プレイヤーの情報を読み込む
	D3DXVECTOR3 posPlayer3D = CPlayer3D::GetPosition();
	D3DXVECTOR3 rotPlayer3D = CPlayer3D::GetRotation();

	//デバック用
#ifdef _DEBUG
	//当たり判定オンオフ
	//キーボードQを押した場合
	if (pInputKeyboard->GetTrigger(DIK_O) == true)
	{
		//可視化スイッチ切り替え
		if (g_bSwitchScene3D == false)
		{
			g_bSwitchScene3D = true;
		}

		else if (g_bSwitchScene3D == true)
		{
			g_bSwitchScene3D = false;
		}
	}
#endif
}

//==============================================================================
// Get関数タイム
//==============================================================================
int CGame::GetTime(void)
{
	return nTimeGame;
}

//==============================================================================
// Get関数 生成したオブジェクト3D　表示用
//==============================================================================
int CGame::GetObjectType3D(void)
{
	return g_nObject3DType;
}

//==============================================================================
// Get関数 生成した頂点数　表示用
//==============================================================================
int CGame::GetVecter(void)
{
	return g_VecCnt;
}

//==============================================================================
// Get関数 可視化切り替えスイッチ
//==============================================================================
bool CGame::GetSwitchScene3D(void)
{
	return g_bSwitchScene3D;
}

//==============================================================================
// Get関数 当たり判定切り替えスイッチ
//==============================================================================
bool CGame::GetSwitchCollision(void)
{
	return g_bSwitchCollision;
}

//==============================================================================
// Get関数 新に設置したオブジェクト3Dの数
//==============================================================================
int CGame::GetMaxObject3D(void)
{
	return g_FileCntObject3D;
}

//==============================================================================
// 生成処理
//==============================================================================
CGame * CGame::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ポインタ変数の生成
	CGame *pGame;
	pGame = new CGame;

	// NULLチェック
	if (pGame != NULL)
	{
		pGame->Init(pos, fSizeX, fSizeY);
	}

	return pGame;
}

//==============================================================================
//BGM音量操作処理
//==============================================================================
void CGame::ControlBGM(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_F1) == true)
	{//サウンドを再生・一時停止
		if (m_nPlayBGM == 2)
		{
			m_nPlayBGM = 1;
		}
		else if (m_nPlayBGM == 1)
		{
			m_nPlayBGM = 2;
		}
		else if (m_nPlayBGM == 0)
		{
			m_nPlayBGM = 1;
			m_pSound->Play(CSound::SOUND_LABEL_BGM_GAME);
			m_pSound->Pause(CSound::SOUND_LABEL_BGM_GAME);
		}

		if (m_nPlayBGM == 1)
		{
			m_pSound->PlayMiddle(CSound::SOUND_LABEL_BGM_GAME);
		}
		else if (m_nPlayBGM == 2)
		{
			m_pSound->Pause(CSound::SOUND_LABEL_BGM_GAME);
		}
	}
	if (pInputKeyboard->GetTrigger(DIK_F2) == true)
	{//サウンドを再生・停止
		if (m_nPlayBGM == 0)
		{
			m_nPlayBGM = 1;
		}
		else if (m_nPlayBGM == 1)
		{
			m_nPlayBGM = 0;
		}
		else if (m_nPlayBGM == 2)
		{
			m_nPlayBGM = 1;
		}

		if (m_nPlayBGM == 1)
		{
			m_pSound->Play(CSound::SOUND_LABEL_BGM_GAME);
		}
		else if (m_nPlayBGM == 0)
		{
			m_pSound->Stop(CSound::SOUND_LABEL_BGM_GAME);
		}
	}

	if (pInputKeyboard->GetPress(DIK_F3) == true)
	{
		m_fVolumeBGM += 0.005f;
		if (m_fVolumeBGM >= 1.0f)
		{
			m_fVolumeBGM = 1.0f;
		}
	}

	else if (pInputKeyboard->GetPress(DIK_F4) == true)
	{
		m_fVolumeBGM -= 0.005f;
		if (m_fVolumeBGM <= 0.0f)
		{
			m_fVolumeBGM = 0.0f;
		}
	}
	else if (pInputKeyboard->GetTrigger(DIK_F5) == true)
	{
		if (m_fVolumeBGM != 0.0f)
		{
			m_fOldVolumeBGM = m_fVolumeBGM;
			m_fVolumeBGM = 0.0f;
		}
		else if (m_fVolumeBGM == 0.0f)
		{
			m_fVolumeBGM = m_fOldVolumeBGM;
		}
	}
	m_pSound->SetVolume(CSound::SOUND_LABEL_BGM_GAME, m_fVolumeBGM);
}
//===========================================================================
//パーティクルの設定をテキストから読み込み
//===========================================================================
void CGame::LoadParticle(void)
{
	FILE *pFile = NULL;

	char aEqual[2] = { NULL };					// 「=」読み取り用変数
	bool bComment = false;						// コメントアウトするか
	char aText[128] = { NULL };			// テキスト読み取り用変数
	char aSetObject[DATA_LENGTH] = { NULL };	// テキスト読み取り用変数
	char aSetObjectEND[DATA_LENGTH] = { NULL };	// テキスト読み取り用変数

												// strcmp読み取り用ポインタ
	char *pText = &aText[0];
	char *pSetObject = &aSetObject[0];
	char *pSetObjectEND = &aSetObjectEND[0];

	pFile = fopen("data/TEXT/ParticleSet.txt", "r");

	aText[0] = { NULL };
	pSetObject[0] = { NULL };
	pSetObjectEND[0] = { NULL };

	while (strcmp("END_SCRIPT", pText) != 0)
	{
		aText[0] = { NULL };

		if (bComment == false)
		{
			// コメントアウトしていない
			fscanf(pFile, "%s", &aText[0]);

			if (aText[0] == '#')
			{
				// 文字列の先頭が「#」ならばコメントアウトへ
				bComment = true;
			}

			else
			{
				if (strcmp("PARTICLECOUNTSET", aText) == 0)
				{
					pSetObjectEND[0] = { NULL };

					while (strcmp("END_PARTICLECOUNTSET", pSetObjectEND) != 0)
					{
						fscanf(pFile, "%s", &pSetObjectEND[0]);

						if (strcmp("Size", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_GSizeX[m_nGParicleText]);
							fscanf(pFile, "%f", &m_GSizeY[m_nGParicleText]);
						}
						if (strcmp("Speed", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_fGSpeed[m_nGParicleText]);
						}
						if (strcmp("Col", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_GParticlecol[m_nGParicleText].r);
							fscanf(pFile, "%f", &m_GParticlecol[m_nGParicleText].g);
							fscanf(pFile, "%f", &m_GParticlecol[m_nGParicleText].b);
							fscanf(pFile, "%f", &m_GParticlecol[m_nGParicleText].a);
						}
						if (strcmp("Life", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_nGLife[m_nGParicleText]);
						}
						if (strcmp("Gravity", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_fGGravity[m_nGParicleText]);
						}
						if (strcmp("ParticleMax", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_nGParicleMax[m_nGParicleText]);
						}
					}
					//カウントアップ
					m_nGParicleText++;
				}
			}
		}
		else if (bComment == true)
		{
			// コメントアウト処理
			// ローカル変数宣言
			char a = NULL;
			char b = NULL;
			fscanf(pFile, "%c", &a);
			while (a != '\n' && b != '\n')
			{
				fscanf(pFile, "%s", &aText[0]);
				fscanf(pFile, "%c", &b);
			}
			bComment = false;
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}
//===========================================================================
//パーティクルの設定
//===========================================================================
void CGame::GameParticle(D3DXVECTOR3 pos, int nType)
{
	for (int nCnt = 0; nCnt < m_nGParicleMax[nType]; nCnt++)
	{
		//パーティクル発生
		CParticle::Create(pos, m_GSizeX[nType], m_GSizeY[nType], m_fGSpeed[nType], m_GParticlecol[nType], m_nGLife[nType], m_fGGravity[nType]);
	}
}

//===========================================================================
//通行人の設定をテキストから読み込み
//===========================================================================
void CGame::LoadPasserby(void)
{
	FILE *pFile = NULL;

	char aEqual[2] = { NULL };					// 「=」読み取り用変数
	bool bComment = false;						// コメントアウトするか
	char aText[228] = { NULL };			// テキスト読み取り用変数
	char aSetObject[256] = { NULL };	// テキスト読み取り用変数
	char aSetObjectEND[256] = { NULL };	// テキスト読み取り用変数

												// strcmp読み取り用ポインタ
	char *pText = &aText[0];
	char *pSetObject = &aSetObject[0];
	char *pSetObjectEND = &aSetObjectEND[0];

	pFile = fopen("data/TEXT/GetPasserby.txt", "r");

	aText[0] = { NULL };
	pSetObject[0] = { NULL };
	pSetObjectEND[0] = { NULL };

	while (strcmp("END_SCRIPT", pText) != 0)
	{
		aText[0] = { NULL };

		if (bComment == false)
		{
			// コメントアウトしていない
			fscanf(pFile, "%s", &aText[0]);

			if (aText[0] == '#')
			{
				// 文字列の先頭が「#」ならばコメントアウトへ
				bComment = true;
			}

			else
			{
				if (strcmp("PASSERBYCOUNTSET", aText) == 0)
				{
					pSetObjectEND[0] = { NULL };

					while (strcmp("END_PASEERBYCOUNTSET", pSetObjectEND) != 0)
					{
						fscanf(pFile, "%s", &pSetObjectEND[0]);

						if (strcmp("Pos", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_PasserbyPos[m_nPasserbyText].x);
							fscanf(pFile, "%f", &m_PasserbyPos[m_nPasserbyText].y);
							fscanf(pFile, "%f", &m_PasserbyPos[m_nPasserbyText].z);
						}
						if (strcmp("GoalPos", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_PasserbyGoalPos[m_nPasserbyText].x);
							fscanf(pFile, "%f", &m_PasserbyGoalPos[m_nPasserbyText].y);
							fscanf(pFile, "%f", &m_PasserbyGoalPos[m_nPasserbyText].z);
						}
						if (strcmp("RandPos", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_PasserbyRandPos[m_nPasserbyText].x);
							fscanf(pFile, "%f", &m_PasserbyRandPos[m_nPasserbyText].y);
							fscanf(pFile, "%f", &m_PasserbyRandPos[m_nPasserbyText].z);
						}
					}
					//カウントアップ
					m_nPasserbyText++;
				}
			}
		}
		else if (bComment == true)
		{
			// コメントアウト処理
			// ローカル変数宣言
			char a = NULL;
			char b = NULL;
			fscanf(pFile, "%c", &a);
			while (a != '\n' && b != '\n')
			{
				fscanf(pFile, "%s", &aText[0]);
				fscanf(pFile, "%c", &b);
			}
			bComment = false;
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}