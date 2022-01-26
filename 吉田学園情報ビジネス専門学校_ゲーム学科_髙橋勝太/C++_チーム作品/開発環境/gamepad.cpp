//==============================================================================
//
// ゲームパッド処理〔gamepad.cpp〕
// AUTHOR : takahashi shota
//
//==============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "gamepad.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_CONTROLLER (2)

//グローバル変数宣言
int CGamepad::m_nController = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CGamepad::CGamepad()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CGamepad::~CGamepad()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGamepad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//初期化呼び出し
	CInput::Init(hInstance, hWnd);

	// IDirectInput8インターフェースの作成
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

	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;	// 絶対値モードの指定(DPROPAXISMODE_RELにしたら相対値)

	if (!m_pDevice)
	{
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < KEYINFO::KEYINFO_MAX; nCnt++)
	{
		m_ButtonStates[nCnt] = ButtonState::ButtonStateNone;
	}

	// データフォーマットの設定
	if (FAILED(m_pDevice->SetDataFormat(
		&c_dfDIJoystick)))
	{
		return E_FAIL;
	}

	// 軸モードを変更
	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return false;
	}

	// 左スティックの初期化
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -10000;
	diprg.lMax = 10000;

	// 右スティックの初期化
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

	// 協調モードの設定
	if (FAILED(m_pDevice->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_EXCLUSIVE))))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGamepad::Uninit(void)
{
	if (m_pDevice != NULL)
	{
		// アクセス権の破棄
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
// 更新処理
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
// スティック＆ボタン設定
//=============================================================================
void CGamepad::ButtonSet(DIJOYSTATE pad_data)
{
	// ローカル変数宣言
	bool is_push[KEYINFO::KEYINFO_MAX];
	int unresponsive_range = 200;
	float rightRange = 32767.0f;

	// 左スティックの範囲
	if (pad_data.lX < -unresponsive_range)
	{// 左に傾けた
		is_push[KEYINFO::LEFT] = true;
	}
	else if (pad_data.lX > unresponsive_range)
	{// 右に傾けた
		is_push[KEYINFO::RIGHT] = true;
	}

	if (pad_data.lY < -unresponsive_range)
	{// 上に傾けた
		is_push[KEYINFO::UP] = true;
	}
	else if (pad_data.lY > unresponsive_range)
	{// 下に傾けた
		is_push[KEYINFO::DOWN] = true;
	}

	// 右スティックの範囲
	if (pad_data.lZ < rightRange / 5.0f)
	{// 左に傾けた
		is_push[KEYINFO::RIGHT2] = true;
	}
	else if (pad_data.lZ > rightRange * 1.2f)
	{// 右に傾けた
		is_push[KEYINFO::LEFT2] = true;
	}

	if (pad_data.lRz < rightRange / 5.0f)
	{// 上に傾けた
		is_push[KEYINFO::UP2] = true;
	}
	else if (pad_data.lRz > rightRange * 1.2f)
	{// 下に傾けた
		is_push[KEYINFO::DOWN2] = true;
	}

	// 十字キーの設定
	if (pad_data.rgdwPOV[0] != 0xFFFFFFFF)
	{
		// 8方向での制御
		switch (pad_data.rgdwPOV[0])
		{
			// 上
		case 0:
			is_push[KEYINFO::UP] = true;
			break;
			// 右上
		case 4500:
			is_push[KEYINFO::UP] = true;
			is_push[KEYINFO::RIGHT] = true;
			break;
			// 右
		case 9000:
			is_push[KEYINFO::RIGHT] = true;
			break;
			// 右下
		case 13500:
			is_push[KEYINFO::DOWN] = true;
			is_push[KEYINFO::RIGHT] = true;
			break;
			// 下
		case 18000:
			is_push[KEYINFO::DOWN] = true;
			break;
			// 左下
		case 22500:
			is_push[KEYINFO::DOWN] = true;
			is_push[KEYINFO::LEFT] = true;
			break;
			// 左
		case 27000:
			is_push[KEYINFO::LEFT] = true;
			break;
			// 左上
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
		case 0:		// Xボタン
			is_push[KEYINFO::A] = true;
			break;

		case 2:		// Aボタン
			is_push[KEYINFO::A] = true;
			break;

		case 3:		// Bボタン
			is_push[KEYINFO::B] = true;
			break;

		case 5:		// LBボタン
			is_push[KEYINFO::LB] = true;
			break;

		case 6:		// RBボタン
			is_push[KEYINFO::RB] = true;
			break;

		case 10:	// Pauseボタン
			is_push[KEYINFO::P] = true;
			break;

		case 11:	// Pauseボタン
			is_push[KEYINFO::P] = true;
			break;

		case 12:	// Pauseボタン
			is_push[KEYINFO::P] = true;
			break;
		}

		break;

	}

	// 入力情報からボタンの状態を更新する
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
// プレス
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
// リリース
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
// トリガー
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
// プレス（複数）
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
// リリース（複数）
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
// トリガー（複数）
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
// コールバック関数
//=============================================================================
BOOL CGamepad::EnumJoysticksCallback(const DIDEVICEINSTANCE * pdidoi, VOID * pContext)
{
	HRESULT hr;

	hr = m_pInput->CreateDevice(pdidoi->guidInstance, &m_pDevice, NULL);

	m_nController++;

	return DIENUM_CONTINUE;
}
