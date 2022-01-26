//=============================================================================
//
// �@�̑I����ʏ��� [select.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _SELECT_H_
#define _SELECT_H_

//==============================================================================
//�C���N���[�h
//==============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// �@�̑I�����
//=============================================================================
class CSelect : public CScene
{
public:
	CSelect();								//�R���X�g���N�^
	~CSelect();								//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	static CSelect *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//��������
};
#endif
