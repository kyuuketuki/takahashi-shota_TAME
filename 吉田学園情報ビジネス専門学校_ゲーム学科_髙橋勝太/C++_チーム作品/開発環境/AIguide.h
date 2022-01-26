//=============================================================================
//
// �V�[��X�@AI�K�C�h�@���� [AIguide.h]
// Author : katano hiroya(���\�[�X:takahashi shota
//
//=============================================================================

#ifndef _AIguide_H_
#define _AIguide_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#include "main.h"
#include "scene.h"
#include "sceneX.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MAX_GUIDE_AREA	(4)			// AI�K�C�h��ݒu����G���A�̍ő吔
#define MAX_GUIDE_POINT (52)		// �P�G���A���Ƃ̍ő�K�C�h�|�C���g��

//==============================================================================
// �G���A�̎��
//==============================================================================
typedef enum
{
	GUIDEAREATYPE_NEUTRAL = 0,	// �j���[�g����
	GUIDEAREATYPE_CITY,			// �X
	GUIDEAREATYPE_PARK,			// ����
	GUIDEAREATYPE_BILL,			// �r���Q
	GUIDEAREATYPE_MAX			// AI�K�C�h�ݒu�G���A�̍ő吔
} GUIDEAREATYPE;

//=============================================================================
// �ړI�n�N���X�̒�`
//=============================================================================
//�p����`��N���X
class CAIguide : public CSceneX
{
public:
	CAIguide(int nPriority = 5, int nType = 0);						//�R���X�g���N�^
	virtual ~CAIguide();											//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);			//����������
	void Uninit(void);													//�I������
	void Update(void);													//�X�V����
	void Draw(void);													//�`�揈��

	static CAIguide *Create(D3DXVECTOR3 pos, char *TextName);			//��������
	static CAIguide *CreateSave(D3DXVECTOR3 pos, char *TextName);		//��������

	void LoadAIguide(void);												//�O���ǂݍ���
	static D3DXVECTOR3 *GetAIguidePos(int nArea, int nPoint);


	static D3DXVECTOR3		m_AIguide[MAX_GUIDE_AREA][MAX_GUIDE_POINT];			//�e���[�V�����̈ʒu�̋L���ꏊ

private:
	D3DXVECTOR3		m_pos;				//�ʒu
	D3DXVECTOR3		m_rot;				//����
	static char		*m_pTextName;		//�e�L�X�g���O

	int						m_NumArea[GUIDEAREATYPE_MAX];						//AI�K�C�h�̍ő�ݒu�G���A�����L��
	int						m_PointSet[GUIDEAREATYPE_MAX];						//�ő�K�C�h�|�C���g�����L��
	
};

#endif
