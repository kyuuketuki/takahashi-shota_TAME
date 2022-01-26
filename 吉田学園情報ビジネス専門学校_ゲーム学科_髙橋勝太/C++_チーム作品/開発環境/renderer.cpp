//=============================================================================
//
// �����_���[���� [renderer.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "renderer.h"
#include "scene.h"
#include "sound.h"
#include "manager.h"
#include "debugproc.h"
#include "player3D.h"
#include "camera.h"
#include "destination.h"
#include "camera.h"
#include "game.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CDebugProc *CRenderer::m_pDebug = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRenderer::CRenderer()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRenderer::~CRenderer()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;										// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;							// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil = TRUE;							// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;										// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice
	)))
	{

		if (FAILED(m_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice
		)))
		{

			if (FAILED(m_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice
			)))
			{
				return E_FAIL;
			}
		}
	}

	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// �������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �k�����ɕ��
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �g�厞�ɕ��
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// U�l�̌J��Ԃ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// V�l�̌J��Ԃ�

																			// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // ���l�̍���
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

	LPDIRECT3DSURFACE9 pRenderDef, pBuffDef;

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//�����_�����O�^�[�Q�b�g�p�̃e�N�X�`���̐���
		m_pD3DDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTextureMT[nCnt], NULL);

		//�e�N�X�`�������_�����O�p�C���^�[�t�F�[�X�̐���
		m_pTextureMT[nCnt]->GetSurfaceLevel(0, &m_pRenderMT[nCnt]);

		//�e�N�X�`�������_�����O�pZ�o�b�t�@�̐���
		m_pD3DDevice->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pBuffMT[nCnt], NULL);

		//���݂̃����_�����O�^�[�Q�b�g���擾
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		//���݂�Z�o�b�t�@���擾
		m_pD3DDevice->GetDepthStencilSurface(&pBuffDef);

		//�����_�����O�^�[�Q�b�g��ݒ�
		m_pD3DDevice->SetRenderTarget(0, m_pRenderMT[nCnt]);

		//Z�o�b�t�@��ݒ�
		m_pD3DDevice->SetDepthStencilSurface(m_pBuffMT[nCnt]);

		//�e�N�X�`��0���N���A����
		m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		//���݂̃����_�����O�^�[�Q�b�g��߂�
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		//���݂�Z�o�b�t�@��߂�
		m_pD3DDevice->SetDepthStencilSurface(pBuffDef);
	}

	//�r���[�|�[�g��ݒ�
	m_ViewportMT.X = NULL;
	m_ViewportMT.Y = NULL;
	m_ViewportMT.Width = SCREEN_WIDTH;
	m_ViewportMT.Height = SCREEN_HEIGHT;
	m_ViewportMT.MinZ = 0.0f;
	m_ViewportMT.MaxZ = 1.0f;

	// ���_�o�b�t�@�̐���
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,							// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuffMT,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);

	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);

	// �e�N�X�`�����_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffMT->Unlock();

	//�f�o�b�N�̐ݒ�
	m_pDebug = new CDebugProc;
	m_pDebug->Init();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̔j��
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif

	// �f�o�C�X�̔j��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{

		if (m_pTextureMT != NULL)
		{
			m_pTextureMT[nCnt]->Release();
			m_pTextureMT[nCnt] = NULL;
		}

		if (m_pRenderMT != NULL)
		{
			m_pRenderMT[nCnt]->Release();
			m_pRenderMT[nCnt] = NULL;
		}

		if (m_pBuffMT != NULL)
		{
			m_pBuffMT[nCnt]->Release();
			m_pBuffMT[nCnt] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CRenderer::Update(void)
{
	CScene::UpdateAll();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CRenderer::Draw(void)
{
	CManager *pManager = GetManager();

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(176, 224, 230, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// ���[�J���ϐ��錾
		D3DVIEWPORT9 viewportDef;

		for (int nCamera = 0; nCamera < 2; nCamera++)
		{
			// ���݂̃r���[�|�[�g���擾
			m_pD3DDevice->GetViewport(&viewportDef);

			//�J�����̐ݒu
			CCamera *pCamera = CManager::GetCamera();
			pCamera->SetCamera(nCamera);

			// �|���S���̕`�揈��
			CScene::DrawAll();

			// �r���[�|�[�g��߂�
			m_pD3DDevice->SetViewport(&viewportDef);
		}

#ifdef _DEBUG
		// FPS�\��
		CRenderer::DrawFPS();

		//BGM
		switch (CGame::GetPlayBGM())
		{
		case 0:
			m_pDebug->Print(1, "BGM�̏��:�ȏI��\n");
			break;
		case 1:
			m_pDebug->Print(1, "BGM�̏��:�Đ���\n");
			break;
		case 2:
			m_pDebug->Print(1, "BGM�̏��:�ꎞ��~\n");
			break;
		default:
			break;
		}

		m_pDebug->Print(1, "���݂�BGM����:%f��\n", CGame::GetVolumeGameBGM() * 100);

		m_pDebug->Print(4, "���f���̈ʒuX:%f\n���f���̈ʒuY:%f\n���f���̈ʒuZ:%f\n\n�J�ډ�%d\n"
			, CPlayer3D::GetPosition().x, CPlayer3D::GetPosition().y, CPlayer3D::GetPosition().z, CManager::GetNumber());
#endif

		//�Q�[����ʒ��ɕ\��
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			//���X�g�\�@����
			m_pDebug->Print(2, "%ds\n\n%ds\n\n%ds\n\n"
				, CDestination::GetLimit(0), CDestination::GetLimit(1), CDestination::GetLimit(2));

			//���X�g�\�@����
			m_pDebug->Print(3, "%dm\n\n%dm\n\n%dm\n\n"
				, CDestination::GetRange(0) / 100, CDestination::GetRange(1) / 100, CDestination::GetRange(2) / 100);
		}

		m_pDebug->Draw();

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		m_pD3DDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void CRenderer::ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU)
{
	//�L���p
	D3DXMATRIX mtxView, mtxProjection;
	float fAspert;

	//�����_���[��ݒu
	m_pD3DDevice->SetRenderTarget(0, m_pRenderMT[0]);

	//�o�b�t�@��ݒ�
	m_pD3DDevice->SetDepthStencilSurface(m_pBuffMT[0]);

	//�r���[�|�[�g��ݒu
	m_pD3DDevice->SetViewport(&m_ViewportMT);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&mtxView, &posV, &posR, &vecU);

	//�r���[�}�g���b�N�X�̐���
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxProjection);

	fAspert = ((float)m_ViewportMT.Width) / (float)m_ViewportMT.Height;

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(
		&mtxProjection,
		D3DXToRadian(45.0f),													// ��p�̐ݒ�
		fAspert,																// ��ʔ䗦�̐ݒ� �r���[�|�[�g�̕�����		
		10.0f,																	// ��O���E�̕`�拗��
		1000.0f);																// �����E�̕`�拗��

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

//=============================================================================
// FPS�Ɋւ��鏈��
//=============================================================================
void CRenderer::DrawFPS()
{
#ifdef _DEBUG
	int nCountFPS = GetFPS();
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];

	//��ʂɕ\��
	wsprintf(str, "FPS:%d\n", nCountFPS);

	// �e�L�X�g�`��
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
#endif
}
