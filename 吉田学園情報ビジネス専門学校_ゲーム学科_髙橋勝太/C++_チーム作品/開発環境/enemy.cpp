//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "bullet.h"
#include "polygon.h"
#include "sound.h"
#include "scene.h"
#include "player.h"
#include "game.h"
#include "explosion.h"
#include "game.h"
#include "game2.h"
#include "particle.h"

// 静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[ENEMY_MAX] = {};
CPlayer *CEnemy::m_pPlayer = NULL;

//目的地場所
D3DXVECTOR3 g_objective2;
D3DXVECTOR3 g_objective3;
D3DXVECTOR3 g_objective4;

//移動させたい番号
int g_nType2 = NULL;
int g_nType3 = NULL;
int g_nType4 = NULL;

//移動保存用
float g_fMove2 = NULL;
float g_fMove3 = NULL;
float g_fMove4 = NULL;

//移動処理に使用
bool g_bUseEnemyMove = false;
bool g_bUseEnemyMove2 = false;
bool g_bUseEnemyMove3 = false;

//特殊攻撃管理
int g_nNextAttack = false;
int g_nNextAttack2 = false;
int g_nNextAttack3 = false;

//弾の発射
int g_nCntEnemyBullet = 0;
int g_nAttack = 0;

//移動管理番号
bool g_bMoveType = false;

float g_fAnimeEnemy = 0.0f;
bool g_bMoveEnemy = false;

//プレイヤーの無敵時間
int g_nHitTimePlayer = 0;
bool g_bInvincibilityPlayer = false;

//=============================================================================
// コンストラクタ
//=============================================================================
CEnemy::CEnemy(int nPriority) : CScene2D(nPriority)
{
	//グローバル変数の初期化
	g_fMove2 = 0;
	g_fMove3 = 0;
	g_fMove4 = 0;

	g_bUseEnemyMove = false;
	g_bUseEnemyMove2 = false;
	g_bUseEnemyMove3 = false;

	g_bInvincibilityPlayer = false;
	g_nHitTimePlayer = 0;

	//メンバ変数の初期化
	m_bAngleEnemy = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{
	
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, float fmove, D3DXVECTOR3 objective, int nLife, float fSizeX, float fSizeY, int nTime, int nType, int nMoveType)
{
	// CScene2Dの初期化処理
	CScene2D::Init(pos, fSizeX, fSizeY);

	//サイズ情報を受け渡す
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;

	//サイズ情報をシーンクラスに記憶
	SetSizeXScene(fSizeX);
	SetSizeYScene(fSizeY);

	//メンバ関数に代入
	m_pos = pos;
	m_objective = objective;
	m_fMove = fmove;
	m_nLife = nLife;
	m_nTime = nTime;
	m_nType = nType;
	m_nMoveType = nMoveType;

	//敵のライフ情報を受け渡す
	CScene::SetLife(nLife);

	//タイプによって変える処理内容
	if (m_nType == ENEMY_06)
	{
		m_fAngleEnemyBullet = D3DX_PI * 100;
	}

	if (m_nType == ENEMY_07)
	{
		m_fAngleEnemyBullet = D3DX_PI * 100;
	}

	// オブジェクトの種類設定
	SetObjType(CScene::OBJTYPE_ENEMY);

	if (m_nType == ENEMY_BOSS_02 || m_nType == ENEMY_BOSS_03 || m_nType == ENEMY_05)
	{
		// オブジェクトの種類設定
		SetObjType(CScene::OBJTYPE_ENEMY2);
	}

	if (m_nType == ENEMY_BOSS_01)
	{
		// オブジェクトの種類設定
		SetObjType(CScene::OBJTYPE_ENEMY3);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	//回転処理　雑魚敵
	if (m_nType != ENEMY_99 && m_nType != ENEMY_BOSS_01  && m_nType != ENEMY_BOSS_02 && m_nType != ENEMY_BOSS_03)
	{
		//回転処理
		if (m_fMoveEnemy >= 0.3f)
		{
			m_bMovEnemy = false;
		}

		if (m_fMoveEnemy <= -0.3f)
		{
			m_bMovEnemy = true;
		}

		if (m_bMovEnemy == false)
		{
			m_fMoveEnemy -= 0.01f;
		}

		if (m_bMovEnemy == true)
		{
			m_fMoveEnemy += 0.01f;
		}
	}

	//回転処理　ボス
	if (m_nType == ENEMY_BOSS_01 || m_nType == ENEMY_BOSS_02 || m_nType == ENEMY_BOSS_03)
	{
		//回転処理
		if (m_fMoveEnemy >= 0.1f)
		{
			m_bMovEnemy = false;
		}

		if (m_fMoveEnemy <= -0.1f)
		{
			m_bMovEnemy = true;
		}

		if (m_bMovEnemy == false)
		{
			m_fMoveEnemy -= 0.002f;
		}

		if (m_bMovEnemy == true)
		{
			m_fMoveEnemy += 0.002f;
		}
	}

	//隕石
	if (m_nType == ENEMY_99)
	{
		m_fMoveEnemy += 0.005f;
	}

	//回転設定　更新
	SetRevolution(m_fMoveEnemy);
	
	//弾が発射られるタイミング（rand）
	int nCntEnemyBulletRand = rand() % 100;

	//乱数の値が0の場合攻撃される
	if (nCntEnemyBulletRand == 0)
	{
		if (m_nType == ENEMY_01)		//敵01
		{
			//弾の発射
			CBullet::Create(pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 200, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
		}

		if (m_nType == ENEMY_02)		//敵02
		{
			//三つの弾を発射
			CBullet::InitAngleBullet(pos, 5.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, 0.0f, OBJTYPE_BULET_ENEMY);
			CBullet::InitAngleBullet(pos, 5.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, 30.0f, OBJTYPE_BULET_ENEMY);
			CBullet::InitAngleBullet(pos, 5.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, -30.0f, OBJTYPE_BULET_ENEMY);
		}

		if (m_nType == ENEMY_03)		//敵03
		{
			//狙い弾
			D3DXVECTOR3 posPlayer = m_pPlayer->GetPlayer();
			CBullet::InitAimingBullet(pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 8.0f, D3DXVECTOR3(posPlayer.x, posPlayer.y, 0.0f));
		}
	}

	//特殊攻撃処理
	Attack();

	//移動処理
	Move();

	//更新
	CScene2D::Update();

	//ライフが0の場合に消す
	if (m_nLife <= 0)
	{
		Uninit();
	}

	//下に行ったら消す
	else if (pos.y >= SCREEN_HEIGHT + 100.0f)
	{
		Uninit();
	}

	//それ以外の場合当たり判定処理をする
	else
	{
		//衝突判定
		Collision();
	}

	//敵の無敵時間
	if (m_bInvincibility == true)
	{
		//無敵時間をカウント
		m_nHitTime++;

		if (m_nHitTime == 1)
		{
			//複数のパーティクル
			for (int nCnt = 0; nCnt < 10; nCnt++)
			{
				//乱数を変数に代入
				int nRand = rand() % 628;
				int nRandSpeed = (rand() % 5) + 5;

				//パーティクル
				CParticl::Create(pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f), 20, (float)nRandSpeed, (float)20, (float)20, (float)nRand);
			}
		}
	}

	//無敵時の色変化の色を戻す
	if (m_nHitTime >= 20)
	{
		//色変更
		CScene2D::SetCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_bInvincibility = false;
		m_nHitTime = 0;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// 衝突判定処理
//=============================================================================
void CEnemy::Collision(void)
{
	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// 敵との当たり判定
	for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
	{
		for (int nCntPriority = 1; nCntPriority < 3; nCntPriority++)
		{
			// オブジェクト情報の取得
			CScene *pScene;
			pScene = GetScene(nCntScene, nCntPriority);

			// NULLチェック
			if (pScene != NULL)
			{
				if (CPlayer::GetBumped() == false)
				{
					// オブジェクトの種類の取得
					CScene::OBJTYPE objType = pScene->GetObjType();

					//ボスの当たり判定を消す
					//if (m_nType != CScene::OBJTYPE_BULET_ENEMY3)
					{
						// オブジェクトの種類がPlayer場合
						if (objType == CScene::OBJTYPE_PLAYER)
						{
							// 位置の取得
							D3DXVECTOR3 posPlayer = pScene->GetPosition();

							// オブジェクトの種類がENEMY_BOSS_01場合当たり判定を調整する
							if (m_nType == ENEMY_BOSS_01)
							{
								float fEnemySizeX = pScene->GetSizeX() - 150.0f;
								float fEnemySizeY = pScene->GetSizeY() - 10.0f;
							}

							// 当たり判定
							if (pos.x + m_fSizeX >= posPlayer.x - (PLAYER_SIZE - 40) && pos.x - m_fSizeX <= posPlayer.x + (PLAYER_SIZE - 40) &&
								pos.y - m_fSizeY <= posPlayer.y + (PLAYER_SIZE - 40) && pos.y + m_fSizeY >= posPlayer.y - (PLAYER_SIZE - 40))
							{
								//無敵状態にする
								g_bInvincibilityPlayer = true;

								//無敵状態にする
								CPlayer::SetBumped(true);

								CManager::DamagePlayerLife(1);

								int nLifeEnemy = CManager::GetPlayerLife();

								//ライフが0以下の場合
								if (nLifeEnemy <= 0)
								{
									// PLAYERの破棄
									pScene->Uninit();
								}

								// 爆発の生成
								CExplosion::Create(posPlayer, 50, 50);
							}
						}
					}
				}
			}
		}
	}

	//無敵時間＆弾に当たったときの色変化
	// 当たり判定
	for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
	{
		// オブジェクト情報の取得
		CScene *pScene;
		pScene = GetScene(nCntScene, 4);

		// NULLチェック
		if (pScene != NULL)
		{
			// オブジェクトの種類の取得
			CScene::OBJTYPE objType = pScene->GetObjType();

			// 位置の取得
			D3DXVECTOR3 posBullet = pScene->GetPosition();

			//ステージ2の場合
			if (CManager::GetGameStage() == 2)
			{
				//ボスの当たり判定
				if (CManager::GetEnemyDead() == 2)		//条件を満たしている場合
				{
					// オブジェクトの種類がENEMY_BOSS_01場合
					if (m_nType == ENEMY_BOSS_01)
					{
						float fEnemySizeX = pScene->GetSizeX() - 150.0f;
						float fEnemySizeY = pScene->GetSizeY();

						//PLAYERから発射された弾の場合
						if (objType == OBJTYPE_BULET_PLAYER)
						{
							// 当たり判定
							if (pos.x + m_fSizeX >= posBullet.x - fEnemySizeX && pos.x - m_fSizeX <= posBullet.x + fEnemySizeX &&
								pos.y - m_fSizeY <= posBullet.y + fEnemySizeY && pos.y + m_fSizeY >= posBullet.y - fEnemySizeY)
							{
								//色変更
								CScene2D::SetCollar(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
								m_bInvincibility = true;
							}
						}
					}
				}
			}

			// オブジェクトの種類がENEMY_BOSS_01以外の場合
			if (m_nType != ENEMY_BOSS_01)
			{
				//PLAYERから発射された弾の場合
				if (objType == OBJTYPE_BULET_PLAYER)
				{
					//サイズを取得
					float fEnemySizeX = pScene->GetSizeX();
					float fEnemySizeY = pScene->GetSizeY() + 10.0f;

					// 当たり判定
					if (pos.x + m_fSizeX >= posBullet.x - fEnemySizeX && pos.x - m_fSizeX <= posBullet.x + fEnemySizeX &&
						pos.y - m_fSizeY <= posBullet.y + fEnemySizeY && pos.y + m_fSizeY >= posBullet.y - fEnemySizeY)
					{
						//色変更
						CScene2D::SetCollar(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
						m_bInvincibility = true;
					}
				}
			}
		}
	}
}

//=============================================================================
// 特殊攻撃処理
//=============================================================================
void CEnemy::Attack(void)
{
	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	//特殊攻撃
	//グローバル変数の値によって攻撃が変わる
	if (g_nNextAttack == 1 || g_nNextAttack2 == 1)
	{
		//カウントを進める
		m_nAttackCnt2++;

		if (m_nAttackCnt2 >= 10)
		{
			if (m_nType == ENEMY_BOSS_02)
			{
				CBullet::Create(pos, D3DXVECTOR3(0.0f, 25.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 0, 20, 150, OBJTYPE_BULET_ENEMY);
			}

			if (m_nType == ENEMY_BOSS_03)
			{
				CBullet::Create(pos, D3DXVECTOR3(0.0f, 25.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 0, 20, 150, OBJTYPE_BULET_ENEMY);
			}

			//カウントリセット
			m_nAttackCnt2 = 0;
		}
	}

	if (g_nNextAttack == 2 || g_nNextAttack2 == 2)
	{
		if (m_nType == ENEMY_04)
		{
			//カウントを進める
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 50)
			{
				//弾の発射
				CBullet::Create(pos, D3DXVECTOR3(5.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
				CBullet::Create(pos, D3DXVECTOR3(-5.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);

				//カウントリセット
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 3 || g_nNextAttack2 == 3)
	{
		if (m_nType == ENEMY_04)
		{
			//カウントを進める
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 50)
			{
				//弾の発射
				CBullet::Create(pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
				CBullet::Create(pos, D3DXVECTOR3(0.0f, -5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);

				//カウントリセット
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 4 || g_nNextAttack2 == 4)
	{
		if (m_nType == ENEMY_04)
		{
			//カウントを進める
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 100)
			{
				//弾の発射
				CBullet::Create(pos, D3DXVECTOR3(5.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
				CBullet::Create(pos, D3DXVECTOR3(-5.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
				CBullet::Create(pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
				CBullet::Create(pos, D3DXVECTOR3(0.0f, -5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);

				//カウントリセット
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 5 || g_nNextAttack2 == 5)
	{
		if (m_nType == ENEMY_05)
		{
			//カウントを進める
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 100)
			{
				//攻撃する
				CPolygon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 200, 0, 0, 5.0f, 0.0f, 0.0f, 1000, 0, OBJTYPE_BULET_ENEMY, 4);
				CPolygon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 200, 0, 0, 5.0f, 1.0f, 0.0f, 1000, 0, OBJTYPE_BULET_ENEMY, 4);

				//カウントリセット
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 6 || g_nNextAttack2 == 6)
	{
		if (m_nType == ENEMY_05)
		{
			//カウントを進める
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 100)
			{
				//攻撃する
				CPolygon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 200, 0, 0, 5.0f, 0.0f, 0.01f, 1000, 0, OBJTYPE_BULET_ENEMY, 4);
				CPolygon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 200, 0, 0, 5.0f, 1.0f, -0.01f, 1000, 0, OBJTYPE_BULET_ENEMY, 4);

				//カウントリセット
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 7 || g_nNextAttack2 == 7)
	{
		if (m_nType == ENEMY_06)
		{
			//カウントを進める
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 20)
			{
				//弾の発射
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -10.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet, OBJTYPE_BULET_ENEMY);
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -10.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet + 30.0f, OBJTYPE_BULET_ENEMY);

				//角度の制限
				if (m_fAngleEnemyBullet >= -((D3DX_PI * 100) / 2))
				{
					m_bAngleEnemy = true;
				}

				if (m_fAngleEnemyBullet <= -D3DX_PI * 100)
				{
					m_bAngleEnemy = false;
				}

				//角度変える
				if (m_bAngleEnemy == false)
				{
					m_fAngleEnemyBullet += 35.0f;
				}

				if (m_bAngleEnemy == true)
				{
					m_fAngleEnemyBullet -= 35.0f;
				}

				//カウントリセット
				m_nAttackCnt2 = 0;
			}
		}

		if (m_nType == ENEMY_07)
		{
			//カウントを進める
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 20)
			{
				//弾の発射
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -10.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet, OBJTYPE_BULET_ENEMY);
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -10.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet - 30.0f, OBJTYPE_BULET_ENEMY);

				//角度の制限
				if (m_fAngleEnemyBullet >= D3DX_PI * 100)
				{
					m_bAngleEnemy = true;
				}

				if (m_fAngleEnemyBullet <= (D3DX_PI * 100) / 2)
				{
					m_bAngleEnemy = false;
				}

				//角度変える
				if (m_bAngleEnemy == false)
				{
					m_fAngleEnemyBullet += 35.0f;
				}

				if (m_bAngleEnemy == true)
				{
					m_fAngleEnemyBullet -= 35.0f;
				}

				//カウントリセット
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 8 || g_nNextAttack2 == 8)
	{
		if (m_nType == ENEMY_06 || m_nType == ENEMY_07)
		{
			//カウントを進める
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 40)
			{
				//弾の発射
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -7.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet, OBJTYPE_BULET_ENEMY);
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -7.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet + 30.0f, OBJTYPE_BULET_ENEMY);
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -7.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet - 30.0f, OBJTYPE_BULET_ENEMY);
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -7.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet + 60.0f, OBJTYPE_BULET_ENEMY);
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -7.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet - 60.0f, OBJTYPE_BULET_ENEMY);

				//角度の制限
				if (m_fAngleEnemyBullet >= (D3DX_PI * 100 + 50))
				{
					m_bAngleEnemy = true;
				}

				if (m_fAngleEnemyBullet <= (D3DX_PI * 100 - 50))
				{
					m_bAngleEnemy = false;
				}

				//角度変える
				if (m_bAngleEnemy == false)
				{
					m_fAngleEnemyBullet += 35.0f;
				}

				if (m_bAngleEnemy == true)
				{
					m_fAngleEnemyBullet -= 35.0f;
				}

				//カウントリセット
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 9 || g_nNextAttack2 == 9)
	{
		if (m_nType == ENEMY_05)
		{
			//カウントを進める
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 40)
			{
				for (int nEnemyBullet = 0; nEnemyBullet <= 20; nEnemyBullet++)
				{
					int nCntBulletRand = rand() % 300;

					float BulletRand = (D3DX_PI * 100) + (nCntBulletRand - 150);

					//弾の発射
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -5.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, (float)BULLET_SIZE, (float)BULLET_SIZE, BulletRand, OBJTYPE_BULET_ENEMY);
				}

				//カウントリセット
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 10 || g_nNextAttack2 == 10)
	{
		if (m_nType == ENEMY_04)
		{
			//カウントを進める
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 120)
			{
				if (m_bAngleEnemy == false)
				{
					//弾の発射
					CBullet::Create(pos, D3DXVECTOR3(5.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
					CBullet::Create(pos, D3DXVECTOR3(-5.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
					CBullet::Create(pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
					CBullet::Create(pos, D3DXVECTOR3(0.0f, -5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);

					m_bAngleEnemy = true;
				}

				else if (m_bAngleEnemy == true)
				{
					//弾の発射
					CBullet::Create(pos, D3DXVECTOR3(5.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
					CBullet::Create(pos, D3DXVECTOR3(-5.0f, -5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
					CBullet::Create(pos, D3DXVECTOR3(5.0f, -5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
					CBullet::Create(pos, D3DXVECTOR3(-5.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);

					m_bAngleEnemy = false;
				}

				//カウントリセット
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 11 || g_nNextAttack2 == 11)
	{
		if (m_nType == ENEMY_05)
		{
			//カウントを進める
			m_nAttackCnt++;

			if (m_nAttackCnt >= 40)
			{
				for (int nEnemyBullet = 0; nEnemyBullet <= 3; nEnemyBullet++)
				{
					int nCntBulletRand = rand() % 300;

					float BulletRand = (D3DX_PI * 100) + (nCntBulletRand - 150);

					//弾の発射
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -2.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, BulletRand, OBJTYPE_BULET_ENEMY);
				}

				//カウントリセット
				m_nAttackCnt = 0;
			}
		}
	}

	//ボス
	if (g_nNextAttack == 12 || g_nNextAttack2 == 12 || g_nNextAttack3 == 12)
	{
		if (m_nType == ENEMY_BOSS_01)
		{
			//カウントを進める
			m_nAttackCnt++;

			if (m_nAttackCnt >= 10)
			{
				for (int nEnemyBullet = 0; nEnemyBullet < 1; nEnemyBullet++)
				{
					//弾の発射
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), 3.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet, OBJTYPE_BULET_ENEMY);
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), 3.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet - 10.0f, OBJTYPE_BULET_ENEMY);
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), 3.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet - 20.0f, OBJTYPE_BULET_ENEMY);
				}

				//カウントリセット
				m_nAttackCnt = 0;
			}
			m_fAngleEnemyBullet += 5.0f;
		}
	}

	if (g_nNextAttack == 13 || g_nNextAttack2 == 13 || g_nNextAttack3 == 13)
	{
		if (m_nType == ENEMY_BOSS_01)
		{
			//カウントを進める
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 10)
			{
				for (int nEnemyBullet = 0; nEnemyBullet < 1; nEnemyBullet++)
				{
					//弾の発射
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), 4.0f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet2, OBJTYPE_BULET_ENEMY);
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), 4.0f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet2 - 10.0f, OBJTYPE_BULET_ENEMY);
				}

				//カウントリセット
				m_nAttackCnt2 = 0;
			}
			m_fAngleEnemyBullet2 += 5.0f;
		}
	}

	if (g_nNextAttack == 14 || g_nNextAttack2 == 14 || g_nNextAttack3 == 14)
	{
		if (m_nType == ENEMY_BOSS_01)
		{
			//カウントを進める
			m_nAttackCnt3++;

			if (m_nAttackCnt3 >= 30)
			{
				for (int nEnemyBullet = 0; nEnemyBullet < 1; nEnemyBullet++)
				{
					//弾の発射
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), 2.0f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE + 20, BULLET_SIZE + 20, m_fAngleEnemyBullet3, OBJTYPE_BULET_ENEMY);
				}

				//カウントリセット
				m_nAttackCnt3 = 0;
			}
			m_fAngleEnemyBullet3 += 3.0f;
		}
	}
}

//=============================================================================
// 移動処理
//=============================================================================
void CEnemy::Move(void)
{
	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	//移動処理
	//定義
	D3DXVECTOR3 move;
	D3DXVECTOR3 vec;
	float fAngle = 0;

	//MoveTypeのクリア
	if (g_bMoveType == true)
	{
		m_nMoveType = NULL;
		g_bMoveType = false;
		g_bUseEnemyMove = false;
		g_bUseEnemyMove2 = false;
	}

	//次の目的地が設定されたら
	if (g_bUseEnemyMove == true)
	{
		//複数目的地がある場合
		if (m_nType == g_nType2)
		{
			//目的地の変更
			m_objective = g_objective2;

			//移動量変更
			m_fMove = g_fMove2;

			g_nType2 = NULL;
			g_bUseEnemyMove = false;
		}

		//複数目的地がある場合
		//向かう前に攻撃をする
		if (m_nType == g_nType3)
		{
			//目的地の変更
			m_objective = g_objective3;

			//移動量変更
			m_fMove = g_fMove3;

			g_nType3 = NULL;
			g_bUseEnemyMove = false;

			//攻撃する
			CPolygon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 1000, 0, 0, 1.0f, 0.0f, 0.0f, 1000, 0, OBJTYPE_BULET_ENEMY, 4);
			CPolygon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 1000, 0, 0, 1.0f, 1.0f, 0.0f, 1000, 0, OBJTYPE_BULET_ENEMY, 4);
		}
	}

	//NestMoveで設定したタイプの移動先を変更させる
	if (g_bUseEnemyMove2 == true)
	{
		//複数目的地がある場合
		if (m_nMoveType == g_nType4)
		{
			//目的地の変更
			m_objective = g_objective4;

			//移動量変更
			m_fMove = g_fMove4;
			g_bUseEnemyMove2 = false;
		}
	}

	//時間経過で下に移動させる
	if (m_nTime == CGame::GetTime())
	{
		m_objective.y = 3000.0f;
	}

	if (m_nTime == CGame2::GetTime())
	{
		m_objective.y = 3000.0f;
	}

	//目的地を計算
	vec = m_objective - pos;

	//位置更新
	fAngle = atan2f(vec.x, vec.y);			//ベクトルの角度を求める
	move.x = sinf(fAngle) * m_fMove;		//移動量を求める
	move.y = cosf(fAngle) * m_fMove;		//移動量を求める

											//目的地に近づいたら更新を止める
	if (pos.y >= m_objective.y - 10.0f && pos.y <= m_objective.y + 10.0f && pos.x >= m_objective.x - 10.0f && pos.x <= m_objective.x + 10.0f)
	{
		//何もしない
	}

	else
	{
		//位置更新をさせる
		pos += move;
	}

	//移動させる
	SetPosition(pos);
}

//=============================================================================
// 生成処理
//=============================================================================
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, float fmove, D3DXVECTOR3 objective, int nLife, float fSizeX, float fSizeY, int nTime, int nType, int nMoveType)
{
	// ローカル変数宣言
	CEnemy *pEnemy = NULL;
	pEnemy = new CEnemy;

	//初期化
	g_bMoveType = false;

	// NULLチェック
	if (pEnemy != NULL)
	{
		//敵の生成
		pEnemy->Init(pos, fmove, objective, nLife, fSizeX, fSizeY, nTime, nType, nMoveType);
	}

	// テクスチャ割り当て
	pEnemy->BindTexture(m_pTexture[nType]);

	return pEnemy;
}

//=============================================================================
// 次の移動場所
//=============================================================================
void CEnemy::NextObjective(D3DXVECTOR3 pos, float fMove, int nType)
{
	g_objective2 = pos;
	g_nType2 = nType;
	g_fMove2 = fMove;

	g_bUseEnemyMove = true;
}

//=============================================================================
// 次の移動場所到着後に攻撃をする
//=============================================================================
void CEnemy::NextObjectiveAttack(D3DXVECTOR3 pos, float fMove, int nType)
{
	g_objective3 = pos;
	g_nType3 = nType;
	g_fMove3 = fMove;

	g_bUseEnemyMove = true;
}

//=============================================================================
// 攻撃パターンを変える
//=============================================================================
void CEnemy::NextAttack(int NextAttack)
{
	g_nNextAttack = NextAttack;
}

//=============================================================================
// 攻撃パターンを変える
//=============================================================================
void CEnemy::NextAttack2(int NextAttack)
{
	g_nNextAttack2 = NextAttack;
}

//=============================================================================
// 攻撃パターンを変える
//=============================================================================
void CEnemy::NextAttack3(int NextAttack)
{
	g_nNextAttack3 = NextAttack;
}

//=============================================================================
// 移動目的地をタイプで途中変更
//=============================================================================
void CEnemy::NextMove(D3DXVECTOR3 pos, float fMove, int nType)
{
	g_nType4 = nType;
	g_objective4 = pos;
	g_fMove4 = fMove;

	//途中変更を許可する
	g_bUseEnemyMove2 = true;
}

//=============================================================================
// m_fMoveの値をNULLにする
//=============================================================================
void CEnemy::MoveClear(void)
{
	g_bMoveType = true;
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CEnemy::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_01.png", &m_pTexture[ENEMY_01]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_02.png", &m_pTexture[ENEMY_02]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_03.png", &m_pTexture[ENEMY_03]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_04.png", &m_pTexture[ENEMY_04]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_05.png", &m_pTexture[ENEMY_05]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_06.png", &m_pTexture[ENEMY_06]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_07.png", &m_pTexture[ENEMY_07]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_99.png", &m_pTexture[ENEMY_99]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_01.png", &m_pTexture[ENEMY_BOSS_01]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_02.png", &m_pTexture[ENEMY_BOSS_02]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_03.png", &m_pTexture[ENEMY_BOSS_03]);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CEnemy::Unload(void)
{
	for (int nCnt = 0; nCnt < ENEMY_MAX; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}