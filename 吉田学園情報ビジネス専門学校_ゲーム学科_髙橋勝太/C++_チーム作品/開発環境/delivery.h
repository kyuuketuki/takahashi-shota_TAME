//==============================================================================
//
// �ו���͂��鏈������[deliver.h]
// Author; takahashi syota
//
//==============================================================================

#ifndef _DELIVERY_H_
#define _DELIVERY_H_
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//==============================================================================
//�C���N���[�h�ǂݍ���
//==============================================================================
#include "main.h"
#include "scene3D.h"

//==============================================================================
//�ו��N���X�̒�`
//==============================================================================
class CDelivery : public CScene3D
{
public:
	CDelivery();                                                            //�R���X�g���N�^
	~CDelivery();                                                           //�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);              //����������
	void Uninit(void);												        //�I������
	void Update(void);												        //�X�V����
	void Draw(void);												        //�`�揈��

	static CDelivery *Create(D3DXVECTOR3 pos);								//��������

private:
	D3DXVECTOR3 m_pos;						                                //�ʒu���
	D3DXVECTOR3 m_move;														//�ړ���
	D3DXVECTOR3 m_rot;						                                //��]���
	D3DXVECTOR3 m_rotDest;						                            //��]���
	D3DXCOLOR	m_col;						                                //�J���[�ݒ�
	int			m_nLife;													//����
	bool		m_bUseAction;												//�`�拖��

	//�ÓI�����o�ϐ�
	static LPDIRECT3DTEXTURE9	m_pTexture;									//�e�N�X�`���ւ̃|�C���^
	static int m_nNumber;													//�͂��ԍ�
};
#endif