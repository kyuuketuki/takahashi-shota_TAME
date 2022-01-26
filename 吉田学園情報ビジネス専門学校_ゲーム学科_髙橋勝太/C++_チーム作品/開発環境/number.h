//=============================================================================
//
// �ԍ����� [number.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _NUMBER_H_
#define _NUMBER_H_

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"

//=============================================================================
// �ԍ��N���X�̒�`
//=============================================================================
class CNumber
{
public:
	CNumber();								//�R���X�g���N�^
	~CNumber();								//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);			//����������
	void Uninit(void);													//�I������
	void Update(void);													//�X�V����
	void Draw(void);													//�`�揈��
	void SetNumber(int nNumber);
	
	static CNumber *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//��������
																			
	static HRESULT Load(void);												//�e�N�X�`���ǂݍ���
	static void Unload(void);												//�e�N�X�`���j��

private:
	static LPDIRECT3DTEXTURE9	m_pTexture;				//�p�ӂ���e�N�X�`���̐�
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;				//�o�b�t�@
	int							m_nNumber;				//�L���p
	float						m_fLength;				//�T�C�Y
	float						m_fAngle;				//�Ίp���v�Z�p
};								

#endif