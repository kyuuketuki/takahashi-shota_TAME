//=============================================================================
//
// ���_�����蔻�菈������ [vertex.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "vertex.h"
#include "manager.h"
#include "scene3D.h"
#include "visualization.h"

//�O���[�o���ϐ��錾
D3DXVECTOR3		g_posObjectVector[1024][4];				//�e�L�X�g�f�[�^�̈ʒu���
int				g_nCntVertex[1024];						//�����ԍ��L��
int				g_nDisplayVertex = 0;					//�����ԍ���\���p
int				g_nDisplayVertex2 = 0;					//�x�N�g���ԍ��̕\���p
int				g_FailCntVecter;						//�����J�E���g
int				g_nMaxFail;								//�e�L�X�g�œǂݍ��񂾉�
float			g_fAtan2Vecter;							//�p�x�̕\���p
bool			g_bObjectVector[1024];					//�e�L�X�g�f�[�^�̉����I���I�t���

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CVertex::CVertex(int nPriority, int nType)
{
	//�ϐ��̏�����
	g_nCntVertex[256] = {};
	g_nDisplayVertex = 0;
	g_nDisplayVertex2 = 0;
	g_fAtan2Vecter = 0;
	g_nMaxFail = 0;
	g_FailCntVecter = 0;

	for (int nCount = 0; nCount < 256; nCount++)
	{
		g_bObjectVector[nCount] = false;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CVertex::~CVertex()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CVertex::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CVertex::Uninit(void)
{
	//�J�E���g���Z�b�g
	g_FailCntVecter = 0;

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CVertex::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CVertex::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CVertex * CVertex::Create(D3DXVECTOR3 pos, D3DXVECTOR3 fMaxSizeX, D3DXVECTOR3 fMinSizeX, D3DXVECTOR3 fMaxSizeY, D3DXVECTOR3 fMinSizeY)
{
	//���f���𐶐�
	CVertex *pVertex = NULL;
	pVertex = new CVertex;

	D3DXVECTOR3 pos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�����蔻��̐���
	pVertex->Init(pos2, 0.0f, 0.0f);
	pVertex->CVertex::LoadObject();

	return pVertex;
}

//=============================================================================
// �ϐ������炷
//=============================================================================
void CVertex::Release(void)
{
	//�����ԍ������炷
	g_FailCntVecter -= 2;
}

//==============================================================================
// �O�ς𗘗p���������蔻��
//==============================================================================
bool CVertex::CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	// �J�������̎擾
	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 rot = pCamera->GetRot();

	bool bLand = false;

	//�v�Z�p
	D3DXVECTOR3 posold;
	D3DXVECTOR3 g_Vec[MAX_OBJECT_VEC][4];
	D3DXVECTOR3 g_oldVec[MAX_OBJECT_VEC][4];
	float PlayerVec[MAX_OBJECT_VEC][4];
	float oldPlayerVec[MAX_OBJECT_VEC][4];

	D3DXVECTOR3 pos = *pPos;			// ����Ώۂ̈ʒu
	posold = *pPosOld;					// ����Ώۂ̈ʒu

	//�ǂݍ��񂾍��W�����Ƃɓ����蔻������
	for (int nCnt = 0; nCnt < g_FailCntVecter + 1; nCnt++)
	{
		m_aPos[nCnt][0] = g_posObjectVector[nCnt][0];
		m_aPos[nCnt][1] = g_posObjectVector[nCnt][1];
		m_aPos[nCnt][2] = g_posObjectVector[nCnt][2];
		m_aPos[nCnt][3] = g_posObjectVector[nCnt][3];

		//���ѕ���������
		//90����Ă���ꍇ
		if (m_aPos[nCnt][0].x > m_aPos[nCnt][2].x && m_aPos[nCnt][0].z < m_aPos[nCnt][2].z)
		{
			D3DXVECTOR3 vecold;
			vecold = m_aPos[nCnt][0];

			m_aPos[nCnt][0] = m_aPos[nCnt][3];
			m_aPos[nCnt][3] = m_aPos[nCnt][2];
			m_aPos[nCnt][2] = m_aPos[nCnt][1];
			m_aPos[nCnt][1] = vecold;
		}
		//180����Ă���ꍇ
		else if (m_aPos[nCnt][0].x < m_aPos[nCnt][2].x && m_aPos[nCnt][0].z < m_aPos[nCnt][2].z)
		{
			D3DXVECTOR3 vecold;
			vecold = m_aPos[nCnt][0];

			D3DXVECTOR3 vecold2;
			vecold2 = m_aPos[nCnt][1];

			m_aPos[nCnt][0] = m_aPos[nCnt][2];
			m_aPos[nCnt][2] = vecold;
			m_aPos[nCnt][1] = m_aPos[nCnt][3];
			m_aPos[nCnt][3] = vecold2;
		}
		//270����Ă���ꍇ
		else if (m_aPos[nCnt][0].x < m_aPos[nCnt][2].x && m_aPos[nCnt][0].z > m_aPos[nCnt][2].z)
		{
			D3DXVECTOR3 vecold;
			vecold = m_aPos[nCnt][0];

			m_aPos[nCnt][0] = m_aPos[nCnt][1];
			m_aPos[nCnt][1] = m_aPos[nCnt][2];
			m_aPos[nCnt][2] = m_aPos[nCnt][3];
			m_aPos[nCnt][3] = vecold;
		}

		//�O�ς̎��@�x�N�g��
		m_Vec[nCnt][0] = m_aPos[nCnt][1] - m_aPos[nCnt][0];
		m_Vec[nCnt][1] = m_aPos[nCnt][2] - m_aPos[nCnt][1];
		m_Vec[nCnt][2] = m_aPos[nCnt][3] - m_aPos[nCnt][2];
		m_Vec[nCnt][3] = m_aPos[nCnt][0] - m_aPos[nCnt][3];

		//�l���_��
		for (int nCount = 0; nCount < 4; nCount++)
		{
			//�O�ς̎�2
			if (nCount == 0)
			{
				g_Vec[nCnt][nCount] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPos[nCnt][nCount];
			}
			else if (nCount == 1)
			{
				g_Vec[nCnt][nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPos[nCnt][nCount];
			}
			else if (nCount == 2)
			{
				g_Vec[nCnt][nCount] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPos[nCnt][nCount];
			}
			else if (nCount == 3)
			{
				g_Vec[nCnt][nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPos[nCnt][nCount];
			}

			//�O�όv�Z
			PlayerVec[nCnt][nCount] = (m_Vec[nCnt][nCount].z * g_Vec[nCnt][nCount].x) - (m_Vec[nCnt][nCount].x * g_Vec[nCnt][nCount].z);
		}

		//if (pPos->y <= m_pos.y + m_MaxStone.y - fHeightMin && pPos->y > m_pos.y - m_MinStone.y - fHeightMax)
		{
			if (PlayerVec[nCnt][0] > 0.0f && PlayerVec[nCnt][1] > 0.0f && PlayerVec[nCnt][2] > 0.0f && PlayerVec[nCnt][3] > 0.0f)
			{
				for (int nCount = 0; nCount < 4; nCount++)
				{
					//�O�ς̎�2
					if (nCount == 0)
					{
						g_oldVec[nCnt][nCount] = posold + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPos[nCnt][nCount];
					}
					else if (nCount == 1)
					{
						g_oldVec[nCnt][nCount] = posold + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPos[nCnt][nCount];
					}
					else if (nCount == 2)
					{
						g_oldVec[nCnt][nCount] = posold + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPos[nCnt][nCount];
					}
					else if (nCount == 3)
					{
						g_oldVec[nCnt][nCount] = posold + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPos[nCnt][nCount];
					}

					oldPlayerVec[nCnt][nCount] = (m_Vec[nCnt][nCount].z * g_oldVec[nCnt][nCount].x) - (m_Vec[nCnt][nCount].x * g_oldVec[nCnt][nCount].z);
				}

				//���E�̓����蔻��
				if (PlayerVec[nCnt][1] > 0 && PlayerVec[nCnt][3] > 0)
				{
					//�\���p
					g_nDisplayVertex = g_nCntVertex[nCnt];

					//��
					if (oldPlayerVec[nCnt][0] <= 0 && PlayerVec[nCnt][0] >= 0 && PlayerVec[nCnt][2] >= 0)
					{
						g_nDisplayVertex2 = 0;

						//2���_�����v�Z
						D3DXVECTOR3 a = m_aPos[nCnt][0] - m_aPos[nCnt][1];

						//�p�x���v�Z
						double nAtan2 = atan2(a.z, a.x);

						if (nAtan2 > 0.0f)						//�Εӂ̏ꍇ
						{
							pPos->z = pPosOld->z;				//1�t���[���O�̈ʒu�ɖ߂�
							pPos->x = pPosOld->x;
							pMove->z = 0.0f;					//�ړ��ʂ��[���ɂ���
							pMove->x = sinf((float)nAtan2) * 5;
						}

						else if (nAtan2 <= 0.0f)				//�Εӂ̏ꍇ �܂��͊p�x��0�̂Ƃ�
						{
							pPos->z = pPosOld->z;				//1�t���[���O�̈ʒu�ɖ߂�
							pPos->x = pPosOld->x;
							pMove->z = 0.0f;					//�ړ��ʂ��[���ɂ���
							pMove->x = sinf((float)-nAtan2) * 5;
						}

						//�\���p
						g_fAtan2Vecter = (float)nAtan2;
					}

					//�E
					else if (oldPlayerVec[nCnt][2] <= 0 && PlayerVec[nCnt][2] >= 0 && PlayerVec[nCnt][1] >= 0)
					{
						//�\���p
						g_nDisplayVertex2 = 2;

						//2���_�����v�Z
						D3DXVECTOR3 a = m_aPos[nCnt][2] - m_aPos[nCnt][3];

						//�p�x���v�Z
						double nAtan2 = atan2(a.z, a.x);

						if (nAtan2 > 0.0f)							//�Εӂ̏ꍇ
						{
							pPos->z = pPosOld->z;					//1�t���[���O�̈ʒu�ɖ߂�
							pPos->x = pPosOld->x;
							pMove->z = 0.0f;						//�ړ��ʂ��[���ɂ���
							pMove->x = sinf((float)-nAtan2) * 5;
						}

						else if (nAtan2 <= 0.0f)					//�Εӂ̏ꍇ �܂��͊p�x��0�̂Ƃ�	
						{
							pPos->z = pPosOld->z;					//1�t���[���O�̈ʒu�ɖ߂�
							pPos->x = pPosOld->x;
							pMove->z = 0.0f;						//�ړ��ʂ��[���ɂ���
							pMove->x = sinf((float)nAtan2) * 5;
						}

						//�\���p
						g_fAtan2Vecter = (float)nAtan2;
					}
				}

				//�O��̓����蔻��
				if (PlayerVec[nCnt][0] > 0 && PlayerVec[nCnt][2] > 0)
				{
					//�\���p
					g_nDisplayVertex = g_nCntVertex[nCnt];

					//��
					if (oldPlayerVec[nCnt][1] <= 0 && PlayerVec[nCnt][1] >= 0)
					{
						//�\���p
						g_nDisplayVertex2 = 1;

						//2���_�����v�Z
						D3DXVECTOR3 a = m_aPos[nCnt][2] - m_aPos[nCnt][1];

						//�p�x���v�Z
						double nAtan2 = atan2(a.z, a.x);

						if (nAtan2 > 0.0f)							//�Εӂ̏ꍇ
						{
							pPos->z = pPosOld->z;					//1�t���[���O�̈ʒu�ɖ߂�
							pPos->x = pPosOld->x;
							pMove->x = 0.0f;						//�ړ��ʂ��[���ɂ���
							pMove->z = cosf((float)nAtan2) * 5;
						}

						else if (nAtan2 <= 0.0f)					//�Εӂ̏ꍇ �܂��͊p�x��0�̂Ƃ�
						{
							pPos->z = pPosOld->z;					//1�t���[���O�̈ʒu�ɖ߂�
							pPos->x = pPosOld->x;
							pMove->x = 0.0f;						//�ړ��ʂ��[���ɂ���
							pMove->z = cosf((float)-nAtan2) * 5;
						}

						//�\���p
						g_fAtan2Vecter = (float)nAtan2;
					}

					//�O
					if (oldPlayerVec[nCnt][3] <= 0 && PlayerVec[nCnt][3] >= 0)
					{
						//�\���p
						g_nDisplayVertex2 = 3;

						//2���_�����v�Z
						D3DXVECTOR3 a = m_aPos[nCnt][0] - m_aPos[nCnt][3];

						//�p�x���v�Z
						double nAtan2 = atan2(a.z, a.x);

						if (nAtan2 > 0.0f)							//�Εӂ̏ꍇ
						{
							pPos->z = pPosOld->z;					//1�t���[���O�̈ʒu�ɖ߂�
							pPos->x = pPosOld->x;
							pMove->x = 0.0f;						//�ړ��ʂ��[���ɂ���
							pMove->z = cosf((float)nAtan2) * 5;
						}

						else if (nAtan2 <= 0.0f)					//�Εӂ̏ꍇ	 �܂��͊p�x��0�̂Ƃ�			
						{
							pPos->z = pPosOld->z;					//1�t���[���O�̈ʒu�ɖ߂�
							pPos->x = pPosOld->x;
							pMove->x = 0.0f;						//�ړ��ʂ��[���ɂ���
							pMove->z = cosf((float)-nAtan2) * 5;
						}

						//�\���p
						g_fAtan2Vecter = (float)nAtan2;
					}
				}
			}
		}
	}

	//�e�L�X�g�Ő�������������
	//�ǂݍ��񂾍��W�����Ƃɓ����蔻������
	for (int nCnt = 0; nCnt < g_nMaxFail + 1; nCnt++)
	{
		//����
		if (g_bObjectVector[nCnt] == false)
		{
			CVisualization::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_posObjectVector[nCnt][0], g_posObjectVector[nCnt][1], g_posObjectVector[nCnt][3], g_posObjectVector[nCnt][2], false, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.3f));
			g_bObjectVector[nCnt] = true;
		}
	}

	return bLand;
	//return false;
}

//==============================================================================
//X�t�@�C���̓ǂݍ���
//==============================================================================
void CVertex::LoadObject(void)
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

	pFile = fopen("data/TEXT/vectorget.txt", "r");

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
				if (strcmp("VECTORSET", aText) == 0)
				{
					pSetObjectEND[0] = { NULL };

					g_FailCntVecter++;

					while (strcmp("END_VECTORSET", pSetObjectEND) != 0)
					{
						fscanf(pFile, "%s", &pSetObjectEND[0]);

						if (strcmp("Vec1POSX", pSetObjectEND) == 0)
						{
							// ���f��posX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][0].x);
							g_nCntVertex[g_FailCntVecter] = g_FailCntVecter + 1;
						}

						if (strcmp("Vec1POSY", pSetObjectEND) == 0)
						{
							// ���f��posY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][0].y);
						}

						if (strcmp("Vec1POSZ", pSetObjectEND) == 0)
						{
							// ���f��posZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][0].z);
						}

						if (strcmp("Vec2POSX", pSetObjectEND) == 0)
						{
							// ���f��posX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][1].x);
						}

						if (strcmp("Vec2POSY", pSetObjectEND) == 0)
						{
							// ���f��posY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][1].y);
						}

						if (strcmp("Vec2POSZ", pSetObjectEND) == 0)
						{
							// ���f��posZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][1].z);
						}

						if (strcmp("Vec3POSX", pSetObjectEND) == 0)
						{
							// ���f��posX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][2].x);
						}

						if (strcmp("Vec3POSY", pSetObjectEND) == 0)
						{
							// ���f��posY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][2].y);
						}

						if (strcmp("Vec3POSZ", pSetObjectEND) == 0)
						{
							// ���f��posZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][2].z);
						}

						if (strcmp("Vec4POSX", pSetObjectEND) == 0)
						{
							// ���f��posX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][3].x);
						}

						if (strcmp("Vec4POSY", pSetObjectEND) == 0)
						{
							// ���f��posY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][3].y);
						}

						if (strcmp("Vec4POSZ", pSetObjectEND) == 0)
						{
							// ���f��posZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntVecter][3].z);
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

	//�e�N�X�g�Ő������鐔���L��
	g_nMaxFail = g_FailCntVecter;

	//�e�L�X�g�����
	fclose(pFile);
}

//==============================================================================
// �������Ă��鐶���ԍ�
//==============================================================================
int CVertex::GetNumber(void)
{
	return g_nDisplayVertex - 1;
}

//==============================================================================
// �������Ă���x�N�g���ԍ�
//==============================================================================
int CVertex::GetNumber2(void)
{
	return g_nDisplayVertex2;
}

//==============================================================================
// �������Ă���Ζʂ̊p�x
//==============================================================================
float CVertex::GetAtan2(void)
{
	return g_fAtan2Vecter;
}
