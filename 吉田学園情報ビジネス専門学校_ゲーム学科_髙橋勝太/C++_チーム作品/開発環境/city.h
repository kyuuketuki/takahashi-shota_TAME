//=============================================================================
//
// �}�b�v���� [city.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _CITY_H_
#define _CITY_H_
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �|���S���N���X�̒�`
//=============================================================================
class CCity
{
public:
	CCity(int nPriority = 0, int nType = 0);						//�R���X�g���N�^
	virtual ~CCity();												//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//����������
	void Uninit(void);												//�I������
	void Update(void);												//�X�V����
	void Draw(void);												//�`�揈��

	void SetPosition(D3DXVECTOR3 pos);
	void SetRevolution(D3DXVECTOR3 rot);

	static CCity *Create(D3DXVECTOR3 pos, char *TextName);		//��������

	//�e�N�X�`���֘A
	static HRESULT Load(void);
	static void Unload(void);

	void SetTexture(int nNumber, LPDIRECT3DTEXTURE9 pTexture);//�e�N�X�`���������Ă���֐�

	//Get�֐�
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRevolution(void) { return m_rot; }
	LPD3DXMESH GetMash(void) { return m_pMesh; }
	LPD3DXBUFFER GetBuff(void) { return m_pBuffMat; }

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[54];		//�e�N�X�`���ւ̃|�C���^

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