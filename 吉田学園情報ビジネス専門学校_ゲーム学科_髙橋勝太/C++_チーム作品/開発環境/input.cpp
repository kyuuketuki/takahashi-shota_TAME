//=============================================================================
//
// ���͏��� [input.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "input.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;
LPDIRECTINPUTDEVICE8 CInput::m_pDevice = NULL;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CInput::CInput()
{
	m_pDeviceKey = NULL;
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CInput::~CInput()
{

}
//=============================================================================
//�L�[�{�[�h�̃R���X�g���N�^
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	//�L�[�̒l���N���A����
	memset(&m_aKeyState[0], 0, sizeof(m_aKeyState));
	memset(&m_aKeyStateTrigger[0], 0, sizeof(m_aKeyStateTrigger));
}
//=============================================================================
//�L�[�{�[�h�̃f�X�g���N�^
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}
//=============================================================================
//����������
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_FALSE;

	if (m_pInput == NULL)
	{
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL);
	}

	return hr;
}
//=============================================================================
//�I������
//=============================================================================
void CInput::Uninit(void)
{
	if (m_pDeviceKey != NULL)
	{
		//�A�N�Z�X���̔j��
		m_pDeviceKey->Unacquire();
		m_pDeviceKey->Release();
		m_pDeviceKey = NULL;
	}

	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}
//=============================================================================
//�L�[�{�[�h�̏���������
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	//�������Ăяo��
	CInput::Init(hInstance, hWnd);

	//���̓f�o�C�X
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDeviceKey, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g�̐ݒ�
	if (FAILED(m_pDeviceKey->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h�̐ݒ�
	if (FAILED(m_pDeviceKey->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�A�N�Z�X���̎擾
	m_pDeviceKey->Acquire();

	return S_OK;
}
//=============================================================================
//�L�[�{�[�h�̏I������
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	//�I���Ăяo��
	CInput::Uninit();
}
//=============================================================================
//�L�[�{�[�h�̍X�V����
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeystate[256];

	//�f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDeviceKey->GetDeviceState(sizeof(aKeystate), &aKeystate[0])))
	{
		for (int nKey = 0; nKey < 256; nKey++)
		{
			m_aKeyStateTrigger[nKey] = (m_aKeyState[nKey] & aKeystate[nKey]) ^ aKeystate[nKey];
			m_aKeyStateRelease[nKey] = (m_aKeyState[nKey] | aKeystate[nKey]) ^ aKeystate[nKey];
			m_aKeyState[nKey] = aKeystate[nKey];
		}
	}
	//�A�N�Z�X���̎擾
	m_pDeviceKey->Acquire();
}
//=============================================================================
//�L�[�{�[�h�v���X����
//=============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	//�L�[�{�[�h���̎擾
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}
//=============================================================================
//�L�[�{�[�h�g���K�[����
//=============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	//�L�[�{�[�h���̎擾
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}
//=============================================================================
//�L�[�{�[�h�g���K�[����
//=============================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	//�L�[�{�[�h���̎擾
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}
