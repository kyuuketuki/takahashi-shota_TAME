//=============================================================================
//
// ゲーム画面2処理 [game2.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
// インクルード読み込み
//=============================================================================
#include "game2.h"
#include "bg.h"
#include "player.h"
#include "score.h"
#include "enemy.h"
#include "polygon.h"
#include "life.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "bullet.h"
#include "item.h"
#include "animation.h"
#include "logo.h"
#include "fade.h"

//静的メンバ変数宣言
CPlayer *CGame2::m_pPlayer = NULL;		//プレイヤーポインター

//グローバル変数宣言
int nTimeGame2 = 0;						//タイム
int nTimeCounter2 = 0;					//タイムカウンター
int g_nAttackCnt = 0;					//攻撃用変数
int g_nAttackCnt2 = 0;					//攻撃用変数
int g_nAttackCnt3 = 0;					//攻撃用変数
float g_fAngle = 0.0f;					//攻撃用変数角度
float g_fAngle2 = 0.0f;					//攻撃用変数角度
float g_fAngle3 = 0.0f;					//攻撃用変数角度
bool g_bAngle = false;					//攻撃用変数角度
bool g_bStage = false;					//ステージ変更
bool g_bCountReset = false;				//ステージ変更

//死亡フラグ解除用
bool g_bDeathFlagGame2 = false;

//==============================================================================
// コンストラクタ
//==============================================================================
CGame2::CGame2()
{
	//初期化
	nTimeGame2 = 0;
	nTimeCounter2 = 0;
	g_nAttackCnt = 0;
	g_nAttackCnt2 = 0;
	g_nAttackCnt3 = 0;
	g_fAngle = 0.0f;
	g_fAngle2 = 0.0f;
	g_fAngle3 = 0.0f;
	g_bAngle = false;
	g_bStage = false;
	g_bCountReset = false;
	g_bDeathFlagGame2 = false;
}

//==============================================================================
// デストラクタ
//==============================================================================
CGame2::~CGame2()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CGame2::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//サウンドBGM
	pSound->Play(CSound::SOUND_LABEL_BGM004);

	//プレイヤーのレベルを初期化
	CPlayer::SetBulletLevel(0);

	// フォントオブジェクトの生成
	D3DXCreateFont(pDevice, 36, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pTimeFontGame);

	//現在のステージをManaagerに記憶させる
	CManager::SetGameStage(2);

	//背景を表示
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	//アニメーション背景を表示
	CAnimasionBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 10, 17);

	//プレイヤーの生成
	CPlayer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), MAX_LIFE, PLAYER_SIZE, PLAYER_SIZE);

	//特殊攻撃をオフにする
	CEnemy::NextAttack(0);

	//HP5固定にする
	CManager::SetMaxPlayerLife(5);

	//HPバー(初期５)
	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		CLife::Create(D3DXVECTOR3(40.0f + (80.0f * nCnt), 100.0f, 0.0f), 30.0f, 30.0f);
	}

	//スコア
	CScore::Create(D3DXVECTOR3(1800.0f, 50.0f, 0.0f), 100.0f, 100.0f);

	//クリア条件
	CManager::SetEnemyDead(0);

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CGame2::Uninit(void)
{
	//サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//BGMを止める
	pSound->Stop(CSound::SOUND_LABEL_BGM004);

	// オブジェクト破棄
	Release();
}

//==============================================================================
// 更新処理
//==============================================================================
void CGame2::Update(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//時間を進める
	nTimeCounter2++;

	//カウンターが60以上になったら
	if (nTimeCounter2 >= 60)
	{
		//秒数を進める
		nTimeGame2++;

		//カウンターを戻す
		nTimeCounter2 = 0;
	}

	//敵の倒した数
	int nDeatCnt = CManager::GetEnemyDead();
	
	if (g_bDeathFlagGame2 == true)
	{
		//クリア条件
		if (nDeatCnt == 3)
		{
			CManager::SetGameClear(true);

			//画面遷移
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));	// シーン遷移
		}
	}

	//更新をして位置フレーム待つ（デットフラグのため）
	if (g_bDeathFlagGame2 == false)
	{
		//クリア条件
		CManager::SetEnemyDead(0);

		g_bDeathFlagGame2 = true;
	}

	//ボスハンド二体倒した場合
	if (nDeatCnt == 2)
	{
		//回復アイテム落下
		if (g_bStage == false)
		{
			//攻撃処理
			Attack2();
		}

		//ボス戦最終　攻撃変化
		if (g_bStage == true)
		{
			//攻撃処理
			Attack3();
		}
	}

	//初めの攻撃
	else
	{
		//タイムに合わせて敵を生成
		if (nTimeGame2 == 1 && nTimeCounter2 == 0)
		{
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, -300.0f, 0.0f), 8.0f, D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 7000, ENEMY_SIZE + 300, ENEMY_SIZE + 200, 1000, ENEMY_BOSS_01, 0);
			CEnemy::Create(D3DXVECTOR3(0.0f, -300.0f, 0.0f), 8.0f, D3DXVECTOR3(SCREEN_WIDTH / 2 - 300, 400.0f, 0.0f), 1500, ENEMY_SIZE + 50, ENEMY_SIZE + 50, 1000, ENEMY_BOSS_02, 5);
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH, -300.0f, 0.0f), 8.0f, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, 400.0f, 0.0f), 1500, ENEMY_SIZE + 50, ENEMY_SIZE + 50, 1000, ENEMY_BOSS_03, 6);		
		}

		//攻撃処理
		Attack();
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void CGame2::Draw(void)
{
#ifdef _DEBUG
	// ローカル変数宣言
	RECT rect = { -1500,30,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	// 画面に表示させる
	//sprintf(&aStr[0], "GAME2画面の経過時間 : %d\n", nTimeGame2);

	//画面に表示させる
	sprintf(&aStr[0], "GAME画面の経過時間 : %d\n", CManager::GetEnemyDead());

	// テキストの描画
	m_pTimeFontGame->DrawText(NULL, &aStr[0], -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
#endif
}

//==============================================================================
// 攻撃＆行動処理
//==============================================================================
//ボスハンド攻撃
void CGame2::Attack(void)
{
	D3DXVECTOR3 posPlayer = m_pPlayer->GetPlayer();

	//boss攻撃
	if (nTimeGame2 >= 4 && nTimeGame2 <= 65 && nTimeCounter2 == 0)
	{
		CBullet::InitAimingBullet(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 8.0f, D3DXVECTOR3(posPlayer.x, posPlayer.y, 0.0f));
	}

	//移動 && 攻撃
	if (nTimeGame2 == 4 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjective(posPlayer, 10.0f, ENEMY_BOSS_02);
	}

	if (nTimeGame2 == 6 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjectiveAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300, 400.0f, 0.0f), 5.0f, ENEMY_BOSS_02);
	}

	if (nTimeGame2 == 9 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjective(posPlayer, 10.0f, ENEMY_BOSS_03);
	}

	if (nTimeGame2 == 11 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjectiveAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, 400.0f, 0.0f), 5.0f, ENEMY_BOSS_03);
	}

	//特殊攻撃に変更
	if (nTimeGame2 == 16 && nTimeCounter2 == 0)
	{
		CEnemy::NextAttack(1);
	}

	//攻撃レーザー
	if (nTimeGame2 == 14 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjective(D3DXVECTOR3(300.0f, 150.0f, 0.0f), 6.0f, ENEMY_BOSS_02);
	}

	if (nTimeGame2 == 14 && nTimeCounter2 == 1)
	{
		CEnemy::NextObjective(D3DXVECTOR3(SCREEN_WIDTH / 2, 150.0f, 0.0f), 6.0f, ENEMY_BOSS_03);
	}

	if (nTimeGame2 == 16 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjective(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, 150.0f, 0.0f), 3.0f, ENEMY_BOSS_02);
	}

	if (nTimeGame2 == 16 && nTimeCounter2 == 1)
	{
		CEnemy::NextObjective(D3DXVECTOR3(SCREEN_WIDTH - 50.0f, 150.0f, 0.0f), 3.0f, ENEMY_BOSS_03);
	}

	if (nTimeGame2 == 20 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjective(D3DXVECTOR3(50.0f, 150.0f, 0.0f), 3.0f, ENEMY_BOSS_02);
	}

	if (nTimeGame2 == 20 && nTimeCounter2 == 1)
	{
		CEnemy::NextObjective(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300.0f, 150.0f, 0.0f), 3.0f, ENEMY_BOSS_03);
	}

	//基本攻撃に変更
	if (nTimeGame2 == 27 && nTimeCounter2 == 0)
	{
		CEnemy::NextAttack(0);
	}

	//初期位置に移動
	if (nTimeGame2 == 27 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjective(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300, 400.0f, 0.0f), 5.0f, ENEMY_BOSS_02);
	}

	if (nTimeGame2 == 27 && nTimeCounter2 == 1)
	{
		CEnemy::NextObjective(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, 400.0f, 0.0f), 5.0f, ENEMY_BOSS_03);
	}

	//boss攻撃2
	if (nTimeGame2 >= 27 && nTimeGame2 <= 65 && nTimeCounter2 == 0)
	{
		if (g_bAngle == false)
		{
			int nRandAngle = rand() % 78;
			g_fAngle = (float)nRandAngle;
			g_bAngle = true;
			CBullet::InitAngleBullet(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle, OBJTYPE_BULET_ENEMY);
			CBullet::InitAngleBullet(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 157.0f, OBJTYPE_BULET_ENEMY);
			CBullet::InitAngleBullet(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 314.0f, OBJTYPE_BULET_ENEMY);
			CBullet::InitAngleBullet(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 471.0f, OBJTYPE_BULET_ENEMY);
		}

		else if (g_bAngle == true)
		{
			int nRandAngle = rand() % 78;
			g_fAngle = (float)nRandAngle;
			g_bAngle = false;
			CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle, OBJTYPE_BULET_ENEMY, 6);
			CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 157.0f, OBJTYPE_BULET_ENEMY, 6);
			CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 314.0f, OBJTYPE_BULET_ENEMY, 6);
			CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 471.0f, OBJTYPE_BULET_ENEMY, 6);
		}
	}

	//移動 && 攻撃
	if (nTimeGame2 == 29 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjective(posPlayer, 10.0f, ENEMY_BOSS_02);
	}

	if (nTimeGame2 == 31 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjectiveAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300, 400.0f, 0.0f), 8.0f, ENEMY_BOSS_02);
	}

	if (nTimeGame2 == 33 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjective(posPlayer, 10.0f, ENEMY_BOSS_03);
	}

	if (nTimeGame2 == 35 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjectiveAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, 400.0f, 0.0f), 8.0f, ENEMY_BOSS_03);
	}

	if (nTimeGame2 == 37 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjective(posPlayer, 10.0f, ENEMY_BOSS_02);
	}

	if (nTimeGame2 == 38 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjectiveAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300, 400.0f, 0.0f), 8.0f, ENEMY_BOSS_02);
	}

	if (nTimeGame2 == 40 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjective(posPlayer, 10.0f, ENEMY_BOSS_03);
	}

	if (nTimeGame2 == 42 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjectiveAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, 400.0f, 0.0f), 8.0f, ENEMY_BOSS_03);
	}

	//特殊攻撃に変更
	if (nTimeGame2 == 46 && nTimeCounter2 == 0)
	{
		CEnemy::NextAttack(1);
	}

	//攻撃レーザー
	if (nTimeGame2 == 44 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjective(D3DXVECTOR3(300.0f, 150.0f, 0.0f), 6.0f, ENEMY_BOSS_02);
	}

	if (nTimeGame2 == 44 && nTimeCounter2 == 1)
	{
		CEnemy::NextObjective(D3DXVECTOR3(SCREEN_WIDTH / 2, 150.0f, 0.0f), 6.0f, ENEMY_BOSS_03);
	}

	if (nTimeGame2 == 47 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjective(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, 150.0f, 0.0f), 3.0f, ENEMY_BOSS_02);
	}

	if (nTimeGame2 == 47 && nTimeCounter2 == 1)
	{
		CEnemy::NextObjective(D3DXVECTOR3(SCREEN_WIDTH - 50.0f, 150.0f, 0.0f), 3.0f, ENEMY_BOSS_03);
	}

	if (nTimeGame2 == 50 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjective(D3DXVECTOR3(50.0f, 150.0f, 0.0f), 3.0f, ENEMY_BOSS_02);
	}

	if (nTimeGame2 == 50 && nTimeCounter2 == 1)
	{
		CEnemy::NextObjective(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300.0f, 150.0f, 0.0f), 3.0f, ENEMY_BOSS_03);
	}

	//基本攻撃に変更
	if (nTimeGame2 == 53 && nTimeCounter2 == 0)
	{
		CEnemy::NextAttack(0);
	}

	//初期位置に移動
	if (nTimeGame2 == 53 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjective(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300, 400.0f, 0.0f), 5.0f, ENEMY_BOSS_02);
	}

	if (nTimeGame2 == 53 && nTimeCounter2 == 1)
	{
		CEnemy::NextObjective(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, 400.0f, 0.0f), 5.0f, ENEMY_BOSS_03);
	}

	//ループ
	if (nTimeGame2 == 54 && nTimeCounter2 == 0)
	{
		nTimeGame2 = 28;
		nTimeCounter2 = 0;
	}
}

//回復アイテム出現
void CGame2::Attack2(void)
{
	if (g_bCountReset == false)
	{
		g_bCountReset = true;

		//カウントをゼロにする
		nTimeGame2 = 0;
		nTimeCounter2 = 0;
	}

	if (nTimeGame2 == 1 && nTimeCounter2 == 0)
	{
		//アイテム生成
		CItem::Create(D3DXVECTOR3(SCREEN_WIDTH - 200.0f, 0.0f, 0.0f), 2.0f, 50, 50, 1);
		CItem::Create(D3DXVECTOR3(SCREEN_WIDTH - 500.0f, 0.0f, 0.0f), 2.0f, 50, 50, 1);
		CItem::Create(D3DXVECTOR3(SCREEN_WIDTH - 700.0f, 0.0f, 0.0f), 2.0f, 50, 50, 1);

		CItem::Create(D3DXVECTOR3(200.0f, 0.0f, 0.0f), 2.0f, 50, 50, 1);
		CItem::Create(D3DXVECTOR3(500.0f, 0.0f, 0.0f), 2.0f, 50, 50, 1);
		CItem::Create(D3DXVECTOR3(700.0f, 0.0f, 0.0f), 2.0f, 50, 50, 1);
	}

	if (nTimeGame2 >= 3 && nTimeCounter2 == 0)
	{
		g_bCountReset = false;
		g_bStage = true;
	}
}

//最終攻撃
void CGame2::Attack3(void)
{
	if(g_bCountReset == false)
	{ 
		g_bCountReset = true;

		//カウントをゼロにする
		nTimeGame2 = 0;
		nTimeCounter2 = 0;
	}

	//プレイヤーの位置を取得
	D3DXVECTOR3 posPlayer = m_pPlayer->GetPlayer();

	//攻撃
	if (nTimeGame2 == 3 && nTimeCounter2 == 0)
	{
		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 1.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, (float)BULLET_SIZE, (float)BULLET_SIZE, 62.8f * nCnt, OBJTYPE_BULET_ENEMY, 1);
			CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 2.0f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, 62.8f * nCnt, OBJTYPE_BULET_ENEMY, 2);
			CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 3.0f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, 62.8f * nCnt, OBJTYPE_BULET_ENEMY, 2);
			CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 4.0f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, 62.8f * nCnt, OBJTYPE_BULET_ENEMY, 2);
		}
	}

	if (nTimeGame2 == 5 && nTimeCounter2 == 0)
	{
		//止める
		CBullet::NextMove(1, 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		CBullet::NextMove2(2, 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (nTimeGame2 == 6 && nTimeCounter2 == 0)
	{
		//目的地に向かって発射
		CBullet::NextAimingBullet(1, 2, posPlayer, 10.0f);
	}

	if (nTimeGame2 == 6 && nTimeCounter2 == 30)
	{
		//目的地に向かって発射
		CBullet::NextAimingBullet2(2, 2, posPlayer, 15.0f);
	}

	//生成しながら移動
	if (nTimeGame2 == 7 && nTimeCounter2 == 0)
	{
		//生成しながら発射
		CBullet::Create2(D3DXVECTOR3(200.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 5.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 100, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY3, 3);
		CBullet::Create2(D3DXVECTOR3(500.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 7.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 140, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY3, 3);
		CBullet::Create2(D3DXVECTOR3(700.0f, 0.0f, 0.0f), D3DXVECTOR3(-2.0f, 5.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 120, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY3, 3);
		CBullet::Create2(D3DXVECTOR3(1000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 50, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY3, 3);
		CBullet::Create2(D3DXVECTOR3(1300.0f, 0.0f, 0.0f), D3DXVECTOR3(2.0f, 7.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 140, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY3, 3);
		CBullet::Create2(D3DXVECTOR3(1800.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 5.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 140, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY3, 3);

		CBullet::Create2(D3DXVECTOR3(250.0f, SCREEN_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, -5.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 80, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY3, 4);
		CBullet::Create2(D3DXVECTOR3(900.0f, SCREEN_HEIGHT, 0.0f), D3DXVECTOR3(1.0f, -7.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 100, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY3, 4);
		CBullet::Create2(D3DXVECTOR3(1100.0f, SCREEN_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, -5.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 100, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY3, 4);
		CBullet::Create2(D3DXVECTOR3(1600.0f, SCREEN_HEIGHT, 0.0f), D3DXVECTOR3(-1.0f, -7.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 80, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY3, 4);
	}

	if (nTimeGame2 == 9 && nTimeCounter2 == 0)
	{
		//移動
		CBullet::NextMove(3, 1, D3DXVECTOR3(3.0f, 0.0f, 0.0f));
		CBullet::NextMove2(4, 1, D3DXVECTOR3(-3.0f, 0.0f, 0.0f));		
	}

	//螺旋発射
	if (nTimeGame2 >= 15 && nTimeGame2 < 22)
	{
		g_nAttackCnt++;
		g_fAngle += 3;

		if (g_nAttackCnt == 5)
		{
			CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 4.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle, OBJTYPE_BULET_ENEMY, 5);
			CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 4.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 157.0f, OBJTYPE_BULET_ENEMY, 5);
			CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 4.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 314.0f, OBJTYPE_BULET_ENEMY, 5);
			CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 4.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle +471.0f, OBJTYPE_BULET_ENEMY, 5);
			g_nAttackCnt = 0;
		}
	}

	if (nTimeGame2 == 22 && nTimeCounter2 == 0)
	{
		//移動
		CBullet::NextMove(5, 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (nTimeGame2 == 23 && nTimeCounter2 == 0)
	{
		//目的地に向かって発射
		CBullet::NextAimingBullet(5, 2, D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 5.0f);
	}

	if (nTimeGame2 == 26 && nTimeCounter2 == 0)
	{
		//移動
		CBullet::NextMove(5, 1, D3DXVECTOR3(0.0f, 5.0f, 0.0f));
	}

	//十字
	if (nTimeGame2 >= 28 && nTimeGame2 < 40)
	{
		g_nAttackCnt++;
		
		if (g_nAttackCnt == 30)
		{
			if (g_bAngle == false)
			{
				int nRandAngle = rand() % 78;
				g_fAngle = (float)nRandAngle;
				g_bAngle = true;
				CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 157.0f, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 314.0f, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 471.0f, OBJTYPE_BULET_ENEMY, 6);

				CBullet::InitAngleBullet2(D3DXVECTOR3(300.0f, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(300.0f, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 157.0f, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(300.0f, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 314.0f, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(300.0f, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 471.0f, OBJTYPE_BULET_ENEMY, 6);

				CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 157.0f, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 314.0f, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 471.0f, OBJTYPE_BULET_ENEMY, 6);
			}

			else if (g_bAngle == true)
			{
				int nRandAngle = rand() % 78;
				g_fAngle = (float)nRandAngle;
				g_bAngle = false;
				CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 157.0f, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 314.0f, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 471.0f, OBJTYPE_BULET_ENEMY, 6);

				CBullet::InitAngleBullet2(D3DXVECTOR3(300.0f, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(300.0f, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 157.0f, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(300.0f, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 314.0f, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(300.0f, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 471.0f, OBJTYPE_BULET_ENEMY, 6);

				CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 157.0f, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 314.0f, OBJTYPE_BULET_ENEMY, 6);
				CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 300.0f, 0.0f), 8.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 471.0f, OBJTYPE_BULET_ENEMY, 6);
			}

			g_nAttackCnt = 0;
		}
	}

	if (nTimeGame2 >= 29 && nTimeCounter2 == 0 || nTimeGame2 >= 29 && nTimeCounter2 == 40)
	{
		//移動
		CBullet::NextMove(6, 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (nTimeGame2 >= 29 && nTimeCounter2 == 20 || nTimeGame2 >= 29 && nTimeCounter2 == 60)
	{
		//移動
		CBullet::NextMove(99, 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	//回転制限
	if (g_fAngle >= 628)
	{
		g_fAngle = g_fAngle - 628;
	}

	if (g_fAngle <= -628)
	{
		g_fAngle = g_fAngle + 628;
	}

	//螺旋発射
	if (nTimeGame2 >= 45 && nTimeGame2 < 70)
	{
		g_nAttackCnt++;
		g_fAngle += 11;

		if (g_nAttackCnt == 4)
		{
			CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 3.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle, OBJTYPE_BULET_ENEMY, 5);
			CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 3.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 10, OBJTYPE_BULET_ENEMY, 5);
			CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 3.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE, BULLET_SIZE, g_fAngle + 20, OBJTYPE_BULET_ENEMY, 5);
			g_nAttackCnt = 0;
		}
	}

	//螺旋発射
	if (nTimeGame2 >= 50 && nTimeGame2 < 70)
	{
		g_nAttackCnt2++;
		g_fAngle2 += 3;

		if (g_nAttackCnt2 == 10)
		{
			CBullet::InitAngleBullet2(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 2.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 2000, 0, BULLET_SIZE + 20, BULLET_SIZE + 20, g_fAngle2, OBJTYPE_BULET_ENEMY, 5);
			g_nAttackCnt2 = 0;
		}
	}

	//カウントをリセット
	if (nTimeGame2 == 70 && nTimeCounter2 == 0)
	{
		nTimeGame2 = 0;
		nTimeCounter2 = 0;
	}
}

//==============================================================================
// Get関数　時間
//==============================================================================
int CGame2::GetTime(void)
{
	return nTimeGame2;
}

//==============================================================================
// 生成処理
//==============================================================================
CGame2 * CGame2::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ポインタ変数の生成
	CGame2 *pGame2;
	pGame2 = new CGame2;

	// NULLチェック
	if (pGame2 != NULL)
	{
		pGame2->Init(pos, fSizeX, fSizeY);
	}

	return pGame2;
}
