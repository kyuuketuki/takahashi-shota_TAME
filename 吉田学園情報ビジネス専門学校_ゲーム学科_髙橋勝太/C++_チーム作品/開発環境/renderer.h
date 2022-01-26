//=============================================================================
//
// �����_���[���� [renderer.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _RENDERE_H_
#define _RENDERE_H_

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"

//�O���錾
class CDebugProc;

//=============================================================================
// �����_���[�N���X�̒�`
//=============================================================================
class CRenderer
{
public:
	CRenderer();								//�R���X�g���N�^
	~CRenderer();								//�f�X�g���N�^
	HRESULT Init(HWND hWnd, bool bWindow);						//����������
	void Uninit(void);											//�I������
	void Update(void);											//�X�V����
	void Draw(void);											//�`�揈��
	void Draw2(void);											//�`�揈��(�c������)

	//�f�o�C�X���
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

	//�}���`�^�[�Q�b�g�֐�
	void ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU);
	LPDIRECT3DTEXTURE9 GetTextureMT(void) { return m_pTextureMT[0]; }

private:
	void DrawFPS();

	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffMT;				//�o�b�t�@
	LPDIRECT3D9					m_pD3D = NULL;				// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9			m_pD3DDevice = NULL;		// Device�I�u�W�F�N�g(�`��ɕK�v)
	LPD3DXFONT					m_pFont = NULL;				// �t�H���g�ւ̃|�C���^

	//�}���`�^�[�Q�b�g�ϐ�
	LPDIRECT3DTEXTURE9 m_pTextureMT[2];
	LPDIRECT3DSURFACE9 m_pRenderMT[2];
	LPDIRECT3DSURFACE9 m_pBuffMT[2];
	D3DVIEWPORT9 m_ViewportMT;

	//�ÓI�����o�ϐ�
	static CDebugProc *m_pDebug;							//�f�o�b�N�\���̃����o�ϐ�
};

#endif
