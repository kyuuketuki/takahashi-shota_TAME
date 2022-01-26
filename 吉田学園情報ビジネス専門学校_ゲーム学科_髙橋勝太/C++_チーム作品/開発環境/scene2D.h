//=============================================================================
//
// �V�[��2D���� [scene2d.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _SCENE2D_H_
#define _SCENE2D_H_
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �V�[��2D�N���X�̒�`
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
	D3DXVECTOR3 g_rotPolygon;
} VERTEX_2D;

//�p����`��N���X
class CScene2D : public CScene
{
public:
	CScene2D(int nPriority = 3, int nType = 0);								//�R���X�g���N�^
	virtual ~CScene2D();													//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos , float fSizeX, float fSizeY);				//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);							//�e�N�X�`���ւ̃|�C���^

	//Set�֐�
	void SetPosition(D3DXVECTOR3 pos);										//�ړ��X�V
	void SetRevolution(float nRot);											//��]�X�V
	void SetSize(float fSizeX, float fSizeY);								//�T�C�Y�X�V
	void SetBg(float nAnime);												//�w�i�ړ�
	void SetCollar(D3DXCOLOR col);											//�J���[�ύX
	void SetEffect(D3DXCOLOR col, float nData);								//�A���t�@�l��������
	void SetTex(float fPageX, float fPageY,int nAnime);						//�A�j���[�V�����X�V
	void SetAnime(float fPageX, float fPageY, int nX, int nY, int nAnime);	//�A�j���[�V�����X�V�@����

	//Get�֐�
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRevolution(void) { return m_rot; }

private:
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffPolygon = NULL;

	//�|���S���̏��
	LPDIRECT3DTEXTURE9			m_pTexture = NULL;		// �v���C���[�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3					m_pos;					//�|���S���̏����ʒu
	D3DXVECTOR3					m_rot;					//�|���S���̉�]�l
	D3DXVECTOR3					m_move;					//�|���S���̈ړ���
	float						m_fLength;				//�Ίp���̒���
	float						m_fAngle;				//�Ίp���̊p�x
	int							m_nAnime;				//�A�j���[�V�����X�s�[�h
	int m_nAnimeX;										//�T�C�Y
	int m_nAnimeY;										//�T�C�Y
	int m_nCountX;										//�v�Z�p
	int m_nCountY;										//�v�Z�p
};														

#endif