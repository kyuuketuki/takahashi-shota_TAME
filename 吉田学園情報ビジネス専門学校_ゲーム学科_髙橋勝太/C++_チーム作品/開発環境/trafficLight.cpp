//=============================================================================
//
// �M���@�@���� [trafficLight.cpp]
// Author : katano hiroya
//
//=============================================================================
#include "trafficLight.h"
#include "sceneX.h"
#include "game.h"
#include "car.h"
#include "input.h"
#include "manager.h"

//TRAFFICLIGHT_DATA CTrafficLight::m_TrafficLight[MAX_LIGHT_AREA][MAX_LIGHT_POINT] = {};
D3DXMATRIX      CTrafficLight::m_mtxWorld;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTrafficLight::CTrafficLight(int nPriority, int nType)
{
	LoadTrafficLight();
	LoadNoneTrafficLight();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTrafficLight::~CTrafficLight()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTrafficLight::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSizeX, float fSizeY)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXLoadMeshFromX("data/MODEL/New-Traffic-light02.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

	if ((m_TrafficLight[m_nArea][m_nPoint].rot.y > D3DX_PI * 1 / 4
		&& m_TrafficLight[m_nArea][m_nPoint].rot.y < D3DX_PI * 3 / 4)
		|| (m_TrafficLight[m_nArea][m_nPoint].rot.y < -D3DX_PI * 1 / 8
			&& m_TrafficLight[m_nArea][m_nPoint].rot.y > -D3DX_PI * 3 / 8))
	{
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/City_Props02.png", &m_pTexture[m_nArea*m_nPoint + m_nPoint][0]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/City_Props01.png", &m_pTexture[m_nArea*m_nPoint + m_nPoint][1]);

		m_TrafficLight[m_nArea][m_nPoint].bSignal = false;
	}
	else
	{
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/City_Props02.png", &m_pTexture[m_nArea*m_nPoint + m_nPoint][0]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/City_Props01.png", &m_pTexture[m_nArea*m_nPoint + m_nPoint][1]);

		m_TrafficLight[m_nArea][m_nPoint].bSignal = true;
	}


	CSceneX::Init(pos, fSizeX, fSizeY);

	m_pos = pos;

	m_rot = rot;

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void CTrafficLight::Uninit(void)
{
	//���b�V���̊J��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//�e�N�X�`���̔j��
	for (int n1 = 0; n1 < 28; n1++)
	{
		for (int n2 = 0; n2 < 2; n2++)
		{
			if (m_pTexture[n1][n2] != NULL)
			{
				m_pTexture[n1][n2]->Release();
				m_pTexture[n1][n2] = NULL;
			}
		}
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTrafficLight::Update(void)
{
	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//TAB���������Ƃ�
	if (m_nCntTime % 1200 == 0)
	{
		m_TrafficLight[m_nArea][m_nPoint].bSignal = !m_TrafficLight[m_nArea][m_nPoint].bSignal;

		//for (int nCntLight = 0; nCntLight < MAX_NONE_LIGHT; nCntLight++)
		//{
		//	m_NoneTrafficLight[nCntLight].bSignal = !m_NoneTrafficLight[nCntLight].bSignal;
		//}
	}

	m_nCntTime++;

	//CSceneX::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTrafficLight::Draw(void)
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
		if (m_TrafficLight[m_nArea][m_nPoint].bSignal == true)
		{
			pDevice->SetTexture(0, m_pTexture[m_nArea*m_nPoint + m_nPoint][0]);
		}
		else if (m_TrafficLight[m_nArea][m_nPoint].bSignal == false)
		{
			pDevice->SetTexture(0, m_pTexture[m_nArea*m_nPoint + m_nPoint][1]);
		}

		//���f���p�[�c�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	//�ۑ������}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	//CSceneX::Draw();
}

//=============================================================================
// ��������
//=============================================================================
CTrafficLight * CTrafficLight::Create(D3DXVECTOR3 pos, int nArea, int nPoint)
{
	CTrafficLight *pTrafficLight = NULL;

	// ���[�J���ϐ��錾
	pTrafficLight = new CTrafficLight;

	// ������
	if (pTrafficLight != NULL)
	{
		pTrafficLight->m_nArea = nArea;

		pTrafficLight->m_nPoint = nPoint;

		pTrafficLight->Init(pTrafficLight->m_TrafficLight[nArea][nPoint].pos, pTrafficLight->m_TrafficLight[nArea][nPoint].rot, 0, 0);
	}

	return pTrafficLight;
}

//=============================================================================
// ��������
//=============================================================================
CTrafficLight * CTrafficLight::CreateNone(D3DXVECTOR3 pos, char * TextName)
{
	CTrafficLight *pTrafficLight = NULL;
	for (int nCntLight = 0; nCntLight < MAX_NONE_LIGHT; nCntLight++)
	{
		// ���[�J���ϐ��錾
		pTrafficLight = new CTrafficLight;

		pTrafficLight->BindTextName(TextName);

		// ������
		if (pTrafficLight != NULL)
		{
			pTrafficLight->Init(pTrafficLight->m_NoneTrafficLight[nCntLight].pos, pTrafficLight->m_NoneTrafficLight[nCntLight].rot, 0, 0);
			pTrafficLight->SetRevolution(pTrafficLight->m_NoneTrafficLight[nCntLight].rot);
		}
	}
	return pTrafficLight;
}

//=============================================================================
// ��������
//=============================================================================
CTrafficLight * CTrafficLight::CreateSave(D3DXVECTOR3 pos, char * TextName)
{
	// ���[�J���ϐ��錾
	CTrafficLight *pTrafficLight = NULL;
	pTrafficLight = new CTrafficLight;

	pTrafficLight->BindTextName(TextName);

	// ������
	if (pTrafficLight != NULL)
	{
		pTrafficLight->Init(pos, D3DXVECTOR3(0, 0, 0), 0, 0);
	}

	return pTrafficLight;
}

//==============================================================================
// .txt�t�H���_�̓ǂݍ���
//==============================================================================
void CTrafficLight::LoadTrafficLight(void)
{
	// ���[�J���ϐ��錾
	FILE *pFile = NULL;
	char aEqual[2] = { NULL };					//�u=�v�ǂݎ��p�ϐ�
	bool bComment = false;						// �R�����g�A�E�g���邩
	char aText[TEXT_LENGTH] = { NULL };			// �e�L�X�g�ǂݎ��p�ϐ�
	char aAreaSet[DATA_LENGTH] = { NULL };		// ���[�V�����ǂݎ��p�ϐ�
	char aPointSet[DATA_LENGTH] = { NULL };		// �L�[�ݒ���ǂݎ��p�ϐ�
	char aPoint[DATA_LENGTH] = { NULL };		// �L�[���ǂݎ��p�ϐ�

												// strcmp�ǂݎ��p�|�C���^
	char *pText = &aText[0];
	char *pAreaSet = &aAreaSet[0];
	char *pPointSet = &aPointSet[0];
	char *pPoint = &aPoint[0];

	// �f�[�^�̓ǂݍ���
	pFile = fopen("data/TEXT/LoadTrafficLight.txt", "r");

	if (pFile != NULL)
	{
		//�t�@�C���W�J�\
		aText[0] = { NULL };
		aAreaSet[0] = { NULL };
		aPointSet[0] = { NULL };
		aPoint[0] = { NULL };

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

				else if (strcmp("LIGHTSET", pText) == 0)
				{
					// �ϐ��̏�����
					aAreaSet[0] = { NULL };

					// ���[�J���ϐ��錾
					int nArea = 0;

					while (strcmp("END_LIGHTSET", pAreaSet) != 0)
					{
						// �K�C�h�̓ǂݍ���
						fscanf(pFile, "%s", &aAreaSet[0]);

						if (strcmp("NUM_AREA", pAreaSet) == 0)
						{
							// �K�C�h�G���A���̓ǂݍ���
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_NumArea);
						}

						else if (strcmp("AREASET", pAreaSet) == 0)
						{
							// �K�C�h�G���A�̓ǂݍ���
							// ���[�J���ϐ��錾
							int nPoint = 0;

							m_PointSet[nPoint] = nPoint;

							// �ϐ��̏�����
							aPointSet[0] = { NULL };

							while (strcmp("END_AREASET", pPointSet) != 0)
							{
								// �K�C�h�|�C���g��񍀖ڂ̓ǂݍ���
								fscanf(pFile, "%s", &aPointSet[0]);

								if (aPointSet[0] == '#')
								{
									// �R�����g�A�E�g
									// ���[�J���ϐ��錾
									char c = NULL;
									char d = NULL;

									fscanf(pFile, "%c", &c);

									while (c != '\n' && d != '\n')
									{
										fscanf(pFile, "%s", &aPoint[0]);
										fscanf(pFile, "%c", &d);
									}
								}

								if (strcmp("POINT", pPointSet) == 0)
								{
									// �K�C�h�|�C���g���̓ǂݍ���
									aPoint[0] = { NULL };
									while (strcmp("END_POINT", pPoint) != 0)
									{
										// �ʒu�̓ǂݍ���
										fscanf(pFile, "%s", &aPoint[0]);

										if (strcmp("POS", pPoint) == 0)
										{
											// �ʒu���̓ǂݍ���
											fscanf(pFile, "%s", &aEqual[0]);

											fscanf(pFile, "%f", &m_TrafficLight[nArea][nPoint].pos.x);
											fscanf(pFile, "%f", &m_TrafficLight[nArea][nPoint].pos.y);
											fscanf(pFile, "%f", &m_TrafficLight[nArea][nPoint].pos.z);
										}
										else if (strcmp("ROT", pPoint) == 0)
										{
											// �ʒu���̓ǂݍ���
											fscanf(pFile, "%s", &aEqual[0]);

											fscanf(pFile, "%f", &m_TrafficLight[nArea][nPoint].rot.x);
											fscanf(pFile, "%f", &m_TrafficLight[nArea][nPoint].rot.y);
											fscanf(pFile, "%f", &m_TrafficLight[nArea][nPoint].rot.z);
										}
									}
									nPoint++;
								}
							}
							nArea++;
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
	}
	fclose(pFile);
}

//==============================================================================
// .txt�t�H���_�̓ǂݍ���
//==============================================================================
void CTrafficLight::LoadNoneTrafficLight(void)
{
	// ���[�J���ϐ��錾
	FILE *pFile = NULL;
	char aEqual[2] = { NULL };					//�u=�v�ǂݎ��p�ϐ�
	bool bComment = false;						// �R�����g�A�E�g���邩
	char aText[TEXT_LENGTH] = { NULL };			// �e�L�X�g�ǂݎ��p�ϐ�
	char aAreaSet[DATA_LENGTH] = { NULL };		// ���[�V�����ǂݎ��p�ϐ�
	char aPointSet[DATA_LENGTH] = { NULL };		// �L�[�ݒ���ǂݎ��p�ϐ�
	char aPoint[DATA_LENGTH] = { NULL };		// �L�[���ǂݎ��p�ϐ�

												// strcmp�ǂݎ��p�|�C���^
	char *pText = &aText[0];
	char *pAreaSet = &aAreaSet[0];
	char *pPointSet = &aPointSet[0];
	char *pPoint = &aPoint[0];

	// �f�[�^�̓ǂݍ���
	pFile = fopen("data/TEXT/LoadNoneTrafficLight.txt", "r");

	if (pFile != NULL)
	{
		//�t�@�C���W�J�\
		aText[0] = { NULL };
		aAreaSet[0] = { NULL };
		aPointSet[0] = { NULL };
		aPoint[0] = { NULL };

		while (strcmp("END_SCRIPT", pText) != 0)
		{
			aText[0] = { NULL };
			if (bComment == false)
			{// �R�����g�A�E�g���Ă��Ȃ�
				fscanf(pFile, "%s", &aText[0]);

				if (aText[0] == '#')
				{// ������̐擪���u#�v�Ȃ�΃R�����g�A�E�g��
					bComment = true;
				}

				else if (strcmp("LIGHTSET", pText) == 0)
				{
					// �ϐ��̏�����
					aAreaSet[0] = { NULL };

					// ���[�J���ϐ��錾
					int nArea = 0;

					while (strcmp("END_LIGHTSET", pAreaSet) != 0)
					{
						// �K�C�h�̓ǂݍ���
						fscanf(pFile, "%s", &aAreaSet[0]);

						if (strcmp("NUM_AREA", pAreaSet) == 0)
						{
							// �K�C�h�G���A���̓ǂݍ���
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_NumArea);
						}

						else if (strcmp("AREASET", pAreaSet) == 0)
						{
							// �K�C�h�G���A�̓ǂݍ���
							// ���[�J���ϐ��錾
							int nPoint = 0;

							m_PointSet[nPoint] = nPoint;

							// �ϐ��̏�����
							aPointSet[0] = { NULL };

							while (strcmp("END_AREASET", pPointSet) != 0)
							{
								// �K�C�h�|�C���g��񍀖ڂ̓ǂݍ���
								fscanf(pFile, "%s", &aPointSet[0]);

								if (aPointSet[0] == '#')
								{
									// �R�����g�A�E�g
									// ���[�J���ϐ��錾
									char c = NULL;
									char d = NULL;

									fscanf(pFile, "%c", &c);

									while (c != '\n' && d != '\n')
									{
										fscanf(pFile, "%s", &aPoint[0]);
										fscanf(pFile, "%c", &d);
									}
								}

								if (strcmp("POINT", pPointSet) == 0)
								{
									// �K�C�h�|�C���g���̓ǂݍ���
									aPoint[0] = { NULL };
									while (strcmp("END_POINT", pPoint) != 0)
									{
										// �ʒu�̓ǂݍ���
										fscanf(pFile, "%s", &aPoint[0]);

										if (strcmp("POS", pPoint) == 0)
										{
											// �ʒu���̓ǂݍ���
											fscanf(pFile, "%s", &aEqual[0]);

											fscanf(pFile, "%f", &m_NoneTrafficLight[nPoint].pos.x);
											fscanf(pFile, "%f", &m_NoneTrafficLight[nPoint].pos.y);
											fscanf(pFile, "%f", &m_NoneTrafficLight[nPoint].pos.z);
										}
										else if (strcmp("ROT", pPoint) == 0)
										{
											// �ʒu���̓ǂݍ���
											fscanf(pFile, "%s", &aEqual[0]);

											fscanf(pFile, "%f", &m_NoneTrafficLight[nPoint].rot.x);
											fscanf(pFile, "%f", &m_NoneTrafficLight[nPoint].rot.y);
											fscanf(pFile, "%f", &m_NoneTrafficLight[nPoint].rot.z);
										}
									}
									nPoint++;
								}
							}
							nArea++;
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
	}
	fclose(pFile);
}