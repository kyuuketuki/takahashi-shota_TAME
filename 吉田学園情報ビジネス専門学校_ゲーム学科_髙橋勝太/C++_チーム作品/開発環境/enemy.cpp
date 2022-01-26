//=============================================================================
//
// �G���� [enemy.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
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

// �ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[ENEMY_MAX] = {};
CPlayer *CEnemy::m_pPlayer = NULL;

//�ړI�n�ꏊ
D3DXVECTOR3 g_objective2;
D3DXVECTOR3 g_objective3;
D3DXVECTOR3 g_objective4;

//�ړ����������ԍ�
int g_nType2 = NULL;
int g_nType3 = NULL;
int g_nType4 = NULL;

//�ړ��ۑ��p
float g_fMove2 = NULL;
float g_fMove3 = NULL;
float g_fMove4 = NULL;

//�ړ������Ɏg�p
bool g_bUseEnemyMove = false;
bool g_bUseEnemyMove2 = false;
bool g_bUseEnemyMove3 = false;

//����U���Ǘ�
int g_nNextAttack = false;
int g_nNextAttack2 = false;
int g_nNextAttack3 = false;

//�e�̔���
int g_nCntEnemyBullet = 0;
int g_nAttack = 0;

//�ړ��Ǘ��ԍ�
bool g_bMoveType = false;

float g_fAnimeEnemy = 0.0f;
bool g_bMoveEnemy = false;

//�v���C���[�̖��G����
int g_nHitTimePlayer = 0;
bool g_bInvincibilityPlayer = false;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(int nPriority) : CScene2D(nPriority)
{
	//�O���[�o���ϐ��̏�����
	g_fMove2 = 0;
	g_fMove3 = 0;
	g_fMove4 = 0;

	g_bUseEnemyMove = false;
	g_bUseEnemyMove2 = false;
	g_bUseEnemyMove3 = false;

	g_bInvincibilityPlayer = false;
	g_nHitTimePlayer = 0;

	//�����o�ϐ��̏�����
	m_bAngleEnemy = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{
	
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, float fmove, D3DXVECTOR3 objective, int nLife, float fSizeX, float fSizeY, int nTime, int nType, int nMoveType)
{
	// CScene2D�̏���������
	CScene2D::Init(pos, fSizeX, fSizeY);

	//�T�C�Y�����󂯓n��
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;

	//�T�C�Y�����V�[���N���X�ɋL��
	SetSizeXScene(fSizeX);
	SetSizeYScene(fSizeY);

	//�����o�֐��ɑ��
	m_pos = pos;
	m_objective = objective;
	m_fMove = fmove;
	m_nLife = nLife;
	m_nTime = nTime;
	m_nType = nType;
	m_nMoveType = nMoveType;

	//�G�̃��C�t�����󂯓n��
	CScene::SetLife(nLife);

	//�^�C�v�ɂ���ĕς��鏈�����e
	if (m_nType == ENEMY_06)
	{
		m_fAngleEnemyBullet = D3DX_PI * 100;
	}

	if (m_nType == ENEMY_07)
	{
		m_fAngleEnemyBullet = D3DX_PI * 100;
	}

	// �I�u�W�F�N�g�̎�ސݒ�
	SetObjType(CScene::OBJTYPE_ENEMY);

	if (m_nType == ENEMY_BOSS_02 || m_nType == ENEMY_BOSS_03 || m_nType == ENEMY_05)
	{
		// �I�u�W�F�N�g�̎�ސݒ�
		SetObjType(CScene::OBJTYPE_ENEMY2);
	}

	if (m_nType == ENEMY_BOSS_01)
	{
		// �I�u�W�F�N�g�̎�ސݒ�
		SetObjType(CScene::OBJTYPE_ENEMY3);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	//��]�����@�G���G
	if (m_nType != ENEMY_99 && m_nType != ENEMY_BOSS_01  && m_nType != ENEMY_BOSS_02 && m_nType != ENEMY_BOSS_03)
	{
		//��]����
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

	//��]�����@�{�X
	if (m_nType == ENEMY_BOSS_01 || m_nType == ENEMY_BOSS_02 || m_nType == ENEMY_BOSS_03)
	{
		//��]����
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

	//覐�
	if (m_nType == ENEMY_99)
	{
		m_fMoveEnemy += 0.005f;
	}

	//��]�ݒ�@�X�V
	SetRevolution(m_fMoveEnemy);
	
	//�e�����˂���^�C�~���O�irand�j
	int nCntEnemyBulletRand = rand() % 100;

	//�����̒l��0�̏ꍇ�U�������
	if (nCntEnemyBulletRand == 0)
	{
		if (m_nType == ENEMY_01)		//�G01
		{
			//�e�̔���
			CBullet::Create(pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 200, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
		}

		if (m_nType == ENEMY_02)		//�G02
		{
			//�O�̒e�𔭎�
			CBullet::InitAngleBullet(pos, 5.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, 0.0f, OBJTYPE_BULET_ENEMY);
			CBullet::InitAngleBullet(pos, 5.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, 30.0f, OBJTYPE_BULET_ENEMY);
			CBullet::InitAngleBullet(pos, 5.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, -30.0f, OBJTYPE_BULET_ENEMY);
		}

		if (m_nType == ENEMY_03)		//�G03
		{
			//�_���e
			D3DXVECTOR3 posPlayer = m_pPlayer->GetPlayer();
			CBullet::InitAimingBullet(pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 8.0f, D3DXVECTOR3(posPlayer.x, posPlayer.y, 0.0f));
		}
	}

	//����U������
	Attack();

	//�ړ�����
	Move();

	//�X�V
	CScene2D::Update();

	//���C�t��0�̏ꍇ�ɏ���
	if (m_nLife <= 0)
	{
		Uninit();
	}

	//���ɍs���������
	else if (pos.y >= SCREEN_HEIGHT + 100.0f)
	{
		Uninit();
	}

	//����ȊO�̏ꍇ�����蔻�菈��������
	else
	{
		//�Փ˔���
		Collision();
	}

	//�G�̖��G����
	if (m_bInvincibility == true)
	{
		//���G���Ԃ��J�E���g
		m_nHitTime++;

		if (m_nHitTime == 1)
		{
			//�����̃p�[�e�B�N��
			for (int nCnt = 0; nCnt < 10; nCnt++)
			{
				//������ϐ��ɑ��
				int nRand = rand() % 628;
				int nRandSpeed = (rand() % 5) + 5;

				//�p�[�e�B�N��
				CParticl::Create(pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f), 20, (float)nRandSpeed, (float)20, (float)20, (float)nRand);
			}
		}
	}

	//���G���̐F�ω��̐F��߂�
	if (m_nHitTime >= 20)
	{
		//�F�ύX
		CScene2D::SetCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_bInvincibility = false;
		m_nHitTime = 0;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �Փ˔��菈��
//=============================================================================
void CEnemy::Collision(void)
{
	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �G�Ƃ̓����蔻��
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
				if (CPlayer::GetBumped() == false)
				{
					// �I�u�W�F�N�g�̎�ނ̎擾
					CScene::OBJTYPE objType = pScene->GetObjType();

					//�{�X�̓����蔻�������
					//if (m_nType != CScene::OBJTYPE_BULET_ENEMY3)
					{
						// �I�u�W�F�N�g�̎�ނ�Player�ꍇ
						if (objType == CScene::OBJTYPE_PLAYER)
						{
							// �ʒu�̎擾
							D3DXVECTOR3 posPlayer = pScene->GetPosition();

							// �I�u�W�F�N�g�̎�ނ�ENEMY_BOSS_01�ꍇ�����蔻��𒲐�����
							if (m_nType == ENEMY_BOSS_01)
							{
								float fEnemySizeX = pScene->GetSizeX() - 150.0f;
								float fEnemySizeY = pScene->GetSizeY() - 10.0f;
							}

							// �����蔻��
							if (pos.x + m_fSizeX >= posPlayer.x - (PLAYER_SIZE - 40) && pos.x - m_fSizeX <= posPlayer.x + (PLAYER_SIZE - 40) &&
								pos.y - m_fSizeY <= posPlayer.y + (PLAYER_SIZE - 40) && pos.y + m_fSizeY >= posPlayer.y - (PLAYER_SIZE - 40))
							{
								//���G��Ԃɂ���
								g_bInvincibilityPlayer = true;

								//���G��Ԃɂ���
								CPlayer::SetBumped(true);

								CManager::DamagePlayerLife(1);

								int nLifeEnemy = CManager::GetPlayerLife();

								//���C�t��0�ȉ��̏ꍇ
								if (nLifeEnemy <= 0)
								{
									// PLAYER�̔j��
									pScene->Uninit();
								}

								// �����̐���
								CExplosion::Create(posPlayer, 50, 50);
							}
						}
					}
				}
			}
		}
	}

	//���G���ԁ��e�ɓ��������Ƃ��̐F�ω�
	// �����蔻��
	for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
	{
		// �I�u�W�F�N�g���̎擾
		CScene *pScene;
		pScene = GetScene(nCntScene, 4);

		// NULL�`�F�b�N
		if (pScene != NULL)
		{
			// �I�u�W�F�N�g�̎�ނ̎擾
			CScene::OBJTYPE objType = pScene->GetObjType();

			// �ʒu�̎擾
			D3DXVECTOR3 posBullet = pScene->GetPosition();

			//�X�e�[�W2�̏ꍇ
			if (CManager::GetGameStage() == 2)
			{
				//�{�X�̓����蔻��
				if (CManager::GetEnemyDead() == 2)		//�����𖞂����Ă���ꍇ
				{
					// �I�u�W�F�N�g�̎�ނ�ENEMY_BOSS_01�ꍇ
					if (m_nType == ENEMY_BOSS_01)
					{
						float fEnemySizeX = pScene->GetSizeX() - 150.0f;
						float fEnemySizeY = pScene->GetSizeY();

						//PLAYER���甭�˂��ꂽ�e�̏ꍇ
						if (objType == OBJTYPE_BULET_PLAYER)
						{
							// �����蔻��
							if (pos.x + m_fSizeX >= posBullet.x - fEnemySizeX && pos.x - m_fSizeX <= posBullet.x + fEnemySizeX &&
								pos.y - m_fSizeY <= posBullet.y + fEnemySizeY && pos.y + m_fSizeY >= posBullet.y - fEnemySizeY)
							{
								//�F�ύX
								CScene2D::SetCollar(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
								m_bInvincibility = true;
							}
						}
					}
				}
			}

			// �I�u�W�F�N�g�̎�ނ�ENEMY_BOSS_01�ȊO�̏ꍇ
			if (m_nType != ENEMY_BOSS_01)
			{
				//PLAYER���甭�˂��ꂽ�e�̏ꍇ
				if (objType == OBJTYPE_BULET_PLAYER)
				{
					//�T�C�Y���擾
					float fEnemySizeX = pScene->GetSizeX();
					float fEnemySizeY = pScene->GetSizeY() + 10.0f;

					// �����蔻��
					if (pos.x + m_fSizeX >= posBullet.x - fEnemySizeX && pos.x - m_fSizeX <= posBullet.x + fEnemySizeX &&
						pos.y - m_fSizeY <= posBullet.y + fEnemySizeY && pos.y + m_fSizeY >= posBullet.y - fEnemySizeY)
					{
						//�F�ύX
						CScene2D::SetCollar(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
						m_bInvincibility = true;
					}
				}
			}
		}
	}
}

//=============================================================================
// ����U������
//=============================================================================
void CEnemy::Attack(void)
{
	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	//����U��
	//�O���[�o���ϐ��̒l�ɂ���čU�����ς��
	if (g_nNextAttack == 1 || g_nNextAttack2 == 1)
	{
		//�J�E���g��i�߂�
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

			//�J�E���g���Z�b�g
			m_nAttackCnt2 = 0;
		}
	}

	if (g_nNextAttack == 2 || g_nNextAttack2 == 2)
	{
		if (m_nType == ENEMY_04)
		{
			//�J�E���g��i�߂�
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 50)
			{
				//�e�̔���
				CBullet::Create(pos, D3DXVECTOR3(5.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
				CBullet::Create(pos, D3DXVECTOR3(-5.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);

				//�J�E���g���Z�b�g
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 3 || g_nNextAttack2 == 3)
	{
		if (m_nType == ENEMY_04)
		{
			//�J�E���g��i�߂�
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 50)
			{
				//�e�̔���
				CBullet::Create(pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
				CBullet::Create(pos, D3DXVECTOR3(0.0f, -5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);

				//�J�E���g���Z�b�g
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 4 || g_nNextAttack2 == 4)
	{
		if (m_nType == ENEMY_04)
		{
			//�J�E���g��i�߂�
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 100)
			{
				//�e�̔���
				CBullet::Create(pos, D3DXVECTOR3(5.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
				CBullet::Create(pos, D3DXVECTOR3(-5.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
				CBullet::Create(pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
				CBullet::Create(pos, D3DXVECTOR3(0.0f, -5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);

				//�J�E���g���Z�b�g
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 5 || g_nNextAttack2 == 5)
	{
		if (m_nType == ENEMY_05)
		{
			//�J�E���g��i�߂�
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 100)
			{
				//�U������
				CPolygon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 200, 0, 0, 5.0f, 0.0f, 0.0f, 1000, 0, OBJTYPE_BULET_ENEMY, 4);
				CPolygon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 200, 0, 0, 5.0f, 1.0f, 0.0f, 1000, 0, OBJTYPE_BULET_ENEMY, 4);

				//�J�E���g���Z�b�g
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 6 || g_nNextAttack2 == 6)
	{
		if (m_nType == ENEMY_05)
		{
			//�J�E���g��i�߂�
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 100)
			{
				//�U������
				CPolygon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 200, 0, 0, 5.0f, 0.0f, 0.01f, 1000, 0, OBJTYPE_BULET_ENEMY, 4);
				CPolygon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 200, 0, 0, 5.0f, 1.0f, -0.01f, 1000, 0, OBJTYPE_BULET_ENEMY, 4);

				//�J�E���g���Z�b�g
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 7 || g_nNextAttack2 == 7)
	{
		if (m_nType == ENEMY_06)
		{
			//�J�E���g��i�߂�
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 20)
			{
				//�e�̔���
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -10.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet, OBJTYPE_BULET_ENEMY);
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -10.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet + 30.0f, OBJTYPE_BULET_ENEMY);

				//�p�x�̐���
				if (m_fAngleEnemyBullet >= -((D3DX_PI * 100) / 2))
				{
					m_bAngleEnemy = true;
				}

				if (m_fAngleEnemyBullet <= -D3DX_PI * 100)
				{
					m_bAngleEnemy = false;
				}

				//�p�x�ς���
				if (m_bAngleEnemy == false)
				{
					m_fAngleEnemyBullet += 35.0f;
				}

				if (m_bAngleEnemy == true)
				{
					m_fAngleEnemyBullet -= 35.0f;
				}

				//�J�E���g���Z�b�g
				m_nAttackCnt2 = 0;
			}
		}

		if (m_nType == ENEMY_07)
		{
			//�J�E���g��i�߂�
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 20)
			{
				//�e�̔���
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -10.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet, OBJTYPE_BULET_ENEMY);
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -10.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet - 30.0f, OBJTYPE_BULET_ENEMY);

				//�p�x�̐���
				if (m_fAngleEnemyBullet >= D3DX_PI * 100)
				{
					m_bAngleEnemy = true;
				}

				if (m_fAngleEnemyBullet <= (D3DX_PI * 100) / 2)
				{
					m_bAngleEnemy = false;
				}

				//�p�x�ς���
				if (m_bAngleEnemy == false)
				{
					m_fAngleEnemyBullet += 35.0f;
				}

				if (m_bAngleEnemy == true)
				{
					m_fAngleEnemyBullet -= 35.0f;
				}

				//�J�E���g���Z�b�g
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 8 || g_nNextAttack2 == 8)
	{
		if (m_nType == ENEMY_06 || m_nType == ENEMY_07)
		{
			//�J�E���g��i�߂�
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 40)
			{
				//�e�̔���
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -7.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet, OBJTYPE_BULET_ENEMY);
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -7.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet + 30.0f, OBJTYPE_BULET_ENEMY);
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -7.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet - 30.0f, OBJTYPE_BULET_ENEMY);
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -7.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet + 60.0f, OBJTYPE_BULET_ENEMY);
				CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -7.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet - 60.0f, OBJTYPE_BULET_ENEMY);

				//�p�x�̐���
				if (m_fAngleEnemyBullet >= (D3DX_PI * 100 + 50))
				{
					m_bAngleEnemy = true;
				}

				if (m_fAngleEnemyBullet <= (D3DX_PI * 100 - 50))
				{
					m_bAngleEnemy = false;
				}

				//�p�x�ς���
				if (m_bAngleEnemy == false)
				{
					m_fAngleEnemyBullet += 35.0f;
				}

				if (m_bAngleEnemy == true)
				{
					m_fAngleEnemyBullet -= 35.0f;
				}

				//�J�E���g���Z�b�g
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 9 || g_nNextAttack2 == 9)
	{
		if (m_nType == ENEMY_05)
		{
			//�J�E���g��i�߂�
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 40)
			{
				for (int nEnemyBullet = 0; nEnemyBullet <= 20; nEnemyBullet++)
				{
					int nCntBulletRand = rand() % 300;

					float BulletRand = (D3DX_PI * 100) + (nCntBulletRand - 150);

					//�e�̔���
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -5.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, (float)BULLET_SIZE, (float)BULLET_SIZE, BulletRand, OBJTYPE_BULET_ENEMY);
				}

				//�J�E���g���Z�b�g
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 10 || g_nNextAttack2 == 10)
	{
		if (m_nType == ENEMY_04)
		{
			//�J�E���g��i�߂�
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 120)
			{
				if (m_bAngleEnemy == false)
				{
					//�e�̔���
					CBullet::Create(pos, D3DXVECTOR3(5.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
					CBullet::Create(pos, D3DXVECTOR3(-5.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
					CBullet::Create(pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
					CBullet::Create(pos, D3DXVECTOR3(0.0f, -5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);

					m_bAngleEnemy = true;
				}

				else if (m_bAngleEnemy == true)
				{
					//�e�̔���
					CBullet::Create(pos, D3DXVECTOR3(5.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
					CBullet::Create(pos, D3DXVECTOR3(-5.0f, -5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
					CBullet::Create(pos, D3DXVECTOR3(5.0f, -5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);
					CBullet::Create(pos, D3DXVECTOR3(-5.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 0, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_ENEMY);

					m_bAngleEnemy = false;
				}

				//�J�E���g���Z�b�g
				m_nAttackCnt2 = 0;
			}
		}
	}

	if (g_nNextAttack == 11 || g_nNextAttack2 == 11)
	{
		if (m_nType == ENEMY_05)
		{
			//�J�E���g��i�߂�
			m_nAttackCnt++;

			if (m_nAttackCnt >= 40)
			{
				for (int nEnemyBullet = 0; nEnemyBullet <= 3; nEnemyBullet++)
				{
					int nCntBulletRand = rand() % 300;

					float BulletRand = (D3DX_PI * 100) + (nCntBulletRand - 150);

					//�e�̔���
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), -2.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, BulletRand, OBJTYPE_BULET_ENEMY);
				}

				//�J�E���g���Z�b�g
				m_nAttackCnt = 0;
			}
		}
	}

	//�{�X
	if (g_nNextAttack == 12 || g_nNextAttack2 == 12 || g_nNextAttack3 == 12)
	{
		if (m_nType == ENEMY_BOSS_01)
		{
			//�J�E���g��i�߂�
			m_nAttackCnt++;

			if (m_nAttackCnt >= 10)
			{
				for (int nEnemyBullet = 0; nEnemyBullet < 1; nEnemyBullet++)
				{
					//�e�̔���
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), 3.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet, OBJTYPE_BULET_ENEMY);
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), 3.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet - 10.0f, OBJTYPE_BULET_ENEMY);
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), 3.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet - 20.0f, OBJTYPE_BULET_ENEMY);
				}

				//�J�E���g���Z�b�g
				m_nAttackCnt = 0;
			}
			m_fAngleEnemyBullet += 5.0f;
		}
	}

	if (g_nNextAttack == 13 || g_nNextAttack2 == 13 || g_nNextAttack3 == 13)
	{
		if (m_nType == ENEMY_BOSS_01)
		{
			//�J�E���g��i�߂�
			m_nAttackCnt2++;

			if (m_nAttackCnt2 >= 10)
			{
				for (int nEnemyBullet = 0; nEnemyBullet < 1; nEnemyBullet++)
				{
					//�e�̔���
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), 4.0f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet2, OBJTYPE_BULET_ENEMY);
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), 4.0f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, m_fAngleEnemyBullet2 - 10.0f, OBJTYPE_BULET_ENEMY);
				}

				//�J�E���g���Z�b�g
				m_nAttackCnt2 = 0;
			}
			m_fAngleEnemyBullet2 += 5.0f;
		}
	}

	if (g_nNextAttack == 14 || g_nNextAttack2 == 14 || g_nNextAttack3 == 14)
	{
		if (m_nType == ENEMY_BOSS_01)
		{
			//�J�E���g��i�߂�
			m_nAttackCnt3++;

			if (m_nAttackCnt3 >= 30)
			{
				for (int nEnemyBullet = 0; nEnemyBullet < 1; nEnemyBullet++)
				{
					//�e�̔���
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x, pos.y, 1.0f), 2.0f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE + 20, BULLET_SIZE + 20, m_fAngleEnemyBullet3, OBJTYPE_BULET_ENEMY);
				}

				//�J�E���g���Z�b�g
				m_nAttackCnt3 = 0;
			}
			m_fAngleEnemyBullet3 += 3.0f;
		}
	}
}

//=============================================================================
// �ړ�����
//=============================================================================
void CEnemy::Move(void)
{
	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	//�ړ�����
	//��`
	D3DXVECTOR3 move;
	D3DXVECTOR3 vec;
	float fAngle = 0;

	//MoveType�̃N���A
	if (g_bMoveType == true)
	{
		m_nMoveType = NULL;
		g_bMoveType = false;
		g_bUseEnemyMove = false;
		g_bUseEnemyMove2 = false;
	}

	//���̖ړI�n���ݒ肳�ꂽ��
	if (g_bUseEnemyMove == true)
	{
		//�����ړI�n������ꍇ
		if (m_nType == g_nType2)
		{
			//�ړI�n�̕ύX
			m_objective = g_objective2;

			//�ړ��ʕύX
			m_fMove = g_fMove2;

			g_nType2 = NULL;
			g_bUseEnemyMove = false;
		}

		//�����ړI�n������ꍇ
		//�������O�ɍU��������
		if (m_nType == g_nType3)
		{
			//�ړI�n�̕ύX
			m_objective = g_objective3;

			//�ړ��ʕύX
			m_fMove = g_fMove3;

			g_nType3 = NULL;
			g_bUseEnemyMove = false;

			//�U������
			CPolygon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 1000, 0, 0, 1.0f, 0.0f, 0.0f, 1000, 0, OBJTYPE_BULET_ENEMY, 4);
			CPolygon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 1000, 0, 0, 1.0f, 1.0f, 0.0f, 1000, 0, OBJTYPE_BULET_ENEMY, 4);
		}
	}

	//NestMove�Őݒ肵���^�C�v�̈ړ����ύX������
	if (g_bUseEnemyMove2 == true)
	{
		//�����ړI�n������ꍇ
		if (m_nMoveType == g_nType4)
		{
			//�ړI�n�̕ύX
			m_objective = g_objective4;

			//�ړ��ʕύX
			m_fMove = g_fMove4;
			g_bUseEnemyMove2 = false;
		}
	}

	//���Ԍo�߂ŉ��Ɉړ�������
	if (m_nTime == CGame::GetTime())
	{
		m_objective.y = 3000.0f;
	}

	if (m_nTime == CGame2::GetTime())
	{
		m_objective.y = 3000.0f;
	}

	//�ړI�n���v�Z
	vec = m_objective - pos;

	//�ʒu�X�V
	fAngle = atan2f(vec.x, vec.y);			//�x�N�g���̊p�x�����߂�
	move.x = sinf(fAngle) * m_fMove;		//�ړ��ʂ����߂�
	move.y = cosf(fAngle) * m_fMove;		//�ړ��ʂ����߂�

											//�ړI�n�ɋ߂Â�����X�V���~�߂�
	if (pos.y >= m_objective.y - 10.0f && pos.y <= m_objective.y + 10.0f && pos.x >= m_objective.x - 10.0f && pos.x <= m_objective.x + 10.0f)
	{
		//�������Ȃ�
	}

	else
	{
		//�ʒu�X�V��������
		pos += move;
	}

	//�ړ�������
	SetPosition(pos);
}

//=============================================================================
// ��������
//=============================================================================
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, float fmove, D3DXVECTOR3 objective, int nLife, float fSizeX, float fSizeY, int nTime, int nType, int nMoveType)
{
	// ���[�J���ϐ��錾
	CEnemy *pEnemy = NULL;
	pEnemy = new CEnemy;

	//������
	g_bMoveType = false;

	// NULL�`�F�b�N
	if (pEnemy != NULL)
	{
		//�G�̐���
		pEnemy->Init(pos, fmove, objective, nLife, fSizeX, fSizeY, nTime, nType, nMoveType);
	}

	// �e�N�X�`�����蓖��
	pEnemy->BindTexture(m_pTexture[nType]);

	return pEnemy;
}

//=============================================================================
// ���̈ړ��ꏊ
//=============================================================================
void CEnemy::NextObjective(D3DXVECTOR3 pos, float fMove, int nType)
{
	g_objective2 = pos;
	g_nType2 = nType;
	g_fMove2 = fMove;

	g_bUseEnemyMove = true;
}

//=============================================================================
// ���̈ړ��ꏊ������ɍU��������
//=============================================================================
void CEnemy::NextObjectiveAttack(D3DXVECTOR3 pos, float fMove, int nType)
{
	g_objective3 = pos;
	g_nType3 = nType;
	g_fMove3 = fMove;

	g_bUseEnemyMove = true;
}

//=============================================================================
// �U���p�^�[����ς���
//=============================================================================
void CEnemy::NextAttack(int NextAttack)
{
	g_nNextAttack = NextAttack;
}

//=============================================================================
// �U���p�^�[����ς���
//=============================================================================
void CEnemy::NextAttack2(int NextAttack)
{
	g_nNextAttack2 = NextAttack;
}

//=============================================================================
// �U���p�^�[����ς���
//=============================================================================
void CEnemy::NextAttack3(int NextAttack)
{
	g_nNextAttack3 = NextAttack;
}

//=============================================================================
// �ړ��ړI�n���^�C�v�œr���ύX
//=============================================================================
void CEnemy::NextMove(D3DXVECTOR3 pos, float fMove, int nType)
{
	g_nType4 = nType;
	g_objective4 = pos;
	g_fMove4 = fMove;

	//�r���ύX��������
	g_bUseEnemyMove2 = true;
}

//=============================================================================
// m_fMove�̒l��NULL�ɂ���
//=============================================================================
void CEnemy::MoveClear(void)
{
	g_bMoveType = true;
}

//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CEnemy::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
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
// �e�N�X�`���̔j��
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