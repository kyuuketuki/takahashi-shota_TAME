//=============================================================================
//
// �V�[��X�@�|�C���g�@���� [pointerX.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _POINTER_H_
#define _POINTER_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#include "main.h"
#include "scene.h"
#include "sceneX.h"

//=============================================================================
// �V�[��X�@�|�C���g�N���X�̒�`
//=============================================================================
class CPointer : public CSceneX
{
public:
	CPointer(int nPriority = 5, int nType = 0);							//�R���X�g���N�^
	virtual ~CPointer();												//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);			//����������
	void Uninit(void);													//�I������
	void Update(void);													//�X�V����
	void Draw(void);													//�`�揈��

	static CPointer *Create(D3DXVECTOR3 pos, char *TextName);			//��������

	static void SetPointNumber(int nNumber);		//��������

private:
	D3DXVECTOR3		m_pos;				//�ʒu
	D3DXVECTOR3		m_rot;				//����
	//static char		*m_pTextName;		//�e�L�X�g���O
};

#endif