//=============================================================================
//
// �Q�[�����2���� [game2.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _GAME2_H_
#define _GAME2_H_

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "player.h"

//=============================================================================
// �Q�[����ʃN���X�̒�`
//=============================================================================
class CGame2 : public CScene
{
public:
	CGame2();									//�R���X�g���N�^
	~CGame2();									//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//����������
	void Uninit(void);											//�I������
	void Update(void);											//�X�V����
	void Draw(void);											//�`�揈��

	//�U���Ǘ�
	void Attack(void);											
	void Attack2(void);											
	void Attack3(void);											

	//����Get
	static int GetTime(void);

	//��������
	static CGame2 *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);

private:
	static CPlayer				*m_pPlayer;			//�v���C���[�ւ̃|�C���^	�z�[�~���O���Ɏg�p
	LPD3DXFONT					m_pTimeFontGame;	// �\���p�t�H���g
};

#endif