//=============================================================================
//
// サウンド処理 [sound.h]
// Author : takahashi syota
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE,		// BGM1
	SOUND_LABEL_BGM_TUTORIAL,	// BGM2
	SOUND_LABEL_BGM_GAME,		// BGM3
	SOUND_LABEL_BGM_RESULT,		// BGM4
	SOUND_LABEL_SE_MIGRATION,	// 移動音
	SOUND_LABEL_SE_DECISION,	// 決定音
	SOUND_LABEL_SE_BIKE_RUNNING,// バイク走行音
	SOUND_LABEL_SE_BIKE_IDLING,	// バイクアイドリング
	SOUND_LABEL_SE_CAT_MEOW,	// 猫の鳴き声
	SOUND_LABEL_SE_CAR_RUNNING,	// 車の走行音
	SOUND_LABEL_SE_POLICE_SIREN,// パトカーのサイレン
	SOUND_LABEL_SE_CAR_HORN,	// 車のクラクション
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
class CSound
{
public:
	CSound();								//コンストラクタ
	virtual ~CSound();						//デストラクタ

	typedef enum
	{
		SOUND_LABEL_BGM_TITLE,		// BGM1
		SOUND_LABEL_BGM_TUTORIAL,	// BGM2
		SOUND_LABEL_BGM_GAME,		// BGM3
		SOUND_LABEL_BGM_RESULT,		// BGM4
		SOUND_LABEL_SE_MIGRATION,	// 移動音
		SOUND_LABEL_SE_DECISION,	// 決定音
		SOUND_LABEL_SE_BIKE_RUNNING,// バイク走行音
		SOUND_LABEL_SE_BIKE_IDLING,	// バイクアイドリング
		SOUND_LABEL_SE_CAT_MEOW,	// 猫の鳴き声
		SOUND_LABEL_SE_CAR_RUNNING,	// 車の走行音
		SOUND_LABEL_SE_POLICE_SIREN,// パトカーのサイレン
		SOUND_LABEL_SE_CAR_HORN,	// 車のクラクション
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
	//パラメータ構造体定義
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;

	HRESULT ChekChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunk(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

																// 各音素材のパラメータ
	SOUNDPARAM m_aParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/TITLE/maou_bgm_cyber16.wav", -1 },				// BGM1
		{ "data/BGM/TUTORIAL/maou_bgm_cyber07.wav", -1 },			// BGM2
		{ "data/BGM/GAME/maou/陽気/maou_game_town05.wav", -1 },		// BGM3
		{ "data/BGM/RESULT/maou_bgm_acoustic10.wav", -1 },			// BGM4
		{ "data/SE/migration.wav", 0 },						// 移動音
		{ "data/SE/decision.wav", 0 },						// 決定音
		{ "data/SE/bike_running.wav", -1 },					// バイク走行音
		{ "data/SE/bike_idling.wav", -1 },					// バイクアイドリング
		{ "data/SE/cat_meow.wav", 0 },						// 猫の鳴き声
		{ "data/SE/car_running.wav", -1 },					// 車の走行音
		{ "data/SE/police_siren.wav", -1 },					// パトカーのサイレン
		{ "data/SE/car_horn.wav", 0 },						// 車のクラクション
	};
};

#endif