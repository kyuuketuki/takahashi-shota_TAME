//=============================================================================
//
// �V�[��2D���� [scene.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "scene2d.h"
#include "renderer.h"
#include "manager.h"
#include "sceneX.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(int nPriority, int nType) : CScene(nPriority, nType)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * 2,						// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuffPolygon,
		NULL)))
	{
		return E_FAIL;
	}

	//�|���S���̈ʒu
	m_pos = pos;

	//�T�C�Y�����
	CScene2D::SetSize(fSizeX, fSizeY);
	
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//�ʒu�̐ݒ�
	//���_�̍��W
	pVtx[0].pos.x = (m_pos.x - sinf(m_fAngle) * m_fLength);
	pVtx[0].pos.y = (m_pos.y + cosf(m_fAngle) * m_fLength);
	pVtx[0].pos.z = (m_pos.z = 0.0f);

	pVtx[1].pos.x = (m_pos.x - sinf(m_fAngle) * m_fLength);
	pVtx[1].pos.y = (m_pos.y - cosf(m_fAngle) * m_fLength);
	pVtx[1].pos.z = (m_pos.z = 0.0f);

	pVtx[2].pos.x = (m_pos.x + sinf(m_fAngle) * m_fLength);
	pVtx[2].pos.y = (m_pos.y + cosf(m_fAngle) * m_fLength);
	pVtx[2].pos.z = (m_pos.z = 0.0f);

	pVtx[3].pos.x = (m_pos.x + sinf(m_fAngle) * m_fLength);
	pVtx[3].pos.y = (m_pos.y - cosf(m_fAngle) * m_fLength);
	pVtx[3].pos.z = (m_pos.z = 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffPolygon->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScene2D::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuffPolygon != NULL)
	{
		m_pVtxBuffPolygon->Release();
		m_pVtxBuffPolygon = NULL;
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScene2D::Update(void)
{
	VERTEX_2D *pVtx;

	// ���_���̍X�V
	if (m_pVtxBuffPolygon != NULL)
	{
		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

		//�ʒu�X�V
		m_pos.x += m_move.x;
		m_pos.y += m_move.y;
		m_rot.z += m_move.z;

		//�ړ��ʂ̌���
		m_move.x += (0 - m_move.x) * 0.2f;
		m_move.y += (0 - m_move.y) * 0.2f;
		m_move.z += (0 - m_move.z) * 0.5f;

		//���_�̍��W
		pVtx[0].pos.x = (m_pos.x - sinf(m_fAngle + m_rot.z) * m_fLength);
		pVtx[0].pos.y = (m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength);
		pVtx[0].pos.z = (m_pos.z = 0.0f);

		pVtx[1].pos.x = (m_pos.x - sinf(m_fAngle - m_rot.z) * m_fLength);
		pVtx[1].pos.y = (m_pos.y - cosf(m_fAngle - m_rot.z) * m_fLength);
		pVtx[1].pos.z = (m_pos.z = 0.0f);

		pVtx[2].pos.x = (m_pos.x + sinf(m_fAngle - m_rot.z) * m_fLength);
		pVtx[2].pos.y = (m_pos.y + cosf(m_fAngle - m_rot.z) * m_fLength);
		pVtx[2].pos.z = (m_pos.z = 0.0f);

		pVtx[3].pos.x = (m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength);
		pVtx[3].pos.y = (m_pos.y - cosf(m_fAngle + m_rot.z) * m_fLength);
		pVtx[3].pos.z = (m_pos.z = 0.0f);

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuffPolygon->Unlock();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene2D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	////�A���t�@�e�X�g
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, 0);
	
	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`�揈��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	////�A���t�@�e�X�g
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
}

//=============================================================================
// �X�V���ꂽ�ʒu�ւ̃|�C���^
//=============================================================================
void CScene2D::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// �|���S���̉�]�ݒ�
//=============================================================================
void CScene2D::SetRevolution(float nRot)
{
	m_rot.z = nRot;
}

//=============================================================================
// �e�N�X�`���ւ̃|�C���^
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// �e�N�X�`���@�A�j���[�V����
//=============================================================================
void CScene2D::SetTex(float fPageX, float fPageY, int nAnime)
{
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f + (fPageX * nAnime), fPageY + (fPageY * nAnime));
	pVtx[1].tex = D3DXVECTOR2(0.0f + (fPageX * nAnime), (fPageY * nAnime));
	pVtx[2].tex = D3DXVECTOR2(fPageX + (fPageX * nAnime), fPageY + (fPageY * nAnime));
	pVtx[3].tex = D3DXVECTOR2(fPageX + (fPageX * nAnime), (fPageY * nAnime));

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// �A�j���[�V����XY�v�Z
//=============================================================================
void CScene2D::SetAnime(float fPageX, float fPageY, int nX, int nY, int nAnime)
{
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//X�̌v�Z
	if ((nX - 1) == nAnime)
	{
		m_nCountX += nX;
		
	}

	m_nAnimeX = nAnime - m_nCountX;

	//Y�̌v�Z
	if (nX != 0)
	{
		if (nAnime % nX == 0)
		{
			m_nCountY++;
		}
	}

	m_nAnimeY = m_nCountY;

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f + (fPageX * m_nAnimeX), fPageY + (fPageY * m_nAnimeY));
	pVtx[1].tex = D3DXVECTOR2(0.0f + (fPageX * m_nAnimeX), 0.0f + (fPageY * m_nAnimeY));
	pVtx[2].tex = D3DXVECTOR2(fPageX + (fPageX * m_nAnimeX), fPageY + (fPageY * m_nAnimeY));
	pVtx[3].tex = D3DXVECTOR2(fPageX + (fPageX * m_nAnimeX), 0.0f + (fPageY * m_nAnimeY));

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// �w�i�̃A�j���[�V����
//=============================================================================
void CScene2D::SetBg(float nAnime)
{
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, nAnime + 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, nAnime);
	pVtx[2].tex = D3DXVECTOR2(1.0f, nAnime + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, nAnime);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// �|���S���̓����x(effect)
//=============================================================================
void CScene2D::SetEffect(D3DXCOLOR col,float nData)
{
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// �|���S���̃J���[�ݒ�
//=============================================================================
void CScene2D::SetCollar(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// �|���S���̃T�C�Y�ݒ�
//=============================================================================
void CScene2D::SetSize(float fSizeX, float fSizeY)
{
	// �Ίp���̐ݒ�
	m_fLength = sqrtf(fSizeX * fSizeX + fSizeY * fSizeY);
	m_fAngle = atan2f(fSizeX, fSizeY);

	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̍��W
	pVtx[0].pos.x = (m_pos.x - sinf(m_fAngle) * m_fLength);
	pVtx[0].pos.y = (m_pos.y + cosf(m_fAngle) * m_fLength);
	pVtx[0].pos.z = (m_pos.z = 0.0f);

	pVtx[1].pos.x = (m_pos.x - sinf(m_fAngle) * m_fLength);
	pVtx[1].pos.y = (m_pos.y - cosf(m_fAngle) * m_fLength);
	pVtx[1].pos.z = (m_pos.z = 0.0f);

	pVtx[2].pos.x = (m_pos.x + sinf(m_fAngle) * m_fLength);
	pVtx[2].pos.y = (m_pos.y + cosf(m_fAngle) * m_fLength);
	pVtx[2].pos.z = (m_pos.z = 0.0f);

	pVtx[3].pos.x = (m_pos.x + sinf(m_fAngle) * m_fLength);
	pVtx[3].pos.y = (m_pos.y - cosf(m_fAngle) * m_fLength);
	pVtx[3].pos.z = (m_pos.z = 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffPolygon->Unlock();
}
