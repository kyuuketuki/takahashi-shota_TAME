//=============================================================================
//
// ���f������ [model.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _MODEL_H_
#define _MODEL_H_

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// ���f���N���X�̒�`
//=============================================================================
class CModel
{
public:
	CModel(int nPriority = 5, int nType = 0);										//�R���X�g���N�^
	virtual ~CModel();																//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pTextName);				//����������
	void Uninit(void);																//�I������
	void Update(void);																//�X�V����
	void Draw(void);																//�`�揈��

	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pTextName);		//��������

	//���[�V�����X�V
	void MotionPos(D3DXVECTOR3 pos);
	void MotionRot(D3DXVECTOR3 rot);

	void SetTexture(LPDIRECT3DTEXTURE9 pTexture);					//�e�N�X�`���������Ă���֐�
	void SetParent(CModel *pModel);									//�e�̊֌W
	D3DXMATRIX GetMtxWorld(void);									//���[���h�}�g���b�N�X���󂯓n���悤
	LPD3DXMESH GetMash(void) { return m_pMesh; }					//���b�V���ւ̃|�C���^
	LPD3DXBUFFER GetBuff(void) { return m_pBuffMat; }				//�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTexture; }		//�e�N�X�`���ւ̃|�C���^

private:
	LPDIRECT3DTEXTURE9	m_pTexture;		//�e�N�X�`���̃|�C���^
	LPD3DXMESH		m_pMesh;			//���b�V��
	LPD3DXBUFFER	m_pBuffMat;			//�o�b�t�@
	DWORD			m_nNumMat;			//�}�g���b�N�X
	D3DXMATRIX		m_mtxWorld;			//���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;				//�ʒu
	D3DXVECTOR3		m_rot;				//����
	CModel			*m_pParent;			//���f���̐e
	int				m_nNumVtx;			//���_��
	float			m_fRot;				//��]��

};
#endif