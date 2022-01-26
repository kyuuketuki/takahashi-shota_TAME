//=============================================================================
//
// ���j���[���� [menu.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _MENU_H_
#define _MENU_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// �^�C�g���N���X�̒�`
//=============================================================================
//�p����`��N���X
class CMenu : public CScene
{
public:
	CMenu();								//�R���X�g���N�^
	~CMenu();								//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);			//����������
	void Uninit(void);													//�I������
	void Update(void);													//�X�V����
	void Draw(void);													//�`�揈��

	static CMenu *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//��������

private:
	int m_nFadeCnt;				// �t�F�[�h�J�E���g
	int  m_mode;				// ��ʑJ�ڗp
	int  m_nPointerMenuX;		// �^�C�v
	bool m_bButtonDownMenu;		// �A�ő΍��p
};
#endif
