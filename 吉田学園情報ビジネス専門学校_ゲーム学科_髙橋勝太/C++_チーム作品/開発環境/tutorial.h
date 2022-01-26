//=============================================================================
//
// �`���[�g���A����ʏ��� [tutorial.h]
// Author : taiki hayasaka
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "model.h"

//=============================================================================
// �`���[�g���A���N���X�̒�`
//=============================================================================
class CTutorial : public CScene
{
public:
	CTutorial();								//�R���X�g���N�^
	~CTutorial();								//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	static CTutorial *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//��������

private:
	CModel *m_apModel;				//���f���ւ̃|�C���^
};
#endif

