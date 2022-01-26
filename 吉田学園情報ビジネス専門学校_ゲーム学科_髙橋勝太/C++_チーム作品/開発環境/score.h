//=============================================================================
//
// �X�R�A���� [score.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _SCORE_H_
#define _SCORE_H_

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "number.h"
#include "scene.h"

//=============================================================================
// �X�R�A�N���X�̒�`
//=============================================================================
class CScore : public CScene
{
public:
	CScore();								//�R���X�g���N�^
	~CScore();								//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//����������
	void Uninit(void);												//�I������
	void Update(void);												//�X�V����
	void Draw(void);												//�`�揈��

	//�l�ύX
	static void SetScore(int nScore);
	
	//���Z����
	static void AddScore(int nValue);	

	//��������
	static CScore *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);

	static int GetScore(void);

private:
	static CNumber *m_apNumber[8];			//����
	static int m_nScore;					//�X�R�A�̒l
};

#endif