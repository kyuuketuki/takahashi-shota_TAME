//=============================================================================
//
// �e�L�X�g�����I�u�W�F�N�g���� [textobject.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _TEXT_H_
#define _TEXT_H_

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "debugproc.h"
#include "object3D.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXT_LENGTH			(128)	// �e�L�X�g�ǂݍ��݂̍ő吔
#define DATA_LENGTH			(64)	// �f�[�^�ǂݍ��݂̍ő吔

//=============================================================================
// �e�L�X�g�I�u�W�F�N�g3D�N���X�̒�`
//=============================================================================
class CTextobject : public CScene
{
public:
	CTextobject();									//�R���X�g���N�^
	~CTextobject();									//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//����������
	void Uninit(void);											//�I������
	void Update(void);											//�X�V����
	void Draw(void);											//�`�揈��

	//�A�h���X���L��
	static CObject3D *GetObject(int nCnt) { return m_pObject3D[nCnt]; }

	static int GetTextObject3D();							//�����J�E���g�`����
	static int GetMaxFileX();								//�����J�E���g�`����
	static char *GetTextName(int nNumber);					//�����J�E���g�`����

	//��������
	static CTextobject *Create();

	void CTextobject::LoadObject3D(void);			//�e�L�X�g�ǂݍ���

private:
	//�ÓI�����o�ϐ�
	static CObject3D	*m_pObject3D[MAX_OBJECT_3D];	//�I�u�W�F�N�g3D�̃A�h���X�|�C���^
};

#endif