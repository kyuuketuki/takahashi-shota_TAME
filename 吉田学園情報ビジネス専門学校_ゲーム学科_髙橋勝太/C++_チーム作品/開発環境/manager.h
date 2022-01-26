//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _MANAGER_H_
#define _MANAGER_H_

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "score.h"
#include "game.h"
#include "gamepad.h"
#include "camera.h"
#include "light.h"
#include "sceneX.h"
#include "texture.h"
#include "menu.h"
#include "title.h"
#include "result.h"
#include "tutorial.h"
#include "pause.h"
#include <stdio.h>

//�O���錾
class CLight;
class CCamera;

//=============================================================================
// �}�l�[�W���[�N���X�̒�`
//=============================================================================
class CManager
{
public:
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_MENU,
		MODE_SELECT,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_GAME2,
		MODE_RESULT,
		MODE_PAUSE,
		MODE_MAX
	}MODE;

	CManager();								//�R���X�g���N�^
	~CManager();							//�f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);		//����������
	static void SetMode(MODE mode);									//�I������
	void Uninit(void);												//�X�V����
	void Update(void);												//�`�揈��
	void Draw(void);
	
	//�ÓI�����o�錾
	static CInputKeyboard	*GetInputKeyboard(void);
	static CTexture			*GetTexture(void);
	static CRenderer		*GetRenderer(void);
	static CSound			*GetSound(void);
	static CCamera			*GetCamera(void);

	//���[�h���
	static MODE GetMode(void) { return m_mode; }

	//�Q�[���p�b�h�̏��擾
	static CGamepad *GetGamepad(void) { return m_pGamepad; }

	//�Q�[���p�b�h�̏�񏑂�����
	static CGamepad *m_pGamePad;

	//�J�ڃJ�E���g
	static int GetNumber();

	//�v���C���[HP�֌W
	static int GetPlayerScore(void);
	static void SetPlayerScore(int nScore);

	// �|�[�Y��Ԃ̎擾
	static bool GetPause();

	// �^�C�}�[�\��
	HRESULT InitTime(void);
	void UninitTime(void);
	void UpdateTime(void);
	void DrawTime(void);

private:
	//�ÓI�����o�ϐ�
	static CRenderer		*m_pRenderer;
	static CTexture			*m_pTexture;
	static CInputKeyboard	*m_pInputKeyboard;
	static CInput			*m_pInput;
	static CSound			*m_pSound;
	static CScene			*m_pScene;
	static CScore			*m_pScore;
	static CNumber			*m_pNumber;
	static CGamepad			*m_pGamepad;
	static CGame			*m_pGame;
	static CPause			*m_pPause;
	static CTitle			*m_pTitle;
	static CMenu			*m_pMenu;
	static CTutorial		*m_pTutorial;
	static CResult			*m_pResult;
	static CCamera			*m_pCamera;
	static CLight			*m_pLight;
	static CSceneX			*m_pSceneX;
	static MODE				m_mode;

	static bool m_bEndGame;						// �I�����̎��S�t���O

	static int m_nPalyerScore;					// �X�R�A�Ǘ�
};

#endif
