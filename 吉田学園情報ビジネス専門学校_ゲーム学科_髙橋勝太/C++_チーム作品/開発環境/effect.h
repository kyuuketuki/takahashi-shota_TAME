//==============================================================================
//
// �G�t�F�N�g3D����[effect3D.h]
// Author; takahashi syota
//
//==============================================================================

#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

//==============================================================================
// �}�N����`
//==============================================================================
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//==============================================================================
//�C���N���[�h�ǂݍ���
//==============================================================================
#include "main.h"
#include "Scene.h"

//==============================================================================
//�G�t�F�N�g3D�N���X�̒�`
//==============================================================================
class CEffect3D : public CScene
{
public:
	CEffect3D(); //�R���X�g���N�^
	~CEffect3D();//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHight);      //����������
	void Uninit(void);												//�I������
	void Update(void);												//�X�V����
	void Draw(void);												//�`�揈��

	static CEffect3D *Create(D3DXVECTOR3 pos, float fWidth, float fHight);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);							//�e�N�X�`���ւ̃|�C���^
	void SetPosition(D3DXVECTOR3 pos);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;  //���_���
	LPDIRECT3DTEXTURE9 m_pTexture;		 //�e�N�X�`�����
	D3DXMATRIX m_mtxWorld;               //���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;                   //�ʒu���
	D3DXVECTOR3 m_pos2;                  //�ʒu���
	D3DXVECTOR3 m_move;                  //�ړ����
	D3DXVECTOR3 m_Rot;                   //��]���
	D3DXCOLOR m_col;                     //�J���[�ݒ�
	int m_nLife;                         //�����̎���
	float m_fAngle;                      //�ړ��̊p�x�ۑ��p
	float m_fWidth;                      //�G�t�F�N�g�̉���
	float m_fHight;                      //�G�t�F�N�g�̏c��
	float m_alpha;
};
#endif

