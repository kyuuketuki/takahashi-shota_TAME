//=============================================================================
//
// �I�u�W�F�N�g3D���� [object3D.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//==============================================================================
//�C���N���[�h
//==============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �I�u�W�F�N�g3D�N���X�̒�`
//=============================================================================
class CObject3D : public CScene
{
public:
	CObject3D(int nPriority = 5, int nType = 0);								//�R���X�g���N�^
	virtual ~CObject3D();														//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	static CObject3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, int nType);			//��������

	D3DXMATRIX GetMtxWorld(void);

private:
	LPD3DXMESH		m_pMesh;			//���b�V��
	LPD3DXBUFFER	m_pBuffMat;			//�o�b�t�@
	DWORD			m_nNumMat;			//�}�g���b�N�X
	D3DXMATRIX		m_mtxWorld;			//���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;				//�ʒu
	D3DXVECTOR3		m_move;				//�ړ���
	D3DXVECTOR3		m_rot;				//��]��
	int				m_nNumVtx;			//���_��
	float			m_fRot;				//��]��Y

	//�ÓI�����o�ϐ�
	static int			m_nType;			//�^�C�v
	static char			*m_pTextName;		//�e�N�X�`�����ւ̃|�C���^
	static D3DXVECTOR3	m_rotStatic;		//��]��
	static D3DXVECTOR3	m_moveStatic;		//��]��
};

#endif