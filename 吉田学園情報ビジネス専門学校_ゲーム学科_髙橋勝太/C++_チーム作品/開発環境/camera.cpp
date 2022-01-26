//=============================================================================
//
// メイン処理 [camera.cpp]
// Author : Koseki Yuto
//
//=============================================================================
#include "camera.h"
#include "player3D.h"

//マクロ定義
#define DISTANCE			(1000.0f)	// プレイヤー(注視点)とカメラの距離
#define MOUSE_SENSITIVITY	(0.0008f)	// マウス操作のカメラ回転速度
#define MOVE_CAMERA_ROT		(0.3f)		// 目的角度に追いつく速度
#define MOVE_SPEED			(0.2f)		// カメラの追従速度
#define POSR_Y				(50)		// 注視点高さ調節用
#define ROT_X_MIN_ANGLE		(0.1f)		// 縦回転の最小値
#define ROT_X_MAX_ANGLE		(1.40f)		// 縦回転の最大値

//静的メンバ変数
D3DXVECTOR3 CCamera::m_rot = {};
float		CCamera::m_fhorizontalAngle = NULL;

//=============================================================================
//グローバル変数
//=============================================================================
int nCameraType = 0;

//=============================================================================
// コンスタクタ
//=============================================================================
CCamera::CCamera()
{
	m_fDistance = DISTANCE;									// カメラと注視点の距離
	m_posV[0] = D3DXVECTOR3(0.0f, 100.0f, 0.0f);			// 視点
	m_posR[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 注視点
	m_vecU[0] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// 法線ベクトル

	m_viewport[0].X = (DWORD)0.0f;							// 描画する画面左上のＸ座標
	m_viewport[0].Y = (DWORD)0.0f;							// 描画する画面左上のＹ座標
	m_viewport[0].Width = (DWORD)300.0f;					// 描画する画面の幅
	m_viewport[0].Height = (DWORD)300.0f;					// 描画する画面の高さ
	m_viewport[0].MinZ = 0.0f;
	m_viewport[0].MaxZ = 1.0f;

	m_posV[1] = D3DXVECTOR3(0.0f, 100.0f, 0.0f);			// 視点
	m_posR[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 注視点
	m_vecU[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// 法線ベクトル

	m_viewport[1].X = (DWORD)0.0f;							// 描画する画面左上のＸ座標
	m_viewport[1].Y = (DWORD)0.0f;							// 描画する画面左上のＹ座標
	m_viewport[1].Width = (DWORD)SCREEN_WIDTH;				// 描画する画面の幅
	m_viewport[1].Height = (DWORD)SCREEN_HEIGHT;			// 描画する画面の高さ
	m_viewport[1].MinZ = 0.0f;
	m_viewport[1].MaxZ = 1.0f;

	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);				// 回転角度
	int m_Rpos = 0;											// 注視点調節用のはずだった
	m_fMouseSpeed = MOUSE_SENSITIVITY;						// マウス感度
	m_fhorizontalAngle = 0.0f;								// 水平アングル
	m_fverticalAngle = 0.0f;								// 垂直アングル
	SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);		// マウスカーソルの位置固定
	m_Mousepoint = { 0,0 };									// マウス位置
}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CCamera::Update(void)
{
	// キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// プレイヤーの位置取得
	D3DXVECTOR3 pos = CPlayer3D::GetPosition();
	D3DXVECTOR3 rot = CPlayer3D::GetRotation();

	//デバック用
#ifdef _DEBUG
	//モード切り替え
	if (pInputKeyboard->GetTrigger(DIK_N) == true)
	{
		//キーボードモードへ
		if (nCameraType == 0)
		{
			nCameraType = 1;
		}

		//カメラモードへ
		else if (nCameraType == 1)
		{
			nCameraType = 0;
		}
	}
#endif

	//マウスモードへ
	if (nCameraType == 0)
	{
		//マウス操作
		Mouseoperation();

		//各々のカメラの視点変更
		m_posV[0].x = pos.x + sinf(m_rot.y) *-300.0f;
		m_posV[0].y = pos.y + sinf(m_rot.x) + cosf(m_rot.x) * 6000.0f;
		m_posV[0].z = pos.z + cosf(m_rot.y) *-300.0f;

		m_posV[1].x = pos.x + sinf(m_rot.y) *-m_fDistance;
		m_posV[1].y = pos.y + sinf(m_rot.x) + cosf(m_rot.x) * 300.0f;
		m_posV[1].z = pos.z + cosf(m_rot.y) *-m_fDistance;

		//二画面以上時に使用
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			// 注視点　プレイヤー
			m_posRDest.x = pos.x;
			m_posRDest.z = pos.z;
			m_posRDest.y = pos.y + POSR_Y;

			// 目的の視点に注視点を代入
			m_posVDest.x = m_posR[nCnt].x;
			m_posVDest.z = m_posR[nCnt].z;
			m_posVDest.y = m_posR[nCnt].y;

			// カメラ位置更新
			m_posV[nCnt] += (m_posVDest - m_posV[nCnt])       * MOVE_SPEED;
			m_posR[nCnt].x += (m_posRDest.x - m_posR[nCnt].x) * MOVE_SPEED;
			m_posR[nCnt].y += (m_posRDest.y - m_posR[nCnt].y) * MOVE_SPEED;
			m_posR[nCnt].z += (m_posRDest.z - m_posR[nCnt].z) * MOVE_SPEED;
		}

		// 回転角度が3.14以上、-3.14以下にならないように
		if (m_rot.y >= D3DX_PI)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}
		if (m_rot.y <= -D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}

		// 回転角度が1.57より上、-0.1より下にならないように
		if (m_rot.x >= ROT_X_MAX_ANGLE)
		{
			m_rot.x = ROT_X_MAX_ANGLE;
		}
		if (m_rot.x <= ROT_X_MIN_ANGLE)
		{
			m_rot.x = ROT_X_MIN_ANGLE;
		}
	}

#ifdef _DEBUG
	//キーボードモードへ
	if (nCameraType == 1)
	{
		//Wを押したとき
		if (pInputKeyboard->GetPress(DIK_U) == true)
		{
			//二画面以上時に使用
			for (int nCnt = 0; nCnt < 2; nCnt++)
			{
				m_posV[nCnt].y += 150.0f;
			}
		}

		//Sを押したとき
		if (pInputKeyboard->GetPress(DIK_J) == true)
		{
			//二画面以上時に使用
			for (int nCnt = 0; nCnt < 2; nCnt++)
			{
				m_posV[nCnt].y -= 150.0f;
			}
		}
	}
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CCamera::Draw(void)
{

}

//=============================================================================
// マウス操作処理
//=============================================================================
void CCamera::Mouseoperation(void)
{
	// キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//ゲームパッドのポインタ宣言
	CGamepad *pGamepad;
	pGamepad = CManager::GetGamepad();

	// ポーズ取得
	bool bPauseCheck = CManager::GetPause();

	// マウス位置取得
	GetCursorPos(&m_Mousepoint);

	// マウスカーソルの位置固定
	SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	//カーソルの非表示
	ShowCursor(false);

	// マウスがどれだけ動いたか計算 (水平方向) m_fMouseSpeedは反映する速度
	m_fhorizontalAngle -= m_fMouseSpeed * float(SCREEN_WIDTH / 2 - m_Mousepoint.x);

	// カーブ
	if (bPauseCheck == false)
	{
		//カーブ
		if (pInputKeyboard->GetPress(DIK_D) == true || pGamepad->IsButtonPush(CInput::LEFT2) == true)
		{
			// 右上方向
			m_fhorizontalAngle += 0.05f;
		}

		if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::RIGHT2) == true)
		{
			// 左上方向
			m_fhorizontalAngle -= 0.05f;
		}

		// マウスがどれだけ動いたか計算 (垂直方向) m_fMouseSpeedは反映する速度
		m_fverticalAngle += m_fMouseSpeed * float(SCREEN_HEIGHT / 2 - m_Mousepoint.y);

		//開始時のカメラの角度を決める
		m_rot.y = (m_fhorizontalAngle + D3DX_PI);
		m_rot.x = m_fverticalAngle;
	}

	// 水平アングルが3.14より上、-3.14より下にならないように
	if (m_fhorizontalAngle >= D3DX_PI)
	{
		m_fhorizontalAngle -= D3DX_PI * 2.0f;
	}
	if (m_fhorizontalAngle <= -D3DX_PI)
	{
		m_fhorizontalAngle += D3DX_PI * 2.0f;
	}

	// 垂直アングルが1.57より上、-0.1より下にならないように
	if (m_fverticalAngle >= ROT_X_MAX_ANGLE)
	{
		m_fverticalAngle = ROT_X_MAX_ANGLE;
	}
	if (m_fverticalAngle <= ROT_X_MIN_ANGLE)
	{
		m_fverticalAngle = ROT_X_MIN_ANGLE;
	}
}

float CCamera::GetMousepos(void)
{
	return m_fhorizontalAngle;
}

//=============================================================================
// カメラ設置
//=============================================================================
void CCamera::SetCamera(int nIdx)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//二画面以上時に使用
	//for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// プロジェクションマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxProjection[nIdx]);

		// ビューポートを取得
		pDevice->SetViewport(&m_viewport[nIdx]);

		// プロジェクションマトリックスの作成
		D3DXMatrixPerspectiveFovLH(
			&m_mtxProjection[nIdx],
			D3DXToRadian(45.0f),									// 画角の設定
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,				// 画面比率の設定 ビューポートの幅高さ
			10.0f,													// 手前限界の描画距離
			50000.0f);												// 奥限界の描画距離

		// プロジェクションマトリックスの設定
		pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection[nIdx]);

		// ビューマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxView[nIdx]);

		// ビューマトリックスの作成
		D3DXMatrixLookAtLH(&m_mtxView[nIdx], &m_posV[nIdx], &m_posR[nIdx], &m_vecU[nIdx]);

		// ビューマトリックスの設定
		pDevice->SetTransform(D3DTS_VIEW, &m_mtxView[nIdx]);
	}
}