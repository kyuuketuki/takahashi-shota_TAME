//=============================================================================
//
// �ԏ��� [car.cpp]
// Author : Hiroya Katano
//
//=============================================================================
#include "car.h"
#include "manager.h"
#include "player3D.h"
#include "input.h"
#include "camera.h"
#include "AIguide.h"
#include "trafficLight.h"
#include "player3D.h"
#include "scene3D.h"

//�ÓI�����o�ϐ�
D3DXVECTOR3		CCar::m_rotStatic = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3		CCar::m_moveStatic = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXMATRIX      CCar::m_mtxWorld;
bool			CCar::m_bPlayCar;

//�O���[�o���ϐ�
int			g_nTypeObjectX = -1;
int			g_nCarNumber = 0;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CCar::CCar(int nPriority, int nType)
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CCar::~CCar()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CCar::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//X�t�@�C���̓ǂݍ���
	if (m_CarType == CARTYPE_NEUTRAL)
	{
		D3DXLoadMeshFromX("data/MODEL/New-Traffic-light.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);
	}
	if (m_CarType == CARTYPE_NORMAL)
	{
		D3DXLoadMeshFromX("data/MODEL/car001.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);
	}
	if (m_CarType == CARTYPE_SUPER)
	{
		D3DXLoadMeshFromX("data/MODEL/car002.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);
	}
	if (m_CarType == CARTYPE_TRUCK)
	{
		D3DXLoadMeshFromX("data/MODEL/truck_blank001.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);
	}
	if (m_CarType == CARTYPE_POLICE)
	{
		D3DXLoadMeshFromX("data/MODEL/PoliceCar.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);
	}

	//�ʒu
	m_pos = pos;
	m_pos.y = -20.0f;			//������

	//����
	m_rot = m_rotStatic;
	m_move = m_moveStatic;

	int nNumVtx;				//���_��
	BYTE *pVtxBuff;				//���_�o�b�t�@�ւ̃|�C���^
	DWORD sizeFVF;				//���_�t�H�[�}�b�g�̃T�C�Y
	D3DXMATERIAL *pMat;			// �}�e���A���ւ̃|�C���^

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//�e�N�X�`���̊��蓖��
	if (m_CarType == CARTYPE_SUPER)
	{
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Lamborginhi_base_phong_baseColor.jpeg", &m_pTexture[m_CarType][0]);
	}

	//�e�N�X�`�����
	if (m_CarType == CARTYPE_TRUCK)
	{
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/GEN4DR_INTERIOR_dif.png", &m_pTexture[m_CarType][0]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/GENERIC_BADGES_DIF.png", &m_pTexture[m_CarType][1]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Numberplates_dif.png", &m_pTexture[m_CarType][2]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UCB_BOTTOM_DIF.pn", &m_pTexture[m_CarType][3]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UCB_GLASS_CLEAN_dif.png", &m_pTexture[m_CarType][4]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/krmlgtbdy85_headlights_dif.png", &m_pTexture[m_CarType][6]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tier_1o.png", &m_pTexture[m_CarType][7]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wheel_truck_stamp_spec.png", &m_pTexture[m_CarType][8]);
	}

	// ���_�����擾
	nNumVtx = m_pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//���_�ʒu���
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		m_vtx = *(D3DXVECTOR3*)pVtxBuff;//���_���W�̑��

		if (m_vtx.x > m_MaxSize.x)
		{
			m_MaxSize.x = m_vtx.x;
		}

		if (m_vtx.x < m_MinSize.x)
		{
			m_MinSize.x = m_vtx.x;
		}

		if (m_vtx.y > m_MaxSize.y)
		{
			m_MaxSize.y = m_vtx.y;
		}

		if (m_vtx.y < m_MinSize.y)
		{
			m_MinSize.y = m_vtx.y;
		}

		if (m_vtx.z > m_MaxSize.z)
		{
			m_MaxSize.z = m_vtx.z;
		}

		if (m_vtx.z < m_MinSize.z)
		{
			m_MinSize.z = m_vtx.z;
		}

		pVtxBuff += sizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	//�l����
	m_posMaxBase = m_MaxSize;
	m_posMinBase = m_MinSize;

	//�A�����b�N
	m_pMesh->UnlockVertexBuffer();

	//������
	m_bHoming = false;
	m_fDestAngle = 0.0f;
	m_bPlayCar = false;

	//�ړI�n�K�C�h
	for (int nCntArea = 0; nCntArea < MAX_GUIDE_AREA; nCntArea++)
	{
		for (int nCntGuide = 0; nCntGuide < MAX_GUIDE_POINT; nCntGuide++)
		{
			m_AIguide[nCntArea][nCntGuide] = CAIguide::GetAIguidePos(nCntArea, nCntGuide);
		}
	}
	for (int nCntArea = 0; nCntArea < MAX_LIGHT_AREA; nCntArea++)
	{
		for (int nCntLight = 0; nCntLight < MAX_LIGHT_POINT; nCntLight++)
		{
			m_Light[nCntArea][nCntLight] = &CGame::GetTrafficLight(nCntArea*nCntLight + nCntLight)->m_TrafficLight[nCntArea][nCntLight].pos;
		}
	}

	//������
	m_nNowGuide = 0;
	m_nNowLight = 0;
	m_bSignalRed = false;
	m_bPlayerVio = false;
	m_bHearCar = false;
	m_bHearSiren = false;

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CCar::Uninit(void)
{
	//���b�V���̊J��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//�}�e���A���̊J��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//�e�N�X�`���̔j��
	for (int n1 = 0; n1 < 5; n1++)
	{
		for (int n2 = 0; n2 < 64; n2++)
		{
			if (m_pTexture[n1][n2] != NULL)
			{
				m_pTexture[n1][n2]->Release();
				m_pTexture[n1][n2] = NULL;
			}
		}
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//==============================================================================
// �X�V����
//==============================================================================
void CCar::Update(void)
{
	//�J�����p�x�擾
	D3DXVECTOR3 RotationCamera = CManager::GetCamera()->GetRot();
	float RotCameraY = RotationCamera.y;

	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	float fCarSpeed;

	//�^�C�v�ʂɍX�V����
	switch (m_CarType)
	{
	case CARTYPE_SUPER:
		fCarSpeed = AUTO_SUPERCAR_SPD;
		break;
	case CARTYPE_TRUCK:
		fCarSpeed = AUTO_NEUTORALCAR_SPD;
		break;
	}

	if (pInputKeyboard->GetTrigger(DIK_LSHIFT) == true && m_nTypeX == 0)
	{
		// LSHIFT�����������Ƀv���C���[����\�ɂ���(����������Ă���ԗ���1��ɂ���K�v������)
		m_bPlayCar = !m_bPlayCar;
		m_move = D3DXVECTOR3(0.f, 0.f, 0.f);
	}

	// �p�x�C��
	if (RotCameraY > D3DX_PI) { RotCameraY -= D3DX_PI * 2; }
	if (RotCameraY < -D3DX_PI) { RotCameraY += D3DX_PI * 2; }

	// �ʒu���ړ��ʕ����炷
	m_pos += m_move;

	// ���ΓI�Ȍ����̌v�Z�֐�
	CalculationCar();

	//�p�x�v�Z
	if (m_rot.y - D3DX_PI / 4 < m_fAngleLight && m_rot.y > m_fAngleLight&&m_fDisLightZX < 3000.f)
	{
		m_bSignalRed = CGame::GetTrafficLight(m_nTypeX*m_nNowLight + m_nNowLight)->GetSignal(m_nTypeX, m_nNowLight);
	}
	else
	{
		m_bSignalRed = false;
	}

	if (m_fDisLightZX > 2500.f)
	{
		// �ԁ̐M���@�Ԃ̋�����2000�ȉ��̎�
		if (m_nNowLight == MAX_LIGHT_POINT - 1)
		{
			// �z����Ō�̐M���ł��������A�z����ŏ��̐M����
			m_nNowLight = 0;
		}
		else if (m_nNowLight != MAX_LIGHT_POINT - 1)
		{
			// �z����̎��̐M����
			m_nNowLight += 1;
		}
	}

	if (m_bPlayCar == false)
	{
		if (m_bSignalRed == true && m_fDisGuideZX < SIGNAL_FOV && m_fDisGuideZX > 50.f)
		{
			// �ԐM�����Ԃƒ��߃K�C�h�̋�����SIGNAL_FOV�ȓ��̎�
			m_move += D3DXVECTOR3((0.f - m_move.x) * 0.01f, 0.f, (0.f - m_move.z) * 0.01f);
		}
		else
		{
			if (m_fDisGuideZX > 50.f && m_bHoming == false)
			{
				// �ԁ̃K�C�h�Ԃ̋�����50��艓���A�K�C�h�ւ̒Ǐ]��OFF�̎�
				m_bHoming = true;
			}
			else if (m_fDisGuideZX <= 100.f)
			{
				// �ԁ̃K�C�h�Ԃ̋�����100�ȉ��̎�
				if (m_nNowGuide == MAX_GUIDE_POINT - 1)
				{
					// �z����Ō�̃K�C�h�ł��������A�z����ŏ��̃K�C�h��
					m_nNowGuide = 0;
				}
				else if (m_nNowGuide != MAX_GUIDE_POINT - 1)
				{
					// �z����̎��̃K�C�h��
					m_nNowGuide += 1;
				}
			}

			if (m_bHoming == true)
			{
				// �K�C�h�ւ̒Ǐ]��ON�̎�
				if (m_rot.y > m_fAngleGuide)
				{
					// �Ԃ̌������ړI�̌������傫����
					if (m_rot.y >= 0)
					{
						// �ړI�̌�����0�ȉ� ���� �Ԃ̌�����0�ȏ�̎�
						m_fRot = m_rot.y - m_fAngleGuide;
						m_fRot *= -1;
					}
					else
					{
						// �ړI�̌�����0��菬���� ���� �Ԃ̌�����0��菬������
						m_fRot = m_fAngleGuide - m_rot.y;
					}
				}
				else if (m_rot.y <= m_fAngleGuide)
				{
					// �ړI�̌������Ԃ̌����ȏ�̎�
					m_fRot = m_fAngleGuide - m_rot.y;
				}

				// ��]�ʂ������ǂ��炩�̃΂𒴂�����
				if (m_fRot > D3DX_PI) { m_fRot -= D3DX_PI * 2; }
				if (m_fRot < -D3DX_PI) { m_fRot += D3DX_PI * 2; }

				// ��]�ʂ̒���
				m_fRot = m_fRot * 0.1f;

				// �Ԃ̌�������]�ʕ����炷
				m_rot.y += m_fRot;

				// �Ԃ̌����������ǂ��炩�̃΂𒴂��Ă���ꍇ�͈͓��ɏC��
				if (m_rot.y > D3DX_PI) { m_rot.y = m_rot.y - D3DX_PI * 2; }
				if (m_rot.y < -D3DX_PI) { m_rot.y = m_rot.y + D3DX_PI * 2; }

				// �Ԃ̌����ɍ��킹�Ĉړ��ʂ��擾
				m_move = D3DXVECTOR3(sinf(m_rot.y) * fCarSpeed, 0.f, cosf(m_rot.y) * fCarSpeed);

				if (m_nTypeX == 1)
				{
					printf("chaos");
				}
			}
		}

		if (m_CarType == CARTYPE_POLICE)
		{
			if (m_rot.y > D3DX_PI * 31 / 32 && m_fAnglePlayer < 0.f)
			{
				m_fAnglePlayer += D3DX_PI * 2;
			}
			else if (m_rot.y < -D3DX_PI * 31 / 32 && m_fAnglePlayer > 0.f)
			{
				m_fAnglePlayer -= D3DX_PI * 2;
			}

			if (m_rot.y - D3DX_PI / 4 < m_fAnglePlayer && m_rot.y + D3DX_PI / 4 > m_fAnglePlayer && m_fDisPlayerZX < 3000.f)
			{
				if (CPlayer3D::GetViolation() == true)
				{
					m_bPlayerVio = true;
				}

				if (m_bPlayerVio == true)
				{
					m_rot.y = m_fAnglePlayer;
					m_move = D3DXVECTOR3(sinf(m_rot.y) * fCarSpeed, 0.f, cosf(m_rot.y) * fCarSpeed);
				}
			}

			else
			{
				if (m_bPlayerVio == true && m_fDisPlayerZX > 100000.f)
				{
					m_bPlayerVio = false;
					float fTemporaryDistance = 9999999.f;	// ���̒l(���ɓ���ƍl������l���傫���l��)

					for (int nCntGuide = 0; nCntGuide < MAX_GUIDE_POINT; nCntGuide++)
					{
						m_fVecGuideX = m_AIguide[m_nTypeX][nCntGuide]->x - m_pos.x;								// �K�C�h�ƎԂ�X���W�̍�
						m_fVecGuideZ = m_AIguide[m_nTypeX][nCntGuide]->z - m_pos.z;								// �K�C�h�ƎԂ�Z���W�̍�
						m_fDisGuideZX = sqrtf((m_fVecGuideX * m_fVecGuideX) + (m_fVecGuideZ * m_fVecGuideZ));	// �K�C�h�ƎԂ̋���
						m_fAngleGuide = atan2f(m_fVecGuideX, m_fVecGuideZ);										// �K�C�h�ɑ΂���Ԃ̊p�x
						if (m_fDisGuideZX < fTemporaryDistance)
						{
							fTemporaryDistance = m_fDisGuideZX;
							m_nNowGuide = nCntGuide;
						}
					}

					if (m_nNowGuide > 0)
					{
						m_fVecGuideX = m_AIguide[m_nTypeX][m_nNowGuide - 1]->x - m_pos.x;	// �K�C�h�ƎԂ�X���W�̍�
						m_fVecGuideZ = m_AIguide[m_nTypeX][m_nNowGuide - 1]->z - m_pos.z;	// �K�C�h�ƎԂ�Z���W�̍�
						m_fAngleGuide = atan2f(m_fVecGuideX, m_fVecGuideZ);					// �K�C�h�ɑ΂���Ԃ̊p�x

						m_move = D3DXVECTOR3(0.f, 0.f, 0.f);
						m_pos = D3DXVECTOR3(m_AIguide[m_nTypeX][m_nNowGuide - 1]->x, 0.f, m_AIguide[m_nTypeX][m_nNowGuide - 1]->z);
						m_rot.y = m_fAngleGuide;
						m_bSignalRed = false;
					}

					else if (m_nNowGuide == 0)
					{
						m_fVecGuideX = m_AIguide[m_nTypeX][MAX_GUIDE_POINT - 1]->x - m_pos.x;	// �K�C�h�ƎԂ�X���W�̍�
						m_fVecGuideZ = m_AIguide[m_nTypeX][MAX_GUIDE_POINT - 1]->z - m_pos.z;	// �K�C�h�ƎԂ�Z���W�̍�
						m_fAngleGuide = atan2f(m_fVecGuideX, m_fVecGuideZ);					// �K�C�h�ɑ΂���Ԃ̊p�x

						m_move = D3DXVECTOR3(0.f, 0.f, 0.f);
						m_pos = D3DXVECTOR3(m_AIguide[m_nTypeX][MAX_GUIDE_POINT - 1]->x, 0.f, m_AIguide[m_nTypeX][MAX_GUIDE_POINT - 1]->z);
						m_rot.y = m_fAngleGuide;
						m_bSignalRed = false;
					}
				}
			}
		}
	}

	else if (m_bPlayCar == true)
	{
		m_rot.y = RotCameraY;

		float fMoveCar = PLAY_CAR_SPD;
		if (pInputKeyboard->GetPress(DIK_COLON) == true)
		{
			fMoveCar = PLAY_CAR_SPDDOWN;
		}

		//�ړ�����
		//�����������Ƃ�
		if (pInputKeyboard->GetPress(DIK_LEFT) == true)
		{
			//����
			m_rot = D3DXVECTOR3(0.0f, RotCameraY - (D3DX_PI / 2), 0.0f);

			if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_DOWN) == true)
			{
				m_move += D3DXVECTOR3(-cosf(RotCameraY) * fMoveCar, 0.f, sinf(RotCameraY) * fMoveCar) * 0.71f;
			}
			else
			{
				m_move += D3DXVECTOR3(-cosf(RotCameraY) * fMoveCar, 0.f, sinf(RotCameraY) * fMoveCar);
			}
		}

		//�����������Ƃ�
		if (pInputKeyboard->GetPress(DIK_RIGHT) == true)
		{
			//����
			m_rot = D3DXVECTOR3(0.0f, RotCameraY + (D3DX_PI / 2), 0.0f);

			if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_DOWN) == true)
			{
				m_move += D3DXVECTOR3(cosf(RotCameraY) * fMoveCar, 0.f, -sinf(RotCameraY) * fMoveCar) * 0.71f;
			}
			else
			{
				m_move += D3DXVECTOR3(cosf(RotCameraY) * fMoveCar, 0.f, -sinf(RotCameraY) * fMoveCar);
			}
		}

		//�����������Ƃ�
		if (pInputKeyboard->GetPress(DIK_UP) == true)
		{
			//����
			m_rot = D3DXVECTOR3(0.0f, RotCameraY, 0.0f);

			if (pInputKeyboard->GetPress(DIK_LEFT) == true || pInputKeyboard->GetPress(DIK_RIGHT) == true)
			{
				m_move += D3DXVECTOR3(sinf(RotCameraY) * fMoveCar, 0.f, cosf(RotCameraY) * fMoveCar) * 0.71f;
			}
			else
			{
				m_move += D3DXVECTOR3(sinf(RotCameraY) * fMoveCar, 0.f, cosf(RotCameraY) * fMoveCar);
			}
		}

		//�����������Ƃ�
		if (pInputKeyboard->GetPress(DIK_DOWN) == true)
		{
			//����
			m_rot = D3DXVECTOR3(0.0f, RotCameraY - D3DX_PI, 0.0f);

			if (pInputKeyboard->GetPress(DIK_LEFT) == true || pInputKeyboard->GetPress(DIK_RIGHT) == true)
			{
				m_move += D3DXVECTOR3(-sinf(RotCameraY) * fMoveCar, 0.f, -cosf(RotCameraY) * fMoveCar) * 0.71f;
			}
			else
			{
				m_move += D3DXVECTOR3(-sinf(RotCameraY) * fMoveCar, 0.f, -cosf(RotCameraY) * fMoveCar);
			}
		}
		m_move += D3DXVECTOR3((0.f - m_move.x) * 0.1f, 0.f, (0.f - m_move.z) * 0.1f);
	}

	//�T�E���h���擾
	CSound *pSound;
	pSound = CManager::GetSound();

	if (m_fDisPlayerZX < 3000.f && m_bHearCar == false)
	{
		m_bHearCar = true;
		pSound->Play(CSound::SOUND_LABEL_SE_CAR_RUNNING);
	}
	else if (m_fDisPlayerZX > 3000.f && m_bHearCar == true)
	{
		m_bHearCar = false;
		pSound->Stop(CSound::SOUND_LABEL_SE_CAR_RUNNING);
	}

	if (m_bPlayerVio == true && m_bHearSiren == false)
	{
		if (m_bHearCar == true)
		{
			m_bHearCar = false;
			pSound->Stop(CSound::SOUND_LABEL_SE_CAR_RUNNING);
		}
		m_bHearSiren = true;
		pSound->Play(CSound::SOUND_LABEL_SE_POLICE_SIREN);
	}
	else if (m_bPlayerVio == false && m_bHearSiren == true)
	{
		m_bHearSiren = false;
		pSound->Stop(CSound::SOUND_LABEL_SE_POLICE_SIREN);
	}
}

//==============================================================================
// �`�揈��
//==============================================================================
void CCar::Draw(void)
{
	//���[�J��
	D3DXMATRIX mtxRot, mtxTrans, revmat;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;					//���݂̃}�e���A���ۑ�
	D3DXMATERIAL *pMat;						//�}�e���A���ւ̃|�C���^

											// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����̔��]
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�̔��]
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_MaxSize.x < 0.0f)
	{
		m_MaxSize.x *= -1.0f;
	}
	if (m_MaxSize.y < 0.0f)
	{
		m_MaxSize.y *= -1.0f;
	}
	if (m_MaxSize.z < 0.0f)
	{
		m_MaxSize.z *= -1.0f;
	}

	if (m_MinSize.x < 0.0f)
	{
		m_MinSize.x *= -1.0f;
	}
	if (m_MinSize.y < 0.0f)
	{
		m_MinSize.y *= -1.0f;
	}
	if (m_MinSize.z < 0.0f)
	{
		m_MinSize.z *= -1.0f;
	}

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���݂̃}�e���A���̎擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^�擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		if (m_pTexture[m_CarType][nCntMat] != NULL)
		{
			pDevice->SetTexture(0, m_pTexture[m_CarType][nCntMat]);
		}

		else
		{
			pDevice->SetTexture(0, NULL);
		}

		//���f���p�[�c�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	//�ۑ������}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//==============================================================================
// ��������
//==============================================================================
CCar * CCar::Create(D3DXVECTOR3 pos, int nType, CARTYPE carType)
{
	//���f���𐶐�
	CCar *pCar = NULL;
	pCar = new CCar;

	//NULL�`�F�b�N
	if (pCar != NULL)
	{
		pCar->m_nTypeX = nType;

		pCar->m_CarType = carType;

		pCar->Init(pos, 0.0f, 0.0f);
	}

	return pCar;
}

//==============================================================================
// �O�ς𗘗p���������蔻��
//==============================================================================
bool CCar::CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	// �J�������̎擾
	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 rot = pCamera->GetRot();

	bool bLand = false;

	D3DXVECTOR3		g_Vec[4];
	D3DXVECTOR3		g_oldVec[4];
	D3DXVECTOR3		posold;
	float			PlayerVec[4];
	float			oldPlayerVec[4];

	D3DXVECTOR3 pos = *pPos;			// ����Ώۂ̈ʒu
	posold = *pPosOld;					// ����Ώۂ̈ʒu

	//aPos�̒l�ݒ�
	m_aPos[0] = D3DXVECTOR3(m_pos.x + m_MinSize.x, 0.0f, m_pos.z + m_MinSize.z);
	m_aPos[1] = D3DXVECTOR3(m_pos.x + m_MinSize.x, 0.0f, m_pos.z + m_MaxSize.z);
	m_aPos[2] = D3DXVECTOR3(m_pos.x + m_MaxSize.x, 0.0f, m_pos.z + m_MaxSize.z);
	m_aPos[3] = D3DXVECTOR3(m_pos.x + m_MaxSize.x, 0.0f, m_pos.z + m_MinSize.z);

	// �Ίp���̐ݒ�
	float fLength[4];
	fLength[0] = sqrtf(m_MinSize.x * m_MinSize.x + m_MinSize.z * m_MinSize.z);
	fLength[1] = sqrtf(m_MinSize.x * m_MinSize.x + m_MaxSize.z * m_MaxSize.z);
	fLength[2] = sqrtf(m_MaxSize.x * m_MaxSize.x + m_MaxSize.z * m_MaxSize.z);
	fLength[3] = sqrtf(m_MaxSize.x * m_MaxSize.x + m_MinSize.z * m_MinSize.z);

	float fAngle[4];
	fAngle[0] = atan2f(m_MinSize.x, m_MinSize.z);
	fAngle[1] = atan2f(m_MinSize.x, m_MaxSize.z);
	fAngle[2] = atan2f(m_MaxSize.x, m_MaxSize.z);
	fAngle[3] = atan2f(m_MaxSize.x, m_MinSize.z);

	float fRot = m_rot.y;

	//���_�̍��W
	m_aPos[0].x = (m_pos.x + sinf(fAngle[0] + fRot) * fLength[0]);
	m_aPos[0].z = (m_pos.z + cosf(fAngle[0] + fRot) * fLength[0]);

	m_aPos[1].x = (m_pos.x + sinf(fAngle[1] - fRot) * fLength[1]);
	m_aPos[1].z = (m_pos.z - cosf(fAngle[1] - fRot) * fLength[1]);

	m_aPos[2].x = (m_pos.x - sinf(fAngle[2] + fRot) * fLength[2]);
	m_aPos[2].z = (m_pos.z - cosf(fAngle[2] + fRot) * fLength[2]);

	m_aPos[3].x = (m_pos.x - sinf(fAngle[3] - fRot) * fLength[3]);
	m_aPos[3].z = (m_pos.z + cosf(fAngle[3] - fRot) * fLength[3]);

	//���ѕ���������
	//90����Ă���ꍇ
	if (m_aPos[0].x > m_aPos[2].x && m_aPos[0].z < m_aPos[2].z)
	{
		D3DXVECTOR3 vecold;
		vecold = m_aPos[0];

		m_aPos[0] = m_aPos[3];
		m_aPos[3] = m_aPos[2];
		m_aPos[2] = m_aPos[1];
		m_aPos[1] = vecold;
	}

	//180����Ă���ꍇ
	else if (m_aPos[0].x < m_aPos[2].x && m_aPos[0].z < m_aPos[2].z)
	{
		D3DXVECTOR3 vecold;
		vecold = m_aPos[0];

		D3DXVECTOR3 vecold2;
		vecold2 = m_aPos[1];

		m_aPos[0] = m_aPos[2];
		m_aPos[2] = vecold;
		m_aPos[1] = m_aPos[3];
		m_aPos[3] = vecold2;
	}

	//270����Ă���ꍇ
	else if (m_aPos[0].x < m_aPos[2].x && m_aPos[0].z > m_aPos[2].z)
	{
		D3DXVECTOR3 vecold;
		vecold = m_aPos[0];

		m_aPos[0] = m_aPos[1];
		m_aPos[1] = m_aPos[2];
		m_aPos[2] = m_aPos[3];
		m_aPos[3] = vecold;
	}

	//�O�ς̎�
	m_Vec[0] = m_aPos[1] - m_aPos[0];
	m_Vec[1] = m_aPos[2] - m_aPos[1];
	m_Vec[2] = m_aPos[3] - m_aPos[2];
	m_Vec[3] = m_aPos[0] - m_aPos[3];

	for (int nCount = 0; nCount < 4; nCount++)
	{
		//�O�ς̎�2
		if (nCount == 0)
		{
			g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPos[nCount];
		}
		else if (nCount == 1)
		{
			g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPos[nCount];
		}
		else if (nCount == 2)
		{
			g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPos[nCount];
		}
		else if (nCount == 3)
		{
			g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPos[nCount];
		}

		PlayerVec[nCount] = (m_Vec[nCount].z * g_Vec[nCount].x) - (m_Vec[nCount].x * g_Vec[nCount].z);
	}

	//�㉺�̓����蔻��
	//if (pPos->y <= m_pos.y + m_MaxSize.y - fHeightMin && pPos->y > m_pos.y - m_MinSize.y - fHeightMax)
	{
		if (PlayerVec[0] > 0.0f && PlayerVec[1] > 0.0f && PlayerVec[2] > 0.0f && PlayerVec[3] > 0.0f)
		{
			for (int nCount = 0; nCount < 4; nCount++)
			{
				//�O�ς̎�2
				if (nCount == 0)
				{
					g_oldVec[nCount] = posold + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPos[nCount];
				}
				else if (nCount == 1)
				{
					g_oldVec[nCount] = posold + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPos[nCount];
				}
				else if (nCount == 2)
				{
					g_oldVec[nCount] = posold + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPos[nCount];
				}
				else if (nCount == 3)
				{
					g_oldVec[nCount] = posold + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPos[nCount];
				}

				oldPlayerVec[nCount] = (m_Vec[nCount].z * g_oldVec[nCount].x) - (m_Vec[nCount].x * g_oldVec[nCount].z);
			}

			//���E�̓����蔻��
			if (PlayerVec[1] > 0 && PlayerVec[3] > 0)
			{
				//��
				if (oldPlayerVec[0] < 0 && PlayerVec[0] > 0)
				{
					//2���_�����v�Z
					D3DXVECTOR3 a = m_aPos[0] - m_aPos[1];

					//�p�x���v�Z
					double nAtan2 = atan2(a.z, a.x);

					if (nAtan2 > 0.0f)						//�Εӂ̏ꍇ
					{
						pPos->z = pPosOld->z;				//1�t���[���O�̈ʒu�ɖ߂�
						pPos->x = pPosOld->x;
						pMove->z = 0.0f;					//�ړ��ʂ��[���ɂ���
						pMove->x = sinf((float)nAtan2) * 35;
					}

					else if (nAtan2 <= 0.0f)				//�Εӂ̏ꍇ�@�����̏ꍇ	
					{
						pPos->z = pPosOld->z;				//1�t���[���O�̈ʒu�ɖ߂�
						pPos->x = pPosOld->x;
						pMove->z = 0.0f;					//�ړ��ʂ��[���ɂ���
						pMove->x = sinf((float)-nAtan2) * 35;
					}
					bLand = true;
				}

				//�E
				else if (oldPlayerVec[2] < 0 && PlayerVec[2] > 0)
				{
					//2���_�����v�Z
					D3DXVECTOR3 a = m_aPos[3] - m_aPos[2];

					//�p�x���v�Z
					double nAtan2 = atan2(a.z, a.x);

					if (nAtan2 > 0.0f)							//�Εӂ̏ꍇ		
					{
						pPos->z = pPosOld->z;					//1�t���[���O�̈ʒu�ɖ߂�
						pPos->x = pPosOld->x;
						pMove->z = 0.0f;						//�ړ��ʂ��[���ɂ���
						pMove->x = sinf((float)-nAtan2) * 35;
					}

					else if (nAtan2 <= 0.0f)					//�Εӂ̏ꍇ�@�����̏ꍇ	
					{
						pPos->z = pPosOld->z;					//1�t���[���O�̈ʒu�ɖ߂�
						pPos->x = pPosOld->x;
						pMove->z = 0.0f;						//�ړ��ʂ��[���ɂ���
						pMove->x = sinf((float)nAtan2) * 35;
					}
					bLand = true;
				}
			}

			//�O��̓����蔻��
			if (PlayerVec[0] > 0 && PlayerVec[2] > 0)
			{
				//��
				if (oldPlayerVec[1] < 0 && PlayerVec[1] > 0)
				{
					//2���_�����v�Z
					D3DXVECTOR3 a = m_aPos[2] - m_aPos[1];

					//�p�x���v�Z
					double nAtan2 = atan2(a.z, a.x);
					//float nAtan2 = m_rot.y;

					if (nAtan2 > 0.0f)							//�Εӂ̏ꍇ
					{
						pPos->z = pPosOld->z;					//1�t���[���O�̈ʒu�ɖ߂�
						pPos->x = pPosOld->x;
						pMove->x = 0.0f;						//�ړ��ʂ��[���ɂ���
						pMove->z = cosf((float)nAtan2) * 35;
					}

					else if (nAtan2 <= 0.0f)					//�Εӂ̏ꍇ�@�����̏ꍇ	
					{
						pPos->z = pPosOld->z;					//1�t���[���O�̈ʒu�ɖ߂�
						pPos->x = pPosOld->x;
						pMove->x = 0.0f;						//�ړ��ʂ��[���ɂ���
						pMove->z = cosf((float)-nAtan2) * 35;
					}
					bLand = true;
				}

				//�O
				if (oldPlayerVec[3] < 0 && PlayerVec[3] > 0)
				{
					//2���_�����v�Z
					D3DXVECTOR3 a = m_aPos[0] - m_aPos[3];

					//�p�x���v�Z
					double nAtan2 = atan2(a.z, a.x);

					if (nAtan2 > 0.0f)							//�Εӂ̏ꍇ
					{
						pPos->z = pPosOld->z;					//1�t���[���O�̈ʒu�ɖ߂�
						pPos->x = pPosOld->x;
						pMove->x = 0.0f;						//�ړ��ʂ��[���ɂ���
						pMove->z = cosf((float)-nAtan2) * 35;
					}

					else if (nAtan2 <= 0.0f)					//�Εӂ̏ꍇ�@�����̏ꍇ	
					{
						pPos->z = pPosOld->z;					//1�t���[���O�̈ʒu�ɖ߂�
						pPos->x = pPosOld->x;
						pMove->x = 0.0f;						//�ړ��ʂ��[���ɂ���
						pMove->z = cosf((float)nAtan2) * 35;
					}
					bLand = true;
				}
			}
		}
	}

	//SE
	if (bLand == true)
	{
		CSound *pSound = CManager::GetSound();

		pSound->Play(CSound::SOUND_LABEL_SE_CAR_HORN);
	}

	return bLand;
}

//==============================================================================
// Get���[���h�}�g���b�N�X
//==============================================================================
D3DXMATRIX CCar::GetMtxWorld(void)
{
	return m_mtxWorld;
}

//==============================================================================
// �e��v�Z
//==============================================================================
void CCar::CalculationCar(void)
{
	m_fVecGuideX = m_AIguide[m_nTypeX][m_nNowGuide]->x - m_pos.x;							// �K�C�h�ƎԂ�X���W�̍�
	m_fVecGuideZ = m_AIguide[m_nTypeX][m_nNowGuide]->z - m_pos.z;							// �K�C�h�ƎԂ�Z���W�̍�
	m_fDisGuideZX = sqrtf((m_fVecGuideX * m_fVecGuideX) + (m_fVecGuideZ * m_fVecGuideZ));	// �K�C�h�ƎԂ̋���
	m_fAngleGuide = atan2f(m_fVecGuideX, m_fVecGuideZ);										// �K�C�h�ɑ΂���Ԃ̊p�x


	m_fVecLightX = m_Light[m_nTypeX][m_nNowLight]->x - m_pos.x;								// �M���@�ƎԂ�X���W�̍�
	m_fVecLightZ = m_Light[m_nTypeX][m_nNowLight]->z - m_pos.z;								// �M���@�ƎԂ�Z���W�̍�
	m_fDisLightZX = sqrtf((m_fVecLightX * m_fVecLightX) + (m_fVecLightZ * m_fVecLightZ));	// �M���@�ƎԂ̋���
	m_fAngleLight = atan2f(m_fVecLightX, m_fVecLightZ);										// �M���@�ɑ΂���Ԃ̊p�x

	m_fVecPlayerX = CPlayer3D::GetPosition().x - m_pos.x;										// player�ƎԂ�X���W�̍�
	m_fVecPlayerZ = CPlayer3D::GetPosition().z - m_pos.z;										// player�ƎԂ�Z���W�̍�
	m_fDisPlayerZX = sqrtf((m_fVecPlayerX * m_fVecPlayerX) + (m_fVecPlayerZ * m_fVecPlayerZ));	// player�ƎԂ̋���
	m_fAnglePlayer = atan2f(m_fVecPlayerX, m_fVecPlayerZ);										// player�ɑ΂���Ԃ̊p�x
}