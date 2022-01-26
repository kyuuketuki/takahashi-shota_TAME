//=============================================================================
//
// �i���o�[���� [number.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "number.h"
#include "manager.h"
#include "scene2D.h"

// �ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CNumber::CNumber()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
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
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	// �Ίp���̐ݒ�
	m_fLength = sqrtf(fSizeX * fSizeX + fSizeY * fSizeY);
	m_fAngle = atan2f(fSizeX, fSizeY);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�ʒu�̐ݒ�
	//���_�̍��W
	pVtx[0].pos.x = (pos.x - sinf(m_fAngle) * m_fLength);
	pVtx[0].pos.y = (pos.y + cosf(m_fAngle) * m_fLength);
	pVtx[0].pos.z = (pos.z = 0.0f);

	pVtx[1].pos.x = (pos.x - sinf(m_fAngle) * m_fLength);
	pVtx[1].pos.y = (pos.y - cosf(m_fAngle) * m_fLength);
	pVtx[1].pos.z = (pos.z = 0.0f);

	pVtx[2].pos.x = (pos.x + sinf(m_fAngle) * m_fLength);
	pVtx[2].pos.y = (pos.y + cosf(m_fAngle) * m_fLength);
	pVtx[2].pos.z = (pos.z = 0.0f);

	pVtx[3].pos.x = (pos.x + sinf(m_fAngle) * m_fLength);
	pVtx[3].pos.y = (pos.y - cosf(m_fAngle) * m_fLength);
	pVtx[3].pos.z = (pos.z = 0.0f);

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

	//���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CNumber::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CNumber::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CNumber::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);
	
	// �|���S���̕`�揈��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �ԍ��ɍ��킹�ăe�N�X�`�������炷
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.1f*nNumber, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f*nNumber, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f*(nNumber + 1), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f*(nNumber + 1), 0.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �ԍ��|���S���̐���
//=============================================================================
CNumber * CNumber::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ���[�J���ϐ��錾
	CNumber *pNumber = NULL;
	pNumber = new CNumber;

	// ������
	if (pNumber != NULL)
	{
		pNumber->Init(pos, fSizeX, fSizeY);
	}

	return pNumber;
}

//=============================================================================
// �e�N�X�`����ǂݍ���
//=============================================================================
HRESULT CNumber::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.jpg", &m_pTexture);
	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CNumber::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}