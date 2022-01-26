//=============================================================================
//
// ���U���g��ʏ��� [game2.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _RESULT_H_
#define _RESULT_H_

//==============================================================================
//�C���N���[�h
//==============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "number.h"

//=============================================================================
// ���U���g��ʃN���X�̒�`
//=============================================================================
class CResult : public CScene
{
public:
	CResult();									//�R���X�g���N�^
	~CResult();									//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//����������
	void Uninit(void);												//�I������
	void Update(void);												//�X�V����
	void Draw(void);												//�`�揈��

	static CResult *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//��������

private:
	LPD3DXFONT			m_pTimeFontGame;		// �\���p�t�H���g
	CNumber				*m_apNumber[5][8];		//����
	int					m_nRankintData[5];		//�����L���O�f�[�^�ۊ�
	
	//�ÓI�����o�ϐ�
	static CScore		*m_pScore[5];					//�A�h���X�ۑ��p�|�C���^
};

#endif