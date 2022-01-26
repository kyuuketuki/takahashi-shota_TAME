//=============================================================================
//
// �V�[��X�@�|�C���g�@���� [pointerX.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "pointerX.h"
#include "sceneX.h"
#include "player3D.h"
#include "destination.h"

int g_PointNumber = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPointer::CPointer(int nPriority, int nType)
{
	g_PointNumber = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPointer::~CPointer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPointer::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	CSceneX::Init(pos, fSizeX, fSizeY);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPointer::Uninit(void)
{
	CSceneX::Uninit();

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPointer::Update(void)
{
	//�ʒu�����󂯎��
	D3DXVECTOR3 p = CPlayer3D::GetPosition();
	m_rot = CSceneX::GetRevolution();

	//�ʒu���v���C���[�̏�ɐݒ�
	p = D3DXVECTOR3(p.x, 100.0f, p.z);
	
	//�ړI�n��ݒ�
	D3DXVECTOR3 posGoal = CDestination::GetPosition(g_PointNumber);
	
	//2���_�����v�Z
	D3DXVECTOR3 a = p - posGoal;

	//�p�x���v�Z
	float nAtan2 = (float)atan2(a.z, a.x);
	m_rot.y = -nAtan2;

	//�ʒu�̍X�V
	CSceneX::SetPosition(p);
	CSceneX::SetRevolution(m_rot);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPointer::Draw(void)
{
	CSceneX::Draw();
}

//=============================================================================
// ��������
//=============================================================================
CPointer * CPointer::Create(D3DXVECTOR3 pos, char * TextName)
{
	// ���[�J���ϐ��錾
	CPointer *pPointer = NULL;
	pPointer = new CPointer;

	pPointer->BindTextName(TextName);

	// ������
	if (pPointer != NULL)
	{
		pPointer->Init(pos, 0, 0);
	}

	return pPointer;
}

void CPointer::SetPointNumber(int nNumber)
{
	g_PointNumber = nNumber;
}
