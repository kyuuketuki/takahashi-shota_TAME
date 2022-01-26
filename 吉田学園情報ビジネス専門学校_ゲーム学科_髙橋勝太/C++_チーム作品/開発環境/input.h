//=============================================================================
//
// 入力処理 [input.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _INPUT_H_
#define _INPUT_H_

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"

//=============================================================================
//インプットクラスの定義
//=============================================================================
class CInput
{
public:
	//列挙型（ボタンの種類)
	typedef enum
	{
		RETURN = 0,
		X,
		W,
		S,
		A,
		D,
		SPACE,
		BACKSPACE,
		F,
		T,
		B,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		LEFT2,
		RIGHT2,
		UP2,
		DOWN2,
		LB,
		RB,
		P,
		KEYINFO_MAX
	} KEYINFO;

	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	LPDIRECTINPUTDEVICE8 m_pDeviceKey;
	static LPDIRECTINPUTDEVICE8 m_pDevice;
	static LPDIRECTINPUT8 m_pInput;
};

//=============================================================================
//インプットキーボードクラスの定義
//=============================================================================
class CInputKeyboard : CInput
{
public:
	CInputKeyboard();									//コンストラク
	~CInputKeyboard();									//デストラクタタ
														
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		//初期化処理
	void Uninit(void);									//終了処理
	void Update(void);									//更新処理
														
	bool GetPress(int nKey);							//プレス
	bool GetTrigger(int nKey);							//トリガー
	bool GetRelease(int nKey);							//リリース
private:
	BYTE m_aKeyState[256];
	BYTE m_aKeyStateTrigger[256];
	BYTE m_aKeyStateRelease[256];
};
#endif
