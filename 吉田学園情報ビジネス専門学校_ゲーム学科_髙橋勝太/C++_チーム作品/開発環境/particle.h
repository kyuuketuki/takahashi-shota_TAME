//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : Miura�@Yudai
//
//=============================================================================

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
// �C���N���[�h�t�@�C���ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene3D.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define DATA_LENGTH (64)

//=============================================================================
// �p�[�e�B�N���N���X�̒�`
//=============================================================================
class CParticle : public CScene3D
{
public:
	CParticle(int nPriority = 0, int nType = 0);								//�R���X�g���N�^
	virtual ~CParticle();														//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fX, float fY, float fSpeed, D3DXCOLOR col, int nLife, float fGravity);			//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	static CParticle *Create(D3DXVECTOR3 pos, float fX, float fY, float fSpeed, D3DXCOLOR col, int nLife, float fGravity);//��������

	static void Load(void);                                                 //�e�N�X�`�����[�h����
	void SetParticle(D3DXVECTOR3 pos, float fX, float fY, float fSpeed, D3DXCOLOR col, int nLife, float fGravity);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`�����
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_move;
	float m_fAngle;
	int m_nType;

	//�e�L�X�g����ǂݍ��ݗp�����o�ϐ�
	int m_nCountText;                  //�p�[�e�B�N���̐�
	float m_SizeX;                     //�p�[�e�B�N���̃T�C�Y
	float m_SizeY;                     //�p�[�e�B�N���̃T�C�Y
	float m_fSpeed;                    //�p�[�e�B�N���̃X�s�[�h
	D3DXCOLOR m_col;                   //�p�[�e�B�N���̃J���[
	int m_nLife;                       //�p�[�e�B�N���̎���
	float m_fGravity;                  //�p�[�e�B�N���̏d��
	int m_nLifeGravity;                //�p�[�e�B�N���̏d�͔����p�ϐ�
};
#endif
