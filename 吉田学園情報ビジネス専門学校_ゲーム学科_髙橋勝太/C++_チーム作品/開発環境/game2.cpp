//=============================================================================
//
// �Q�[�����2���� [game2.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
// �C���N���[�h�ǂݍ���
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

//�ÓI�����o�ϐ��錾
CPlayer *CGame2::m_pPlayer = NULL;		//�v���C���[�|�C���^�[

//�O���[�o���ϐ��錾
int nTimeGame2 = 0;						//�^�C��
int nTimeCounter2 = 0;					//�^�C���J�E���^�[
int g_nAttackCnt = 0;					//�U���p�ϐ�
int g_nAttackCnt2 = 0;					//�U���p�ϐ�
int g_nAttackCnt3 = 0;					//�U���p�ϐ�
float g_fAngle = 0.0f;					//�U���p�ϐ��p�x
float g_fAngle2 = 0.0f;					//�U���p�ϐ��p�x
float g_fAngle3 = 0.0f;					//�U���p�ϐ��p�x
bool g_bAngle = false;					//�U���p�ϐ��p�x
bool g_bStage = false;					//�X�e�[�W�ύX
bool g_bCountReset = false;				//�X�e�[�W�ύX

//���S�t���O�����p
bool g_bDeathFlagGame2 = false;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CGame2::CGame2()
{
	//������
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
// �f�X�g���N�^
//==============================================================================
CGame2::~CGame2()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CGame2::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�T�E���h���擾
	CSound *pSound;
	pSound = CManager::GetSound();

	//�T�E���hBGM
	pSound->Play(CSound::SOUND_LABEL_BGM004);

	//�v���C���[�̃��x����������
	CPlayer::SetBulletLevel(0);

	// �t�H���g�I�u�W�F�N�g�̐���
	D3DXCreateFont(pDevice, 36, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pTimeFontGame);

	//���݂̃X�e�[�W��Manaager�ɋL��������
	CManager::SetGameStage(2);

	//�w�i��\��
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	//�A�j���[�V�����w�i��\��
	CAnimasionBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 10, 17);

	//�v���C���[�̐���
	CPlayer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), MAX_LIFE, PLAYER_SIZE, PLAYER_SIZE);

	//����U�����I�t�ɂ���
	CEnemy::NextAttack(0);

	//HP5�Œ�ɂ���
	CManager::SetMaxPlayerLife(5);

	//HP�o�[(�����T)
	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		CLife::Create(D3DXVECTOR3(40.0f + (80.0f * nCnt), 100.0f, 0.0f), 30.0f, 30.0f);
	}

	//�X�R�A
	CScore::Create(D3DXVECTOR3(1800.0f, 50.0f, 0.0f), 100.0f, 100.0f);

	//�N���A����
	CManager::SetEnemyDead(0);

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CGame2::Uninit(void)
{
	//�T�E���h���擾
	CSound *pSound;
	pSound = CManager::GetSound();

	//BGM���~�߂�
	pSound->Stop(CSound::SOUND_LABEL_BGM004);

	// �I�u�W�F�N�g�j��
	Release();
}

//==============================================================================
// �X�V����
//==============================================================================
void CGame2::Update(void)
{
	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//���Ԃ�i�߂�
	nTimeCounter2++;

	//�J�E���^�[��60�ȏ�ɂȂ�����
	if (nTimeCounter2 >= 60)
	{
		//�b����i�߂�
		nTimeGame2++;

		//�J�E���^�[��߂�
		nTimeCounter2 = 0;
	}

	//�G�̓|������
	int nDeatCnt = CManager::GetEnemyDead();
	
	if (g_bDeathFlagGame2 == true)
	{
		//�N���A����
		if (nDeatCnt == 3)
		{
			CManager::SetGameClear(true);

			//��ʑJ��
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));	// �V�[���J��
		}
	}

	//�X�V�����Ĉʒu�t���[���҂i�f�b�g�t���O�̂��߁j
	if (g_bDeathFlagGame2 == false)
	{
		//�N���A����
		CManager::SetEnemyDead(0);

		g_bDeathFlagGame2 = true;
	}

	//�{�X�n���h��̓|�����ꍇ
	if (nDeatCnt == 2)
	{
		//�񕜃A�C�e������
		if (g_bStage == false)
		{
			//�U������
			Attack2();
		}

		//�{�X��ŏI�@�U���ω�
		if (g_bStage == true)
		{
			//�U������
			Attack3();
		}
	}

	//���߂̍U��
	else
	{
		//�^�C���ɍ��킹�ēG�𐶐�
		if (nTimeGame2 == 1 && nTimeCounter2 == 0)
		{
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, -300.0f, 0.0f), 8.0f, D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 7000, ENEMY_SIZE + 300, ENEMY_SIZE + 200, 1000, ENEMY_BOSS_01, 0);
			CEnemy::Create(D3DXVECTOR3(0.0f, -300.0f, 0.0f), 8.0f, D3DXVECTOR3(SCREEN_WIDTH / 2 - 300, 400.0f, 0.0f), 1500, ENEMY_SIZE + 50, ENEMY_SIZE + 50, 1000, ENEMY_BOSS_02, 5);
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH, -300.0f, 0.0f), 8.0f, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, 400.0f, 0.0f), 1500, ENEMY_SIZE + 50, ENEMY_SIZE + 50, 1000, ENEMY_BOSS_03, 6);		
		}

		//�U������
		Attack();
	}
}

//==============================================================================
// �`�揈��
//==============================================================================
void CGame2::Draw(void)
{
#ifdef _DEBUG
	// ���[�J���ϐ��錾
	RECT rect = { -1500,30,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	// ��ʂɕ\��������
	//sprintf(&aStr[0], "GAME2��ʂ̌o�ߎ��� : %d\n", nTimeGame2);

	//��ʂɕ\��������
	sprintf(&aStr[0], "GAME��ʂ̌o�ߎ��� : %d\n", CManager::GetEnemyDead());

	// �e�L�X�g�̕`��
	m_pTimeFontGame->DrawText(NULL, &aStr[0], -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
#endif
}

//==============================================================================
// �U�����s������
//==============================================================================
//�{�X�n���h�U��
void CGame2::Attack(void)
{
	D3DXVECTOR3 posPlayer = m_pPlayer->GetPlayer();

	//boss�U��
	if (nTimeGame2 >= 4 && nTimeGame2 <= 65 && nTimeCounter2 == 0)
	{
		CBullet::InitAimingBullet(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 8.0f, D3DXVECTOR3(posPlayer.x, posPlayer.y, 0.0f));
	}

	//�ړ� && �U��
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

	//����U���ɕύX
	if (nTimeGame2 == 16 && nTimeCounter2 == 0)
	{
		CEnemy::NextAttack(1);
	}

	//�U�����[�U�[
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

	//��{�U���ɕύX
	if (nTimeGame2 == 27 && nTimeCounter2 == 0)
	{
		CEnemy::NextAttack(0);
	}

	//�����ʒu�Ɉړ�
	if (nTimeGame2 == 27 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjective(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300, 400.0f, 0.0f), 5.0f, ENEMY_BOSS_02);
	}

	if (nTimeGame2 == 27 && nTimeCounter2 == 1)
	{
		CEnemy::NextObjective(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, 400.0f, 0.0f), 5.0f, ENEMY_BOSS_03);
	}

	//boss�U��2
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

	//�ړ� && �U��
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

	//����U���ɕύX
	if (nTimeGame2 == 46 && nTimeCounter2 == 0)
	{
		CEnemy::NextAttack(1);
	}

	//�U�����[�U�[
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

	//��{�U���ɕύX
	if (nTimeGame2 == 53 && nTimeCounter2 == 0)
	{
		CEnemy::NextAttack(0);
	}

	//�����ʒu�Ɉړ�
	if (nTimeGame2 == 53 && nTimeCounter2 == 0)
	{
		CEnemy::NextObjective(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300, 400.0f, 0.0f), 5.0f, ENEMY_BOSS_02);
	}

	if (nTimeGame2 == 53 && nTimeCounter2 == 1)
	{
		CEnemy::NextObjective(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, 400.0f, 0.0f), 5.0f, ENEMY_BOSS_03);
	}

	//���[�v
	if (nTimeGame2 == 54 && nTimeCounter2 == 0)
	{
		nTimeGame2 = 28;
		nTimeCounter2 = 0;
	}
}

//�񕜃A�C�e���o��
void CGame2::Attack2(void)
{
	if (g_bCountReset == false)
	{
		g_bCountReset = true;

		//�J�E���g���[���ɂ���
		nTimeGame2 = 0;
		nTimeCounter2 = 0;
	}

	if (nTimeGame2 == 1 && nTimeCounter2 == 0)
	{
		//�A�C�e������
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

//�ŏI�U��
void CGame2::Attack3(void)
{
	if(g_bCountReset == false)
	{ 
		g_bCountReset = true;

		//�J�E���g���[���ɂ���
		nTimeGame2 = 0;
		nTimeCounter2 = 0;
	}

	//�v���C���[�̈ʒu���擾
	D3DXVECTOR3 posPlayer = m_pPlayer->GetPlayer();

	//�U��
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
		//�~�߂�
		CBullet::NextMove(1, 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		CBullet::NextMove2(2, 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (nTimeGame2 == 6 && nTimeCounter2 == 0)
	{
		//�ړI�n�Ɍ������Ĕ���
		CBullet::NextAimingBullet(1, 2, posPlayer, 10.0f);
	}

	if (nTimeGame2 == 6 && nTimeCounter2 == 30)
	{
		//�ړI�n�Ɍ������Ĕ���
		CBullet::NextAimingBullet2(2, 2, posPlayer, 15.0f);
	}

	//�������Ȃ���ړ�
	if (nTimeGame2 == 7 && nTimeCounter2 == 0)
	{
		//�������Ȃ��甭��
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
		//�ړ�
		CBullet::NextMove(3, 1, D3DXVECTOR3(3.0f, 0.0f, 0.0f));
		CBullet::NextMove2(4, 1, D3DXVECTOR3(-3.0f, 0.0f, 0.0f));		
	}

	//��������
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
		//�ړ�
		CBullet::NextMove(5, 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (nTimeGame2 == 23 && nTimeCounter2 == 0)
	{
		//�ړI�n�Ɍ������Ĕ���
		CBullet::NextAimingBullet(5, 2, D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), 5.0f);
	}

	if (nTimeGame2 == 26 && nTimeCounter2 == 0)
	{
		//�ړ�
		CBullet::NextMove(5, 1, D3DXVECTOR3(0.0f, 5.0f, 0.0f));
	}

	//�\��
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
		//�ړ�
		CBullet::NextMove(6, 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (nTimeGame2 >= 29 && nTimeCounter2 == 20 || nTimeGame2 >= 29 && nTimeCounter2 == 60)
	{
		//�ړ�
		CBullet::NextMove(99, 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	//��]����
	if (g_fAngle >= 628)
	{
		g_fAngle = g_fAngle - 628;
	}

	if (g_fAngle <= -628)
	{
		g_fAngle = g_fAngle + 628;
	}

	//��������
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

	//��������
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

	//�J�E���g�����Z�b�g
	if (nTimeGame2 == 70 && nTimeCounter2 == 0)
	{
		nTimeGame2 = 0;
		nTimeCounter2 = 0;
	}
}

//==============================================================================
// Get�֐��@����
//==============================================================================
int CGame2::GetTime(void)
{
	return nTimeGame2;
}

//==============================================================================
// ��������
//==============================================================================
CGame2 * CGame2::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �|�C���^�ϐ��̐���
	CGame2 *pGame2;
	pGame2 = new CGame2;

	// NULL�`�F�b�N
	if (pGame2 != NULL)
	{
		pGame2->Init(pos, fSizeX, fSizeY);
	}

	return pGame2;
}
