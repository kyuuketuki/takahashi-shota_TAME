//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "player.h"
#include "manager.h"
#include "bullet.h"
#include "sound.h"
#include "logo.h"
#include "polygon.h"
#include "animation.h"
#include "gamepad.h"

// マクロ定義
#define MOVE_NUMBER (7)

// 静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture[PLAYER_TYPE] = {};

//グローバル宣言
D3DXVECTOR3 g_pos;					//Get用
int g_nCntPlayer;					//弾の発射速度
int g_nBulletLevel = 0;				//弾の強化Level
int g_nTimePlayer = 0;				//無敵時間記憶

int g_nCntPlayerBullet = 0;			//弾の発射間隔用
int g_nCntPlayerBullet2 = 0;		//弾の発射間隔用
int g_nCntPlayerBullet3 = 0;		//弾の発射間隔用

float g_fAnglePlayer;				//弾の発射角度
float g_fAnglePlayerBullet = 0.0f;	//弾の発射角度

bool g_bAnglePlayer = false;		//弾の発射角度
bool g_bBumpedPlayer = false;		//無敵状態

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority) : CScene2D(nPriority)
{
	//初期化
	g_nCntPlayerBullet = 0;
	g_nCntPlayerBullet2 = 0;
	g_nCntPlayerBullet3 = 0;
	g_nTimePlayer = 0;
	g_nBulletLevel = 0;

	g_fAnglePlayerBullet = 0.0f;

	m_nBullet = false;
	m_nBullet2 = false;
	g_bAnglePlayer = false;
	g_bBumpedPlayer = false;
	m_nBulletLevelUP = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, int nLife, float fSizeX, float fSizeY)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CScene2D::Init(pos, fSizeX, fSizeY);

	//対角線の角度
	g_fAnglePlayer = atan2f(fSizeX, fSizeY);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 10, 10, PLAYER_COLLISION_PINTER);

	//現在のタイプを取得
	m_nType = CManager::GetPlayerType();

	// オブジェクトの種類設定
	SetObjType(CScene::OBJTYPE_PLAYER);

	//タイプによって最大HPと移動速度を変える
	if (m_nType == 0)
	{
		CManager::SetMaxPlayerLife(10);
		m_nSpeed = 12;
	}

	if (m_nType == 1)
	{
		CManager::SetMaxPlayerLife(10);
		m_nSpeed = 10;
	}

	if (m_nType == 2)
	{
		CManager::SetMaxPlayerLife(10);
		m_nSpeed = 10;
	}

	if (m_nType == 3)
	{
		CManager::SetMaxPlayerLife(7);
		m_nSpeed = 15;
	}

	if (m_nType == 4)
	{
		CManager::SetMaxPlayerLife(14);
		m_nSpeed = 7;
	}

	// オブジェクトの種類設定
	SetObjType(CScene::OBJTYPE_PLAYER);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//ゲームパッドのポインタ宣言
	CGamepad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//プレイヤー位置
	D3DXVECTOR3 pos;

	int nSpeed = 0;

	//無敵状態
	if (g_bBumpedPlayer == true)
	{
		//プレイヤーの色を赤くする
		CScene2D::SetCollar(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		g_nTimePlayer++;
	}

	//無敵状態時から
	if (g_nTimePlayer == 60)
	{
		CScene2D::SetCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		g_bBumpedPlayer = false;
		g_nTimePlayer = 0;
	}

	//現在の位置情報
	pos = GetPosition();

	//移動処理
	//Aを押したとき
	if (pInputKeyboard->GetPress(DIK_A) == true || pInputKeyboard->GetPress(DIK_LEFT) == true || pGamepad->IsButtonPush(CInput::LEFT))
	{
		//Wを押したとき
		if (pInputKeyboard->GetPress(DIK_W) == true || pInputKeyboard->GetPress(DIK_UP) == true || pGamepad->IsButtonPush(CInput::UP))
		{
			pos.x += cosf(D3DX_PI) * ((m_nSpeed) / 1.5f);
			pos.y += cosf(D3DX_PI) * ((m_nSpeed) / 1.5f);
		}

		//AとSを押したとき
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInputKeyboard->GetPress(DIK_RIGHT) == true || pGamepad->IsButtonPush(CInput::DOWN))
		{
			pos.x += cosf(D3DX_PI) * ((m_nSpeed) / 1.5f);
			pos.y -= cosf(D3DX_PI) * ((m_nSpeed) / 1.5f);
		}

		//Aを押したとき
		else
		{
			pos.x -= (m_nSpeed);
			pos.y += 0;
		}
	}

	//Dを押したとき
	else if (pInputKeyboard->GetPress(DIK_D) == true || pInputKeyboard->GetPress(DIK_RIGHT) == true || pGamepad->IsButtonPush(CInput::RIGHT))
	{
		//DとWを押したとき
		if (pInputKeyboard->GetPress(DIK_W) == true || pInputKeyboard->GetPress(DIK_UP) == true || pGamepad->IsButtonPush(CInput::UP))
		{
			pos.x -= cosf(D3DX_PI) * ((m_nSpeed) / 1.5f);
			pos.y += cosf(D3DX_PI) * ((m_nSpeed) / 1.5f);
		}

		//DとSを押したとき
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInputKeyboard->GetPress(DIK_DOWN) == true || pGamepad->IsButtonPush(CInput::DOWN))
		{
			pos.x -= cosf(D3DX_PI) * ((m_nSpeed) / 1.5f);
			pos.y -= cosf(D3DX_PI) * ((m_nSpeed) / 1.5f);
		}

		//Dを押したとき
		else
		{
			pos.x -= -(m_nSpeed);
			pos.y -= 0;
		}
	}

	//Wを押したとき
	else if (pInputKeyboard->GetPress(DIK_W) == true || pInputKeyboard->GetPress(DIK_UP) == true || pGamepad->IsButtonPush(CInput::CInput::UP))
	{
		pos.y -= (m_nSpeed);
	}

	//Sを押したとき
	else if (pInputKeyboard->GetPress(DIK_S) == true || pInputKeyboard->GetPress(DIK_DOWN) == true || pGamepad->IsButtonPush(CInput::DOWN))
	{
		pos.y += (m_nSpeed);
	}

	//Aを押したときの加速量
	if (pInputKeyboard->GetPress(DIK_A) == true || pInputKeyboard->GetPress(DIK_LEFT) == true || pGamepad->IsButtonPush(CInput::LEFT))
	{
		m_move.x -= sinf(g_fAnglePlayer) * 1;
	}

	//Dを押したときの加速量
	if (pInputKeyboard->GetPress(DIK_D) == true || pInputKeyboard->GetPress(DIK_RIGHT) == true || pGamepad->IsButtonPush(CInput::RIGHT))
	{
		m_move.x += sinf(g_fAnglePlayer) * 1;
	}

	//Wを押したときの加速量
	if (pInputKeyboard->GetPress(DIK_W) == true || pInputKeyboard->GetPress(DIK_UP) == true || pGamepad->IsButtonPush(CInput::UP))
	{
		m_move.y -= cosf(g_fAnglePlayer) * 1;
	}

	//Sを押したときの加速量
	if (pInputKeyboard->GetPress(DIK_S) == true || pInputKeyboard->GetPress(DIK_DOWN) == true || pGamepad->IsButtonPush(CInput::DOWN))
	{
		m_move.y += cosf(g_fAnglePlayer) * 1;
	}

	//プレイヤー別攻撃処理
	TypePlayer();

	//移動制限
	if (pos.x >= SCREEN_WIDTH)
	{
		pos.x = SCREEN_WIDTH;
	}
	if (pos.x <= 0)
	{
		pos.x = 0;
	}

	if (pos.y >= SCREEN_HEIGHT)
	{
		pos.y = SCREEN_HEIGHT;
	}
	if (pos.y <= 0)
	{
		pos.y = 0;
	}

	pos += m_move;

	//位置情報をメンバ変数に入れる
	g_pos = pos;
	m_pos = pos;

	//移動量の減衰
	m_move.x += (0 - m_move.x) * 0.3f;
	m_move.y += (0 - m_move.y) * 0.3f;

	//移動させる
	SetPosition(pos);

	//更新
	CScene2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// 生成処理
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, int nLife, float fSizeX, float fSizeY)
{
	// プレイヤーの生成
	CPlayer *pPlayer = NULL;
	pPlayer = new CPlayer;

	// 初期化
	if (pPlayer != NULL)
	{
		pPlayer->Init(pos, nLife, fSizeX, fSizeY);
	}

	// テクスチャ割り当て
	pPlayer->BindTexture(m_pTexture[CManager::GetPlayerType()]);

	return pPlayer;
}

//=============================================================================
// テクスチャのロード
//=============================================================================
HRESULT CPlayer::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_01.png", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_02.png", &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_03.png", &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_04.png", &m_pTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_05.png", &m_pTexture[4]);

	return S_OK;
}

//=============================================================================
// テクスチャのアンロード
//=============================================================================
void CPlayer::Unload(void)
{
	for (int nCnt = 0; nCnt < PLAYER_TYPE; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// プレイヤーのタイプ別処理
//=============================================================================
void CPlayer::TypePlayer()
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//ゲームパッドのポインタ宣言
	CGamepad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//プレイヤー位置
	D3DXVECTOR3 pos;

	int nSpeed = 0;

	//現在の位置情報
	pos = GetPosition();


	//プレイヤーのタイプが0の時
	if (m_nType == 0)
	{
		//弾の発射(Press)
		if (pInputKeyboard->GetPress(DIK_SPACE) == true || pGamepad->IsButtonPush(CInput::A) == true)
		{
			if (g_nCntPlayer == 0)
			{
				//サウンドSE
				pSound->Play(CSound::SOUND_LABEL_SE_SHOT);

				//バレットレベル0の時に
				if (g_nBulletLevel == 0)
				{
					//弾の発射
					CBullet::Create(D3DXVECTOR3(pos.x, pos.y - 50.0f, 1.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 150, 25, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
				}

				//バレットレベル0の時に
				if (g_nBulletLevel == 1)
				{
					//弾の発射
					CBullet::Create(D3DXVECTOR3(pos.x + 30.0f, pos.y - 40.0f, 1.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 150, 25, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 30.0f, pos.y - 40.0f, 1.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 150, 25, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
				}

				//バレットレベル2の時に
				if (g_nBulletLevel >= 2)
				{
					CPolygon::PlayerBullet(20);
				}
			}

			//バレット発射
			g_nCntPlayer++;

			//バレット発射間隔
			if (g_nCntPlayer >= 10)
			{
				g_nCntPlayer = 0;
			}
		}

		//バレットレベル2で出現させる
		if (g_nBulletLevel >= 2)
		{
			//以前の状態を初期化
			if (g_nCntPlayerBullet == 0)
			{
				g_nCntPlayerBullet = 1;			//初期化を完了状態にする
			}

			//初期化状態から次のフレーム以降
			if (m_nBulletLevelUP == true)
			{
				if (m_nBullet == false)
				{
					//レベル2状態に出現するポリゴン
					CPolygon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 300, 50, 50, 0.0f, 0.0f, 0.05f, 50, 50, OBJTYPE_BULET_PLAYER, 1);
					m_nBullet = true;
				}
			}

			//レベルアップ状態にする
			m_nBulletLevelUP = true;

			if (m_nBullet2 == true)
			{
				//レベルアップオフ状態にする
				m_nBulletLevelUP = false;
			}
		}
	}

	//プレイヤーのタイプが1の時
	if (m_nType == 1)
	{
		//弾の発射(Press)
		if (pInputKeyboard->GetPress(DIK_SPACE) == true || pGamepad->IsButtonPush(CInput::A) == true)
		{
			if (g_nCntPlayer == 0)
			{
				//サウンドSE
				pSound->Play(CSound::SOUND_LABEL_SE_SHOT);

				//バレットレベル0
				if (g_nBulletLevel == 0)
				{
					//弾の発射
					CBullet::Create(D3DXVECTOR3(pos.x + 30.0f, pos.y - 50.0f, 1.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 30.0f, pos.y - 50.0f, 1.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
				}

				//バレットレベル1
				if (g_nBulletLevel == 1)
				{
					//弾の発射
					CBullet::Create(D3DXVECTOR3(pos.x, pos.y - 40.0f, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x + 30.0f, pos.y - 50.0f, 1.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 30.0f, pos.y - 50.0f, 1.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
				}

				//バレットレベル2
				if (g_nBulletLevel == 2)
				{
					//弾の発射
					CPolygon::PlayerBullet(20);
				}

				if (g_nBulletLevel >= 3)
				{
					//弾の発射
					CBullet::Create(D3DXVECTOR3(pos.x, pos.y - 40.0f, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x + 100.0f, pos.y + 100.0f, 1.0f), D3DXVECTOR3(-1.0f, -15.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 100.0f, pos.y + 100.0f, 1.0f), D3DXVECTOR3(1.0f, -15.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x + 300.0f, pos.y, 1.0f), D3DXVECTOR3(-4.0f, -15.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 300.0f, pos.y, 1.0f), D3DXVECTOR3(4.0f, -15.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
				}
			}

			//バレット発射
			g_nCntPlayer++;

			//バレット発射間隔
			if (g_nCntPlayer >= 10)
			{
				g_nCntPlayer = 0;
			}
		}

		//バレットレベル2で出現させる
		if (g_nBulletLevel == 2)
		{
			if (g_nCntPlayerBullet == 0)
			{
				g_nCntPlayerBullet = 1;			//初期化を完了状態にする
			}

			//初期化状態から次のフレーム以降
			if (m_nBulletLevelUP == true)
			{
				if (m_nBullet == false)
				{
					//レベル2状態に出現するポリゴン
					CPolygon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 300, 50, 50, 1.0f, 0.0f, 0.05f, 100, 50, OBJTYPE_BULET_PLAYER, 1);
					m_nBullet = true;
				}
			}

			//レベルアップ状態にする
			m_nBulletLevelUP = true;

			if (m_nBullet2 == true)
			{
				//レベルアップオフ状態にする
				m_nBulletLevelUP = false;
			}
		}

		//バレットレベル4で出現させる
		if (g_nBulletLevel >= 3)
		{
			if (g_nCntPlayerBullet2 == 0)
			{
				g_nCntPlayerBullet2 = 1;		//初期化を完了状態にする
			}

			//初期化状態から次のフレーム以降
			if (m_nBulletLevelUP == true)
			{
				if (m_nBullet2 == false)
				{
					//レベル2状態に出現するアニメーション
					CAnimasion::Create(D3DXVECTOR3(100.0f, 100.0f, 0.0f), 100, 100, 10, 9);
					CAnimasion::Create(D3DXVECTOR3(-100.0f, 100.0f, 0.0f), 100, 100, 10, 9);
					CAnimasion::Create(D3DXVECTOR3(300.0f, 0.0f, 0.0f), 100, 100, 10, 9);
					CAnimasion::Create(D3DXVECTOR3(-300.0f, 0.0f, 0.0f), 100, 100, 10, 9);
					m_nBullet2 = true;
				}
			}

			//レベルアップ状態にする
			m_nBulletLevelUP = true;

			if (m_nBullet2 == true)
			{
				//レベルアップオフ状態にする
				m_nBulletLevelUP = false;
			}
		}
	}

	//プレイヤーのタイプが2の時
	if (m_nType == 2)
	{
		//弾の発射(Press)
		if (pInputKeyboard->GetPress(DIK_SPACE) == true || pGamepad->IsButtonPush(CInput::A) == true)
		{
			if (g_nCntPlayer == 0)
			{
				//サウンドSE
				pSound->Play(CSound::SOUND_LABEL_SE_SHOT);

				//バレットレベル0
				if (g_nBulletLevel == 0)
				{
					//弾の発射
					CBullet::Create(D3DXVECTOR3(pos.x + 30.0f, pos.y - 30.0f, 1.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 30.0f, pos.y - 30.0f, 1.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
				}

				//バレットレベル2
				if (g_nBulletLevel >= 1 && g_nBulletLevel <= 2)
				{
					//弾の発射
					CBullet::Create(D3DXVECTOR3(pos.x, pos.y - 50.0f, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x + 30.0f, pos.y - 30.0f, 1.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 30.0f, pos.y - 30.0f, 1.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
				}

				//バレットレベル2
				if (g_nBulletLevel >= 3 && g_nBulletLevel <= 5)
				{
					CPolygon::PlayerBullet(20);
				}

				//バレットレベル3
				if (g_nBulletLevel >= 6)
				{
					//弾の発射
					CBullet::Create(D3DXVECTOR3(pos.x, pos.y - 50.0f, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 150, 20, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x, pos.y + 50.0f, 1.0f), D3DXVECTOR3(0.0f, 15.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 150, 20, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);

					CBullet::Create(D3DXVECTOR3(pos.x + 100.0f, pos.y, 1.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 100.0f, pos.y, 1.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);

					CBullet::Create(D3DXVECTOR3(pos.x + 100.0f, pos.y, 1.0f), D3DXVECTOR3(10.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 100.0f, pos.y, 1.0f), D3DXVECTOR3(-10.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);

					CBullet::Create(D3DXVECTOR3(pos.x + 100.0f, pos.y, 1.0f), D3DXVECTOR3(10.0f, -10.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 100.0f, pos.y, 1.0f), D3DXVECTOR3(-10.0f, -10.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
				}
			}

			//バレット発射
			g_nCntPlayer++;

			//バレット発射間隔
			if (g_nCntPlayer >= 10)
			{
				g_nCntPlayer = 0;
			}
		}

		//バレットレベル3で出現させる
		if (g_nBulletLevel >= 3 && g_nBulletLevel <= 5)
		{
			if (g_nCntPlayerBullet == 0)
			{
				g_nCntPlayerBullet = 1;			//初期化を完了状態にする
			}

			//初期化状態から次のフレーム以降
			if (m_nBulletLevelUP == true)
			{
				if (m_nBullet == false)
				{
					//レベル2状態に出現するポリゴン
					CPolygon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 300, 50, 50, 1.0f, 0.0f, 0.05f, 100, 50, OBJTYPE_BULET_PLAYER, 1);
					m_nBullet = true;
				}
			}

			//レベルアップ状態にする
			m_nBulletLevelUP = true;

			if (m_nBullet2 == true)
			{
				//レベルアップオフ状態にする
				m_nBulletLevelUP = false;
			}
		}

		//バレットレベル3で出現させる
		if (g_nBulletLevel >= 6)
		{
			if (g_nCntPlayerBullet2 == 0)
			{
				g_nCntPlayerBullet = 1;			//初期化を完了状態にする
			}

			//初期化状態から次のフレーム以降
			if (m_nBulletLevelUP == true)
			{
				if (m_nBullet2 == false)
				{
					//レベル2状態に出現するアニメーション
					CAnimasion::Create(D3DXVECTOR3(100.0f, 0.0f, 0.0f), 100, 100, 10, 6);
					CAnimasion::Create(D3DXVECTOR3(-100.0f, 0.0f, 0.0f), 100, 100, 10, 6);
					m_nBullet2 = true;
				}
			}

			//レベルアップ状態にする
			m_nBulletLevelUP = true;

			if (m_nBullet2 == true)
			{
				//レベルアップオフ状態にする
				m_nBulletLevelUP = false;
			}
		}
	}

	//プレイヤーのタイプが3の時
	if (m_nType == 3)
	{
		//弾の発射(Press)
		if (pInputKeyboard->GetPress(DIK_SPACE) == true || pGamepad->IsButtonPush(CInput::A) == true)
		{
			if (g_nCntPlayer == 0)
			{
				//サウンドSE
				pSound->Play(CSound::SOUND_LABEL_SE_SHOT);

				//バレットレベル0
				if (g_nBulletLevel == 0)
				{
					//弾の発射
					CBullet::Create(D3DXVECTOR3(pos.x + 40.0f, pos.y, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 40.0f, pos.y, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
				}

				//バレットレベル1
				if (g_nBulletLevel >= 1 && g_nBulletLevel <= 2)
				{
					//弾の発射
					CBullet::Create(D3DXVECTOR3(pos.x, pos.y - 50.0f, 1.0f), D3DXVECTOR3(0.0f, -20.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 150, 20, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x + 40.0f, pos.y, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 40.0f, pos.y, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
				}

				//バレットレベル1
				if (g_nBulletLevel >= 3 && g_nBulletLevel <= 5)
				{
					//弾の発射
					CBullet::Create(D3DXVECTOR3(pos.x, pos.y - 50.0f, 1.0f), D3DXVECTOR3(0.0f, -20.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 150, 20, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x + 100.0f, pos.y, 1.0f), -10.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, g_fAnglePlayerBullet, OBJTYPE_BULET_PLAYER);
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x - 100.0f, pos.y, 1.0f), -10.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 15, BULLET_SIZE, BULLET_SIZE, g_fAnglePlayerBullet, OBJTYPE_BULET_PLAYER);
				}

				//バレットレベル1
				if (g_nBulletLevel >= 6)
				{
					//弾の発射
					CBullet::Create(D3DXVECTOR3(pos.x, pos.y - 50.0f, 1.0f), D3DXVECTOR3(0.0f, -20.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x + 100.0f, pos.y, 1.0f), -10.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 13, BULLET_SIZE, BULLET_SIZE, g_fAnglePlayerBullet + 50, OBJTYPE_BULET_PLAYER);
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x - 100.0f, pos.y, 1.0f), -10.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 13, BULLET_SIZE, BULLET_SIZE, g_fAnglePlayerBullet + 50, OBJTYPE_BULET_PLAYER);
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x + 100.0f, pos.y, 1.0f), -10.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 13, BULLET_SIZE, BULLET_SIZE, g_fAnglePlayerBullet, OBJTYPE_BULET_PLAYER);
					CBullet::InitAngleBullet(D3DXVECTOR3(pos.x - 100.0f, pos.y, 1.0f), -10.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 500, 13, BULLET_SIZE, BULLET_SIZE, g_fAnglePlayerBullet, OBJTYPE_BULET_PLAYER);
				}
			}

			//バレット発射
			g_nCntPlayer++;

			//バレット発射間隔
			if (g_nCntPlayer >= 8)
			{
				g_nCntPlayer = 0;
			}

			//バレットレベルが3以上になったら
			if (g_nBulletLevel >= 3)
			{
				//角度を変える
				if (g_bAnglePlayer == false)
				{
					g_fAnglePlayerBullet += 5.0f;
				}

				if (g_bAnglePlayer == true)
				{
					g_fAnglePlayerBullet -= 5.0f;
				}

				//角度の制限
				if (g_fAnglePlayerBullet >= 50)
				{
					g_bAnglePlayer = true;
				}

				if (g_fAnglePlayerBullet <= -50)
				{
					g_bAnglePlayer = false;
				}
			}
		}

		//バレットレベル3で出現させる
		if (g_nBulletLevel >= 3)
		{
			if (g_nCntPlayerBullet2 == 0)
			{
				g_nCntPlayerBullet = 1;			//初期化を完了状態にする
			}

			//初期化状態から次のフレーム以降
			if (m_nBulletLevelUP == true)
			{
				if (m_nBullet2 == false)
				{
					//レベル2状態に出現するアニメーション
					CAnimasion::Create(D3DXVECTOR3(100.0f, 0.0f, 0.0f), 100, 100, 10, 11);
					CAnimasion::Create(D3DXVECTOR3(-100.0f, 0.0f, 0.0f), 100, 100, 10, 11);
					m_nBullet2 = true;
				}
			}

			//レベルアップ状態にする
			m_nBulletLevelUP = true;

			if (m_nBullet2 == true)
			{
				//レベルアップオフ状態にする
				m_nBulletLevelUP = false;
			}
		}
	}

	//プレイヤーのタイプが4の時
	if (m_nType == 4)
	{
		//弾の発射(Press)
		if (pInputKeyboard->GetPress(DIK_SPACE) == true || pGamepad->IsButtonPush(CInput::A) == true)
		{
			if (g_nCntPlayer == 0)
			{
				//サウンドSE
				pSound->Play(CSound::SOUND_LABEL_SE_SHOT);

				//バレットレベル0
				if (g_nBulletLevel == 0)
				{
					//弾の発射
					CBullet::Create(D3DXVECTOR3(pos.x, pos.y - 50.0f, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 50.0f, pos.y - 10.0f, 1.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x + 50.0f, pos.y - 10.0f, 1.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
				}

				//バレットレベル1
				if (g_nBulletLevel == 1)
				{
					//弾の発射
					CBullet::Create(D3DXVECTOR3(pos.x, pos.y - 100.0f, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x + 100.0f, pos.y, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 100.0f, pos.y, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
				}

				//バレットレベル1
				if (g_nBulletLevel >= 2 && g_nBulletLevel <= 4)
				{
					//弾の発射
					CBullet::Create(D3DXVECTOR3(pos.x, pos.y - 100.0f, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);

					CBullet::Create(D3DXVECTOR3(pos.x + 100.0f, pos.y - 70.0f, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 100.0f, pos.y - 70.0f, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x + 150.0f, pos.y, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 150.0f, pos.y, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
				}

				//バレットレベル1
				if (g_nBulletLevel >= 5)
				{
					//弾の発射
					CBullet::Create(D3DXVECTOR3(pos.x, pos.y - 100.0f, 1.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 150, 15, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);

					CBullet::Create(D3DXVECTOR3(pos.x + 150.0f, pos.y, 1.0f), D3DXVECTOR3(10.0f, -15.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 150.0f, pos.y, 1.0f), D3DXVECTOR3(-10.0f, -15.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x + 100.0f, pos.y + 70.0f, 1.0f), D3DXVECTOR3(5.0f, -15.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 100.0f, pos.y + 70.0f, 1.0f), D3DXVECTOR3(-5.0f, -15.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x + 100.0f, pos.y - 70.0f, 1.0f), D3DXVECTOR3(1.0f, -20.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
					CBullet::Create(D3DXVECTOR3(pos.x - 100.0f, pos.y - 70.0f, 1.0f), D3DXVECTOR3(-1.0f, -20.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 150, 10, BULLET_SIZE, BULLET_SIZE, OBJTYPE_BULET_PLAYER);
				}
			}

			//バレット発射
			g_nCntPlayer++;

			//バレット発射間隔
			if (g_nCntPlayer >= 10)
			{
				g_nCntPlayer = 0;
			}
		}

		//バレットレベル1で出現させる
		if (g_nBulletLevel == 1)
		{
			if (g_nCntPlayerBullet == 0)
			{
				g_nCntPlayerBullet = 1;
			}

			//初期化状態から次のフレーム以降
			if (m_nBulletLevelUP == true)
			{
				if (m_nBullet == false)
				{
					CAnimasion::UninitTypeClear();
					//レベル1状態に出現するアニメーション
					CAnimasion::Create(D3DXVECTOR3(0.0f, -100.0f, 0.0f), 100, 100, 10, 13);
					CAnimasion::Create(D3DXVECTOR3(100.0f, 0.0f, 0.0f), 100, 100, 10, 5);
					CAnimasion::Create(D3DXVECTOR3(-100.0f, 0.0f, 0.0f), 100, 100, 10, 5);

					m_nBullet = true;
				}
			}

			//レベルアップ状態にする
			m_nBulletLevelUP = true;

			if (m_nBullet == true)
			{
				//レベルアップオフ状態にする
				m_nBulletLevelUP = false;
			}
		}

		//バレットレベル2で出現させる
		if (g_nBulletLevel >= 2 && g_nBulletLevel <= 4)
		{
			if (g_nCntPlayerBullet2 == 0)
			{
				g_nCntPlayerBullet2 = 1;
			}

			//初期化状態から次のフレーム以降
			if (m_nBulletLevelUP == true)
			{
				if (m_nBullet2 == false)
				{
					CAnimasion::UninitTypeClear();
					//レベル2以上の状態に出現するアニメーション
					//CAnimasion::Create(D3DXVECTOR3(0.0f, -100.0f, 0.0f), 100, 100, 10, 13);
					CAnimasion::Create(D3DXVECTOR3(150.0f, 0.0f, 0.0f), 100, 100, 10, 5);
					CAnimasion::Create(D3DXVECTOR3(-150.0f, 0.0f, 0.0f), 100, 100, 10, 5);
					CAnimasion::Create(D3DXVECTOR3(100.0f, -70.0f, 0.0f), 100, 100, 10, 5);
					CAnimasion::Create(D3DXVECTOR3(-100.0f, -70.0f, 0.0f), 100, 100, 10, 5);

					m_nBullet2 = true;
				}
			}

			//レベルアップ状態にする
			m_nBulletLevelUP = true;

			if (m_nBullet2 == true)
			{
				//レベルアップオフ状態にする
				m_nBulletLevelUP = false;
			}
		}

		//バレットレベル5で出現させる
		if (g_nBulletLevel >= 5)
		{
			if (g_nCntPlayerBullet3 == 0)
			{
				g_nCntPlayerBullet3 = 1;
			}

			//初期化状態から次のフレーム以降
			if (m_nBulletLevelUP == true)
			{
				if (m_nBullet3 == false)
				{
					CAnimasion::UninitTypeClear();

					//レベル5以上の状態に出現するアニメーション
					CAnimasion::Create(D3DXVECTOR3(100.0f, 70.0f, 0.0f), 100, 100, 10, 5);
					CAnimasion::Create(D3DXVECTOR3(-100.0f, 70.0f, 0.0f), 100, 100, 10, 5);

					m_nBullet3 = true;
				}
			}

			//レベルアップ状態にする
			m_nBulletLevelUP = true;

			if (m_nBullet3 == true)
			{
				//レベルアップオフ状態にする
				m_nBulletLevelUP = false;
			}
		}
	}
}

//=============================================================================
// バレットレベル
//=============================================================================
void CPlayer::BulletLevel(void)
{
	g_nBulletLevel++;
}

void CPlayer::SetBulletLevel(int nData)
{
	g_nBulletLevel = nData;
}

//=============================================================================
// Get&Set関数　無敵状態
//=============================================================================
bool CPlayer::GetBumped(void)
{
	return g_bBumpedPlayer;
}
void CPlayer::SetBumped(bool bBumped)
{
	g_bBumpedPlayer = bBumped;
}

//=============================================================================
// Get関数　位置情報
//=============================================================================
D3DXVECTOR3 CPlayer::GetPlayer(void)
{
	return g_pos;
}
