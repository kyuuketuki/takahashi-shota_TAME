//==============================================================================
//
// �V�[��3D����[Visualization.h]
// Author; takahashi syota
//
//==============================================================================

#ifndef _VISUALIZATION_H_
#define _VISUALIZATION_H_
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//==============================================================================
//�C���N���[�h�ǂݍ���
//==============================================================================
#include "main.h"
#include "Scene.h"

//==============================================================================
//�V�[��3D�N���X�̒�`
//==============================================================================
class CVisualization : public CScene
{
public:
	CVisualization();			//�R���X�g���N�^
	~CVisualization();			//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);      //����������
	void Uninit(void);												//�I������
	void Update(void);												//�X�V����
	void Draw(void);												//�`�揈��

																	//��������
	static CVisualization *Create(D3DXVECTOR3 pos, D3DXVECTOR3 fMaxSizeX, D3DXVECTOR3 fMinSizeX, D3DXVECTOR3 fMaxSizeY, D3DXVECTOR3 fMinSizeY, bool bLife, D3DXCOLOR col);

	//Get�֐�
	D3DXVECTOR3 GetPosition(void) { return m_pos; }			//�ʒu����`����

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_���
	LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`�����
	D3DXMATRIX m_mtxWorld;					//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;						//�ʒu���
	D3DXVECTOR3 m_Rot;						//��]���
	int			m_nLife;

	//�ÓI�����o�ϐ�
	static D3DXVECTOR3	size[4];				//4���_���L��
	static D3DXCOLOR	m_col;					//�F���
	static bool			m_bLife;				//4���_���L��
};
#endif
