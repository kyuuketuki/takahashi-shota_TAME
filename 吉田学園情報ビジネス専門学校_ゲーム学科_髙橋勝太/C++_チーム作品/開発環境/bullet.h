//=============================================================================
//
// �e���� [bullet.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
#include "scene2D.h"

//�}�N����`
#define BULLET_SIZE (10)		//�e�̑傫��

//=============================================================================
// �e�N���X�̒�`
//=============================================================================
class CBullet : public CScene2D
{
public:
	CBullet(int nPriority = 4);				//�R���X�g���N�^
	virtual ~CBullet();						//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSpeed, int nLife, int nDamage, float fSizeX, float fSizeY, int nType, int nTypeMove);					 //����������
	void Uninit(void);																																							 //�I������
	void Update(void);																																							 //�X�V����
	void Draw(void);																																							 //�`�揈��

	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, int nDamage, float fSizeX, float fSizeY, int nType);									 //��������
	static CBullet *Create2(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSpeed, int nLife, int nDamage, float fSizeX, float fSizeY, int nType, int nTypeMove);		//��������
	static HRESULT Load(void);
	static void Unload(void);

	//Create�֐�
	static void InitAngleBullet(D3DXVECTOR3 pos, float fSpeed, D3DXCOLOR col, int nLife, int nDamage, float fSizeX, float fSizeY, float fAngle, int nType);
	static void InitAngleBullet2(D3DXVECTOR3 pos, float fSpeed, D3DXCOLOR col, int nLife, int nDamage, float fSizeX, float fSizeY, float fAngle, int nType, int nTypeMove);
	static void InitAimingBullet(D3DXVECTOR3 posBullet, D3DXCOLOR col, float speed, D3DXVECTOR3 posPlayer);
	static void InitAimingBullet2(D3DXVECTOR3 posBullet, D3DXCOLOR col, float speed, D3DXVECTOR3 posPlayer, int nTypeMove);
	static void InitDestinations(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSpeed, int nLife, int nDamage, float fSizeX, float fSizeY, int nType);
	static void InitDestinations2(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSpeed, int nLife, int nDamage, float fSizeX, float fSizeY, int nType, int nTypeMove);

	//����ړ��@�X�V
	static void NextMove(int nTypeMove,int nAction, D3DXVECTOR3 fSpeed);
	static void NextAimingBullet(int nTypeMove, int nAction, D3DXVECTOR3 posPlayer, float fSpeed);

	static void NextMove2(int nTypeMove, int nAction, D3DXVECTOR3 fSpeed);
	static void NextAimingBullet2(int nTypeMove, int nAction, D3DXVECTOR3 posPlayer, float fSpeed);

private:
	D3DXVECTOR3 m_move;							//�ړ���
	D3DXVECTOR3 m_oldpos;						//�ړ���
	D3DXVECTOR3 m_createpos;					//�ړ���
	D3DXCOLOR	m_col;							//�F���
	int			m_nLife;						//���C�t
	int			m_nDamage;						//�_���[�W��
	int			m_nType;						//�^�C�v�@�G�̒e���@���@�̒e��
	int			m_TypeMove;						//�^�C�v�@����ړ�
	float		m_fRot;							//��]��
	float		m_fSizeX;						//�T�C�Y
	float		m_fSizeY;						//�T�C�Y
	float		m_fSpeedDestinations;			//�ړI�n�v�Z���Ɏg���X�s�[�h
	bool		m_bUseAming = false;			//�_�������p
	bool		m_bCreatepos = false;			//�����e�p
	bool		m_bChangeSpeed = false;			//�X�s�[�h�ύX

	static LPDIRECT3DTEXTURE9	m_pTexture;							//�e�N�X�`���̃|�C���^
	static D3DXVECTOR3			m_nextpos;							//�ړ���
	static D3DXVECTOR3			m_nextmove;							//�ړ���
	static int					m_TypeAction;						//���ꏈ��
};

#endif