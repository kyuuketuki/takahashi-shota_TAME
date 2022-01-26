//=============================================================================
//
// �V�[��X���� [sceneX.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "sceneX.h"
#include "manager.h"
#include "player3D.h"

//�ÓI�����o�ϐ��錾
char *CSceneX::m_pTextName;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSceneX::CSceneX(int nPriority, int nType) : CScene(nPriority, nType)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSceneX::~CSceneX()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSceneX::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
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
void CSceneX::Uninit(void)
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

	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSceneX::Update(void)
{
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSceneX::Draw(void)
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

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		//���f���p�[�c�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	//�ۑ������}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �e�N�X�`���ǂݍ���
//=============================================================================
void CSceneX::BindTextName(char *TextName)
{
	m_pTextName = TextName;
}

//=============================================================================
// Set�ʒu
//=============================================================================
void CSceneX::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// Set����
//=============================================================================
void CSceneX::SetRevolution(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// ��������
//=============================================================================
CSceneX *CSceneX::Create(D3DXVECTOR3 pos, char *TextName)
{
	//�V�[��X�𐶐�
	CSceneX *pSceneX = NULL;
	pSceneX = new CSceneX;

	m_pTextName = TextName;

	//NULL�`�F�b�N
	if (pSceneX != NULL)
	{
		pSceneX->Init(pos, 0.0f, 0.0f);
	}

	return pSceneX;
}
