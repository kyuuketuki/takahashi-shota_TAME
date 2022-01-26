//=============================================================================
//
// �A�C�e������ [item.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "item.h"
#include "manager.h"
#include "life.h"
#include "animation.h"

// �ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 CItem::m_pTexture[ITEM_TYPE] = {};

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CItem::CItem()
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CItem::~CItem()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, float fmove, float fSizeX, float fSizeY, int nType)
{
	CScene2D::Init(pos, fSizeX, fSizeY);

	m_fMove = fmove;
	m_fAnimeX = 0.2f;			//�e�N�X�`������
	m_fAnimeY = 0.25f;			//�e�N�X�`������
	m_nX = 5;
	m_nY = 4;

	m_nType = nType;

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CItem::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// �X�V����
//==============================================================================
void CItem::Update(void)
{
	//�T�E���h���擾
	CSound *pSound;
	pSound = CManager::GetSound();

	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	pos.y += m_fMove;

	//�ړ�������
	SetPosition(pos);

	//�X�V
	CScene2D::Update();

	//�A�j���[�V�����X�V�֐�
	Animasion();

	//���ɍs���������
	if (pos.y >= 1500.0f)
	{
		Uninit();
	}

	//����ȊO�̏ꍇ�����蔻�菈��������
	else
	{
		// �G�Ƃ̓����蔻��
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			// �I�u�W�F�N�g���̎擾
			CScene *pScene;
			pScene = GetScene(nCntScene, 2);

			// NULL�`�F�b�N
			if (pScene != NULL)
			{
				// �I�u�W�F�N�g�̎�ނ̎擾
				CScene::OBJTYPE objType = pScene->GetObjType();

				// �I�u�W�F�N�g�̎�ނ�Enemy�ꍇ
				if (objType == CScene::OBJTYPE_PLAYER)
				{
					// �ʒu�̎擾
					D3DXVECTOR3 posPlayer = pScene->GetPosition();

					// �����蔻��
					if (pos.x + 50 >= posPlayer.x - (PLAYER_SIZE - 40) && pos.x - 50 <= posPlayer.x + (PLAYER_SIZE - 40) &&
						pos.y - 50 <= posPlayer.y + (PLAYER_SIZE - 40) && pos.y + 50 >= posPlayer.y - (PLAYER_SIZE - 40))
					{
						if (m_nType == 0)
						{
							//�o���b�g����
							CPlayer::BulletLevel();
							CAnimasion::Create(pos, 100, 100, 5, 8);
						}

						if (m_nType == 1)
						{
							//�ő�HP�ȉ��̏ꍇ�񕜂�����
							if (CManager::GetPlayerLife() <= (CManager::GetMaxPlayerLife() - 1))
							{
								//PLAYER�̃��C�t��ǂݍ���
								int nLifePlayer = pScene->GetLife();

								//���C�t�����󂯓n��
								pScene->SetLife(nLifePlayer + 1);

								//�񕜃A�j���[�V����
								CAnimasion::Create(pos, 100, 100, 5, 7);

								//���W���v�Z
								float nX = ((CManager::GetPlayerLife()) * 80.0f) + 40.0f;

								//�ŐV�̍��ɐ�������
								CLife::Create(D3DXVECTOR3(nX, 100.0f, 0.0f), 30.0f, 30.0f);						
							}
						}

						//�T�E���hSE
						pSound->Play(CSound::SOUND_LABEL_SE_RECOVER);

						// �e�̔j��
						Uninit();
					}
				}
			}
		}
	}
}

//==============================================================================
// �`�揈��
//==============================================================================
void CItem::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// �A�j���[�V�����̍X�V
//==============================================================================
void CItem::Animasion(void)
{
	//�A�j���[�V�����̍X�V����
	if ((m_nCounterAnime % 8) == 0)
	{
		//�A�j���[�V�����p�^�[���̍X�V
		m_nPatternAnime = (m_nPatternAnime + 1);
	}

	//�A�j���[�V�����̍X�V
	CScene2D::SetAnime(m_fAnimeX, m_fAnimeY, m_nX, m_nY, m_nPatternAnime);

	//�J�E���^�[��i�߂�
	m_nCounterAnime++;

	//�X�V
	CScene2D::Update();

	//�A�j���[�V�������I�����Z�b�g
	if (m_nPatternAnime == m_nX * m_nY)
	{
		//�J�E���^�[�̃��Z�b�g
		m_nPatternAnime = 0;
	}
}

//==============================================================================
// ��������
//==============================================================================
CItem * CItem::Create(D3DXVECTOR3 pos, float fmove, float fSizeX, float fSizeY, int nType)
{
	// ���[�J���ϐ��錾
	CItem *pItem = NULL;
	pItem = new CItem;

	// ������
	if (pItem != NULL)
	{
		//�A�C�e���̐���
		pItem->Init(pos, fmove, fSizeX, fSizeY, nType);
	}

	// �e�N�X�`�����蓖��
	pItem->BindTexture(m_pTexture[nType]);

	return pItem;
}

//==============================================================================
//�n�[�g�̌`�ɐ���
//==============================================================================
void CItem::Heart(D3DXVECTOR3 pos, float fmove, float fSizeX, float fSizeY, int nType)
{
	//�A�C�e������
	CItem::Create(D3DXVECTOR3(pos.x, 0.0f, 0.0f), fmove, 50, 50, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 50.0f, -50.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 50.0f, -50.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 100.0f, -100.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 100.0f, -100.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 150.0f, -150.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 150.0f, -150.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 100.0f, -200.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 100.0f, -200.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 50.0f, -200.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 50.0f, -200.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x, -150.0f, 0.0f), 2.0f, 50, 50, 1);
}

void CItem::Heart2(D3DXVECTOR3 pos, float fmove, float fSizeX, float fSizeY, int nType, int nType2)
{
	//�A�C�e������
	CItem::Create(D3DXVECTOR3(pos.x, 0.0f, 0.0f), fmove, 50, 50, nType);

	CItem::Create(D3DXVECTOR3(pos.x, -50.0f, 0.0f), fmove, fSizeX, fSizeY, nType2);
	CItem::Create(D3DXVECTOR3(pos.x - 50.0f, -50.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 50.0f, -50.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 50.0f, -100.0f, 0.0f), fmove, fSizeX, fSizeY, nType2);
	CItem::Create(D3DXVECTOR3(pos.x + 50.0f, -100.0f, 0.0f), fmove, fSizeX, fSizeY, nType2);
	CItem::Create(D3DXVECTOR3(pos.x - 100.0f, -100.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 100.0f, -100.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 150.0f, -150.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 150.0f, -150.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 100.0f, -200.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 100.0f, -200.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x - 50.0f, -200.0f, 0.0f), fmove, fSizeX, fSizeY, nType);
	CItem::Create(D3DXVECTOR3(pos.x + 50.0f, -200.0f, 0.0f), fmove, fSizeX, fSizeY, nType);

	CItem::Create(D3DXVECTOR3(pos.x, -150.0f, 0.0f), 2.0f, 50, 50, 1);
}

//==============================================================================
// �e�N�X�`���̓ǂݍ���
//==============================================================================
HRESULT CItem::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pipo-mapeffect012e.png", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pipo-mapeffect012h.png", &m_pTexture[1]);

	return S_OK;
}

//==============================================================================
// �e�N�X�`���̔j��
//==============================================================================
void CItem::Unload(void)
{
	for (int nCnt = 0; nCnt < ITEM_TYPE; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
