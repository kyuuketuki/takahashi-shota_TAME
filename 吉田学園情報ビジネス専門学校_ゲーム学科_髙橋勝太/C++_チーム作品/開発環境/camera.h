//=============================================================================
//
// �J�������� [camera.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"
#include "manager.h"

//=============================================================================
// �e�N���X�̒�`
//=============================================================================

//�p����`��N���X
class CCamera
{
public:
	CCamera();								// �R���X�g���N�^
	virtual ~CCamera();						// �f�X�g���N�^

	HRESULT Init(void);						// ����������
	void Uninit(void);						// �I������
	void Update(void);						// �X�V����
	void Draw(void);						// �`�揈��

	void SetCamera(int nIdx);					// ��������
	void Mouseoperation(void);				// �}�E�X���쏈��

	static float GetMousepos(void);

	static D3DXVECTOR3 GetRot(void) { return m_rot; }		// �����擾�p

private:
	D3DXMATRIX m_mtxProjection[2];		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView[2];			// �r���[�}�g���b�N�X	
	D3DXVECTOR3 m_posVDest;				// �ړI�̎��_	
	D3DXVECTOR3 m_posRDest;				// �ړI�̒����_
	D3DXVECTOR3 m_vecU[2];				// ������x�N�g��
	int m_Rpos;							// ���f���̑O
	float m_fDistance;					// �J�����Ƃ̋���
	float m_fMouseSpeed;				// �J������]���x�i�}�E�X����j
	POINT m_Mousepoint;					// �}�E�X���W
	static float m_fhorizontalAngle;	// ����
	float m_fverticalAngle;				// ����

	D3DXVECTOR3 m_posV[2];				// ���݂̎��_
	D3DXVECTOR3 m_posR[2];				// ���݂̒����_
	static D3DXVECTOR3 m_rot;			// �J�����̊p�x
	D3DVIEWPORT9 m_viewport[2];			//�r���[�|�[�g
};
#endif