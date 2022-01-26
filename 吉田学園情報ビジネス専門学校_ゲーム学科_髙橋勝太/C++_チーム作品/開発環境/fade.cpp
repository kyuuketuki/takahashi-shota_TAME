//=============================================================================
//
//�t�F�[�h����[fade.cpp]
// Author; takahashi syota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "fade.h"
#include "Renderer.h"
#include "manager.h"
#include "input.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
CFade::FADE CFade::m_fade = FADE_IN;							//��Ԃ��m�[�}��
CManager::MODE CFade::m_modenext = CManager::MODE_TITLE;		//�������[�h

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CFade::CFade(int nPriority, int nType) : CScene(nPriority, nType)
{

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
//�t�F�[�h�̐ݒ�E�����֐�
//=============================================================================
CFade * CFade::Create(CManager::MODE modenext)
{
	//�C���X�^���X����
	CFade *pfade = NULL;
	pfade = new CFade;

	//�������Ăяo��
	pfade->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH);

	//�J�ڐ��m�点��
	m_modenext = modenext;

	return pfade;
}
//=============================================================================
//�t�F�[�h�̏���������
//=============================================================================
HRESULT CFade::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	m_fade = FADE_IN;//�t�F�[�h�̏��
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);//�t�F�[�h�F

	//���_�o�b�t�@�����b�N���A���_���̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�ʒu���̑��
	m_pos = D3DXVECTOR3(pos);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0, fHight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(fWidth, fHight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fWidth, 0, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//�t�F�[�h�̏I������
//=============================================================================
void CFade::Uninit(void)
{
	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
//�t�F�[�h�̍X�V����
//=============================================================================
void CFade::Update(void)
{
	//�\���̂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�t�F�[�h�C���̎�
	if (m_fade == FADE_IN)
	{
		m_col.a -= 0.01f;
		if (m_col.a <= 0.0f)
		{
			m_col.a = 0.0f;
			m_fade = FADE_NONE;
		}
	}
	//�t�F�[�h�A�E�g�̎�
	if (m_fade == FADE_OUT)
	{
		m_col.a += 0.01f;
		if (m_col.a >= 1.0f)
		{
			m_col.a = 1.0f;
			m_fade = FADE_IN;
			CManager::SetMode(m_modenext);
		}
	}

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
//�t�F�[�h�̕`�揈��
//=============================================================================
void CFade::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
}

//=============================================================================
//�t�F�[�h�̃Z�b�g�֐�
//=============================================================================
void CFade::SetFade(FADE fade, CManager::MODE modeNext)
{
	m_fade = fade;
	m_modenext = modeNext;
}
