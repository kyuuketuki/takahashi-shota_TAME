//=============================================================================
//
// �V�[������ [scene.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _SCENE_H_
#define _SCENE_H_

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"

//VERTEX_3D�̐錾
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 nor;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_3D;

//=============================================================================
// �V�[���N���X�̒�`
//=============================================================================
class CScene
{
public:

	//UI��typedef enum
	typedef enum
	{
		FRAME = 0,
		GAMEFRAME,

		TITLE_LOGO,
		TITLE_UI_PRESS_ENTER,
		TITLE_UI_GAMESTART,
		TITLE_UI_TUTORIAL,
		TITLE_UI_END,
		MENU_FLAME,

		CONTROLS_BG,

		PAUSE01,
		PAUSE_RESTART,
		PAUSE_MENU,
		PAUSE_EXIT,

		LIST,

		START,

		RESULT_SCORE,

		MAX_SCORE,

		RANKING_LOGO,
		RANKING_FIRST,
		RANKING_SECOND,
		RANKING_THIRD,
		RANKING_FOUR,
		RANKING_FIVE,

		RANKING_BG,

		RANKING_ADD,

		RANKING_EQUAL,

		RANKING_TUTORIAL,

		RANKING_FREAM,

		GAMR_UI01,

		RULE,

		MAX_UI,
	}UI;

	CScene(int nPriority = 1, int nType = 0);				//�R���X�g���N�^
	virtual ~CScene();										//�f�X�g���N�^

	virtual HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY) = 0;	//����������
	virtual void Uninit(void) = 0;											//�I������
	virtual void Update(void) = 0;											//�X�V����
	virtual void Draw(void) = 0;											//�`�揈��

	static void ReleaseAll(void);											//���ׂĂ̏I������
	static void UpdateAll(void);											//���ׂĂ̍X�V����
	static void DrawAll(void);												//���ׂĂ̕`�揈��

	//�D�揇�ʂ̐ݒ�
	static CScene *GetScene(int nIdx, int nPriority);

	//�T�C�Y
	float GetSizeX(void) { return m_fSizeX; }
	void SetSizeXScene(float nSize) { m_fSizeX = nSize; }
	float GetSizeY(void) { return m_fSizeY; }
	void SetSizeYScene(float nSize) { m_fSizeY = nSize; }

	//�ʒu�Ǘ�
	virtual D3DXVECTOR3 GetPosition(void);

protected:
	void Release(void);

private:
	int				m_nID;					//�i�[��̔ԍ�
	int				m_nPriority;			// �D�揇��
	int				m_nType;				//�^�C�v
	float			m_fSizeX;				//�T�C�Y
	float			m_fSizeY;				//�T�C�Y
											
	//�ÓI�����o�ϐ�						 
	static int		m_NumAll;				//�����ԍ�

	//���X�g���p�ϐ�
	static CScene *m_pTop[6][3];			//�őO��
	static CScene *m_pCur[6][3];			//�Ō��
	CScene *m_pPrev;						//�O�̃|�C���^�[
	CScene *m_pNext;						//��̃|�C���^�[
	bool m_bDeath  = false;					//���S�t���O
};

#endif
