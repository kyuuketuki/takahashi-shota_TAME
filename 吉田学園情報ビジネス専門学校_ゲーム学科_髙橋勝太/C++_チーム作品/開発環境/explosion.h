//=============================================================================
//
// �������� [explosion.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
// ���j�N���X�̒�`
//=============================================================================
class CExplosion : public CScene2D
{
public:
	CExplosion();								//�R���X�g���N�^
	virtual ~CExplosion();						//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);							//����������
	void Uninit(void);																	//�I������
	void Update(void);																	//�X�V����
	void Draw(void);																	//�`�揈��

	static CExplosion *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//��������
	static HRESULT Load(void);															//�e�N�X�`���ǂݍ���
	static void Unload(void);															//�e�N�X�`���j��

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`���̃|�C���^
	int m_nCounterAnime;					//�A�j���[�V�����J�E���^�[
	int m_nPatternAnime;					//�A�j���[�V�����p�^�[��
};

#endif