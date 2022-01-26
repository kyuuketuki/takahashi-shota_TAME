//=============================================================================
//
// �Ƌ����� [License.h]
// Author : koseki yuto
//
//=============================================================================

#ifndef _LICENCE_H_
#define _LICENCE_H_

#include "main.h"
#include "scene2D.h"

//=============================================================================
// ���C�Z���X�N���X�̒�`
//=============================================================================
class CLicense : public CScene2D
{
public:
	CLicense(int nPriority = 3, int nType = 0);			//�R���X�g���N�^
	~CLicense();										//�f�X�g���N�^

	typedef enum
	{
		LISENCETYPE_GOLD = 0,
		LISENCETYPE_NORMAL,
		LISENCETYPE_MAX,
	}LISENCETYPE;

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	static CLicense *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//��������
	static HRESULT Load(void);												//�e�N�X�`���ǂݍ���
	static void Unload(void);												//�e�N�X�`���j��

	static LISENCETYPE GetLisenceType(void) { return m_nLicenseType; }
	static void SetLisenceType(LISENCETYPE lisencetype);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;									// �p�ӂ���e�N�X�`���̐�
	static LISENCETYPE m_nLicenseType;										// �Ƌ��̈�
};

#endif 
