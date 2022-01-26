//=============================================================================
//
// �v���C���[3D���� [player3D.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _MAP_H_
#define _MAP_H_

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"
#include "model.h"
#include "city.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MAX_MAP_TEXTURE			(54)	// �ǂݍ��ރe�N�X�`���̍ő吔
#define MAX_MAP_OPBJECT			(29)	// �ǂݍ��ރI�u�W�F�N�g�̍ő吔

//=============================================================================
// �|���S���N���X�̒�`
//=============================================================================
class CMap : public CScene
{
public:
	CMap(int nPriority = 5, int nType = 0);										//�R���X�g���N�^
	virtual ~CMap();															//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);					//����������
	void Uninit(void);															//�I������
	void Update(void);															//�X�V����
	void Draw(void);															//�`�揈��

	static CMap *Create();														//��������

	//�e�N�X�`���֘A
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_MAP_TEXTURE];		//�e�N�X�`���ւ̃|�C���^

	LPD3DXMESH				m_pMesh;			//���b�V��
	LPD3DXBUFFER			m_pBuffMat;			//�o�b�t�@

	//�e�L�X�g�p
	CCity			*m_apMap[MAX_MAP_OPBJECT];					//�I�u�W�F�N�g�ւ̃|�C���^
	char			*m_pTextName[MAX_MAP_OPBJECT];				//���O�ۑ�
};

#endif