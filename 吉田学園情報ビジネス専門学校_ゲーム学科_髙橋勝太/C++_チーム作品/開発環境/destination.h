//=============================================================================
//
// �V�[��X�@�ړI�n�@���� [destination.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _DESTINATION_H_
#define _DESTINATION_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#include "main.h"
#include "scene.h"
#include "sceneX.h"

//=============================================================================
// �ړI�n�N���X�̒�`
//=============================================================================
//�p����`��N���X
class CDestination : public CSceneX
{
public:
	CDestination(int nPriority = 5, int nType = 0);						//�R���X�g���N�^
	virtual ~CDestination();											//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);			//����������
	void Uninit(void);													//�I������
	void Update(void);													//�X�V����
	void Draw(void);													//�`�揈��

	D3DXVECTOR3 RandomPos(void);														//�����_���Ȉʒu��ݒ�

	static CDestination *Create(D3DXVECTOR3 pos, char *TextName, int RandNumber);		//��������
	static D3DXVECTOR3	GetPosition(int nCnt);											//�ʒu�擾�p
	static int	GetLimit(int nCnt);														//�������Ԃ��擾�p
	static int	GetRange(int nCnt);														//�������Ԃ��擾�p
	static bool GetMotion(void) {return m_bMotion; }                                    //���[�V�����ω��p�t���O
	static bool GetUse(void) { return m_bUseDestination; }                              //���[�V�����ω��p�t���O

private:
	D3DXVECTOR3		m_pos;					//�ʒu
	D3DXVECTOR3		m_rot;					//����
	static char		*m_pTextName;			//�e�L�X�g���O
	int				m_nNumber;
	int				m_nRandomNumber;
	int				m_nRandomNumber2;
	static bool		m_bUseDestination;
	int				m_TimeLimit;
	int				m_MaxTimeLimit;
	int				m_nTime;
	int				m_nTimeCounter;
	static bool		m_bMotion;				//���[�V�����ω��p�t���O
	D3DXVECTOR3		m_posdes;
};

#endif