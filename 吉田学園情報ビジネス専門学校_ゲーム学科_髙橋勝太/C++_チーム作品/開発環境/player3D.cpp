//=============================================================================
//
// �v���C���[3D���� [player3D.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "player3D.h"
#include "manager.h"
#include "model.h"
#include "camera.h"
#include "shadowS.h"
#include "vertex.h"
#include "game.h"
#include "trafficLight.h"
#include "car.h"
#include "effect.h"
#include "passerby.h"

//�ÓI�����o�錾
D3DXVECTOR3		CPlayer3D::m_pos;
D3DXVECTOR3		CPlayer3D::m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
int				CPlayer3D::m_nNumModel = 0;
int				CPlayer3D::m_MotionType = NULL;
bool			CPlayer3D::m_bViolation;
bool			CPlayer3D::m_bMoveUse;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CPlayer3D::CPlayer3D(int nPriority, int nType)
{
	//�����o�ϐ�������
	m_bMotionChange = false;
	m_nBrendFrame = 5;
	m_MotionChange = MOTIONTYPE_NEUTRAL;
	m_bMoveUse = false;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CPlayer3D::~CPlayer3D()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CPlayer3D::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//���[�V�����ǂݍ���
	LoadMotion();

	for (int nCntModel = 0; nCntModel < (int)m_nNumModel; nCntModel++)
	{
		//���f���𐶐�
		m_apModel[nCntModel] = CModel::Create(D3DXVECTOR3(m_aOffset[nCntModel][0], m_aOffset[nCntModel][1], m_aOffset[nCntModel][2]),
			D3DXVECTOR3(m_aOffset[nCntModel][3], m_aOffset[nCntModel][4], m_aOffset[nCntModel][5]), &m_pModelName[nCntModel][0]);

		m_apModel[nCntModel]->SetParent(m_apModel[m_aIndexParent[nCntModel]]);
	}

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�}�e���A���F�̕ύX
	for (int nCntModel = 0; nCntModel < 13; nCntModel++)
	{
		D3DXMATERIAL *pMat;				//�}�e���A���ւ̃|�C���^

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)m_apModel[nCntModel]->GetBuff()->GetBufferPointer();

		//�}�e���A���̐F�ύX
		pMat->MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	}

	//�ʒu
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CPlayer3D::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < (int)m_nNumModel; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{
			//���f���𐶐�
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
		}
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//==============================================================================
// �X�V����
//==============================================================================
void CPlayer3D::Update(void)
{
	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�Q�[���p�b�h�̃|�C���^�錾
	CGamepad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//�����蔻���ԏ��
	bool bUse = CPasserby::GetUse();

	if (bUse == true && m_nCnt == 0)
	{
		//�����o�ϐ�������
		m_fCounterMotion = 0;					//�t���[���J�E���g�����Z�b�g
		m_Key = 0;								//�t���[���L�[�����Z�b�g
		m_GetKey = 0;							//�t���[���L�[�����Z�b�g

		//���[�V�����ύX
		m_MotionChange = MOTIONTYPE_FALLMOVE;
		m_nCnt++;								//�J�E���g�A�b�v
	}

	//�]�ԃ��[�V�������n�܂������J�E���g�𑝂₷
	if (m_nCnt >= 1)
	{
		//�J�E���g�A�b�v
		m_nCnt++;
	}

	//�J�E���g��150�̎������ʒu�ɖ߂��Ă�����
	if (m_nCnt == 150)
	{
		m_pos = D3DXVECTOR3(0.0f, 70.0f, 0.0f);		//�v���C���[�̏����ʒu�ɖ߂�
		CPasserby::SetUse(false);					//�t���O������������

		m_fCounterMotion = 0;						//�t���[���J�E���g�����Z�b�g
		m_Key = 0;									//�t���[���L�[�����Z�b�g
		m_GetKey = 0;								//�t���[���L�[�����Z�b�g
		m_nNextKey = false;							//���[�V�������Z�b�g

		m_MotionChange = MOTIONTYPE_NEUTRAL;		//���[�V�������j���[�g�����ɖ߂�
		m_nCnt = 0;									//�J�E���g��������
	}

	//�v���C���[��ԏ�����
	if (pInputKeyboard->GetPress(DIK_8) == true)
	{
		//���[�V�����ύX
		m_MotionChange = MOTIONTYPE_NEUTRAL;
	}

	//���񑬓x
	float fCamera = 0.03f;
	//float fCamera = 0.2f;

	//�ʒu�X�V
	m_posold = m_pos;

	//���[�V�����̍X�V
	UpdateMotion(m_MotionChange, false);

	//�ړ������I���I�t
	if (m_bMoveUse == true)
	{
		// �ړ��ʂ̉��Z
		m_pos += m_move;
	}

	//�J�����p�x�擾
	D3DXVECTOR3 RotationCamera = CManager::GetCamera()->GetRot();
	float RotCameraY = RotationCamera.y;

	// �p�x�C��
	if (RotCameraY > D3DX_PI) { RotCameraY -= D3DX_PI * 2; }
	if (RotCameraY < -D3DX_PI) { RotCameraY += D3DX_PI * 2; }

	m_rotDest.y = RotCameraY - D3DX_PI;

	if (bUse == false)
	{
		// S�L�[�̃��f���̈ړ�
		if (pInputKeyboard->GetPress(DIK_S) == true || pGamepad->IsButtonPush(CInput::DOWN) == true)
		{
			if (m_nMoveCnt < 20)
			{
				m_nMoveCnt++;
			}

			//���[�V�����̕ω�
			m_MotionChange = MOTIONTYPE_MOVE;

			// �������
			if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT) == true)
			{
				//�J�E���g�A�b�v
				m_nCounter++;

				//���[�V�����̕ω�
				m_MotionChange = MOTIONTYPE_LEFTMOVE;

				//�ړ��ʌv�Z
				m_move.x -= cosf(RotCameraY + D3DX_PI / 4) * 0.5f;
				m_move.z += sinf(RotCameraY + D3DX_PI / 4) * 0.5f;
			}

			// �E�����
			else if (pInputKeyboard->GetPress(DIK_D) == true || pGamepad->IsButtonPush(CInput::RIGHT) == true)
			{
				//�J�E���g�A�b�v
				m_nCounter++;

				//���[�V�����̕ω�
				m_MotionChange = MOTIONTYPE_RIGHTMOVE;

				m_move.x += cosf(RotCameraY - D3DX_PI / 4) * 0.5f;
				m_move.z -= sinf(RotCameraY - D3DX_PI / 4) * 0.5f;
			}
			else
			{
				// ������
				m_move.x -= sinf(RotCameraY) * 0.3f;
				m_move.z -= cosf(RotCameraY) * 0.3f;
			}
		}

		// W�L�[�̃��f���̈ړ�
		if (pInputKeyboard->GetPress(DIK_W) == true ||pGamepad->IsButtonPush(CInput::UP) == true)
		{
			//�G�t�F�N�g���N���G�C�g
			CEffect3D::Create(D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43), 50.0f, 50.0f);

			if (m_nMoveCnt < 20)
			{
				//�J�E���g�A�b�v
				m_nMoveCnt++;
			}

			//���[�V�����̕ω�
			m_MotionChange = MOTIONTYPE_MOVE;

			// ��������
			if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT) == true)
			{
				//�J�E���g�A�b�v
				m_nCounter++;

				//�����o�ϐ�������
				m_fCounterMotion = 0;//�t���[���J�E���g�����Z�b�g
				m_Key = 0;//�t���[���L�[�����Z�b�g
				m_GetKey = 0;//�t���[���L�[�����Z�b�g

				//���[�V�����̕ω�
				m_MotionChange = MOTIONTYPE_LEFTMOVE;

				//�ړ��ʌv�Z
				m_move.x -= cosf(m_rot.y - D3DX_PI / 4) * 0.5f;
				m_move.z += sinf(m_rot.y - D3DX_PI / 4) * 0.5f;
				m_rotDest.y = RotCameraY - D3DX_PI / -2;
				fCamera = 0.04f;
			}

			// �E������
			else if (pInputKeyboard->GetPress(DIK_D) == true || pGamepad->IsButtonPush(CInput::RIGHT) == true)
			{
				//�J�E���g�A�b�v
				m_nCounter++;

				//�����o�ϐ�������
				m_fCounterMotion = 0;//�t���[���J�E���g�����Z�b�g
				m_Key = 0;//�t���[���L�[�����Z�b�g
				m_GetKey = 0;//�t���[���L�[�����Z�b�g

				//���[�V�����̕ω�
				m_MotionChange = MOTIONTYPE_RIGHTMOVE;

				//�ړ��ʌv�Z
				m_move.x += cosf(m_rot.y + D3DX_PI / 4) * 0.5f;
				m_move.z -= sinf(m_rot.y + D3DX_PI / 4) * 0.5f;
				m_rotDest.y = RotCameraY - D3DX_PI / 2;
				fCamera = 0.04f;
			}

			//����厖����������Ȃ��ł�(^��^)/
			if (pInputKeyboard->GetTrigger(DIK_A) == true || pInputKeyboard->GetTrigger(DIK_D) == true ||
				 pGamepad->IsButtonUp(CInput::RIGHT) == true || pGamepad->IsButtonUp(CInput::LEFT) == true)
			{
			}

			else
			{// ������
				m_move.x -= sinf(m_rot.y) * 2.0f;
				m_move.z -= cosf(m_rot.y) * 2.0f;
			}
		}
		else
		{
			if (m_nMoveCnt > 10)
			{
				//�J�E���g�����炵�Ă���
				m_nMoveCnt--;
			}
		}

		// �ړ����[�V��������ҋ@���[�V������
		if (m_MotionChange == MOTIONTYPE_MOVE && m_nMoveCnt == 10)
		{
			//�����o�ϐ�������
			m_fCounterMotion = 0;//�t���[���J�E���g�����Z�b�g
			m_Key = 0;//�t���[���L�[�����Z�b�g
			m_GetKey = 0;//�t���[���L�[�����Z�b�g
			m_MotionChange = MOTIONTYPE_NEUTRAL;//���[�V�������j���[�g�����ɂ���
		}

		//�E�ړ����[�V�����̎� && �J�E���g��1�̎�
		if (m_MotionChange == MOTIONTYPE_RIGHTMOVE && m_nCounter == 1)
		{
			//�����o�ϐ�������
			m_fCounterMotion = 0;//�t���[���J�E���g�����Z�b�g
			m_Key = 0;//�t���[���L�[�����Z�b�g
			m_GetKey = 0;//�t���[���L�[�����Z�b�g
		}
		//���ړ����[�V�����̎� && �J�E���g��1�̎�
		if (m_MotionChange == MOTIONTYPE_LEFTMOVE && m_nCounter == 1)
		{
			//�����o�ϐ�������
			m_fCounterMotion = 0;//�t���[���J�E���g�����Z�b�g
			m_Key = 0;//�t���[���L�[�����Z�b�g
			m_GetKey = 0;//�t���[���L�[�����Z�b�g
		}
	}

	//�e�p�[�c�̍X�V����
	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->MotionPos(D3DXVECTOR3(m_aOffset[nCnt][0], m_aOffset[nCnt][1], m_aOffset[nCnt][2]));
		m_apModel[nCnt]->MotionRot(D3DXVECTOR3(m_aOffset[nCnt][3], m_aOffset[nCnt][4], m_aOffset[nCnt][5]));
	}

	// �ړI�̉�]�p�̏��
	if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rotDest.y += D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rotDest.y -= D3DX_PI * 2.0f;
	}

	// �����̍X�V
	m_rot.y += (m_rotDest.y - m_rot.y) * fCamera;

	// ���݂̉�]�p�̏��
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	// ������̌�������
	m_move.x += (0.0f - m_move.x) * 0.05f;
	m_move.z += (0.0f - m_move.z) * 0.05f;

	// ��]�p�x��3.14�ȏ�A-3.14�ȉ��ɂȂ�Ȃ��悤��
	if (m_rot.y >= D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	//���_�I�u�W�F�N�g�̓����蔻��
	CVertex *pVer;
	pVer = CGame::GetVertex();

	if (pVer != NULL)
	{
		pVer->CollisionVec(&m_pos, &m_posold, &m_move, m_MaxPlayer.x - 10.0f, m_MinPlayer.x + 10.0f, m_MaxPlayer.z - 10.0f, m_MinPlayer.z + 10.0f, m_MaxPlayer.y, m_MinPlayer.y);
	}
	
	//�񐔕�
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//�Ԃ̓����蔻��
		CCar *pCar;
		pCar = CGame::GetCar(nCnt);
		pCar->CollisionVec(&m_pos, &m_posold, &m_move, m_MaxPlayer.x - 30.0f, m_MinPlayer.x + 30.0f, m_MaxPlayer.z - 30.0f, m_MinPlayer.z + 30.0f, m_MaxPlayer.y, m_MinPlayer.y);
	}
}

//==============================================================================
// �`�揈��
//==============================================================================
void CPlayer3D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���[�J��
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����̔��]
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�̔��]
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->Draw();
	}
}

//==============================================================================
// ��������
//==============================================================================
CPlayer3D * CPlayer3D::Create()
{
	//�v���C���[�𐶐�
	CPlayer3D *pPlayer3D = NULL;
	pPlayer3D = new CPlayer3D;

	//NULL�`�F�b�N
	if (pPlayer3D != NULL)
	{
		pPlayer3D->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
	}

	return pPlayer3D;
}

//==============================================================================
// .txt�t�H���_�̓ǂݍ���
//==============================================================================
void CPlayer3D::LoadMotion(void)
{
	// ���[�J���ϐ��錾
	FILE *pFile = NULL;
	char aEqual[2] = { NULL };					// �u=�v�ǂݎ��p�ϐ�
	bool bComment = false;						// �R�����g�A�E�g���邩
	char aText[TEXT_LENGTH] = { NULL };			// �e�L�X�g�ǂݎ��p�ϐ�
	char aCharaSet[DATA_LENGTH] = { NULL };		// �L�����f�[�^�ǂݎ��p�ϐ�
	char aPartsSet[DATA_LENGTH] = { NULL };		// �p�[�c�f�[�^�ǂݎ��p�ϐ�
	char aMotionSet[DATA_LENGTH] = { NULL };	// ���[�V�����ǂݎ��p�ϐ�
	char aKeySet[DATA_LENGTH] = { NULL };		// �L�[�ݒ���ǂݎ��p�ϐ�
	char aKey[DATA_LENGTH] = { NULL };			// �L�[���ǂݎ��p�ϐ�

	// strcmp�ǂݎ��p�|�C���^
	char *pText = &aText[0];
	char *pCharaSet = &aCharaSet[0];
	char *pPartsSet = &aPartsSet[0];
	char *pMotionSet = &aMotionSet[0];
	char *pKeySet = &aKeySet[0];
	char *pKey = &aKey[0];

	// �f�[�^�̓ǂݍ���
	pFile = fopen("data/TEXT/motion_player00.txt", "r");

	if (pFile != NULL)
	{ //�t�@�C���W�J�\
		aText[0] = { NULL };
		aCharaSet[0] = { NULL };
		aPartsSet[0] = { NULL };
		aMotionSet[0] = { NULL };
		aKeySet[0] = { NULL };
		aKey[0] = { NULL };

		//���[�J���ϐ��錾
		int nIdxParts = 0;			// �p�[�c��
		int nMotion = 0;			// ���[�V�����̐�
		int nCntFile = 0;			// X�f�[�^�̐�
		int nAs = 0;

		while (strcmp("END_SCRIPT", pText) != 0)
		{
			aText[0] = { NULL };
			if (bComment == false)
			{// �R�����g�A�E�g���Ă��Ȃ�
				fscanf(pFile, "%s", &aText[0]);

				if (aText[0] == '#')
				{// ������̐擪���u#�v�Ȃ�΃R�����g�A�E�g��
					bComment = true;
				}
				else
				{// �ʏ펞
					if (strcmp("NUM_MODEL", pText) == 0)
					{// ���f���̐�
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%d", &m_nNumModel);
					}
					else if (strcmp("MODEL_FILENAME", pText) == 0)
					{// X�t�@�C�����̓ǂݍ���
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%s", &m_pModelName[nCntFile][0]);
						nCntFile++;
					}
					else if (strcmp("CHARACTERSET", pText) == 0)
					{// �L�����̏��
						while (strcmp("END_CHARACTERSET", pCharaSet) != 0)
						{// �L�������̓ǂݎ��
							fscanf(pFile, "%s", &aCharaSet[0]);
							if (strcmp("NUM_PARTS", pCharaSet) == 0)
							{

							}
							else if (strcmp("PARTSSET", pCharaSet) == 0)
							{// �p�[�c���
								aPartsSet[0] = { NULL };

								while (strcmp("END_PARTSSET", pPartsSet) != 0)
								{
									//��x���[�V�����̑S�Ă�������
									for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
									{
										for (int nCntKey = 0; nCntKey < 10; nCntKey++)
										{
											m_nFrame[nCntMotion][nCntKey] = 40;

											for (int nCntPart = 0; nCntPart < 30; nCntPart++)
											{
												for (int nCntPosRot = 0; nCntPosRot < 6; nCntPosRot++)
												{
													//���[�V�����̏�����
													m_Player3D[nCntMotion][nCntKey][nCntPart][nCntPosRot] = m_aOffset[nCntPart][nCntPosRot];
												}
											}
										}
									}

									fscanf(pFile, "%s", &aPartsSet[0]);
									if (strcmp("INDEX", pPartsSet) == 0)
									{// �p�[�c���̓ǂݎ��
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &nIdxParts);
									}
									else if (strcmp("PARENT", pPartsSet) == 0)
									{// �p�[�c�̐e���
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &m_aIndexParent[nIdxParts]);
									}
									else if (strcmp("POS", pPartsSet) == 0)
									{// �p�[�c�̈ʒu���
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][0]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][1]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][2]);
									}
									else if (strcmp("ROT", pPartsSet) == 0)
									{// �p�[�c�̊p�x���
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][3]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][4]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][5]);
									}
								}
							}
						}
					}
					else if (strcmp("MOTIONSET", pText) == 0)
					{
						// �ϐ��̏�����
						aMotionSet[0] = { NULL };

						// ���[�J���ϐ��錾
						int nLoop = 0;
						int nKeySet = 0;

						while (strcmp("END_MOTIONSET", pMotionSet) != 0)
						{// ���[�V�����̓ǂݍ���
						 //�l��������
							fscanf(pFile, "%s", &aMotionSet[0]);
							if (strcmp("LOOP", pMotionSet) == 0)
							{// ���[�v���邩
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%d", &nLoop);
								if (nLoop == 1)
								{// �P�̏ꍇ���[�v����
									m_bLoopKeep = true;
									m_Loop[nMotion] = true;
								}
							}

							else if (strcmp("NUM_KEY", pMotionSet) == 0)
							{// �L�[���̓ǂݍ���
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%d", &m_nNumKeyKeep);
							}

							if (strcmp("KEYSET", pMotionSet) == 0)
							{// �L�[�̓ǂݍ���
							 // ���[�J���ϐ��錾
								int nKey = 0;
								// �ϐ��̏�����
								aKeySet[0] = { NULL };

								while (strcmp("END_KEYSET", pKeySet) != 0)
								{// �L�[��񍀖ڂ̓ǂݍ���
									fscanf(pFile, "%s", &aKeySet[0]);

									if (aKeySet[0] == '#')
									{// �R�����g�A�E�g
									 // ���[�J���ϐ��錾
										char c = NULL;
										char d = NULL;
										fscanf(pFile, "%c", &c);
										while (c != '\n' && d != '\n')
										{
											fscanf(pFile, "%s", &aKey[0]);
											fscanf(pFile, "%c", &d);
										}
									}

									if (strcmp("KEY", pKeySet) == 0)
									{// �L�[���̓ǂݍ���
										aKey[0] = { NULL };
										while (strcmp("END_KEY", pKey) != 0)
										{// �ʒu�E�p�x�̓ǂݍ���
											fscanf(pFile, "%s", &aKey[0]);
											if (strcmp("POS", pKey) == 0)
											{// �ʒu���̓ǂݍ���
												fscanf(pFile, "%s", &aEqual[0]);

												// �ʒu���̓ǂݍ���
												fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][0]);
												fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][1]);
												fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][2]);
											}
											else if (strcmp("ROT", pKey) == 0)
											{// �p�x���̓ǂݍ���
												fscanf(pFile, "%s", &aEqual[0]);
												fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][3]);
												fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][4]);
												fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][5]);
											}
										}
										//���̃p�[�c�ʒu�Ɉړ�����POS�̒l�𑫂�
										m_Player3D[nMotion][nKeySet][nKey][0] += m_aOffset[nKey][0];
										m_Player3D[nMotion][nKeySet][nKey][1] += m_aOffset[nKey][1];
										m_Player3D[nMotion][nKeySet][nKey][2] += m_aOffset[nKey][2];

										nKey++;
									}
									else if (strcmp("FRAME", pKeySet) == 0)
									{// �L�[�t���[�����̓ǂݍ���
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &m_nFrame[nMotion][nKeySet]);
										m_nFrameTotal[nMotion] += m_nFrame[nMotion][nKeySet];
										if (m_Loop[nMotion] == false && nKeySet == (m_NumKey[nMotion] - 1))
										{
											m_nFrameTotal[nMotion] -= m_nFrame[nMotion][nKeySet];
										}
									}
								}

								//�L�[���v���X
								nKeySet++;
							}
						}

						// �L�[���̐ݒ�
						m_NumKey[nMotion] = m_nNumKeyKeep;

						//���[�V�����̃J�E���g�A�b�v
						nMotion++;
					}
				}
			}
			else if (bComment == true)
			{// �R�����g�A�E�g����
			 // ���[�J���ϐ��錾
				char a = NULL;
				char b = NULL;
				fscanf(pFile, "%c", &a);
				while (a != '\n' && b != '\n')
				{
					fscanf(pFile, "%s", &aText[0]);
					fscanf(pFile, "%c", &b);
				}
				bComment = false;
			}
		}
		fclose(pFile);
	}
	else
	{ // �t�@�C���W�J�s��
	}
}

//==============================================================================
// ���[�V�����̍X�V
//==============================================================================
void CPlayer3D::UpdateMotion(int MOTIONTYPE, bool bUse)
{
	// �L�[���ƃ��[�v���̑��
	m_Key = m_GetKey;
	m_TotalKey = (m_nNumKeyKeep = m_NumKey[MOTIONTYPE]);
	m_bLoopKeep = m_Loop[MOTIONTYPE];

	m_MotionType = m_MotionChange;

	// ���[�V�����J�E���^�[�̉��Z
	if (m_bMotionChange == true)
	{// ���[�V�����u�����h���s��
		if (m_fCounterMotion < m_nBrendFrame)
		{// �J�E���g���
			m_fCounterMotion++;
		}
	}
	else
	{// �ʏ펞
		if (m_fCounterMotion < m_nFrame[MOTIONTYPE][m_Key])
		{// �J�E���g���
			if (bUse == false)
			{
				//���[�V�����̃J�E���g�A�b�v
				m_fCounterMotion++;
			}
		}
	}

	// ���f���p�[�c���Ƃ̃��[�V�����A�j���[�V����
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_Key == m_TotalKey - 1 && m_bLoopKeep == true)
		{// ���[�v���čŏ��ɖ߂�Ƃ�
			m_aOffset[nCnt][0] = m_Player3D[MOTIONTYPE][m_Key][nCnt][0] + (m_Player3D[MOTIONTYPE][0][nCnt][0] - m_Player3D[MOTIONTYPE][m_Key][nCnt][0]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][1] = m_Player3D[MOTIONTYPE][m_Key][nCnt][1] + (m_Player3D[MOTIONTYPE][0][nCnt][1] - m_Player3D[MOTIONTYPE][m_Key][nCnt][1]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][2] = m_Player3D[MOTIONTYPE][m_Key][nCnt][2] + (m_Player3D[MOTIONTYPE][0][nCnt][2] - m_Player3D[MOTIONTYPE][m_Key][nCnt][2]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][3] = m_Player3D[MOTIONTYPE][m_Key][nCnt][3] + (m_Player3D[MOTIONTYPE][0][nCnt][3] - m_Player3D[MOTIONTYPE][m_Key][nCnt][3]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][4] = m_Player3D[MOTIONTYPE][m_Key][nCnt][4] + (m_Player3D[MOTIONTYPE][0][nCnt][4] - m_Player3D[MOTIONTYPE][m_Key][nCnt][4]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][5] = m_Player3D[MOTIONTYPE][m_Key][nCnt][5] + (m_Player3D[MOTIONTYPE][0][nCnt][5] - m_Player3D[MOTIONTYPE][m_Key][nCnt][5]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
			m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
			m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
			m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
			m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
			m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
		}
		else if (m_bMotionChange == true)
		{// ���[�V�����u�����h
			m_aOffset[nCnt][0] = m_aOffsetMemo[nCnt][0] + (m_Player3D[m_MotionType][0][nCnt][0] - m_aOffsetMemo[nCnt][0]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][1] = m_aOffsetMemo[nCnt][1] + (m_Player3D[m_MotionType][0][nCnt][1] - m_aOffsetMemo[nCnt][1]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][2] = m_aOffsetMemo[nCnt][2] + (m_Player3D[m_MotionType][0][nCnt][2] - m_aOffsetMemo[nCnt][2]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][3] = m_aOffsetMemo[nCnt][3] + (m_Player3D[m_MotionType][0][nCnt][3] - m_aOffsetMemo[nCnt][3]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][4] = m_aOffsetMemo[nCnt][4] + (m_Player3D[m_MotionType][0][nCnt][4] - m_aOffsetMemo[nCnt][4]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][5] = m_aOffsetMemo[nCnt][5] + (m_Player3D[m_MotionType][0][nCnt][5] - m_aOffsetMemo[nCnt][5]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
			m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
			m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
			m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
			m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
			m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
		}
		else
		{// �ʏ펞
			if (m_nNextKey == false)
			{
				m_aOffset[nCnt][0] = m_Player3D[MOTIONTYPE][m_Key][nCnt][0] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][0] - m_Player3D[MOTIONTYPE][m_Key][nCnt][0]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][1] = m_Player3D[MOTIONTYPE][m_Key][nCnt][1] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][1] - m_Player3D[MOTIONTYPE][m_Key][nCnt][1]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][2] = m_Player3D[MOTIONTYPE][m_Key][nCnt][2] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][2] - m_Player3D[MOTIONTYPE][m_Key][nCnt][2]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][3] = m_Player3D[MOTIONTYPE][m_Key][nCnt][3] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][3] - m_Player3D[MOTIONTYPE][m_Key][nCnt][3]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][4] = m_Player3D[MOTIONTYPE][m_Key][nCnt][4] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][4] - m_Player3D[MOTIONTYPE][m_Key][nCnt][4]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][5] = m_Player3D[MOTIONTYPE][m_Key][nCnt][5] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][5] - m_Player3D[MOTIONTYPE][m_Key][nCnt][5]) * ((float)m_fCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
				m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
				m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
				m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
				m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
				m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
			}
		}

		if (m_bMotionChange == false)
		{// ���[�V�����ω��Ȃ�
			if (m_fCounterMotion == m_nFrame[MOTIONTYPE][m_Key])
			{// �L�[�t���[���I����
				if (m_Key != m_TotalKey - 2 || m_bLoopKeep == true)
				{// �W�����v�ȊO�ŃL�[�I�������Z�b�g
					if (m_nNextKey == false)
					{
						m_fCounterMotion = 0;
					}
				}

				if (m_Key == m_TotalKey - 1 && m_bLoopKeep == true)
				{// ���[�v����Ƃ�
					m_Key = 0;
					m_GetKey = m_Key;
				}
				else if (m_Key == m_TotalKey - 2 && m_bLoopKeep == false)
				{// ���[�v�����I������Ƃ�
					m_fCounterMotion = m_nFrame[MOTIONTYPE][m_Key];
					m_Key++;
					m_GetKey = m_Key;
					m_nNextKey = true;
				}

				else if (m_Key < m_TotalKey && m_nNextKey != true)
				{// �L�[�̃J�E���g
					m_Key++;
					m_GetKey = m_Key;
				}
			}
			else if (m_MotionTypeOld != m_MotionType)
			{// ���[�V�����ω���	
				m_Key = 0;
				m_fCounterMotion = 0;
				m_bMotionChange = true;

				if (m_MotionTypeOld == MOTIONTYPE_NEUTRAL && m_MotionType == MOTIONTYPE_MOVE)
				{// �ړ�����j���[�g������
					m_nBrendFrame = 10;
				}
				if (m_MotionTypeOld == MOTIONTYPE_MOVE && m_MotionType == MOTIONTYPE_NEUTRAL)
				{// �j���[�g��������ړ���
					m_nBrendFrame = 100;
				}
				if (m_MotionTypeOld == MOTIONTYPE_LEFTMOVE && m_MotionType == MOTIONTYPE_RIGHTMOVE)
				{// �j���[�g��������ړ���
					m_nBrendFrame = 10;
				}
				if (m_MotionTypeOld == MOTIONTYPE_RIGHTMOVE && m_MotionType == MOTIONTYPE_LEFTMOVE)
				{// �j���[�g��������ړ���
					m_nBrendFrame = 10;
				}
				break;
			}
		}
		else if (m_bMotionChange == true)
		{// ���[�V�����u�����h���̃J�E���g���Z�b�g
			if (m_fCounterMotion == m_nBrendFrame || m_MotionTypeOld != m_MotionType)
			{
				m_Key = 0;
				m_fCounterMotion = 0;
				m_bMotionChange = false;
			}
		}
	}

	// ���O�̃��[�V������Ԃ̕ۑ�
	m_MotionTypeOld = m_MotionType;
}

//==============================================================================
// Get�֐�
//==============================================================================
D3DXVECTOR3 CPlayer3D::GetPosition(void)
{
	return m_pos;
}

D3DXVECTOR3 CPlayer3D::GetRotation(void)
{
	return m_rot;
}

void CPlayer3D::SetMoveUse(bool bMoveUse)
{
	m_bMoveUse = bMoveUse;
}

