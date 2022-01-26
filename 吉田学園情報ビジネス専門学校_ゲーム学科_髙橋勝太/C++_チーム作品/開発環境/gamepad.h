//==============================================================================
//
// ゲームパッド処理[gamepad.h]
// Author; takahashi shota
//
//==============================================================================

#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

//==============================================================================
//インクルード
//==============================================================================
#include "main.h"
#include "input.h"

//=============================================================================
// ゲームパッドクラスの定義
//=============================================================================
class CGamepad : CInput
{
public:

	//列挙型（ボタンの種類）
	typedef enum
	{
		ButtonStateNone = 0,
		ButtonStateDown,
		ButtonStatePush,
		ButtonStateUp,
		ButtonStateMax,
	} ButtonState;

	CGamepad();												 //コンストラクタ
	~CGamepad();											 //デストラクタ
															
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);			 //初期化処理
	void Uninit(void);										 //終了処理
	void Update(void);										 //更新処理
															 
// スティック＆ボタン設定
	void ButtonSet(DIJOYSTATE pad_data);					 //スティック＆ボタン設定
	bool IsButtonPush(KEYINFO button);						 //プレス
	bool IsButtonUp(KEYINFO button);						 //リリース
	bool IsButtonDown(KEYINFO button);						 //トリガー
	bool BuPu(KEYINFO button, int nCnt);					 //プレス（複数）
	bool BuUp(KEYINFO button, int nCnt);					 //リリース（複数）
	bool BuDo(KEYINFO button, int nCnt);					 //トリガー（複数）

	static BOOL CALLBACK EnumJoysticksCallback(
		const DIDEVICEINSTANCE *pdidoi, VOID *pContext);		// コールバック関数
	int GetNumControl(void) { return m_nController; }			// コントローラーの数を取得

private:
	ButtonState m_ButtonStates[KEYINFO::KEYINFO_MAX];
	static int m_nController;
};
#endif

