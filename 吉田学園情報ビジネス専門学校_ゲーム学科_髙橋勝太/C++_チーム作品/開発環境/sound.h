//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : takahashi syota
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE,		// BGM1
	SOUND_LABEL_BGM_TUTORIAL,	// BGM2
	SOUND_LABEL_BGM_GAME,		// BGM3
	SOUND_LABEL_BGM_RESULT,		// BGM4
	SOUND_LABEL_SE_MIGRATION,	// �ړ���
	SOUND_LABEL_SE_DECISION,	// ���艹
	SOUND_LABEL_SE_BIKE_RUNNING,// �o�C�N���s��
	SOUND_LABEL_SE_BIKE_IDLING,	// �o�C�N�A�C�h�����O
	SOUND_LABEL_SE_CAT_MEOW,	// �L�̖���
	SOUND_LABEL_SE_CAR_RUNNING,	// �Ԃ̑��s��
	SOUND_LABEL_SE_POLICE_SIREN,// �p�g�J�[�̃T�C����
	SOUND_LABEL_SE_CAR_HORN,	// �Ԃ̃N���N�V����
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
class CSound
{
public:
	CSound();								//�R���X�g���N�^
	virtual ~CSound();						//�f�X�g���N�^

	typedef enum
	{
		SOUND_LABEL_BGM_TITLE,		// BGM1
		SOUND_LABEL_BGM_TUTORIAL,	// BGM2
		SOUND_LABEL_BGM_GAME,		// BGM3
		SOUND_LABEL_BGM_RESULT,		// BGM4
		SOUND_LABEL_SE_MIGRATION,	// �ړ���
		SOUND_LABEL_SE_DECISION,	// ���艹
		SOUND_LABEL_SE_BIKE_RUNNING,// �o�C�N���s��
		SOUND_LABEL_SE_BIKE_IDLING,	// �o�C�N�A�C�h�����O
		SOUND_LABEL_SE_CAT_MEOW,	// �L�̖���
		SOUND_LABEL_SE_CAR_RUNNING,	// �Ԃ̑��s��
		SOUND_LABEL_SE_POLICE_SIREN,// �p�g�J�[�̃T�C����
		SOUND_LABEL_SE_CAR_HORN,	// �Ԃ̃N���N�V����
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	HRESULT Init(HWND hWnd);
	void	Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void	Stop(SOUND_LABEL label);

	HRESULT PlayMiddle(SOUND_LABEL label);
	void	Pause(SOUND_LABEL label);

	void	SetVolume(SOUND_LABEL label, float fVolume);

private:
	//�p�����[�^�\���̒�`
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDPARAM;

	HRESULT ChekChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunk(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y

																// �e���f�ނ̃p�����[�^
	SOUNDPARAM m_aParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/TITLE/maou_bgm_cyber16.wav", -1 },				// BGM1
		{ "data/BGM/TUTORIAL/maou_bgm_cyber07.wav", -1 },			// BGM2
		{ "data/BGM/GAME/maou/�z�C/maou_game_town05.wav", -1 },		// BGM3
		{ "data/BGM/RESULT/maou_bgm_acoustic10.wav", -1 },			// BGM4
		{ "data/SE/migration.wav", 0 },						// �ړ���
		{ "data/SE/decision.wav", 0 },						// ���艹
		{ "data/SE/bike_running.wav", -1 },					// �o�C�N���s��
		{ "data/SE/bike_idling.wav", -1 },					// �o�C�N�A�C�h�����O
		{ "data/SE/cat_meow.wav", 0 },						// �L�̖���
		{ "data/SE/car_running.wav", -1 },					// �Ԃ̑��s��
		{ "data/SE/police_siren.wav", -1 },					// �p�g�J�[�̃T�C����
		{ "data/SE/car_horn.wav", 0 },						// �Ԃ̃N���N�V����
	};
};

#endif