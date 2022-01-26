//=============================================================================
//
// �A�C�e������ [item.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _ITEM_H_
#define _ITEM_H_

//==============================================================================
//�C���N���[�h
//==============================================================================
#include "main.h"
#include "player.h"
#include "scene2D.h"

//�}�N����`
#define ITEM_SIZE (70)
#define ITEM_TYPE (2)

//=============================================================================
// �A�C�e���N���X�̒�`
//=============================================================================
class CItem : public CScene2D
{
public:
	CItem();								//�R���X�g���N�^
	virtual ~CItem();						//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fmove, float fSizeX, float fSizeY, int nType);	//����������
	void Uninit(void);																	//�I������
	void Update(void);																	//�X�V����
	void Draw(void);																	//�`�揈��

	void Animasion(void);		//�A�j���V��������

	//��������
	static CItem *Create(D3DXVECTOR3 pos, float fmove, float fSizeX, float fSizeY, int nType);
	static void Heart(D3DXVECTOR3 pos, float fmove, float fSizeX, float fSizeY, int nType);
	static void Heart2(D3DXVECTOR3 pos, float fmove, float fSizeX, float fSizeY, int nType, int nType2);

	//�e�N�X�`���֘A
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[ITEM_TYPE];			//�e�N�X�`���ւ̃|�C���^

	D3DXVECTOR3					m_pos;							//�ʒu
	int							m_nType;						//�G�̎�ނ��^�C�v�ŊǗ�
	float						m_fMove;						//�ړ��X�s�[�h
	
	//�A�j���[�V�����֘A
	int							m_nCounterAnime;				//�A�j���[�V�����J�E���^�[
	int							m_nPatternAnime;				//�A�j���[�V�����p�^�[��
	float						m_fAnimeX;						//�T�C�Y
	float						m_fAnimeY;						//�T�C�Y
	int							m_nX;							//�T�C�Y
	int							m_nY;							//�T�C�Y
};															

#endif