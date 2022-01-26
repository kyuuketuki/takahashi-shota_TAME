//=============================================================================
//
// �o���b�g�|���S������ [polygon.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _POLYGON_H_
#define _POLYGON_H_

//==============================================================================
//�C���N���[�h
//==============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
// �}�`�e�N���X�̒�`
//=============================================================================
class CPolygon : public CScene2D
{
public:
	CPolygon();								//�R���X�g���N�^
	virtual ~CPolygon();						//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fSizeX, float fSizeY, float fmove, float fRot, float fRotMove, int nMaxSize, int MinSize, int nType, int nTypeNumber);				//����������
	void Uninit(void);																																																	//�I������
	void Update(void);																																																	//�X�V����
	void Draw(void);																																																	//�`�揈��

	//Create�֐�																																																		  //��������
	static CPolygon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fSizeX, float fSizeY, float fmove, float fRot, float fRotMove, int nMaxSize, int MinSize, int nType, int nTypeNumber);	//�e�N�X�`���ǂݍ���
	static void PlayerBullet(int nDamage);																																												//�e�N�X�`���j��
	static void UninitBullet(void);

private:
	static bool	m_bUsePolygon;				//�G�̎�ނ��^�C�v�ŊǗ�

	D3DXVECTOR3 m_move;							//�ړ���
	D3DXCOLOR   m_col;							//�J���[���
	int			m_nLife;						//���C�t
	int			m_nType;						//�^�C�v�@�e
	int			m_CirclePattern;				//�����]�p
	int			m_nMaxSize;						//�T�C�Y
	int			m_nMinSize;						//�T�C�Y
	float		m_fmove;						//�ړ���
	float		m_fRot;							//��]�p�x
	float		m_fSizeX;						//X�̑傫��
	float		m_fSizeY;						//Y�̑傫��
	float		m_fRotMove;						//��]��
	bool		m_bUse;							//�g�p���Ă��邩

	//�_���[�W��
	static int	m_nDamage;
	static int	m_nTypeNumber;					//�^�C�v
};

#endif