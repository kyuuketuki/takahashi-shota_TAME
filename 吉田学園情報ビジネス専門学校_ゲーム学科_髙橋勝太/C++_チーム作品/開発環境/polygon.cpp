//=============================================================================
//
// �o���b�g�|���S������ [polygon.cpp]	(�}�`�z�u�̒e�𐶐��ł���)
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "polygon.h"
#include "bullet.h"
#include "player.h"
#include "manager.h"
#include "animation.h"

//�ÓI�����o�ϐ��錾
//�e�̃_���[�W
int CPolygon::m_nDamage = NULL;				//�U����
int CPolygon::m_nTypeNumber = NULL;			//�U���̎�ސ؂�ւ��p
bool CPolygon::m_bUsePolygon = false;		//�g�p���Ă��邩

//�O���[�o���ϐ�
bool g_bPoygonBullet = false;				//�r���I��(PLAYER�p)
bool g_bUse = false;						//����(PLAYER�p)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPolygon::CPolygon()
{
	//������
	m_CirclePattern = 0;
	m_nMaxSize = 0;
	m_nMaxSize = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPolygon::~CPolygon()
{
	//������
	m_bUse = false;
	g_bPoygonBullet = false;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPolygon::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fSizeX, float fSizeY, float fmove, float fRot, float fRotMove, int nMaxSize, int MinSize, int nType, int nTypeNumber)
{
	// CScene2D�̏���������
	CScene2D::Init(pos, fSizeX, fSizeY);

	//��]�ݒ�
	SetRevolution(fRot);

	//�����o�ϐ��ɑ��
	m_move = move;
	m_fmove = fmove;
	m_col = col;
	m_nLife = nLife;
	m_nType = nType;
	m_nTypeNumber = nTypeNumber;
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;
	m_nMaxSize = nMaxSize;
	m_nMinSize = MinSize;
	m_fRotMove = fRotMove;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPolygon::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPolygon::Update(void)
{
	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	int nDamage = 0;

	if (m_nType == OBJTYPE_BULET_ENEMY)
	{
		//�ʒu�X�V
		pos += m_move;
		nDamage = 0;
	}

	//�v���C���̈ʒu�ɍX�V
	if (m_nType == OBJTYPE_BULET_PLAYER)
	{
		pos = CPlayer::GetPlayer();
		nDamage = 5;
	}

	//�ړ�������
	SetPosition(pos);

	// ���݂̉�]�̎擾
	D3DXVECTOR3 rot = GetRevolution();

	//��]�X�V
	rot.z += m_fRotMove;

	//��]�ݒ�
	SetRevolution(rot.z);

	//�X�V
	CScene2D::Update();

	//�}�`�̊g��
	if (m_CirclePattern == 0)
	{
		//�g��k��
		m_fSizeX += m_fmove;
		m_fSizeY += m_fmove;

		//�g�傳����ő�l
		if (m_fSizeX >= m_nMaxSize)
		{
			m_CirclePattern = 1;
		}

	}

	//�}�`�̏k��
	if (m_CirclePattern == 1)
	{
		//�g��k��
		m_fSizeX -= m_fmove;
		m_fSizeY -= m_fmove;

		//�k��������ŏ��l
		if (m_fSizeX <= m_nMinSize)
		{
			m_CirclePattern = 0;
		}
	}

	//���C�t��0�ȉ��ɂȂ����Ƃ�
	if (m_nLife <= 0)
	{
		Uninit();
	}

	//�͈͊O�̏ꍇ
	if (pos.y <= 0 - 500.0f)
	{
		Uninit();
	}
	else if (pos.y >= SCREEN_HEIGHT + 500.0f)
	{
		Uninit();
	}

	if (g_bPoygonBullet == true)
	{
		if (m_nType == OBJTYPE_BULET_PLAYER)
		{
			g_bPoygonBullet = false;
			Uninit();
		}
	}

	// �ʒu
	D3DXVECTOR3 posBullet[4];

	// �Ίp���̐ݒ�
	float fLength = sqrtf(m_fSizeX * m_fSizeX + m_fSizeY * m_fSizeY);
	float fAngle = atan2f(m_fSizeX, m_fSizeY);

	//���_�̍��W
	posBullet[0].x = (pos.x - sinf(fAngle + rot.z) * fLength);
	posBullet[0].y = (pos.y + cosf(fAngle + rot.z) * fLength);
	posBullet[0].z = (pos.z = 0.0f);

	posBullet[1].x = (pos.x - sinf(fAngle - rot.z) * fLength);
	posBullet[1].y = (pos.y - cosf(fAngle - rot.z) * fLength);
	posBullet[1].z = (pos.z = 0.0f);

	posBullet[2].x = (pos.x + sinf(fAngle - rot.z) * fLength);
	posBullet[2].y = (pos.y + cosf(fAngle - rot.z) * fLength);
	posBullet[2].z = (pos.z = 0.0f);

	posBullet[3].x = (pos.x + sinf(fAngle + rot.z) * fLength);
	posBullet[3].y = (pos.y - cosf(fAngle + rot.z) * fLength);
	posBullet[3].z = (pos.z = 0.0f);
	
	//�G���甭�˂��ꂽ�e�̏ꍇ
	if (m_nType == OBJTYPE_BULET_ENEMY)
	{
		if (m_nTypeNumber == 1)
		{
			CBullet::Create(posBullet[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_col, 1, nDamage, BULLET_SIZE, BULLET_SIZE, m_nType);
		}

		if (m_nTypeNumber == 4)
		{
			CBullet::Create(posBullet[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_col, 1, nDamage, BULLET_SIZE, BULLET_SIZE, m_nType);
			CBullet::Create(posBullet[1], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_col, 1, nDamage, BULLET_SIZE, BULLET_SIZE, m_nType);
			CBullet::Create(posBullet[2], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_col, 1, nDamage, BULLET_SIZE, BULLET_SIZE, m_nType);
			CBullet::Create(posBullet[3], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_col, 1, nDamage, BULLET_SIZE, BULLET_SIZE, m_nType);
		}
	}

	//�v���C���[���甭�˂������e�̏ꍇ
	if (m_nType == OBJTYPE_BULET_PLAYER)
	{
		//��������
		if (m_nTypeNumber == 1)
		{
			CBullet::Create(posBullet[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_col, 1, nDamage, BULLET_SIZE, BULLET_SIZE, m_nType);
			CBullet::Create(posBullet[1], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_col, 1, nDamage, BULLET_SIZE, BULLET_SIZE, m_nType);
			CBullet::Create(posBullet[2], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_col, 1, nDamage, BULLET_SIZE, BULLET_SIZE, m_nType);
			CBullet::Create(posBullet[3], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_col, 1, nDamage, BULLET_SIZE, BULLET_SIZE, m_nType);

			if (g_bUse == true)
			{
				//�e�̔���
				CBullet::Create(posBullet[0], D3DXVECTOR3(0.0f, -10.0f, 0.0f), m_col, 150, m_nDamage, BULLET_SIZE, BULLET_SIZE, m_nType);
				CBullet::Create(posBullet[1], D3DXVECTOR3(0.0f, -10.0f, 0.0f), m_col, 150, m_nDamage, BULLET_SIZE, BULLET_SIZE, m_nType);
				CBullet::Create(posBullet[2], D3DXVECTOR3(0.0f, -10.0f, 0.0f), m_col, 150, m_nDamage, BULLET_SIZE, BULLET_SIZE, m_nType);
				CBullet::Create(posBullet[3], D3DXVECTOR3(0.0f, -10.0f, 0.0f), m_col, 150, m_nDamage, BULLET_SIZE, BULLET_SIZE, m_nType);

				g_bUse = false;
			}
		}

		//�����L��
		if (m_nTypeNumber == 2)
		{
			CBullet::Create(posBullet[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_col, 1, 1, BULLET_SIZE, BULLET_SIZE, m_nType);
			CBullet::Create(posBullet[1], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_col, 1, 1, BULLET_SIZE, BULLET_SIZE, m_nType);
			CBullet::Create(posBullet[2], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_col, 1, 1, BULLET_SIZE, BULLET_SIZE, m_nType);
			CBullet::Create(posBullet[3], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_col, 1, 1, BULLET_SIZE, BULLET_SIZE, m_nType);
		}
	}

	//�G���甭�˂��ꂽ�e�̏ꍇ
	if (m_nType == OBJTYPE_BULET_ENEMY)
	{
		//�e�̃��C�t�����炷
		m_nLife--;
	}

	//�v���C���[���甭�˂������e�̏ꍇ
	if (m_nType == OBJTYPE_BULET_PLAYER)
	{
		//�^�C�v�����L���̏ꍇ
		if (m_nTypeNumber == 2)
		{
			//�e�̃��C�t�����炷
			m_nLife--;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPolygon::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CPolygon * CPolygon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fSizeX, float fSizeY, float fmove, float fRot, float fRotMove, int nMaxSize, int MinSize, int nType, int nTypeNumber)
{
	//�}�`�e�𐶐�
	CPolygon *pPolgon = NULL;
	pPolgon = new CPolygon;

	pPolgon->Init(pos, move, col, nLife, fSizeX, fSizeY, fmove, fRot, fRotMove, nMaxSize, MinSize, nType, nTypeNumber);

	return pPolgon;
}

//=============================================================================
// �v���C���[�̔��˗p
//=============================================================================
void CPolygon::PlayerBullet(int nDamage)
{
	g_bUse = true;
	m_nDamage = nDamage;
}

//=============================================================================
// �r���j��
//=============================================================================
void CPolygon::UninitBullet(void)
{
	//�e�𐶐�
	CPolygon *pPolgon = NULL;
	pPolgon = new CPolygon;

	g_bPoygonBullet = true;
}