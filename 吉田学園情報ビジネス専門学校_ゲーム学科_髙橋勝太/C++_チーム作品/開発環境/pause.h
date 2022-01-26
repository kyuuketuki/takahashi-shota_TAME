//=============================================================================
//
// �|�[�Y���� [pause.h]
// Author : taiki hayasaka
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

// �C���N���[�h�ǂݍ���
#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// �|�[�Y�����N���X�̒�`
//=============================================================================
class CPause : public CScene
{
public:
	CPause(int nPriority = 0, int nType = 2);							// �R���X�g���N�^								
	~CPause();															// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);			// ����������
	void Uninit(void);													// �I������
	void Update(void);													// �X�V����
	void Draw(void);													// �`�揈��
																		   
	static CPause *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	// ��������

private:
	int  m_nFadeCntPause;		// �t�F�[�h�J�E���g
	int  m_mode;				// ��ʑJ�ڗp
};

#endif