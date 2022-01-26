//=============================================================================
//
// �A�j���[�V�������� [animasion.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _ANIMASION_H_
#define _ANIMASION_H_

#include "main.h"
#include "scene2D.h"

//�}�N����`
#define MAX_ANIME (3)

//=============================================================================
// ���j�N���X�̒�`
//=============================================================================

//�p����`��N���X
class CAnimasion : public CScene2D
{
public:
	CAnimasion(int nPriority = 0, int nType = 0);			//�R���X�g���N�^
	virtual ~CAnimasion();									//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nAnime, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CAnimasion *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nAnime, int nType);
	static HRESULT Load(void);
	static void Unload(void);

	static void UninitType(int nUninitType);
	static void UninitTypeClear(void);

protected:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_ANIME];	//�e�N�X�`���̃|�C���^
	static int m_nUninitType;							//�r���j���Ɏg�p
	D3DXVECTOR3 m_pos;									//�ړ���
	int m_nType;										//�^�C�v�@�e�N�X�`������
	int m_nTypeBuulet;									//����ړ����Ɏg�p
	int m_nCounterAnime;								//�A�j���[�V�����J�E���^�[
	int m_nPatternAnime;								//�A�j���[�V�����p�^�[��
	int m_fCriss;										//���[�v����
	float m_fAnimeX;									//�摜����X
	float m_fAnimeY;									//�摜����Y
	int m_nX;											//�摜����
	int m_nY;											//�摜����
	int m_nAnime;										//�A�j���[�V�����X�s�[�h
	float m_fColorFa = 1.0f;							//�A���t�@�l
};

//=============================================================================
// �A�j���[�V�����N���X�̒�`(�w�i)
//=============================================================================
class CAnimasionBg : public CAnimasion
{
public:
	CAnimasionBg(int nPriority = 1, int nType = 0);		//�R���X�g���N�^
	virtual ~CAnimasionBg();							//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nAnime, int nType);					   //����������
	void Uninit(void);																					   //�I������
	void Update(void);																					   //�X�V����
	void Draw(void);																					   //�`�揈��

	static CAnimasionBg *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nAnime, int nType);	   //��������
	static void UninitTypeClear(void);																	   //�r���j��
};

#endif