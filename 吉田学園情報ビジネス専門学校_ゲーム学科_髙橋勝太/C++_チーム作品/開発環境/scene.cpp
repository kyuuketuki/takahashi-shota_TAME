//=============================================================================
//
// ���X�g���� [scene.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "camera.h"

// �ÓI�����o�֐�
int CScene::m_NumAll = 0;
CScene *CScene::m_pTop[6][3];
CScene *CScene::m_pCur[6][3];

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene::CScene(int nPriority, int nType)
{
	m_nPriority = nPriority;
	m_nType = nType;

	// Top�ɉ��������ĂȂ��Ƃ�
	if (m_pTop[nPriority][nType] == NULL)		// NULL�̎�
	{
		m_pTop[nPriority][nType] = this;		// Top�ɃA�h���X������
		this->m_pNext = NULL;
		this->m_pPrev = NULL;
	}

	else // �Ō��
	{
		// ���X�g�֒ǉ�
		m_pCur[nPriority][nType]->m_pNext = this;	// �Ō���̎�������			�C�Â���
		this->m_pPrev = m_pCur[nPriority][nType];	// �����̑O���Ō���ɂ���	�F��������
		this->m_pNext = NULL;
	}

	m_pCur[nPriority][nType] = this;		// �Ō���ł���

											// ���S�t���O�̏�����
	m_bDeath = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// ���ׂĂ̔j��
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCntType = 0; nCntType < 3; nCntType++)
	{
		for (int nCntPriority = 0; nCntPriority < 6; nCntPriority++)
		{
			CScene *pScene = m_pTop[nCntPriority][nCntType];

			while (pScene)
			{
				// �|�C���^���L��������
				CScene *pSceneNext = pScene->m_pNext;

				// �X�V����
				pScene->Uninit();

				// �L�����������󂯓n��
				pScene = pSceneNext;
			}
		}
	}
}

//=============================================================================
// ���ׂĂ̍X�V�����@�p����
//=============================================================================
void CScene::UpdateAll(void)
{
	// �Q�[�����
	if (CManager::GetPause() == false)
	{
		for (int nCntType = 0; nCntType < 2; nCntType++)
		{
			// �Q�[����ʂ݂̂�`��
			{
				for (int nCntPriority = 0; nCntPriority < 6; nCntPriority++)
				{
					CScene *pScene = m_pTop[nCntPriority][nCntType];

					while (pScene)
					{
						// �|�C���^���L��������
						CScene *pSceneNext = pScene->m_pNext;

						if (pScene->m_bDeath == false)
						{
							// �X�V����
							pScene->Update();
						}

						// �L�����������󂯓n��
						pScene = pSceneNext;
					}
				}
			}
		}
	}

	// �|�[�Y���
	if (CManager::GetPause() == true)
	{
		for (int nCntType = 0; nCntType < 3; nCntType++)
		{
			// �|�[�Y��ʂƃt�F�C�h��ʂ̏ꍇ
			if (nCntType == 1 || nCntType == 2)
			{
				for (int nCntPriority = 0; nCntPriority < 6; nCntPriority++)
				{
					CScene *pScene = m_pTop[nCntPriority][nCntType];

					while (pScene)
					{
						// �|�C���^���L��������
						CScene *pSceneNext = pScene->m_pNext;

						if (pScene->m_bDeath == false)
						{
							// �X�V����
							pScene->Update();
						}

						//�L�����������󂯓n��
						pScene = pSceneNext;
					}
				}
			}
		}
	}

	// ���S�t���O���`�F�b�N���ăI�u�W�F�N�g��j��
	for (int nCntType = 0; nCntType < 3; nCntType++)
	{
		for (int nCntPriority = 0; nCntPriority < 6; nCntPriority++)
		{
			// �擪�I�u�W�F�N�g���̎擾
			CScene *pScene = m_pTop[nCntPriority][nCntType];

			// �Ō���܂Ń��[�v
			while (pScene)
			{
				// ���̏����擾
				CScene *pSceneNext = pScene->m_pNext;

				if (pScene->m_bDeath == true)
				{
					// �O��Ɏ��������������Ƃ�m�点��
					if (pScene->m_pPrev != NULL)
					{
						pScene->m_pPrev->m_pNext = pScene->m_pNext;
					}

					if (pScene->m_pNext != NULL)
					{
						pScene->m_pNext->m_pPrev = pScene->m_pPrev;
					}

					if (pScene == m_pCur[nCntPriority][nCntType])
					{
						m_pCur[nCntPriority][nCntType] = pScene->m_pPrev;
					}

					if (pScene == m_pTop[nCntPriority][nCntType])
					{
						m_pTop[nCntPriority][nCntType] = pScene->m_pNext;
					}

					// ��������
					delete pScene;
					pScene = NULL;
				}

				// ���̏��ɂ���
				pScene = pSceneNext;
			}
		}
	}
}

//=============================================================================
// ���ׂĂ̕`�揈���@�p����
//=============================================================================
void CScene::DrawAll(void)
{
	// �Q�[�����
	if (CManager::GetPause() == false)
	{
		for (int nCntType = 0; nCntType < 2; nCntType++)
		{
			for (int nCntPriority = 0; nCntPriority < 6; nCntPriority++)
			{
				CScene *pScene = m_pTop[nCntPriority][nCntType];

				while (pScene)
				{
					// �|�C���^���L��������
					CScene *pSceneNext = pScene->m_pNext;

					// �X�V����
					pScene->Draw();

					// ���̏��ɂ���
					pScene = pSceneNext;
				}
			}
		}
	}

	// �|�[�Y���
	if (CManager::GetPause() == true)
	{
		for (int nCntType = 0; nCntType < 3; nCntType++)
			{
		for (int nCntPriority = 0; nCntPriority < 6; nCntPriority++)
		{
			
				CScene *pScene = m_pTop[nCntPriority][nCntType];

				while (pScene)
				{
					// �|�C���^���L��������
					CScene *pSceneNext = pScene->m_pNext;

					// �X�V����
					pScene->Draw();

					// ���̏��ɂ���
					pScene = pSceneNext;
				}
			}
		}
	}
}

//=============================================================================
// Get�֐�
//=============================================================================
CScene * CScene::GetScene(int nIdx, int nPriority)
{
	// �擪�I�u�W�F�N�g���̎擾
	CScene *pScene = m_pTop[nPriority][0];

	int nCnt = 0;	// �v�Z�p

					// ���o�������|�C���^�܂Ń��[�v
	while (pScene)
	{
		// �|�C���^���L��������
		CScene *pSceneNext = pScene->m_pNext;

		// ���o�������|�C���^�̏ꍇ
		if (nIdx == nCnt)
		{
			break;			// ���[�v���甲���o��
		}

		// ���̃|�C���^�Ɉڂ�
		pScene = pSceneNext;

		// ���炷
		nCnt++;
	}

	return pScene;
}

//=============================================================================
// �ʒu�Ǘ�
//=============================================================================
D3DXVECTOR3 CScene::GetPosition(void)
{
	return D3DXVECTOR3();
}

//=============================================================================
// �j��
//=============================================================================
void CScene::Release(void)
{
	// ���S�t���O
	this->m_bDeath = true;
}
