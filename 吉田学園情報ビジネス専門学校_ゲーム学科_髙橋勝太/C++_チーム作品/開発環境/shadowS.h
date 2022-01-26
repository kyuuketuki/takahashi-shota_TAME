//=============================================================================
//
// �X�e���V���V���h�E���� [shodowS.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _SHADOWS_H_
#define _SHADOWS_H_

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "sceneX.h"

//=============================================================================
// �|���S���N���X�̒�`
//=============================================================================
class CShadowS : public CSceneX
{
public:
	CShadowS(int nPriority = 5, int nType = 0);						//�R���X�g���N�^
	virtual ~CShadowS();											//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//����������
	void Uninit(void);												//�I������
	void Update(void);												//�X�V����
	void Draw(void);												//�`�揈��

	static CShadowS *Create();										//��������

private:
	//�V���h�E�̃��f�������o�ϐ�
	LPD3DXMESH		m_pMesh;			//���b�V��
	LPD3DXBUFFER	m_pBuffMat;			//�o�b�t�@
	DWORD			m_nNumMat;			//�}�g���b�N�X
	D3DXMATRIX		m_mtxWorld;			//���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;				//�ʒu
	D3DXVECTOR3		m_rot;				//����
	int				m_nNumVtx;			//���_��
	float			m_fRot;				//��]��

	//�|���S��2D�̃����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff = NULL;				//���_���
	LPDIRECT3DTEXTURE9			m_pTexture = NULL;				//�v���C���[�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3					m_pospolygon;					//�|���S���̏����ʒu
	D3DXVECTOR3					m_rotpolygon;					//�|���S���̉�]�l
};

#endif