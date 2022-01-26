//=============================================================================
//
// �e�L�X�g�����I�u�W�F�N�g���� [textobject.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "textobject.h"
#include "object3D.h"

//�O���[�o���ϐ��錾
CObject3D *CTextobject::m_pObject3D[MAX_OBJECT_3D] = {};		//���������|�C���^
D3DXVECTOR3 g_posObject3DText[256];								//�ǂݍ��񂾈ʒu���
D3DXVECTOR3 g_rotObject3DText[256];								//�ǂݍ��񂾈ʒu���
int g_nCntObject3DText;											//�������������L��
int g_nTypeObject3DText[256];									//�^�C�v�����L��
int g_nMaxFileX;												//�ǂݍ��񂾍ő僂�f����
int g_nCntName = 0;												//�ǂݍ���URL�����L��
char g_aNameObject3D[256][128];									//�ǂݍ����URL���

//==============================================================================
// 
//==============================================================================
CTextobject::CTextobject()
{
	g_posObject3DText[256] = {};
	g_rotObject3DText[256] = {};
	g_nCntObject3DText = 0;
	g_nTypeObject3DText[256] = {};
	g_aNameObject3D[256][128] = {};
	g_nCntName = 0;
}

//==============================================================================
// 
//==============================================================================
CTextobject::~CTextobject()
{
}

//==============================================================================
// ����������
//==============================================================================
HRESULT CTextobject::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	LoadObject3D();

	//�ǂݍ��񂾉񐔕�����
	for (int nCount = 0; nCount < g_nCntObject3DText; nCount++)
	{
		m_pObject3D[nCount] = CObject3D::Create(g_posObject3DText[nCount], g_rotObject3DText[nCount], g_nTypeObject3DText[nCount]);
	}

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CTextobject::Uninit(void)
{

}

//==============================================================================
// �X�V����
//==============================================================================
void CTextobject::Update(void)
{

}

//==============================================================================
// �`�揈��
//==============================================================================
void CTextobject::Draw(void)
{

}

//==============================================================================
// ���������񐔂�`����
//==============================================================================
int CTextobject::GetTextObject3D()
{
	return g_nCntObject3DText;
}

//==============================================================================
// �ǂݍ���X�t�@�C���ő吔
//==============================================================================
int CTextobject::GetMaxFileX()
{
	return g_nMaxFileX;
}

//==============================================================================
// �e�L�X�gURL��`���� ��
//==============================================================================
char * CTextobject::GetTextName(int nNumber)
{
	return g_aNameObject3D[nNumber];
}

//==============================================================================
// ��������
//==============================================================================
CTextobject * CTextobject::Create()
{
	// �|�C���^�ϐ��̐���
	CTextobject *pTextobject;
	pTextobject = new CTextobject;

	// NULL�`�F�b�N
	if (pTextobject != NULL)
	{
		pTextobject->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
	}

	return pTextobject;
}

//==============================================================================
//X�t�@�C���̓ǂݍ���
//==============================================================================
void CTextobject::LoadObject3D(void)
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

	pFile = fopen("data/TEXT/object3Dget.txt", "r");

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
				if (strcmp("MODEL_FILENAME", pText) == 0)
				{
					// X�t�@�C�����̓ǂݍ���
					fscanf(pFile, "%s", &aEqual[0]);
					fscanf(pFile, "%s", &g_aNameObject3D[g_nCntName][0]);
					g_nCntName++;
				}

				if (strcmp("MAX_MODEL", pText) == 0)
				{
					// X�t�@�C�����̓ǂݍ���
					fscanf(pFile, "%s", &aEqual[0]);
					fscanf(pFile, "%d", &g_nMaxFileX);
				}

				else if (strcmp("OBJECTSET", aText) == 0)
				{
					pSetObjectEND[0] = { NULL };

					while (strcmp("END_OBJECTSET", pSetObjectEND) != 0)
					{
						fscanf(pFile, "%s", &pSetObjectEND[0]);

						if (strcmp("POSX", pSetObjectEND) == 0)
						{
							// ���f��posX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObject3DText[g_nCntObject3DText].x);
						}

						if (strcmp("POSY", pSetObjectEND) == 0)
						{
							// ���f��posY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObject3DText[g_nCntObject3DText].y);
						}

						if (strcmp("POSZ", pSetObjectEND) == 0)
						{
							// ���f��posZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObject3DText[g_nCntObject3DText].z);
						}

						if (strcmp("ROTX", pSetObjectEND) == 0)
						{
							// ���f��posX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_rotObject3DText[g_nCntObject3DText].x);
						}

						if (strcmp("ROTY", pSetObjectEND) == 0)
						{
							// ���f��posY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_rotObject3DText[g_nCntObject3DText].y);
						}

						if (strcmp("ROTZ", pSetObjectEND) == 0)
						{
							// ���f��posZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_rotObject3DText[g_nCntObject3DText].z);
						}

						if (strcmp("TYPE", pSetObjectEND) == 0)
						{
							// ���f��posZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &g_nTypeObject3DText[g_nCntObject3DText]);
						}
					}
					g_nCntObject3DText++;
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
