//=============================================================================
//
// �e���� [bullet.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
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

// �ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;
int CBullet::m_TypeAction = NULL;

//�ړI�n��ύX
D3DXVECTOR3 g_nextmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 g_nextmove2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 g_nextmove3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//�ړI�n��ύX
D3DXVECTOR3 g_nextpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 g_nextpos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 g_nextpos3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//����s���Ɏg�p
int g_TypeMoveAction = NULL;
int g_TypeMoveAction2 = NULL;
int g_TypeMoveAction3 = NULL;

//����s���Ɏg�p
int g_Action = NULL;
int g_Action2 = NULL;
int g_Action3 = NULL;

//����s���Ɏg�p
float g_fSpeed = 0.0f;
float g_fSpeed2 = 0.0f;
float g_fSpeed3 = 0.0f;

//�ړ��U���ω�
int g_TypeMove = NULL;
int g_nAction = 0;

//���G����
int g_BulletTime;					//�^�C����ۑ�
int g_BulletTime2;					//�^�C����ۑ�
bool g_bBumped = false;				//���G���

//=============================================================================
// �C���X�g���N�^
//=============================================================================
CBullet::CBullet(int nPriority) : CScene2D(nPriority)
{
	m_nLife = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSpeed, int nLife, int nDamage, float fSizeX, float fSizeY, int nType, int nTypeMove)
{
	// CScene2D�̏���������
	CScene2D::Init(pos, fSizeX, fSizeY);

	//�����̏��������o�֐��ɓ����
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

	//�v���C���[����̔��˂��ꂽ�e�̏ꍇ
	if (nType == OBJTYPE_BULET_PLAYER)
	{
		// �I�u�W�F�N�g�̎�ސݒ�
		SetObjType(CScene::OBJTYPE_BULET_PLAYER);
		m_nType = OBJTYPE_BULET_PLAYER;
	}

	//�G����̔��˂��ꂽ�e�̏ꍇ
	else if (nType == OBJTYPE_BULET_ENEMY || nType == OBJTYPE_BULET_ENEMY2 || nType == OBJTYPE_BULET_ENEMY3)
	{
		// �I�u�W�F�N�g�̎�ސݒ�
		SetObjType(CScene::OBJTYPE_BULET_ENEMY);
		m_nType = nType;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBullet::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBullet::Update(void)
{
	//�T�E���h���擾
	CSound *pSound;
	pSound = CManager::GetSound();

	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	//�v���C���[����̒e�@�ꍇ
	if (m_nType == OBJTYPE_BULET_PLAYER)
	{
		//�G�t�F�N�g�𐶐�
		CEffect::Create(pos, 1, 25, 25, m_col);
	}

	//�G����̒e�@�ꍇ
	if (m_nType == OBJTYPE_BULET_ENEMY || m_nType == OBJTYPE_BULET_ENEMY2 || m_nType == OBJTYPE_BULET_ENEMY3)
	{
		//�G�t�F�N�g�𐶐�
		CEffect::Create(pos, 1, m_fSizeX * 3, m_fSizeY * 3, m_col);
	}
	
	//�ʒu�X�V
	pos += m_move;

	//�v���C���[�ȊO�̒e�̍X�V
	//�G���甭�˂��ꂽ�e
	if (m_nType != OBJTYPE_BULET_PLAYER)
	{
		//�_�������@�ړI�n�ɔ��˂����@
		if (m_nType == OBJTYPE_BULET_ENEMY2)
		{
			//��`
			D3DXVECTOR3 move;
			D3DXVECTOR3 vec;
			float fAngle;

			//�ʒu�X�V
			pos -= m_move;

			//�ړI�n���v�Z
			vec = m_move - pos;

			//�ʒu�X�V�@�p�x�v�Z
			fAngle = atan2f(vec.x, vec.y);						//�x�N�g���̊p�x�����߂�
			move.x = sinf(fAngle) * m_fSpeedDestinations;		//�ړ��ʂ����߂�
			move.y = cosf(fAngle) * m_fSpeedDestinations;		//�ړ��ʂ����߂�

			//�ړI�n�ɋ߂Â�����X�V���~�߂�
			if (pos.y >= m_move.y - 10.0f && pos.y <= m_move.y + 10.0f && pos.x >= m_move.x - 10.0f && pos.x <= m_move.x + 10.0f)
			{
				//�������Ȃ�
				m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_nType = OBJTYPE_BULET_ENEMY;
			}

			else
			{
				//�ʒu�X�V��������
				pos += move;
			}
		}

		//�ړ����ɐV���ɒe�𐶐������e
		if (m_nType == OBJTYPE_BULET_ENEMY3)
		{
			if (m_bCreatepos == false)
			{
				m_createpos = pos;
				m_bCreatepos = true;
			}

			//�ǂ�J��ړ��������v�ZX
			if (pos.x > (m_createpos.x + 50.0f) || pos.x < (m_createpos.x - 50.0f))
			{
				CBullet::Create2(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 1500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY, m_TypeMove);
				m_bCreatepos = false;
			}

			//�ǂ�J��ړ��������v�ZY
			else if (pos.y > m_createpos.y + 50.0f || pos.y < m_createpos.y - 50.0f)
			{
				CBullet::Create2(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, 1500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY, m_TypeMove);
				m_bCreatepos = false;
			}
		}

		//�O���[�o���ϐ��ŊǗ�
		//����ړ��U��
		if (g_TypeMoveAction == m_TypeMove)
		{
			m_nType = OBJTYPE_BULET_ENEMY;

			//�X�s�[�h�ς���
			if (g_Action == 1)
			{
				m_bUseAming = false;

				//�ʒu�X�V
				pos -= m_move;
				pos += g_nextmove;
			}

			//�_������
			if (g_Action == 2)
			{
				if (m_bUseAming == false)
				{
					m_oldpos = pos;

					m_bUseAming = true;
				}

				//�ʒu�X�V
				pos -= m_move;

				// �ڕW�܂ł̋���d�����߂�
				double d = sqrt((g_nextpos.x - m_oldpos.x)*(g_nextpos.x - m_oldpos.x) + (g_nextpos.y - m_oldpos.y)*(g_nextpos.y - m_oldpos.y));

				float vx;
				float vy;

				// ���������lspeed�ɂȂ�悤�ɑ��x�����߂�
				vx = (g_nextpos.x - m_oldpos.x) / (float)d*g_fSpeed;
				vy = (g_nextpos.y - m_oldpos.y) / (float)d*g_fSpeed;

				pos += D3DXVECTOR3(vx, vy, 0.0f);
			}

			//�ړI�n�Ɉړ�
			if (g_Action == 3)
			{
				m_bUseAming = false;

				//��`
				D3DXVECTOR3 move;
				D3DXVECTOR3 vec;
				float fAngle;

				//�ʒu�X�V
				pos -= m_move;

				//�ړI�n���v�Z
				vec = g_nextpos - pos;

				//�ʒu�X�V
				fAngle = atan2f(vec.x, vec.y);			//�x�N�g���̊p�x�����߂�
				move.x = sinf(fAngle) * g_fSpeed;		//�ړ��ʂ����߂�
				move.y = cosf(fAngle) * g_fSpeed;		//�ړ��ʂ����߂�

														//�ړI�n�ɋ߂Â�����X�V���~�߂�
				if (pos.y >= g_nextpos.y - 10.0f && pos.y <= g_nextpos.y + 10.0f && pos.x >= g_nextpos.x - 10.0f && pos.x <= g_nextpos.x + 10.0f)
				{
					//�������Ȃ�
				}

				else
				{
					//�ʒu�X�V��������
					pos += move;
				}
			}
		}

		else if (g_TypeMoveAction2 == m_TypeMove)
		{
			m_nType = OBJTYPE_BULET_ENEMY;

			//�X�s�[�h�ς���
			if (g_Action2 == 1)
			{

				m_bUseAming = false;

				//�ʒu�X�V
				pos -= m_move;
				pos += g_nextmove2;
			}

			//�_������
			if (g_Action2 == 2)
			{
				if (m_bUseAming == false)
				{
					m_oldpos = pos;

					m_bUseAming = true;
				}

				//�ʒu�X�V
				pos -= m_move;

				// �ڕW�܂ł̋���d�����߂�
				double d = sqrt((g_nextpos2.x - m_oldpos.x)*(g_nextpos2.x - m_oldpos.x) + (g_nextpos2.y - m_oldpos.y)*(g_nextpos2.y - m_oldpos.y));

				float vx;
				float vy;

				// ���������lspeed�ɂȂ�悤�ɑ��x�����߂�
				vx = (g_nextpos2.x - m_oldpos.x) / (float)d*g_fSpeed2;
				vy = (g_nextpos2.y - m_oldpos.y) / (float)d*g_fSpeed2;

				pos += D3DXVECTOR3(vx, vy, 0.0f);
			}

			//�ړI�n�Ɉړ�
			if (g_Action2 == 3)
			{
				m_bUseAming = false;

				//��`
				D3DXVECTOR3 move;
				D3DXVECTOR3 vec;
				float fAngle;

				//�ʒu�X�V
				pos -= m_move;

				//�ړI�n���v�Z
				vec = g_nextpos2 - pos;

				//�ʒu�X�V
				fAngle = atan2f(vec.x, vec.y);			//�x�N�g���̊p�x�����߂�
				move.x = sinf(fAngle) * g_fSpeed2;		//�ړ��ʂ����߂�
				move.y = cosf(fAngle) * g_fSpeed2;		//�ړ��ʂ����߂�

														//�ړI�n�ɋ߂Â�����X�V���~�߂�
				if (pos.y >= g_nextpos2.y - 10.0f && pos.y <= g_nextpos2.y + 10.0f && pos.x >= g_nextpos2.x - 10.0f && pos.x <= g_nextpos2.x + 10.0f)
				{
					//�������Ȃ�
				}

				else
				{
					//�ʒu�X�V��������
					pos += move;
				}
			}
		}

		else if (g_TypeMoveAction3 == m_TypeMove)
		{
			m_nType = OBJTYPE_BULET_ENEMY;

			//�X�s�[�h�ς���
			if (g_Action3 == 1)
			{
				m_bUseAming = false;

				//�ʒu�X�V
				pos -= m_move;
				pos += g_nextmove3;
			}

			//�_������
			if (g_Action3 == 2)
			{
				if (m_bUseAming == false)
				{
					m_oldpos = pos;

					m_bUseAming = true;
				}

				//�ʒu�X�V
				pos -= m_move;

				// �ڕW�܂ł̋���d�����߂�
				double d = sqrt((g_nextpos3.x - m_oldpos.x)*(g_nextpos3.x - m_oldpos.x) + (g_nextpos3.y - m_oldpos.y)*(g_nextpos3.y - m_oldpos.y));

				float vx;
				float vy;

				// ���������lspeed�ɂȂ�悤�ɑ��x�����߂�
				vx = (g_nextpos3.x - m_oldpos.x) / (float)d*g_fSpeed3;
				vy = (g_nextpos3.y - m_oldpos.y) / (float)d*g_fSpeed3;

				pos += D3DXVECTOR3(vx, vy, 0.0f);
			}

			//�ړI�n�Ɉړ�
			if (g_Action3 == 3)
			{
				m_bUseAming = false;

				//��`
				D3DXVECTOR3 move;
				D3DXVECTOR3 vec;
				float fAngle;

				//�ʒu�X�V
				pos -= m_move;

				//�ړI�n���v�Z
				vec = g_nextpos3 - pos;

				//�ʒu�X�V
				fAngle = atan2f(vec.x, vec.y);			//�x�N�g���̊p�x�����߂�
				move.x = sinf(fAngle) * g_fSpeed3;		//�ړ��ʂ����߂�
				move.y = cosf(fAngle) * g_fSpeed3;		//�ړ��ʂ����߂�

														//�ړI�n�ɋ߂Â�����X�V���~�߂�
				if (pos.y >= g_nextpos3.y - 10.0f && pos.y <= g_nextpos3.y + 10.0f && pos.x >= g_nextpos3.x - 10.0f && pos.x <= g_nextpos3.x + 10.0f)
				{
					//�������Ȃ�
				}

				else
				{
					//�ʒu�X�V��������
					pos += move;
				}
			}

		}
	}

	//�ړ�������
	SetPosition(pos);

	//�X�V
	CScene2D::Update();

	//�͈͊O�̏ꍇ
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

	//�����蔻��
	else
	{
		//����ȊO�̏ꍇ�����蔻�菈��������
		// �����蔻��
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			for (int nCntPriority = 1; nCntPriority < 3; nCntPriority++)
			{
				// �I�u�W�F�N�g���̎擾
				CScene *pScene;
				pScene = GetScene(nCntScene, nCntPriority);

				// NULL�`�F�b�N
				if (pScene != NULL)
				{
					// �I�u�W�F�N�g�̎�ނ̎擾
					CScene::OBJTYPE objType = pScene->GetObjType();

					//PLAYER���甭�˂��ꂽ�e�̏ꍇ
					if (m_nType == OBJTYPE_BULET_PLAYER)
					{
						// �I�u�W�F�N�g�̎�ނ�Enemy�ꍇ
						if (objType == CScene::OBJTYPE_ENEMY || objType == CScene::OBJTYPE_ENEMY2)
						{
							// �ʒu�̎擾
							D3DXVECTOR3 posEnemy = pScene->GetPosition();
							float fEnemySizeX = pScene->GetSizeX();
							float fEnemySizeY = pScene->GetSizeY();

							// �����蔻��
							if (pos.x + m_fSizeX >= posEnemy.x - fEnemySizeX && pos.x - m_fSizeX <= posEnemy.x + fEnemySizeX &&
								pos.y - m_fSizeY <= posEnemy.y + fEnemySizeY && pos.y + m_fSizeY >= posEnemy.y - fEnemySizeY)
							{
								//�T�E���hSE
								pSound->Play(CSound::SOUND_LABEL_SE_HIT);

								//�G�Ƀ_���[�W��^����
								pScene->Damage(m_nDamage);

								//�G�̃��C�t��ǂݍ���
								int nLifeEnemy = pScene->GetLife();

								//���C�t��0�ȉ��̏ꍇ
								if (nLifeEnemy <= 0)
								{
									//�T�E���hSE
									pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);

									// �唚���̐���
									CExplosion::Create(posEnemy, 100, 100);

									if (objType == CScene::OBJTYPE_ENEMY)
									{
										int nRand = rand() % 8;

										if (nRand == 1)
										{
											//�A�C�e������
											CItem::Create(pos, 2.0f, 50, 50, 0);
										}

										else if (nRand == 2)
										{
											//�A�C�e������
											CItem::Create(pos, 2.0f, 50, 50, 1);
										}
									}

									// �G�̔j��
									pScene->Uninit();
								}

								else
								{
									//�����_���Ȉʒu�ɔ��j���o -50�`50
									int nRandX = (rand() % 100) - 50;
									int nRandY = (rand() % 100) - 50;

									// �����̐���
									CExplosion::Create(D3DXVECTOR3(posEnemy.x + nRandX, posEnemy.y + nRandY, 0.0f), 30, 30);
								}

								//�X�R�A���Z
								CScore::AddScore(20);

								// �e�̔j��
								Uninit();
							}
						}
					}

					//PLAYER���甭�˂��ꂽ�e�̏ꍇ	�{�X�̓����蔻��
					if (m_nType == OBJTYPE_BULET_PLAYER)
					{
						//�����𖞂����Ă��邩�`�F�b�N
						int nDeatCnt = CManager::GetEnemyDead();

						//�X�e�[�W2�̏ꍇ
						if (CManager::GetGameStage() == 2)
						{
							//���{�X��3�̓|�����ꍇ
							if (nDeatCnt == 2)
							{
								// �I�u�W�F�N�g�̎�ނ�Enemy�ꍇ
								if (objType == CScene::OBJTYPE_ENEMY3)
								{
									// �ʒu�̎擾
									D3DXVECTOR3 posEnemy = pScene->GetPosition();
									float fEnemySizeX = pScene->GetSizeX() - 150.0f;
									float fEnemySizeY = pScene->GetSizeY() - 10.0f;

									// �����蔻��
									if (pos.x + m_fSizeX >= posEnemy.x - fEnemySizeX && pos.x - m_fSizeX <= posEnemy.x + fEnemySizeX &&
										pos.y - m_fSizeY <= posEnemy.y + fEnemySizeY && pos.y + m_fSizeY >= posEnemy.y - fEnemySizeY)
									{
										//�T�E���hSE
										pSound->Play(CSound::SOUND_LABEL_SE_HIT);

										//�G�Ƀ_���[�W��^����
										pScene->Damage(m_nDamage);

										//�G�̃��C�t��ǂݍ���
										int nLifeEnemy = pScene->GetLife();

										//���C�t��0�ȉ��̏ꍇ
										if (nLifeEnemy <= 0)
										{
											//�T�E���hSE
											pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);

											// �唚���̐���
											CExplosion::Create(posEnemy, 100, 100);

											if (objType == CScene::OBJTYPE_ENEMY)
											{
												int nRand = rand() % 8;

												if (nRand == 1)
												{
													//�A�C�e������
													CItem::Create(pos, 2.0f, 50, 50, 0);
												}

												else if (nRand == 2)
												{
													//�A�C�e������
													CItem::Create(pos, 2.0f, 50, 50, 1);
												}
											}

											//�|�����̂����{�X�̏ꍇ
											if (objType == CScene::OBJTYPE_ENEMY2)
											{
												CManager::SetEnemyDead(1);
											}

											// �G�̔j��
											pScene->Uninit();
										}

										else
										{
											//�����_���Ȉʒu�ɔ��j���o -50�`50
											int nRandX = (rand() % 100) - 50;
											int nRandY = (rand() % 100) - 50;

											// �����̐���
											CExplosion::Create(D3DXVECTOR3(posEnemy.x + nRandX, posEnemy.y + nRandY, 0.0f), 30, 30);
										}

										//�X�R�A���Z
										CScore::AddScore(20);

										// �e�̔j��
										Uninit();
									}
								}
							}
						}
					}

					//ENEMY���甭�˂��ꂽ�e�̏ꍇ
					if (m_nType == OBJTYPE_BULET_ENEMY || m_nType == OBJTYPE_BULET_ENEMY2 || m_nType == OBJTYPE_BULET_ENEMY3)
					{
						if (CPlayer::GetBumped() == false)
						{
							// �I�u�W�F�N�g�̎�ނ�Player�ꍇ
							if (objType == CScene::OBJTYPE_PLAYER)
							{
								// �ʒu�̎擾
								D3DXVECTOR3 posPlayer = pScene->GetPosition();

								// �����蔻��
								if (pos.x + m_fSizeX >= posPlayer.x - (PLAYER_SIZE - 40) && pos.x - m_fSizeX <= posPlayer.x + (PLAYER_SIZE - 40) &&
									pos.y - m_fSizeY <= posPlayer.y + (PLAYER_SIZE - 40) && pos.y + m_fSizeY >= posPlayer.y - (PLAYER_SIZE - 40))
								{

									//�T�E���hSE
									pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);

									//���������Ƃ��̃^�C�����L��
									g_BulletTime = CGame::GetTime();
									g_BulletTime2 = CGame2::GetTime();

									//���G��Ԃɂ���
									CPlayer::SetBumped(true);

									//���݂�HP�Ƀ_���[�W��^����
									CManager::DamagePlayerLife(1);

									//���݂�HP���L��
									int nLife = CManager::GetPlayerLife();

									//���C�t��0�ȉ��̏ꍇ
									if (nLife <= 0)
									{
										// PLAYER�̔j��
										pScene->Uninit();
									}

									// �����̐���
									CExplosion::Create(posPlayer, 50, 50);

									// �e�̔j��
									Uninit();
								}
							}
						}
					}
				}
			}
		}
	}

	//�e�̃��C�t��0�ȉ��ɂȂ����Ƃ�
	if (m_nLife <= 0)
	{
		Uninit();
	}

	//�e�̃��C�t�����炷
	m_nLife--;
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBullet::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �e�̐���
//=============================================================================
CBullet * CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, int nDamage, float fSizeX, float fSizeY, int nType)
{
	//�e�𐶐�
	CBullet *pBullet = NULL;
	pBullet = new CBullet;

	pBullet->Init(pos, move, col, 0.0f, nLife, nDamage, fSizeX, fSizeY, nType, 0);

	//�e�N�X�`��������
	pBullet->BindTexture(m_pTexture);

	return pBullet;
}

//=============================================================================
// �e�̐���
//=============================================================================
CBullet * CBullet::Create2(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSpeed, int nLife, int nDamage, float fSizeX, float fSizeY, int nType, int nTypeMove)
{
	//�e�𐶐�
	CBullet *pBullet = NULL;
	pBullet = new CBullet;

	pBullet->Init(pos, move, col, fSpeed, nLife, nDamage, fSizeX, fSizeY, nType, nTypeMove);

	//�e�N�X�`��������
	pBullet->BindTexture(m_pTexture);

	//m_TypeMove = nTypeMove;

	return pBullet;
}

//=============================================================================
// �e�N�X�`���̐���
//=============================================================================
HRESULT CBullet::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,"data/TEXTURE/bullet000.png",&m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
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
// �p�x�����߂���e�̐���
//=============================================================================
void CBullet::InitAngleBullet(D3DXVECTOR3 pos, float fSpeed, D3DXCOLOR col, int nLife, int nDamage, float fSizeX, float fSizeY, float fAngle, int nType)
{
	//�e�̗���
	D3DXVECTOR3 move;
	float fA;
	fA = (float)(fAngle) / 100.0f;
	move.x = sinf(fA) * fSpeed;
	move.y = cosf(fA) * fSpeed;

	CBullet::Create(pos, move, col, nLife, nDamage, fSizeX, fSizeY, nType);
}

//=============================================================================
// �p�x�����߂���e�̐���
//=============================================================================
void CBullet::InitAngleBullet2(D3DXVECTOR3 pos, float fSpeed, D3DXCOLOR col, int nLife, int nDamage, float fSizeX, float fSizeY, float fAngle, int nType, int nTypeMove)
{
	//�e�̗���
	D3DXVECTOR3 move;
	float fA;
	fA = (float)(fAngle) / 100.0f;
	move.x = sinf(fA) * fSpeed;
	move.y = cosf(fA) * fSpeed;

	//m_TypeMove = nTypeMove;

	CBullet::Create2(pos, move, col, 0.0f, nLife, nDamage, fSizeX, fSizeY, nType, nTypeMove);
}

//=============================================================================
// �_�������e�̐���
//=============================================================================
void CBullet::InitAimingBullet(D3DXVECTOR3 posBullet, D3DXCOLOR col, float speed, D3DXVECTOR3 posPlayer)
{
	// �ڕW�܂ł̋���d�����߂�
	double d = sqrt((posPlayer.x - posBullet.x)*(posPlayer.x - posBullet.x) + (posPlayer.y - posBullet.y)*(posPlayer.y - posBullet.y));

	float vx;
	float vy;

	// ���������lspeed�ɂȂ�悤�ɑ��x�����߂�
	vx = (posPlayer.x - posBullet.x) / (float)d*speed;
	vy = (posPlayer.y - posBullet.y) / (float)d*speed;

	//�e�̐���
	CBullet::Create(D3DXVECTOR3(posBullet.x, posBullet.y, 0.0f), D3DXVECTOR3(vx, vy, 0.0f), col, 1000, 0, 10.0f, 10.0f, OBJTYPE_BULET_ENEMY);
}

//=============================================================================
// �_�������e�̐���
//=============================================================================
void CBullet::InitAimingBullet2(D3DXVECTOR3 posBullet, D3DXCOLOR col, float speed, D3DXVECTOR3 posPlayer, int nTypeMove)
{
	// �ڕW�܂ł̋���d�����߂�
	double d = sqrt((posPlayer.x - posBullet.x)*(posPlayer.x - posBullet.x) + (posPlayer.y - posBullet.y)*(posPlayer.y - posBullet.y));

	float vx;
	float vy;

	// ���������lspeed�ɂȂ�悤�ɑ��x�����߂�
	vx = (posPlayer.x - posBullet.x) / (float)d*speed;
	vy = (posPlayer.y - posBullet.y) / (float)d*speed;

	//m_TypeMove = nTypeMove;

	//�e�̐���
	CBullet::Create2(D3DXVECTOR3(posBullet.x, posBullet.y, 0.0f), D3DXVECTOR3(vx, vy, 0.0f), col, 0.0f, 1000, 0, 10.0f, 10.0f, OBJTYPE_BULET_ENEMY, nTypeMove);
}

//=============================================================================
// �ړI�n�����e�̐���
//=============================================================================
void CBullet::InitDestinations(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSpeed, int nLife, int nDamage, float fSizeX, float fSizeY, int nType)
{
	m_TypeAction = 1;
	//m_nextpos = move;
	
	CBullet::Create(pos, move, col, nLife, nDamage, fSizeX, fSizeY, nType);
}

//=============================================================================
// �ړI�n�����e�̐���
//=============================================================================
void CBullet::InitDestinations2(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSpeed, int nLife, int nDamage, float fSizeX, float fSizeY, int nType, int nTypeMove)
{
	m_TypeAction = 1;
	//m_nextpos = move;
	
	CBullet::Create2(pos, move, col, fSpeed, nLife, nDamage, fSizeX, fSizeY, nType, nTypeMove);
}

//=============================================================================
// ����ړ�
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
