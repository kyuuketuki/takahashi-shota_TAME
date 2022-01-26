//=============================================================================
//
// 入力処理 [input.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "input.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;
LPDIRECTINPUTDEVICE8 CInput::m_pDevice = NULL;

//=============================================================================
//コンストラクタ
//=============================================================================
CInput::CInput()
{
	m_pDeviceKey = NULL;
}
//=============================================================================
//デストラクタ
//=============================================================================
CInput::~CInput()
{

}
//=============================================================================
//キーボードのコンストラクタ
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	//キーの値をクリアする
	memset(&m_aKeyState[0], 0, sizeof(m_aKeyState));
	memset(&m_aKeyStateTrigger[0], 0, sizeof(m_aKeyStateTrigger));
}
//=============================================================================
//キーボードのデストラクタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}
//=============================================================================
//初期化処理
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
//終了処理
//=============================================================================
void CInput::Uninit(void)
{
	if (m_pDeviceKey != NULL)
	{
		//アクセス権の破棄
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
//キーボードの初期化処理
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	//初期化呼び出し
	CInput::Init(hInstance, hWnd);

	//入力デバイス
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDeviceKey, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットの設定
	if (FAILED(m_pDeviceKey->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードの設定
	if (FAILED(m_pDeviceKey->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//アクセス権の取得
	m_pDeviceKey->Acquire();

	return S_OK;
}
//=============================================================================
//キーボードの終了処理
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	//終了呼び出し
	CInput::Uninit();
}
//=============================================================================
//キーボードの更新処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeystate[256];

	//デバイスからデータを取得
	if (SUCCEEDED(m_pDeviceKey->GetDeviceState(sizeof(aKeystate), &aKeystate[0])))
	{
		for (int nKey = 0; nKey < 256; nKey++)
		{
			m_aKeyStateTrigger[nKey] = (m_aKeyState[nKey] & aKeystate[nKey]) ^ aKeystate[nKey];
			m_aKeyStateRelease[nKey] = (m_aKeyState[nKey] | aKeystate[nKey]) ^ aKeystate[nKey];
			m_aKeyState[nKey] = aKeystate[nKey];
		}
	}
	//アクセス権の取得
	m_pDeviceKey->Acquire();
}
//=============================================================================
//キーボードプレス処理
//=============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	//キーボード情報の取得
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}
//=============================================================================
//キーボードトリガー処理
//=============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	//キーボード情報の取得
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}
//=============================================================================
//キーボードトリガー処理
//=============================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	//キーボード情報の取得
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}
