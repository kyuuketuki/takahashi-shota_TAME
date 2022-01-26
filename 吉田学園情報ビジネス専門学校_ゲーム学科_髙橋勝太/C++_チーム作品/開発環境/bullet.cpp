//=============================================================================
//
// 弾処理 [bullet.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "explosion.h"
#include "enemy.h"
#include "effect.h"
#include "polygon.h"
#include "life.h"
#include "item.h"
#include "particle.h"

// 静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;
int CBullet::m_TypeAction = NULL;

//目的地を変更
D3DXVECTOR3 g_nextmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 g_nextmove2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 g_nextmove3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//目的地を変更
D3DXVECTOR3 g_nextpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 g_nextpos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 g_nextpos3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//特殊行動に使用
int g_TypeMoveAction = NULL;
int g_TypeMoveAction2 = NULL;
int g_TypeMoveAction3 = NULL;

//特殊行動に使用
int g_Action = NULL;
int g_Action2 = NULL;
int g_Action3 = NULL;

//特殊行動に使用
float g_fSpeed = 0.0f;
float g_fSpeed2 = 0.0f;
float g_fSpeed3 = 0.0f;

//移動攻撃変化
int g_TypeMove = NULL;
int g_nAction = 0;

//無敵時間
int g_BulletTime;					//タイムを保存
int g_BulletTime2;					//タイムを保存
bool g_bBumped = false;				//無敵状態

//=============================================================================
// インストラクタ
//=============================================================================
CBullet::CBullet(int nPriority) : CScene2D(nPriority)
{
	m_nLife = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSpeed, int nLife, int nDamage, float fSizeX, float fSizeY, int nType, int nTypeMove)
{
	// CScene2Dの初期化処理
	CScene2D::Init(pos, fSizeX, fSizeY);

	//引数の情報をメンバ関数に入れる
	m_move = move;
	m_col = col;
	m_nLife = nLife;
	m_nDamage = nDamage;
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;
	m_fSpeedDestinations = fSpeed;

	m_TypeMove = nTypeMove;

	SetSizeXScene(fSizeX);
	SetSizeYScene(fSizeY);

	//プレイヤーからの発射された弾の場合
	if (nType == OBJTYPE_BULET_PLAYER)
	{
		// オブジェクトの種類設定
		SetObjType(CScene::OBJTYPE_BULET_PLAYER);
		m_nType = OBJTYPE_BULET_PLAYER;
	}

	//敵からの発射された弾の場合
	else if (nType == OBJTYPE_BULET_ENEMY || nType == OBJTYPE_BULET_ENEMY2 || nType == OBJTYPE_BULET_ENEMY3)
	{
		// オブジェクトの種類設定
		SetObjType(CScene::OBJTYPE_BULET_ENEMY);
		m_nType = nType;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBullet::Update(void)
{
	//サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	//プレイヤーからの弾　場合
	if (m_nType == OBJTYPE_BULET_PLAYER)
	{
		//エフェクトを生成
		CEffect::Create(pos, 1, 25, 25, m_col);
	}

	//敵からの弾　場合
	if (m_nType == OBJTYPE_BULET_ENEMY || m_nType == OBJTYPE_BULET_ENEMY2 || m_nType == OBJTYPE_BULET_ENEMY3)
	{
		//エフェクトを生成
		CEffect::Create(pos, 1, m_fSizeX * 3, m_fSizeY * 3, m_col);
	}
	
	//位置更新
	pos += m_move;

	//プレイヤー以外の弾の更新
	//敵から発射された弾
	if (m_nType != OBJTYPE_BULET_PLAYER)
	{
		//狙い撃ち　目的地に発射せれる　
		if (m_nType == OBJTYPE_BULET_ENEMY2)
		{
			//定義
			D3DXVECTOR3 move;
			D3DXVECTOR3 vec;
			float fAngle;

			//位置更新
			pos -= m_move;

			//目的地を計算
			vec = m_move - pos;

			//位置更新　角度計算
			fAngle = atan2f(vec.x, vec.y);						//ベクトルの角度を求める
			move.x = sinf(fAngle) * m_fSpeedDestinations;		//移動量を求める
			move.y = cosf(fAngle) * m_fSpeedDestinations;		//移動量を求める

			//目的地に近づいたら更新を止める
			if (pos.y >= m_move.y - 10.0f && pos.y <= m_move.y + 10.0f && pos.x >= m_move.x - 10.0f && pos.x <= m_move.x + 10.0f)
			{
				//何もしない
				m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_nType = OBJTYPE_BULET_ENEMY;
			}

			else
			{
				//位置更新をさせる
				pos += move;
			}
		}

		//移動中に新たに弾を生成される弾
		if (m_nType == OBJTYPE_BULET_ENEMY3)
		{
			if (m_bCreatepos == false)
			{
				m_createpos = pos;
				m_bCreatepos = true;
			}

			//どれ繰り移動したか計算X
			if (pos.x > (m_createpos.x + 50.0f) || pos.x < (m_createpos.x - 50.0f))
			{
				CBullet::Create2(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 1500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY, m_TypeMove);
				m_bCreatepos = false;
			}

			//どれ繰り移動したか計算Y
			else if (pos.y > m_createpos.y + 50.0f || pos.y < m_createpos.y - 50.0f)
			{
				CBullet::Create2(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 1500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY, m_TypeMove);
				m_bCreatepos = false;
			}
		}

		//グローバル変数で管理
		//特殊移動攻撃
		if (g_TypeMoveAction == m_TypeMove)
		{
			m_nType = OBJTYPE_BULET_ENEMY;

			//スピード変える
			if (g_Action == 1)
			{
				m_bUseAming = false;

				//位置更新
				pos -= m_move;
				pos += g_nextmove;
			}

			//狙い撃ち
			if (g_Action == 2)
			{
				if (m_bUseAming == false)
				{
					m_oldpos = pos;

					m_bUseAming = true;
				}

				//位置更新
				pos -= m_move;

				// 目標までの距離dを求める
				double d = sqrt((g_nextpos.x - m_oldpos.x)*(g_nextpos.x - m_oldpos.x) + (g_nextpos.y - m_oldpos.y)*(g_nextpos.y - m_oldpos.y));

				float vx;
				float vy;

				// 速さが一定値speedになるように速度を求める
				vx = (g_nextpos.x - m_oldpos.x) / (float)d*g_fSpeed;
				vy = (g_nextpos.y - m_oldpos.y) / (float)d*g_fSpeed;

				pos += D3DXVECTOR3(vx, vy, 0.0f);
			}

			//目的地に移動
			if (g_Action == 3)
			{
				m_bUseAming = false;

				//定義
				D3DXVECTOR3 move;
				D3DXVECTOR3 vec;
				float fAngle;

				//位置更新
				pos -= m_move;

				//目的地を計算
				vec = g_nextpos - pos;

				//位置更新
				fAngle = atan2f(vec.x, vec.y);			//ベクトルの角度を求める
				move.x = sinf(fAngle) * g_fSpeed;		//移動量を求める
				move.y = cosf(fAngle) * g_fSpeed;		//移動量を求める

														//目的地に近づいたら更新を止める
				if (pos.y >= g_nextpos.y - 10.0f && pos.y <= g_nextpos.y + 10.0f && pos.x >= g_nextpos.x - 10.0f && pos.x <= g_nextpos.x + 10.0f)
				{
					//何もしない
				}

				else
				{
					//位置更新をさせる
					pos += move;
				}
			}
		}

		else if (g_TypeMoveAction2 == m_TypeMove)
		{
			m_nType = OBJTYPE_BULET_ENEMY;

			//スピード変える
			if (g_Action2 == 1)
			{

				m_bUseAming = false;

				//位置更新
				pos -= m_move;
				pos += g_nextmove2;
			}

			//狙い撃ち
			if (g_Action2 == 2)
			{
				if (m_bUseAming == false)
				{
					m_oldpos = pos;

					m_bUseAming = true;
				}

				//位置更新
				pos -= m_move;

				// 目標までの距離dを求める
				double d = sqrt((g_nextpos2.x - m_oldpos.x)*(g_nextpos2.x - m_oldpos.x) + (g_nextpos2.y - m_oldpos.y)*(g_nextpos2.y - m_oldpos.y));

				float vx;
				float vy;

				// 速さが一定値speedになるように速度を求める
				vx = (g_nextpos2.x - m_oldpos.x) / (float)d*g_fSpeed2;
				vy = (g_nextpos2.y - m_oldpos.y) / (float)d*g_fSpeed2;

				pos += D3DXVECTOR3(vx, vy, 0.0f);
			}

			//目的地に移動
			if (g_Action2 == 3)
			{
				m_bUseAming = false;

				//定義
				D3DXVECTOR3 move;
				D3DXVECTOR3 vec;
				float fAngle;

				//位置更新
				pos -= m_move;

				//目的地を計算
				vec = g_nextpos2 - pos;

				//位置更新
				fAngle = atan2f(vec.x, vec.y);			//ベクトルの角度を求める
				move.x = sinf(fAngle) * g_fSpeed2;		//移動量を求める
				move.y = cosf(fAngle) * g_fSpeed2;		//移動量を求める

														//目的地に近づいたら更新を止める
				if (pos.y >= g_nextpos2.y - 10.0f && pos.y <= g_nextpos2.y + 10.0f && pos.x >= g_nextpos2.x - 10.0f && pos.x <= g_nextpos2.x + 10.0f)
				{
					//何もしない
				}

				else
				{
					//位置更新をさせる
					pos += move;
				}
			}
		}

		else if (g_TypeMoveAction3 == m_TypeMove)
		{
			m_nType = OBJTYPE_BULET_ENEMY;

			//スピード変える
			if (g_Action3 == 1)
			{
				m_bUseAming = false;

				//位置更新
				pos -= m_move;
				pos += g_nextmove3;
			}

			//狙い撃ち
			if (g_Action3 == 2)
			{
				if (m_bUseAming == false)
				{
					m_oldpos = pos;

					m_bUseAming = true;
				}

				//位置更新
				pos -= m_move;

				// 目標までの距離dを求める
				double d = sqrt((g_nextpos3.x - m_oldpos.x)*(g_nextpos3.x - m_oldpos.x) + (g_nextpos3.y - m_oldpos.y)*(g_nextpos3.y - m_oldpos.y));

				float vx;
				float vy;

				// 速さが一定値speedになるように速度を求める
				vx = (g_nextpos3.x - m_oldpos.x) / (float)d*g_fSpeed3;
				vy = (g_nextpos3.y - m_oldpos.y) / (float)d*g_fSpeed3;

				pos += D3DXVECTOR3(vx, vy, 0.0f);
			}

			//目的地に移動
			if (g_Action3 == 3)
			{
				m_bUseAming = false;

				//定義
				D3DXVECTOR3 move;
				D3DXVECTOR3 vec;
				float fAngle;

				//位置更新
				pos -= m_move;

				//目的地を計算
				vec = g_nextpos3 - pos;

				//位置更新
				fAngle = atan2f(vec.x, vec.y);			//ベクトルの角度を求める
				move.x = sinf(fAngle) * g_fSpeed3;		//移動量を求める
				move.y = cosf(fAngle) * g_fSpeed3;		//移動量を求める

														//目的地に近づいたら更新を止める
				if (pos.y >= g_nextpos3.y - 10.0f && pos.y <= g_nextpos3.y + 10.0f && pos.x >= g_nextpos3.x - 10.0f && pos.x <= g_nextpos3.x + 10.0f)
				{
					//何もしない
				}

				else
				{
					//位置更新をさせる
					pos += move;
				}
			}

		}
	}

	//移動させる
	SetPosition(pos);

	//更新
	CScene2D::Update();

	//範囲外の場合
	if (pos.y <= 0)
	{
		Uninit();
	}
	else if (pos.y >= SCREEN_HEIGHT)
	{
		Uninit();
	}
	else if (pos.x <= 0)
	{
		Uninit();
	}
	else if (pos.x >= SCREEN_WIDTH)
	{
		Uninit();
	}

	//当たり判定
	else
	{
		//それ以外の場合当たり判定処理をする
		// 当たり判定
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
					// オブジェクトの種類の取得
					CScene::OBJTYPE objType = pScene->GetObjType();

					//PLAYERから発射された弾の場合
					if (m_nType == OBJTYPE_BULET_PLAYER)
					{
						// オブジェクトの種類がEnemy場合
						if (objType == CScene::OBJTYPE_ENEMY || objType == CScene::OBJTYPE_ENEMY2)
						{
							// 位置の取得
							D3DXVECTOR3 posEnemy = pScene->GetPosition();
							float fEnemySizeX = pScene->GetSizeX();
							float fEnemySizeY = pScene->GetSizeY();

							// 当たり判定
							if (pos.x + m_fSizeX >= posEnemy.x - fEnemySizeX && pos.x - m_fSizeX <= posEnemy.x + fEnemySizeX &&
								pos.y - m_fSizeY <= posEnemy.y + fEnemySizeY && pos.y + m_fSizeY >= posEnemy.y - fEnemySizeY)
							{
								//サウンドSE
								pSound->Play(CSound::SOUND_LABEL_SE_HIT);

								//敵にダメージを与える
								pScene->Damage(m_nDamage);

								//敵のライフを読み込む
								int nLifeEnemy = pScene->GetLife();

								//ライフが0以下の場合
								if (nLifeEnemy <= 0)
								{
									//サウンドSE
									pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);

									// 大爆発の生成
									CExplosion::Create(posEnemy, 100, 100);

									if (objType == CScene::OBJTYPE_ENEMY)
									{
										int nRand = rand() % 8;

										if (nRand == 1)
										{
											//アイテム生成
											CItem::Create(pos, 2.0f, 50, 50, 0);
										}

										else if (nRand == 2)
										{
											//アイテム生成
											CItem::Create(pos, 2.0f, 50, 50, 1);
										}
									}

									// 敵の破棄
									pScene->Uninit();
								}

								else
								{
									//ランダムな位置に爆破演出 -50～50
									int nRandX = (rand() % 100) - 50;
									int nRandY = (rand() % 100) - 50;

									// 爆発の生成
									CExplosion::Create(D3DXVECTOR3(posEnemy.x + nRandX, posEnemy.y + nRandY, 0.0f), 30, 30);
								}

								//スコア加算
								CScore::AddScore(20);

								// 弾の破棄
								Uninit();
							}
						}
					}

					//PLAYERから発射された弾の場合	ボスの当たり判定
					if (m_nType == OBJTYPE_BULET_PLAYER)
					{
						//条件を満たしているかチェック
						int nDeatCnt = CManager::GetEnemyDead();

						//ステージ2の場合
						if (CManager::GetGameStage() == 2)
						{
							//中ボスを3体倒した場合
							if (nDeatCnt == 2)
							{
								// オブジェクトの種類がEnemy場合
								if (objType == CScene::OBJTYPE_ENEMY3)
								{
									// 位置の取得
									D3DXVECTOR3 posEnemy = pScene->GetPosition();
									float fEnemySizeX = pScene->GetSizeX() - 150.0f;
									float fEnemySizeY = pScene->GetSizeY() - 10.0f;

									// 当たり判定
									if (pos.x + m_fSizeX >= posEnemy.x - fEnemySizeX && pos.x - m_fSizeX <= posEnemy.x + fEnemySizeX &&
										pos.y - m_fSizeY <= posEnemy.y + fEnemySizeY && pos.y + m_fSizeY >= posEnemy.y - fEnemySizeY)
									{
										//サウンドSE
										pSound->Play(CSound::SOUND_LABEL_SE_HIT);

										//敵にダメージを与える
										pScene->Damage(m_nDamage);

										//敵のライフを読み込む
										int nLifeEnemy = pScene->GetLife();

										//ライフが0以下の場合
										if (nLifeEnemy <= 0)
										{
											//サウンドSE
											pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);

											// 大爆発の生成
											CExplosion::Create(posEnemy, 100, 100);

											if (objType == CScene::OBJTYPE_ENEMY)
											{
												int nRand = rand() % 8;

												if (nRand == 1)
												{
													//アイテム生成
													CItem::Create(pos, 2.0f, 50, 50, 0);
												}

												else if (nRand == 2)
												{
													//アイテム生成
													CItem::Create(pos, 2.0f, 50, 50, 1);
												}
											}

											//倒したのが中ボスの場合
											if (objType == CScene::OBJTYPE_ENEMY2)
											{
												CManager::SetEnemyDead(1);
											}

											// 敵の破棄
											pScene->Uninit();
										}

										else
										{
											//ランダムな位置に爆破演出 -50～50
											int nRandX = (rand() % 100) - 50;
											int nRandY = (rand() % 100) - 50;

											// 爆発の生成
											CExplosion::Create(D3DXVECTOR3(posEnemy.x + nRandX, posEnemy.y + nRandY, 0.0f), 30, 30);
										}

										//スコア加算
										CScore::AddScore(20);

										// 弾の破棄
										Uninit();
									}
								}
							}
						}
					}

					//ENEMYから発射された弾の場合
					if (m_nType == OBJTYPE_BULET_ENEMY || m_nType == OBJTYPE_BULET_ENEMY2 || m_nType == OBJTYPE_BULET_ENEMY3)
					{
						if (CPlayer::GetBumped() == false)
						{
							// オブジェクトの種類がPlayer場合
							if (objType == CScene::OBJTYPE_PLAYER)
							{
								// 位置の取得
								D3DXVECTOR3 posPlayer = pScene->GetPosition();

								// 当たり判定
								if (pos.x + m_fSizeX >= posPlayer.x - (PLAYER_SIZE - 40) && pos.x - m_fSizeX <= posPlayer.x + (PLAYER_SIZE - 40) &&
									pos.y - m_fSizeY <= posPlayer.y + (PLAYER_SIZE - 40) && pos.y + m_fSizeY >= posPlayer.y - (PLAYER_SIZE - 40))
								{

									//サウンドSE
									pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);

									//当たったときのタイムを記憶
									g_BulletTime = CGame::GetTime();
									g_BulletTime2 = CGame2::GetTime();

									//無敵状態にする
									CPlayer::SetBumped(true);

									//現在のHPにダメージを与える
									CManager::DamagePlayerLife(1);

									//現在のHPを記憶
									int nLife = CManager::GetPlayerLife();

									//ライフが0以下の場合
									if (nLife <= 0)
									{
										// PLAYERの破棄
										pScene->Uninit();
									}

									// 爆発の生成
									CExplosion::Create(posPlayer, 50, 50);

									// 弾の破棄
									Uninit();
								}
							}
						}
					}
				}
			}
		}
	}

	//弾のライフが0以下になったとき
	if (m_nLife <= 0)
	{
		Uninit();
	}

	//弾のライフを減らす
	m_nLife--;
}

//=============================================================================
// 描画処理
//=============================================================================
void CBullet::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// 弾の生成
//=============================================================================
CBullet * CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, int nDamage, float fSizeX, float fSizeY, int nType)
{
	//弾を生成
	CBullet *pBullet = NULL;
	pBullet = new CBullet;

	pBullet->Init(pos, move, col, 0.0f, nLife, nDamage, fSizeX, fSizeY, nType, 0);

	//テクスチャを入れる
	pBullet->BindTexture(m_pTexture);

	return pBullet;
}

//=============================================================================
// 弾の生成
//=============================================================================
CBullet * CBullet::Create2(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSpeed, int nLife, int nDamage, float fSizeX, float fSizeY, int nType, int nTypeMove)
{
	//弾を生成
	CBullet *pBullet = NULL;
	pBullet = new CBullet;

	pBullet->Init(pos, move, col, fSpeed, nLife, nDamage, fSizeX, fSizeY, nType, nTypeMove);

	//テクスチャを入れる
	pBullet->BindTexture(m_pTexture);

	//m_TypeMove = nTypeMove;

	return pBullet;
}

//=============================================================================
// テクスチャの生成
//=============================================================================
HRESULT CBullet::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,"data/TEXTURE/bullet000.png",&m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CBullet::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 角度を決められる弾の生成
//=============================================================================
void CBullet::InitAngleBullet(D3DXVECTOR3 pos, float fSpeed, D3DXCOLOR col, int nLife, int nDamage, float fSizeX, float fSizeY, float fAngle, int nType)
{
	//弾の乱射
	D3DXVECTOR3 move;
	float fA;
	fA = (float)(fAngle) / 100.0f;
	move.x = sinf(fA) * fSpeed;
	move.y = cosf(fA) * fSpeed;

	CBullet::Create(pos, move, col, nLife, nDamage, fSizeX, fSizeY, nType);
}

//=============================================================================
// 角度を決められる弾の生成
//=============================================================================
void CBullet::InitAngleBullet2(D3DXVECTOR3 pos, float fSpeed, D3DXCOLOR col, int nLife, int nDamage, float fSizeX, float fSizeY, float fAngle, int nType, int nTypeMove)
{
	//弾の乱射
	D3DXVECTOR3 move;
	float fA;
	fA = (float)(fAngle) / 100.0f;
	move.x = sinf(fA) * fSpeed;
	move.y = cosf(fA) * fSpeed;

	//m_TypeMove = nTypeMove;

	CBullet::Create2(pos, move, col, 0.0f, nLife, nDamage, fSizeX, fSizeY, nType, nTypeMove);
}

//=============================================================================
// 狙い撃ち弾の生成
//=============================================================================
void CBullet::InitAimingBullet(D3DXVECTOR3 posBullet, D3DXCOLOR col, float speed, D3DXVECTOR3 posPlayer)
{
	// 目標までの距離dを求める
	double d = sqrt((posPlayer.x - posBullet.x)*(posPlayer.x - posBullet.x) + (posPlayer.y - posBullet.y)*(posPlayer.y - posBullet.y));

	float vx;
	float vy;

	// 速さが一定値speedになるように速度を求める
	vx = (posPlayer.x - posBullet.x) / (float)d*speed;
	vy = (posPlayer.y - posBullet.y) / (float)d*speed;

	//弾の生成
	CBullet::Create(D3DXVECTOR3(posBullet.x, posBullet.y, 0.0f), D3DXVECTOR3(vx, vy, 0.0f), col, 1000, 0, 10.0f, 10.0f, OBJTYPE_BULET_ENEMY);
}

//=============================================================================
// 狙い撃ち弾の生成
//=============================================================================
void CBullet::InitAimingBullet2(D3DXVECTOR3 posBullet, D3DXCOLOR col, float speed, D3DXVECTOR3 posPlayer, int nTypeMove)
{
	// 目標までの距離dを求める
	double d = sqrt((posPlayer.x - posBullet.x)*(posPlayer.x - posBullet.x) + (posPlayer.y - posBullet.y)*(posPlayer.y - posBullet.y));

	float vx;
	float vy;

	// 速さが一定値speedになるように速度を求める
	vx = (posPlayer.x - posBullet.x) / (float)d*speed;
	vy = (posPlayer.y - posBullet.y) / (float)d*speed;

	//m_TypeMove = nTypeMove;

	//弾の生成
	CBullet::Create2(D3DXVECTOR3(posBullet.x, posBullet.y, 0.0f), D3DXVECTOR3(vx, vy, 0.0f), col, 0.0f, 1000, 0, 10.0f, 10.0f, OBJTYPE_BULET_ENEMY, nTypeMove);
}

//=============================================================================
// 目的地撃ち弾の生成
//=============================================================================
void CBullet::InitDestinations(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSpeed, int nLife, int nDamage, float fSizeX, float fSizeY, int nType)
{
	m_TypeAction = 1;
	//m_nextpos = move;
	
	CBullet::Create(pos, move, col, nLife, nDamage, fSizeX, fSizeY, nType);
}

//=============================================================================
// 目的地撃ち弾の生成
//=============================================================================
void CBullet::InitDestinations2(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSpeed, int nLife, int nDamage, float fSizeX, float fSizeY, int nType, int nTypeMove)
{
	m_TypeAction = 1;
	//m_nextpos = move;
	
	CBullet::Create2(pos, move, col, fSpeed, nLife, nDamage, fSizeX, fSizeY, nType, nTypeMove);
}

//=============================================================================
// 特殊移動
//=============================================================================
void CBullet::NextMove(int nTypeMove, int nAction, D3DXVECTOR3 fSpeed)
{
	g_TypeMoveAction = nTypeMove;
	g_Action = nAction;
	g_nextmove = fSpeed;
}

void CBullet::NextAimingBullet(int nTypeMove, int nAction, D3DXVECTOR3 posPlayer, float fSpeed)
{
	g_TypeMoveAction = nTypeMove;
	g_Action = nAction;
	g_nextpos = posPlayer;
	g_fSpeed = fSpeed;
}

void CBullet::NextMove2(int nTypeMove, int nAction, D3DXVECTOR3 fSpeed)
{
	g_TypeMoveAction2 = nTypeMove;
	g_Action2 = nAction;
	g_nextmove2 = fSpeed;
}

void CBullet::NextAimingBullet2(int nTypeMove, int nAction, D3DXVECTOR3 posPlayer, float fSpeed)
{
	g_TypeMoveAction2 = nTypeMove;
	g_Action2 = nAction;
	g_nextpos2 = posPlayer;
	g_fSpeed2 = fSpeed;
}
