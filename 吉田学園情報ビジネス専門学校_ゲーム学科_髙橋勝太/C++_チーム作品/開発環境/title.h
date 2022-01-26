//=============================================================================
//
// �^�C�g����ʏ��� [title.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "model.h"

//=============================================================================
// �^�C�g���N���X�̒�`
//=============================================================================
//�p����`��N���X
class CTitle : public CScene
{
public:
	CTitle();								//�R���X�g���N�^
	~CTitle();								//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	static CTitle *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//��������

private:
	CModel			*m_apModel;				//���f���ւ̃|�C���^
};
#endif
