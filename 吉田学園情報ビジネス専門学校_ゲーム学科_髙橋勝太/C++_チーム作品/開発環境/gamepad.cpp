//==============================================================================
//
// �Q�[���p�b�h�����kgamepad.cpp�l
// AUTHOR : takahashi shota
//
//==============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "gamepad.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define MAX_CONTROLLER (2)

//�O���[�o���ϐ��錾
int CGamepad::m_nController = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGamepad::CGamepad()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGamepad::~CGamepad()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGamepad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//�������Ăяo��
	CInput::Init(hInstance, hWnd);

	// IDirectInput8�C���^�[�t�F�[�X�̍쐬
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInput,
		NULL)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		EnumJoysticksCallback,
		NULL,
		DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	// �����[�h���Βl���[�h�Ƃ��Đݒ�
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;	// ��Βl���[�h�̎w��(DPROPAXISMODE_REL�ɂ����瑊�Βl)

	if (!m_pDevice)
	{
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < KEYINFO::KEYINFO_MAX; nCnt++)
	{
		m_ButtonStates[nCnt] = ButtonState::ButtonStateNone;
	}

	// �f�[�^�t�H�[�}�b�g�̐ݒ�
	if (FAILED(m_pDevice->SetDataFormat(
		&c_dfDIJoystick)))
	{
		return E_FAIL;
	}

	// �����[�h��ύX
	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return false;
	}

	// ���X�e�B�b�N�̏�����
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -10000;
	diprg.lMax = 10000;

	// �E�X�e�B�b�N�̏�����
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(dipdw);
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
	dipdw.diph.dwHow = DIPH_BYOFFSET;
	dipdw.dwData = 0;

	if (FAILED(m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	// �������[�h�̐ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_EXCLUSIVE))))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGamepad::Uninit(void)
{
	if (m_pDevice != NULL)
	{
		// �A�N�Z�X���̔j��
		m_pDevice->Unacquire();

		m_pDevice->Release();
		m_pDevice = NULL;
	}

	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CGamepad::Update(void)
{
	if (m_pDevice == nullptr)
	{
		return;
	}

	DIJOYSTATE pad_data;

	HRESULT hr = m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &pad_data);

	if (FAILED(hr))
	{
		if (FAILED(m_pDevice->Acquire()))
		{
			for (int nCntButton = 0; nCntButton < KEYINFO::KEYINFO_MAX; nCntButton++)
			{
				m_ButtonStates[nCntButton] = ButtonState::ButtonStateNone;
			}
			m_pDevice->Poll();
		}
		return;
	}
	ButtonSet(pad_data);
}

//=============================================================================
// �X�e�B�b�N���{�^���ݒ�
//=============================================================================
void CGamepad::ButtonSet(DIJOYSTATE pad_data)
{
	// ���[�J���ϐ��錾
	bool is_push[KEYINFO::KEYINFO_MAX];
	int unresponsive_range = 200;
	float rightRange = 32767.0f;

	// ���X�e�B�b�N�͈̔�
	if (pad_data.lX < -unresponsive_range)
	{// ���ɌX����
		is_push[KEYINFO::LEFT] = true;
	}
	else if (pad_data.lX > unresponsive_range)
	{// �E�ɌX����
		is_push[KEYINFO::RIGHT] = true;
	}

	if (pad_data.lY < -unresponsive_range)
	{// ��ɌX����
		is_push[KEYINFO::UP] = true;
	}
	else if (pad_data.lY > unresponsive_range)
	{// ���ɌX����
		is_push[KEYINFO::DOWN] = true;
	}

	// �E�X�e�B�b�N�͈̔�
	if (pad_data.lZ < rightRange / 5.0f)
	{// ���ɌX����
		is_push[KEYINFO::RIGHT2] = true;
	}
	else if (pad_data.lZ > rightRange * 1.2f)
	{// �E�ɌX����
		is_push[KEYINFO::LEFT2] = true;
	}

	if (pad_data.lRz < rightRange / 5.0f)
	{// ��ɌX����
		is_push[KEYINFO::UP2] = true;
	}
	else if (pad_data.lRz > rightRange * 1.2f)
	{// ���ɌX����
		is_push[KEYINFO::DOWN2] = true;
	}

	// �\���L�[�̐ݒ�
	if (pad_data.rgdwPOV[0] != 0xFFFFFFFF)
	{
		// 8�����ł̐���
		switch (pad_data.rgdwPOV[0])
		{
			// ��
		case 0:
			is_push[KEYINFO::UP] = true;
			break;
			// �E��
		case 4500:
			is_push[KEYINFO::UP] = true;
			is_push[KEYINFO::RIGHT] = true;
			break;
			// �E
		case 9000:
			is_push[KEYINFO::RIGHT] = true;
			break;
			// �E��
		case 13500:
			is_push[KEYINFO::DOWN] = true;
			is_push[KEYINFO::RIGHT] = true;
			break;
			// ��
		case 18000:
			is_push[KEYINFO::DOWN] = true;
			break;
			// ����
		case 22500:
			is_push[KEYINFO::DOWN] = true;
			is_push[KEYINFO::LEFT] = true;
			break;
			// ��
		case 27000:
			is_push[KEYINFO::LEFT] = true;
			break;
			// ����
		case 31500:
			is_push[KEYINFO::UP] = true;
			is_push[KEYINFO::LEFT] = true;
			break;
		}
	}

	for (int nCntButton = 0; nCntButton < 32; nCntButton++)
	{
		if (!(pad_data.rgbButtons[nCntButton] & 0x80))
		{
			continue;
		}

		switch (nCntButton)
		{
		case 0:		// X�{�^��
			is_push[KEYINFO::A] = true;
			break;

		case 2:		// A�{�^��
			is_push[KEYINFO::A] = true;
			break;

		case 3:		// B�{�^��
			is_push[KEYINFO::B] = true;
			break;

		case 5:		// LB�{�^��
			is_push[KEYINFO::LB] = true;
			break;

		case 6:		// RB�{�^��
			is_push[KEYINFO::RB] = true;
			break;

		case 10:	// Pause�{�^��
			is_push[KEYINFO::P] = true;
			break;

		case 11:	// Pause�{�^��
			is_push[KEYINFO::P] = true;
			break;

		case 12:	// Pause�{�^��
			is_push[KEYINFO::P] = true;
			break;
		}

		break;

	}

	// ���͏�񂩂�{�^���̏�Ԃ��X�V����
	for (int nCntButton = 0; nCntButton < KEYINFO::KEYINFO_MAX; nCntButton++)
	{
		if (is_push[nCntButton] == true)
		{
			if (m_ButtonStates[nCntButton] == ButtonState::ButtonStateNone)
			{
				m_ButtonStates[nCntButton] = ButtonState::ButtonStateDown;
			}
			else
			{
				m_ButtonStates[nCntButton] = ButtonState::ButtonStatePush;
			}
		}
		else
		{
			if (m_ButtonStates[nCntButton] == ButtonState::ButtonStatePush)
			{
				m_ButtonStates[nCntButton] = ButtonState::ButtonStateUp;
			}
			else
			{
				m_ButtonStates[nCntButton] = ButtonState::ButtonStateNone;
			}
		}
	}
}

//=============================================================================
// �v���X
//=============================================================================
bool CGamepad::IsButtonPush(KEYINFO button)
{
	if (m_ButtonStates[button] == ButtonState::ButtonStatePush)
	{
		return true;
	}

	return false;
}

//=============================================================================
// �����[�X
//=============================================================================
bool CGamepad::IsButtonUp(KEYINFO button)
{
	if (m_ButtonStates[button] == ButtonState::ButtonStateUp)
	{
		return true;
	}

	return false;
}

//=============================================================================
// �g���K�[
//=============================================================================
bool CGamepad::IsButtonDown(KEYINFO button)
{
	if (m_ButtonStates[button] == ButtonState::ButtonStateDown)
	{
		return true;
	}

	return false;
}

//=============================================================================
// �v���X�i�����j
//=============================================================================
bool CGamepad::BuPu(KEYINFO button, int nCnt)
{
	if (m_ButtonStates[button] == ButtonState::ButtonStatePush)
	{
		return true;
	}

	return false;
}

//=============================================================================
// �����[�X�i�����j
//=============================================================================
bool CGamepad::BuUp(KEYINFO button, int nCnt)
{
	if (m_ButtonStates[button] == ButtonState::ButtonStateUp)
	{
		return true;
	}

	return false;
}

//=============================================================================
// �g���K�[�i�����j
//=============================================================================
bool CGamepad::BuDo(KEYINFO button, int nCnt)
{
	if (m_ButtonStates[button] == ButtonState::ButtonStateDown)
	{
		return true;
	}

	return false;
}

//=============================================================================
// �R�[���o�b�N�֐�
//=============================================================================
BOOL CGamepad::EnumJoysticksCallback(const DIDEVICEINSTANCE * pdidoi, VOID * pContext)
{
	HRESULT hr;

	hr = m_pInput->CreateDevice(pdidoi->guidInstance, &m_pDevice, NULL);

	m_nController++;

	return DIENUM_CONTINUE;
}
