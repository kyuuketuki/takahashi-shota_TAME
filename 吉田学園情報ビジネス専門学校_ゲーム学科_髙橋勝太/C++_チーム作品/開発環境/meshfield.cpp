//=============================================================================
//
// ���b�V���t�B�[���h����[meshfield.cpp]
// Author; takahashi syota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "meshfield.h"
#include "scene3D.h"
#include "manager.h"
#include "renderer.h"

//�O���[�o���ϐ��錾
float CMeshfield::m_fX = NULL;
float CMeshfield::m_fZ = NULL;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CMeshfield::CMeshfield()
{

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CMeshfield::~CMeshfield()
{

}

//=============================================================================
//���b�V���t�B�[���h�̏���������
//=============================================================================
HRESULT CMeshfield::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	// ���[�J���ϐ��錾
	VERTEX_3D *pVtx;

	LPDIRECT3DDEVICE9 pDevice;
	WORD *pIdx;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�g�ǂݍ���
	LoadObject();

	// �t�@�C�������g�p���ăe�N�X�`����ǂݍ���
	D3DXCreateTextureFromFile(pDevice, aTextureName, &m_pTexture);

	//���S�����킹��
	m_pos = D3DXVECTOR3(-(m_nWidth*m_fWidth / 2), -35.0f, (m_nDepth*m_fDepth / 2));

	//���|���S����
	m_nPolygon = m_nWidth * m_nDepth * 2 + (4 * (m_nDepth - 1));

	//�C��
	m_nDepth += 1;
	m_nWidth += 1;

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							//��]�p�x
	m_nAllPoint = m_nDepth * 2 * (m_nWidth + 2) - 2;				//�����_��
	m_nIdxPoint = m_nWidth * m_nDepth;								//�C���f�b�N�X�o�b�t�@�ł̒��_��

																	//�ő�T�C�Y�̌v�Z
	float fWidthMax = fWidth * (float)m_nWidth;
	float fDepthMax = fHight * (float)m_nDepth;

	//�v���C���[�̈ړ��������Ɏg�p
	m_fX = m_nWidth*m_fWidth / 2 - m_fWidth / 2;
	m_fZ = m_nDepth*m_fDepth / 2 - m_fDepth / 2;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nIdxPoint, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_nDepth; nCnt++)
	{
		// ���s��
		for (int nCntA = 0; nCntA < m_nWidth; nCntA++)
		{
			// ����	 
			// �|���S���̊e���_���W
			pVtx[0].pos = D3DXVECTOR3(-fWidthMax / 2.0f + (float)nCntA * m_fWidth, m_pos.y, fDepthMax / 2.0f - (float)nCnt * m_fDepth);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �e���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����_���̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(1.0f * nCntA, 1.0f * nCnt);

			pVtx++;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nAllPoint, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);

	// �C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//���ёւ�����
	for (int nCntA = 0; nCntA < m_nDepth; nCntA++)
	{
		for (int nCnt = 0; nCnt < m_nWidth + 1; nCnt++, pIdx += 2)
		{
			if (nCnt != 0 && nCnt == m_nWidth && nCntA != m_nDepth - 1)
			{
				// �E�[����܂�Ԃ���
				pIdx[0] = pIdx[-1];
				pIdx[1] = pIdx[-2] + 1;
			}
			else if (nCntA == m_nDepth - 1 && nCnt == m_nWidth)
			{
				// �I�����ɖ�������
				break;
			}
			else
			{
				// �ʏ�z�u
				pIdx[0] = m_nWidth + (m_nWidth * nCntA) + nCnt;
				pIdx[1] = pIdx[0] - m_nWidth;
			}
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();

	return S_OK;
}


//=============================================================================
//���b�V���t�B�[���h�̏I������
//=============================================================================
void CMeshfield::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̊J��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//���b�V���t�B�[���h�̍X�V����
//=============================================================================
void CMeshfield::Update(void)
{

}

//=============================================================================
//���b�V���t�B�[���h�̕`�揈��
//=============================================================================
void CMeshfield::Draw(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;// �v�Z�p�}�g���b�N�X

								//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,
		0,
		m_nIdxPoint,			// ���_��
		0,
		m_nPolygon);			// �v���~�e�B�u��
}

//=============================================================================
//���b�V���t�B�[���h�̃N���G�C�g����
//=============================================================================
CMeshfield * CMeshfield::Create()
{
	//�C���X�^���X����
	CMeshfield *pMeshfield = NULL;
	pMeshfield = new CMeshfield;

	//�������Ăяo��
	pMeshfield->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

	return pMeshfield;
}

//=============================================================================
// �e�L�X�g�ǂݍ���
//=============================================================================
void CMeshfield::LoadObject(void)
{
	FILE *pFile = NULL;
	char aEqual[2] = { NULL };					// �u=�v�ǂݎ��p�ϐ�
	bool bComment = false;						// �R�����g�A�E�g���邩
	char aText[TEXT_LENGTH] = { NULL };			// �e�L�X�g�ǂݎ��p�ϐ�
	char aSetObject[DATA_LENGTH] = { NULL };	// �e�L�X�g�ǂݎ��p�ϐ�
	char aSetObjectEND[DATA_LENGTH] = { NULL };	// �e�L�X�g�ǂݎ��p�ϐ�

												// strcmp�ǂݎ��p�|�C���^
	char *pText = &aText[0];
	char *pSetObject = &aSetObject[0];
	char *pSetObjectEND = &aSetObjectEND[0];

	pFile = fopen("data/TEXT/meshfield.txt", "r");

	aText[0] = { NULL };
	pSetObject[0] = { NULL };
	pSetObjectEND[0] = { NULL };

	while (strcmp("END_SCRIPT", pText) != 0)
	{
		aText[0] = { NULL };

		if (bComment == false)
		{
			// �R�����g�A�E�g���Ă��Ȃ�
			fscanf(pFile, "%s", &aText[0]);

			if (aText[0] == '#')
			{
				// ������̐擪���u#�v�Ȃ�΃R�����g�A�E�g��
				bComment = true;
			}

			else
			{
				if (strcmp("TEXTURE_FILENAME", pText) == 0)
				{
					// X�t�@�C�����̓ǂݍ���
					fscanf(pFile, "%s", &aEqual[0]);
					fscanf(pFile, "%s", &aTextureName[0]);
				}

				else if (strcmp("MESHFILDSET", aText) == 0)
				{
					pSetObjectEND[0] = { NULL };

					while (strcmp("END_MESHFILDSET", pSetObjectEND) != 0)
					{
						fscanf(pFile, "%s", &pSetObjectEND[0]);

						if (strcmp("SIZE_X", pSetObjectEND) == 0)
						{
							// ���f��posX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_fWidth);
						}

						if (strcmp("SIZE_Z", pSetObjectEND) == 0)
						{
							// ���f��posY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_fDepth);
						}

						if (strcmp("WIDTH", pSetObjectEND) == 0)
						{
							// ���f��posZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_nWidth);
						}

						if (strcmp("DEPTH", pSetObjectEND) == 0)
						{
							// ���f��posZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_nDepth);
						}
					}
				}
			}
		}

		else if (bComment == true)
		{
			// �R�����g�A�E�g����
			// ���[�J���ϐ��錾
			char a = NULL;
			char b = NULL;
			fscanf(pFile, "%c", &a);
			while (a != '\n' && b != '\n')
			{
				fscanf(pFile, "%s", &aText[0]);
				fscanf(pFile, "%c", &b);
			}
			bComment = false;
		}
	}

	//�e�L�X�g�����
	fclose(pFile);
}

//=============================================================================
// Get�֐��@���b�V���t�B�[���h�T�C�Y��m�点��X
//=============================================================================
float CMeshfield::GetMeshFildX(void)
{
	return m_fX;
}

//=============================================================================
// Get�֐��@���b�V���t�B�[���h�T�C�Y��m�点��Y
//=============================================================================
float CMeshfield::GetMeshFildZ(void)
{
	return m_fZ;
}
