//=============================================================================
//
// �V�[��X���� [sceneX.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _SCENEX_H_
#define _SCENEX_H_
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �|���S���N���X�̒�`
//=============================================================================
class CSceneX : public CScene
{
public:
	CSceneX(int nPriority = 0, int nType = 0);						//�R���X�g���N�^
	virtual ~CSceneX();					//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//����������
	void Uninit(void);												//�I������
	void Update(void);												//�X�V����
	void Draw(void);												//�`�揈��

	void BindTextName(char *TextName);								//�e�N�X�g�ւ̃|�C���^

	void SetPosition(D3DXVECTOR3 pos);
	void SetRevolution(D3DXVECTOR3 rot);

	static CSceneX *Create(D3DXVECTOR3 pos, char *TextName);		//��������

	//Get�֐�
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRevolution(void) { return m_rot; }
																	
private:															
	LPD3DXMESH		m_pMesh;			//���b�V��
	LPD3DXBUFFER	m_pBuffMat;			//�o�b�t�@
	DWORD			m_nNumMat;			//�}�g���b�N�X
	D3DXMATRIX		m_mtxWorld;			//���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;				//�ʒu
	D3DXVECTOR3		m_rot;				//����
	int				m_nNumVtx;			//���_��
	float			m_fRot;				//��]��

	//�ÓI�����o�ϐ�
	static char		*m_pTextName;		//X�t�@�C���ǂݍ���
};

#endif