//=============================================================================
//
// �ԏ��� [car.h]
// Author : Hiroya Katano
//
//=============================================================================

#ifndef _CAR_H_
#define _CAR_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#include "main.h"
#include "scene.h"
#include "player3D.h"
#include "AIguide.h"
#include "trafficLight.h"
#include "model.h"

#define MAX_FOV		(3000.f)
#define SIGNAL_FOV	(1500.f)
#define AUTO_NEUTORALCAR_SPD	(10.f)
#define AUTO_SUPERCAR_SPD		(20.f)
#define PLAY_CAR_SPD		(20.f)
#define PLAY_CAR_SPDDOWN		(2.f)

enum CARAREATYPE
{
	CARAREATYPE_NEUTRAL = 0,	// �j���[�g����
	CARAREATYPE_CITY,			// �X
	CARAREATYPE_PARK,			// ����
	CARAREATYPE_BILL,			// �r���Q
	CARAREATYPE_MAX				// �ԗ����񃋁[�g�̍ő吔
};

enum CARTYPE
{
	CARTYPE_NEUTRAL = 0,	// �j���[�g����
	CARTYPE_NORMAL,			// ����
	CARTYPE_SUPER,			// �X�[�p�[�J�[
	CARTYPE_TRUCK,			// �g���b�N
	CARTYPE_POLICE,			// �p�g�J�[
	CARTYPE_MAX				// �ԗ��̎�ނ̍ő吔
};

//=============================================================================
// �|���S���N���X�̒�`
//=============================================================================
//�p����`��N���X
class CCar : public CScene
{
public:
	CCar(int nPriority = 0, int nType = 0);									//�R���X�g���N�^
	virtual ~CCar();														//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	D3DXVECTOR3	GetPosition(void) { return m_pos; }					//�ʒu�擾�p
	D3DXVECTOR3	GetRotation(void) { return m_rot; }					//�����擾�p

	static bool GetPlayCar(void) { return m_bPlayCar; }

	static CCar *Create(D3DXVECTOR3 pos, int nType, CARTYPE carType);			//��������
	bool CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);

	D3DXMATRIX GetMtxWorld(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; }

	void CalculationCar(void);

	int GetNowGuide(int NumCar)
	{
		if (m_nTypeX == NumCar) { return m_nNowGuide; }
		else { return 0; }
	}
	int
		GetNowLight(int NumCar)
	{
		if (m_nTypeX == NumCar) { return m_nNowLight; }
		else { return 0; }
	}

	float GetNowGuideAngle(int NumCar)
	{
		if (m_nTypeX == NumCar) { return m_fAngleGuide; }
		else { return 0; }
	}

private:
	DWORD				m_nNumMat;					//�}�g���b�N�X
	LPD3DXMESH			m_pMesh;					//���b�V��
	LPD3DXBUFFER		m_pBuffMat;					//�o�b�t�@
	D3DXVECTOR3			m_pos;						//�ʒu
	D3DXVECTOR3			m_move;						//�ړ���
	D3DXVECTOR3			m_rot;						//��]
	D3DXVECTOR3			m_vtx;						//���_��
	LPDIRECT3DTEXTURE9	m_pTexture[5][64];			//�e�N�X�`���̃|�C���^
	int					m_nTypeX;					//�^�C�v
	float				m_fRot;						//��]��

	//�ÓI�����o�ϐ�
	static D3DXMATRIX		m_mtxWorld;				//���[���h�}�g���b�N�X

	//�����蔻��p
	D3DXVECTOR3		m_MaxSize;				//�ő�l
	D3DXVECTOR3		m_MinSize;				//�ŏ��l
	D3DXVECTOR3		m_aPos[4];				//�l���_(�����蔻��)
	D3DXVECTOR3		m_Vec[4];				//�x�N�g���ۑ�
	D3DXVECTOR3		m_posMaxBase;			//�ő�l
	D3DXVECTOR3		m_posMinBase;			//�ŏ��l

	//�ÓI�����o�ϐ�
	static D3DXVECTOR3	m_rotStatic;		//��]��
	static D3DXVECTOR3	m_moveStatic;		//�ړ���

	//�K�C�h�p
	int m_nNowGuide;
	int m_nNowLight;

	float m_fDestAngle;

	float m_fVecGuideX;
	float m_fVecGuideZ;
	float m_fDisGuideZX;
	float m_fAngleGuide;

	float m_fVecLightX;
	float m_fVecLightZ;
	float m_fDisLightZX;
	float m_fAngleLight;

	float m_fVecPlayerX;
	float m_fVecPlayerZ;
	float m_fDisPlayerZX;
	float m_fAnglePlayer;

	bool m_bHoming;

	bool m_bSignalRed;
	bool m_bPlayerVio;
	bool m_bHearCar;
	bool m_bHearSiren;

	CARTYPE m_CarType;

	D3DXVECTOR3		*m_AIguide[MAX_GUIDE_AREA][MAX_GUIDE_POINT];		//�e�K�C�h�̈ʒu���L��
	D3DXVECTOR3		*m_Light[MAX_LIGHT_AREA][MAX_LIGHT_POINT];			//�e�M���@�̈ʒu���L��

	//�ÓI�����o�ϐ�
	static bool m_bPlayCar;
};

#endif