//=============================================================================
//
// �G���� [enemy.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "player.h"
#include "scene2D.h"

//�}�N����`
#define ENEMY_SIZE (70)

//=============================================================================
// �G�N���X�̒�`
//=============================================================================
class CEnemy : public CScene2D
{
public:
	CEnemy(int nPriority = 1);				//�R���X�g���N�^
	virtual ~CEnemy();						//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fmove, D3DXVECTOR3 objective, int nLife, float fSizeX, float fSizeY, int nTime, int nType, int nMoveType);	   //����������
	void Uninit(void);																																   //�I������
	void Update(void);																																   //�X�V����
	void Draw(void);																																   //�`�揈��

	void Collision(void);		//�����蔻��	

	void Attack(void);
	void Move(void);
																																					   
	//��������																																				   																															   //�e�N�X�`���j��
	static CEnemy *Create(D3DXVECTOR3 pos, float fmove, D3DXVECTOR3 objective, int nLife, float fSizeX, float fSizeY, int nTime, int nType, int nMoveType);

	//����X�V
	static void NextObjective(D3DXVECTOR3 pos, float fMove, int nType);
	static void NextObjectiveAttack(D3DXVECTOR3 pos, float fMove, int nType);

	static void NextAttack(int NextAttack);
	static void NextAttack2(int NextAttack);
	static void NextAttack3(int NextAttack);

	static void NextMove(D3DXVECTOR3 pos, float fMove, int nType);
	static void MoveClear(void);

	//�e�N�X�`���֘A
	static HRESULT Load(void);
	static void Unload(void);

private:
	//�ÓI�����o�ϐ�
	static CPlayer				*m_pPlayer;				//�v���C���[�ւ̃|�C���^	�z�[�~���O���Ɏg�p
	static LPDIRECT3DTEXTURE9	m_pTexture[ENEMY_MAX];	//�e�N�X�`���ւ̃|�C���^
														
	//�����o�ϐ�
	D3DXVECTOR3					m_pos;					//�ʒu
	D3DXVECTOR3					m_objective;			//�S�[���n�_
	D3DXCOLOR					m_col;					//�F���
	int							m_nLife;				//����
	int							m_nAttack;				//�U����i
	int							m_nTime;				//�s�����Ǘ����鎞��
	int							m_nType;				//�G�̎�ނ��^�C�v�ŊǗ�
	int							m_nMoveType;			//�����𓝈ꂳ����^�C�v
	float						m_fMove;				//�ړ��X�s�[�h
	float						m_fSizeX;				//�T�C�Y
	float						m_fSizeY;				//�T�C�Y
	float						m_fMoveEnemy;			//�G�̈ړ����x

	int							m_nAttackCnt;			//�U������^�C�~���O
	int							m_nAttackCnt2;			//�U������^�C�~���O
	int							m_nAttackCnt3;			//�U������^�C�~���O

	//�p�x�v�Z�p
	float						m_fAngleEnemyBullet = 0.0f;
	float						m_fAngleEnemyBullet2 = 0.0f;
	float						m_fAngleEnemyBullet3 = 0.0f;

	bool						m_bAttackPattern;		//�U���p�^�[���p
	bool						m_bMovEnemy;			//�ړ����邩����
	bool						m_bAngleEnemy = false;	//�e�̔��˗p

	//�G�̖��G����
	int							m_nHitTime;
	bool						m_bInvincibility = false;
};

#endif