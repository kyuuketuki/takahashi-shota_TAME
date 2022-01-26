//=============================================================================
//
// �M���@�@���� [TrafficLight.h]
// Author : katano hiroya
//=============================================================================

#ifndef _trafficLight_H_
#define _trafficLight_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#include "main.h"
#include "scene.h"
#include "sceneX.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MAX_LIGHT_AREA	(2)		// AI�K�C�h��ݒu����G���A�̍ő吔
#define MAX_LIGHT_POINT (14)	// �P�G���A���Ƃ̍ő�ݒu��
#define MAX_NONE_LIGHT (41)	// �P�G���A���Ƃ̍ő�ݒu��

//==============================================================================
// ���[�V�����̎��
//==============================================================================
typedef enum
{
	LIGHTAREATYPE_NEUTRAL = 0,	// �j���[�g����
	LIGHTAREATYPE_CITY,			// �X
	LIGHTAREATYPE_PARK,			// ����
	LIGHTAREATYPE_BILL,			// �r���Q
	LIGHTAREATYPE_MAX			// AI�K�C�h�ݒu�G���A�̍ő吔
} LIGHTAREATYPE;

typedef enum
{
	LIGHT_POS = 0,			// Pos
	LIGHT_ROT,				// Rot
	LIGHT_POS_OR_ROT_MAX	// Pos��Rot��
} LIGHT_POS_OR_ROT;

typedef struct
{
	D3DXVECTOR3	pos;
	D3DXVECTOR3	rot;
	bool		bSignal;
}TRAFFICLIGHT_DATA;

//=============================================================================
// �ړI�n�N���X�̒�`
//=============================================================================
class CTrafficLight : public CSceneX
{
public:
	CTrafficLight(int nPriority = 5, int nType = 0);						//�R���X�g���N�^
	virtual ~CTrafficLight();												//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSizeX, float fSizeY);			//����������
	void Uninit(void);																	//�I������
	void Update(void);																	//�X�V����
	void Draw(void);																	//�`�揈��

	static CTrafficLight *Create(D3DXVECTOR3 pos, int nArea, int nPoint);		//������������
	static CTrafficLight *CreateNone(D3DXVECTOR3 pos, char *TextName);			//������������
	static CTrafficLight *CreateSave(D3DXVECTOR3 pos, char *TextName);			//�V�K��������

	void LoadTrafficLight(void);					//�O���ǂݍ���
	void LoadNoneTrafficLight(void);				//�O���ǂݍ���

	bool GetSignal(int nArea, int nPoint) { return m_TrafficLight[nArea][nPoint].bSignal; }
	D3DXVECTOR3	GetLightPos(int nArea, int nPoint) { return m_TrafficLight[nArea][nPoint].pos; }
	D3DXVECTOR3 GetNoneLightPos(int nPoint) { return m_NoneTrafficLight[nPoint].pos; }
	float GetRot(int nArea, int nPoint) { return m_TrafficLight[nArea][nPoint].rot.y; }
	float GetRotNone(int nPoint) { return m_NoneTrafficLight[nPoint].rot.y; }

	TRAFFICLIGHT_DATA		m_TrafficLight[MAX_LIGHT_AREA][MAX_LIGHT_POINT];			//�e�f�[�^�̋L���ꏊ
	TRAFFICLIGHT_DATA		m_NoneTrafficLight[MAX_NONE_LIGHT];							//�e�f�[�^�̋L���ꏊ

private:
	D3DXVECTOR3		m_pos;								//�ʒu
	D3DXVECTOR3		m_rot;								//����
	static char		*m_pTextName;						//�e�L�X�g���O

	int				m_NumArea[LIGHTAREATYPE_MAX];		//AI�K�C�h�̍ő�ݒu�G���A�����L��
	int				m_PointSet[LIGHTAREATYPE_MAX];		//�ő�K�C�h�|�C���g�����L��
	int				m_nCntTime;


	DWORD				m_nNumMat;												//�}�g���b�N�X
	LPD3DXMESH			m_pMesh;												//���b�V��
	LPD3DXBUFFER		m_pBuffMat;												//�o�b�t�@
	D3DXVECTOR3			m_vtx;													//���_��
	LPDIRECT3DTEXTURE9	m_pTexture[MAX_LIGHT_AREA*MAX_LIGHT_POINT][2];			//�e�N�X�`���̃|�C���^

	int					m_nTypeX;					//�^�C�v
	int					m_nArea, m_nPoint;
	float				m_fRot;						//��]��

	//�ÓI�����o�ϐ�
	static D3DXMATRIX	m_mtxWorld;				//���[���h�}�g���b�N�X
};

#endif
