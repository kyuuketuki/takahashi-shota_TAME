//=============================================================================
//
// �}�b�v���� [city.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "city.h"
#include "manager.h"
#include "player3D.h"

//�ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 CCity::m_pTexture[54] = {};
char *CCity::m_pTextName;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCity::CCity(int nPriority, int nType)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCity::~CCity()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCity::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//model�ǂݍ���
	D3DXLoadMeshFromX(m_pTextName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

	int nNumVtx;			//���_��
	DWORD sizeFVF;			//���_�t�H�[�}�b�g�̃T�C�Y
	D3DXMATERIAL *pMat;		//�}�e���A���ւ̃|�C���^

	//���_�����擾
	nNumVtx = m_pMesh->GetNumVertices();
	
	m_nNumVtx = nNumVtx;

	//�}�e���A���f�[�^�ւ̃|�C���^�擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//�ʒu
	m_pos = pos;

	//����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCity::Uninit(void)
{
	//���b�V���̊J��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//�}�e���A���̊J��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < 54; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CCity::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CCity::Draw(void)
{
	//���[�J��
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ�
	D3DXMATERIAL *pMat;				//�}�e���A���ւ̃|�C���^

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����̔��]
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�̔��]
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���݂̃}�e���A���̎擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^�擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		if (m_pTexture != NULL)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_pTexture[nCntMat]);
		}

		else
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);
		}

		//���f���p�[�c�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	//�ۑ������}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// Set�ʒu
//=============================================================================
void CCity::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// Set����
//=============================================================================
void CCity::SetRevolution(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// ��������
//=============================================================================
CCity *CCity::Create(D3DXVECTOR3 pos, char *TextName)
{
	//�V�[��X�𐶐�
	CCity *pCity = NULL;
	pCity = new CCity;

	m_pTextName = TextName;

	//NULL�`�F�b�N
	if (pCity != NULL)
	{
		pCity->Init(pos, 0.0f, 0.0f);
	}

	return pCity;
}

//==============================================================================
//�e�N�X�`���ݒ�
//==============================================================================
void CCity::SetTexture(int nNumber, LPDIRECT3DTEXTURE9 pTexture)
{
	//��������e�N�X�`���������Ă���
	m_pTexture[nNumber] = pTexture;
}
