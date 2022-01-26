//==============================================================================
//
// �Q�[���p�b�h����[gamepad.h]
// Author; takahashi shota
//
//==============================================================================

#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

//==============================================================================
//�C���N���[�h
//==============================================================================
#include "main.h"
#include "input.h"

//=============================================================================
// �Q�[���p�b�h�N���X�̒�`
//=============================================================================
class CGamepad : CInput
{
public:

	//�񋓌^�i�{�^���̎�ށj
	typedef enum
	{
		ButtonStateNone = 0,
		ButtonStateDown,
		ButtonStatePush,
		ButtonStateUp,
		ButtonStateMax,
	} ButtonState;

	CGamepad();												 //�R���X�g���N�^
	~CGamepad();											 //�f�X�g���N�^
															
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);			 //����������
	void Uninit(void);										 //�I������
	void Update(void);										 //�X�V����
															 
// �X�e�B�b�N���{�^���ݒ�
	void ButtonSet(DIJOYSTATE pad_data);					 //�X�e�B�b�N���{�^���ݒ�
	bool IsButtonPush(KEYINFO button);						 //�v���X
	bool IsButtonUp(KEYINFO button);						 //�����[�X
	bool IsButtonDown(KEYINFO button);						 //�g���K�[
	bool BuPu(KEYINFO button, int nCnt);					 //�v���X�i�����j
	bool BuUp(KEYINFO button, int nCnt);					 //�����[�X�i�����j
	bool BuDo(KEYINFO button, int nCnt);					 //�g���K�[�i�����j

	static BOOL CALLBACK EnumJoysticksCallback(
		const DIDEVICEINSTANCE *pdidoi, VOID *pContext);		// �R�[���o�b�N�֐�
	int GetNumControl(void) { return m_nController; }			// �R���g���[���[�̐����擾

private:
	ButtonState m_ButtonStates[KEYINFO::KEYINFO_MAX];
	static int m_nController;
};
#endif

