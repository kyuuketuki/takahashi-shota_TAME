//=============================================================================
//
//���b�V���t�B�[���h����[meshfield.h]
// Author; takahashi syota
//
//=============================================================================

#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXT_LENGTH			(128)	// �e�L�X�g�ǂݍ��݂̍ő吔
#define DATA_LENGTH			(64)	// �f�[�^�ǂݍ��݂̍ő吔

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "Scene.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define WIDTHNUM	(30)		// ��(��)
#define DEPTHNUM	(30)		// �s��(���s)
#define WIDTH		(100.0f)	// ���̑傫��
#define DEPTH		(100.0f)	// ���s�̑傫��
#define FIELD_MAX	(1)			// �t�B�[���h�̍ő吔

//=============================================================================
//���b�V���t�B�[���h�N���X�̒�`
//=============================================================================
class CMeshfield : public CScene
{
public:
	CMeshfield();															//�R���X�g���N�^
	~CMeshfield();															//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos,float fHight, float fWidth);				//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��
	
	static CMeshfield *Create();	//��������

	//�ʒu���`����
	D3DXVECTOR3 GetPosition(void) { return m_pos; }

	void CMeshfield::LoadObject(void);			//�e�L�X�g�̓ǂݍ���

	//�T�C�Y����`����@�v���C���[�̈ړ������Ɏg�p
	static float GetMeshFildX(void);
	static float GetMeshFildZ(void);

private:
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff = NULL;			// �o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff = NULL;			// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9			m_pTexture = NULL;			// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3					m_pos;						// �ʒu
	D3DXVECTOR3					m_rot;						// ����
	D3DXMATRIX					m_mtxWorld;					// ���[���h�}�g���b�N�X
	int							m_nAllPoint;				// �����_��
	int							m_nPolygon;					// �|���S����
	int							m_nIdxPoint;				// �C���f�b�N�X�o�b�t�@�̕K�v�Ȋm�ې�
	int							m_nWidth;					// �����̒��_��
	int							m_nDepth;					// ���s�̒��_��
	float						m_fWidth;					// �����̒[
	float						m_fDepth;					// ���s�̒[
	char						aTextureName[128];			// �e�N�X�`���ǂݍ���

	//�ÓI�����o�ϐ�
	static float				m_fX;						//�T�C�Y���L��X
	static float				m_fZ;						//�T�C�Y���L��Z
};
#endif
