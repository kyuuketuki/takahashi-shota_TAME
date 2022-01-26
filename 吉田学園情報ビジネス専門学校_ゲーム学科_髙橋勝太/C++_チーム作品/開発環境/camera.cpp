//=============================================================================
//
// ���C������ [camera.cpp]
// Author : Koseki Yuto
//
//=============================================================================
#include "camera.h"
#include "player3D.h"

//�}�N����`
#define DISTANCE			(1000.0f)	// �v���C���[(�����_)�ƃJ�����̋���
#define MOUSE_SENSITIVITY	(0.0008f)	// �}�E�X����̃J������]���x
#define MOVE_CAMERA_ROT		(0.3f)		// �ړI�p�x�ɒǂ������x
#define MOVE_SPEED			(0.2f)		// �J�����̒Ǐ]���x
#define POSR_Y				(50)		// �����_�������ߗp
#define ROT_X_MIN_ANGLE		(0.1f)		// �c��]�̍ŏ��l
#define ROT_X_MAX_ANGLE		(1.40f)		// �c��]�̍ő�l

//�ÓI�����o�ϐ�
D3DXVECTOR3 CCamera::m_rot = {};
float		CCamera::m_fhorizontalAngle = NULL;

//=============================================================================
//�O���[�o���ϐ�
//=============================================================================
int nCameraType = 0;

//=============================================================================
// �R���X�^�N�^
//=============================================================================
CCamera::CCamera()
{
	m_fDistance = DISTANCE;									// �J�����ƒ����_�̋���
	m_posV[0] = D3DXVECTOR3(0.0f, 100.0f, 0.0f);			// ���_
	m_posR[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �����_
	m_vecU[0] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// �@���x�N�g��

	m_viewport[0].X = (DWORD)0.0f;							// �`�悷���ʍ���̂w���W
	m_viewport[0].Y = (DWORD)0.0f;							// �`�悷���ʍ���̂x���W
	m_viewport[0].Width = (DWORD)300.0f;					// �`�悷���ʂ̕�
	m_viewport[0].Height = (DWORD)300.0f;					// �`�悷���ʂ̍���
	m_viewport[0].MinZ = 0.0f;
	m_viewport[0].MaxZ = 1.0f;

	m_posV[1] = D3DXVECTOR3(0.0f, 100.0f, 0.0f);			// ���_
	m_posR[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �����_
	m_vecU[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// �@���x�N�g��

	m_viewport[1].X = (DWORD)0.0f;							// �`�悷���ʍ���̂w���W
	m_viewport[1].Y = (DWORD)0.0f;							// �`�悷���ʍ���̂x���W
	m_viewport[1].Width = (DWORD)SCREEN_WIDTH;				// �`�悷���ʂ̕�
	m_viewport[1].Height = (DWORD)SCREEN_HEIGHT;			// �`�悷���ʂ̍���
	m_viewport[1].MinZ = 0.0f;
	m_viewport[1].MaxZ = 1.0f;

	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);				// ��]�p�x
	int m_Rpos = 0;											// �����_���ߗp�̂͂�������
	m_fMouseSpeed = MOUSE_SENSITIVITY;						// �}�E�X���x
	m_fhorizontalAngle = 0.0f;								// �����A���O��
	m_fverticalAngle = 0.0f;								// �����A���O��
	SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);		// �}�E�X�J�[�\���̈ʒu�Œ�
	m_Mousepoint = { 0,0 };									// �}�E�X�ʒu
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCamera::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CCamera::Update(void)
{
	// �L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// �v���C���[�̈ʒu�擾
	D3DXVECTOR3 pos = CPlayer3D::GetPosition();
	D3DXVECTOR3 rot = CPlayer3D::GetRotation();

	//�f�o�b�N�p
#ifdef _DEBUG
	//���[�h�؂�ւ�
	if (pInputKeyboard->GetTrigger(DIK_N) == true)
	{
		//�L�[�{�[�h���[�h��
		if (nCameraType == 0)
		{
			nCameraType = 1;
		}

		//�J�������[�h��
		else if (nCameraType == 1)
		{
			nCameraType = 0;
		}
	}
#endif

	//�}�E�X���[�h��
	if (nCameraType == 0)
	{
		//�}�E�X����
		Mouseoperation();

		//�e�X�̃J�����̎��_�ύX
		m_posV[0].x = pos.x + sinf(m_rot.y) *-300.0f;
		m_posV[0].y = pos.y + sinf(m_rot.x) + cosf(m_rot.x) * 6000.0f;
		m_posV[0].z = pos.z + cosf(m_rot.y) *-300.0f;

		m_posV[1].x = pos.x + sinf(m_rot.y) *-m_fDistance;
		m_posV[1].y = pos.y + sinf(m_rot.x) + cosf(m_rot.x) * 300.0f;
		m_posV[1].z = pos.z + cosf(m_rot.y) *-m_fDistance;

		//���ʈȏ㎞�Ɏg�p
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			// �����_�@�v���C���[
			m_posRDest.x = pos.x;
			m_posRDest.z = pos.z;
			m_posRDest.y = pos.y + POSR_Y;

			// �ړI�̎��_�ɒ����_����
			m_posVDest.x = m_posR[nCnt].x;
			m_posVDest.z = m_posR[nCnt].z;
			m_posVDest.y = m_posR[nCnt].y;

			// �J�����ʒu�X�V
			m_posV[nCnt] += (m_posVDest - m_posV[nCnt])       * MOVE_SPEED;
			m_posR[nCnt].x += (m_posRDest.x - m_posR[nCnt].x) * MOVE_SPEED;
			m_posR[nCnt].y += (m_posRDest.y - m_posR[nCnt].y) * MOVE_SPEED;
			m_posR[nCnt].z += (m_posRDest.z - m_posR[nCnt].z) * MOVE_SPEED;
		}

		// ��]�p�x��3.14�ȏ�A-3.14�ȉ��ɂȂ�Ȃ��悤��
		if (m_rot.y >= D3DX_PI)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}
		if (m_rot.y <= -D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}

		// ��]�p�x��1.57����A-0.1��艺�ɂȂ�Ȃ��悤��
		if (m_rot.x >= ROT_X_MAX_ANGLE)
		{
			m_rot.x = ROT_X_MAX_ANGLE;
		}
		if (m_rot.x <= ROT_X_MIN_ANGLE)
		{
			m_rot.x = ROT_X_MIN_ANGLE;
		}
	}

#ifdef _DEBUG
	//�L�[�{�[�h���[�h��
	if (nCameraType == 1)
	{
		//W���������Ƃ�
		if (pInputKeyboard->GetPress(DIK_U) == true)
		{
			//���ʈȏ㎞�Ɏg�p
			for (int nCnt = 0; nCnt < 2; nCnt++)
			{
				m_posV[nCnt].y += 150.0f;
			}
		}

		//S���������Ƃ�
		if (pInputKeyboard->GetPress(DIK_J) == true)
		{
			//���ʈȏ㎞�Ɏg�p
			for (int nCnt = 0; nCnt < 2; nCnt++)
			{
				m_posV[nCnt].y -= 150.0f;
			}
		}
	}
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCamera::Draw(void)
{

}

//=============================================================================
// �}�E�X���쏈��
//=============================================================================
void CCamera::Mouseoperation(void)
{
	// �L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�Q�[���p�b�h�̃|�C���^�錾
	CGamepad *pGamepad;
	pGamepad = CManager::GetGamepad();

	// �|�[�Y�擾
	bool bPauseCheck = CManager::GetPause();

	// �}�E�X�ʒu�擾
	GetCursorPos(&m_Mousepoint);

	// �}�E�X�J�[�\���̈ʒu�Œ�
	SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	//�J�[�\���̔�\��
	ShowCursor(false);

	// �}�E�X���ǂꂾ�����������v�Z (��������) m_fMouseSpeed�͔��f���鑬�x
	m_fhorizontalAngle -= m_fMouseSpeed * float(SCREEN_WIDTH / 2 - m_Mousepoint.x);

	// �J�[�u
	if (bPauseCheck == false)
	{
		//�J�[�u
		if (pInputKeyboard->GetPress(DIK_D) == true || pGamepad->IsButtonPush(CInput::LEFT2) == true)
		{
			// �E�����
			m_fhorizontalAngle += 0.05f;
		}

		if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::RIGHT2) == true)
		{
			// �������
			m_fhorizontalAngle -= 0.05f;
		}

		// �}�E�X���ǂꂾ�����������v�Z (��������) m_fMouseSpeed�͔��f���鑬�x
		m_fverticalAngle += m_fMouseSpeed * float(SCREEN_HEIGHT / 2 - m_Mousepoint.y);

		//�J�n���̃J�����̊p�x�����߂�
		m_rot.y = (m_fhorizontalAngle + D3DX_PI);
		m_rot.x = m_fverticalAngle;
	}

	// �����A���O����3.14����A-3.14��艺�ɂȂ�Ȃ��悤��
	if (m_fhorizontalAngle >= D3DX_PI)
	{
		m_fhorizontalAngle -= D3DX_PI * 2.0f;
	}
	if (m_fhorizontalAngle <= -D3DX_PI)
	{
		m_fhorizontalAngle += D3DX_PI * 2.0f;
	}

	// �����A���O����1.57����A-0.1��艺�ɂȂ�Ȃ��悤��
	if (m_fverticalAngle >= ROT_X_MAX_ANGLE)
	{
		m_fverticalAngle = ROT_X_MAX_ANGLE;
	}
	if (m_fverticalAngle <= ROT_X_MIN_ANGLE)
	{
		m_fverticalAngle = ROT_X_MIN_ANGLE;
	}
}

float CCamera::GetMousepos(void)
{
	return m_fhorizontalAngle;
}

//=============================================================================
// �J�����ݒu
//=============================================================================
void CCamera::SetCamera(int nIdx)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���ʈȏ㎞�Ɏg�p
	//for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// �v���W�F�N�V�����}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxProjection[nIdx]);

		// �r���[�|�[�g���擾
		pDevice->SetViewport(&m_viewport[nIdx]);

		// �v���W�F�N�V�����}�g���b�N�X�̍쐬
		D3DXMatrixPerspectiveFovLH(
			&m_mtxProjection[nIdx],
			D3DXToRadian(45.0f),									// ��p�̐ݒ�
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,				// ��ʔ䗦�̐ݒ� �r���[�|�[�g�̕�����
			10.0f,													// ��O���E�̕`�拗��
			50000.0f);												// �����E�̕`�拗��

		// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection[nIdx]);

		// �r���[�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxView[nIdx]);

		// �r���[�}�g���b�N�X�̍쐬
		D3DXMatrixLookAtLH(&m_mtxView[nIdx], &m_posV[nIdx], &m_posR[nIdx], &m_vecU[nIdx]);

		// �r���[�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_VIEW, &m_mtxView[nIdx]);
	}
}