//=============================================================================
//
// �V�[��X�@AI�K�C�h�@���� [AIguide.cpp]
// Author : katano hiroya(���\�[�X:takahashi shota
//
//=============================================================================
#include "AIguide.h"
#include "sceneX.h"
#include "game.h"
#include "car.h"
#include "input.h"
#include "manager.h"

//�ÓI�����o�ϐ��錾
D3DXVECTOR3 CAIguide::m_AIguide[MAX_GUIDE_AREA][MAX_GUIDE_POINT] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CAIguide::CAIguide(int nPriority, int nType)
{
	LoadAIguide();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CAIguide::~CAIguide()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CAIguide::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void CAIguide::Uninit(void)
{
	
}

//=============================================================================
// �X�V����
//=============================================================================
void CAIguide::Update(void)
{
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void CAIguide::Draw(void)
{
	
}

//=============================================================================
// ��������
//=============================================================================
CAIguide * CAIguide::Create(D3DXVECTOR3 pos, char * TextName)
{
	CAIguide *pAIguide = NULL;
	for (int nCntArea = 0; nCntArea < MAX_GUIDE_AREA; nCntArea++)
	{
		for (int nCntPoint = 0; nCntPoint < MAX_GUIDE_POINT; nCntPoint++)
		{
			// ���[�J���ϐ��錾
			pAIguide = new CAIguide;

			pAIguide->BindTextName(TextName);

			// ������
			if (pAIguide != NULL)
			{
				pAIguide->Init(m_AIguide[nCntArea][nCntPoint], 0, 0);
			}

		}
	}
	return pAIguide;
}

//=============================================================================
// ��������
//=============================================================================
CAIguide * CAIguide::CreateSave(D3DXVECTOR3 pos, char * TextName)
{
	// ���[�J���ϐ��錾
	CAIguide *pAIguide = NULL;
	pAIguide = new CAIguide;

	pAIguide->BindTextName(TextName);

	// ������
	if (pAIguide != NULL)
	{
		pAIguide->Init(pos, 0, 0);
	}

	return pAIguide;
}

//==============================================================================
// .txt�t�H���_�̓ǂݍ���
//==============================================================================
void CAIguide::LoadAIguide(void)
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
	pFile = fopen("data/TEXT/LoadAIguide.txt", "r");

	if (pFile != NULL)
	{
		//�t�@�C���W�J�\
		aText[0] = { NULL };
		aAreaSet[0] = { NULL };
		aPointSet[0] = { NULL };
		aPoint[0] = { NULL };
		int nIdxParts = 0;			// �p�[�c��
		int nArea = 0;			// ���[�V�����̐�
		int nCntFile = 0;			// X�f�[�^�̐�
		int nAs = 0;

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

				else if (strcmp("GUIDESET", pText) == 0)
				{
					// �ϐ��̏�����
					aAreaSet[0] = { NULL };

					// ���[�J���ϐ��錾
					int nArea = 0;

					while (strcmp("END_GUIDESET", pAreaSet) != 0)
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

											fscanf(pFile, "%f", &m_AIguide[nArea][nPoint].x);
											fscanf(pFile, "%f", &m_AIguide[nArea][nPoint].y);
											fscanf(pFile, "%f", &m_AIguide[nArea][nPoint].z);
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
D3DXVECTOR3 *CAIguide::GetAIguidePos(int nArea, int nPoint)
{
	return &m_AIguide[nArea][nPoint];
}

