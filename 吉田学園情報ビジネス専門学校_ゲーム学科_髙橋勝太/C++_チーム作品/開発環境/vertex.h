//=============================================================================
//
// ���_�����蔻�菈�� [vertex.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _VERTEX_H_
#define _VERTEX_H_
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"
#include "player3D.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_OBJECT_VEC (1024)			//�����蔻��̍ő吔

//=============================================================================
// �|���S���N���X�̒�`
//=============================================================================

//�p����`��N���X
class CVertex : public CScene
{
public:
	CVertex(int nPriority = 0, int nType = 0);								//�R���X�g���N�^
	virtual ~CVertex();														//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	//��������
	static CVertex *Create(D3DXVECTOR3 pos, D3DXVECTOR3 fMaxSizeX, D3DXVECTOR3 fMinSizeX, D3DXVECTOR3 fMaxSizeY, D3DXVECTOR3 fMinSizeY);
	
	//��������
	static void Release(void);

	//�����蔻��
	bool CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);

	void CVertex::LoadObject(void);			//�e�L�X�g�̓ǂݍ���

	//�\���@�󂯓n���p
	static int GetNumber(void);
	static int GetNumber2(void);
	static float GetAtan2(void);

private:
	LPD3DXMESH		m_pMesh;							//���b�V��
	LPD3DXBUFFER	m_pBuffMat;							//�o�b�t�@
	DWORD			m_nNumMat;							//�}�g���b�N�X
	
	D3DXVECTOR3		m_pos;								//�ʒu
	D3DXVECTOR3		m_CollisionVecPos[4];				//�����蔻��x�N�g��
	D3DXVECTOR3		m_move;								//�ʒu
	D3DXVECTOR3		m_rot;								//�ʒu
	D3DXVECTOR3     m_vtx;								//���_��
	D3DXVECTOR3		m_MaxStone;							//�ő�l
	D3DXVECTOR3		m_MinStone;							//�ŏ��l
	D3DXVECTOR3		m_aPos[MAX_OBJECT_VEC][4];			//�ʒu���v�Z��̕ۑ��ꏊ
	D3DXVECTOR3		m_Vec[MAX_OBJECT_VEC][4];			//�x�N�g�����v�Z��̕ۑ��ꏊ
	int				m_nNumVtx;							//���_��
	float			m_fRot;								//��]��
	float			m_fLength;							//�Ίp���̒���
	float			m_fAngle;							//�Ίp���̊p�x
	float			m_fLength2;							//�Ίp���̒���

	//�ÓI�����o�ϐ�
	static D3DXMATRIX		m_mtxWorld;					//���[���h�}�g���b�N�X
	static D3DXVECTOR3		m_rotStatic;				//��]��
	static D3DXVECTOR3		m_moveStatic;				//�ړ���
	static int				m_nType;					//�^�C�v
	static char				*m_pTextName;				//�e�L�X�g��
};

#endif