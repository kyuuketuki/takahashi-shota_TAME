//=============================================================================
//
// ロゴ処理 [logo.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "logo.h"
#include "manager.h"
#include "scene2D.h"
#include "scene.h"
#include "player3D.h"

// 静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CLogo::m_apTextureLogo[MAX_UI] = {};

//グローバル宣言
int g_nTypeLogo = -1;			//特殊処理をするタイプを入れる
int g_nTypeMoveLogo = -1;		//特殊処理をするタイプを入れる
int g_nTypeCollarLogo = -1;		//特殊処理をするタイプを入れる
D3DXVECTOR3 g_posLogo;			//位置
D3DXCOLOR	g_col;				//色情報

//=============================================================================
// コンストラクタ
//=============================================================================
CLogo::CLogo(int nPriority, int nType) : CScene2D(nPriority, nType)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CLogo::~CLogo()
{
	g_nTypeLogo = -1;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLogo::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType)
{
	// CScene2Dの初期化処理
	CScene2D::Init(pos, fSizeX, fSizeY);

	//タイプを代入
	m_nType = nType;
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;

	//メニュー画面フレーム
	if (m_nType == FRAME)
	{
		// テクスチャ座標変更
		SetTex(1.0f, 0.5f, 1);
	}

	//チュートリアル画面　背景
	if (m_nType == CONTROLS_BG)
	{
		// テクスチャ座標変更
		SetTex(0.5f, 1.0f, 1);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLogo::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLogo::Update(void)
{
	// キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//ゲームパッドのポインタ宣言
	CGamepad *pGamepad;
	pGamepad = CManager::GetGamepad();

	// 操作説明の画像変更
	if (m_nType == CONTROLS_BG)
	{
		// エンターが押されたら
		if (pInputKeyboard->GetTrigger(DIK_D) == true || pGamepad->IsButtonPush(CInput::RIGHT) == true)
		{
			// テクスチャ座標変更
			SetTex(0.5f, 1.0f, 2);
		}

		// エンターが押されたら
		if (pInputKeyboard->GetTrigger(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT) == true)
		{
			// テクスチャ座標変更
			SetTex(0.5f, 1.0f, 1);
		}
	}

	//スタート時の拡大処理
	if (m_nType == START)
	{
		//寿命チェック
		if (m_nCountSize >= 35)
		{
			//プレイヤーの移動許可
			CPlayer3D::SetMoveUse(true);

			//スタートUI消去
			Uninit();
		}

		//拡大終了
		else if (m_nCountSize >= 30)
		{
			//何もしない
		}

		else
		{
			//拡大処理
			//サイズを情報
			CScene2D::SetSize(m_fSizeX + ((m_fSizeX / 10) * m_nCountSize), m_fSizeY + ((m_fSizeY / 10) * m_nCountSize));
		}

		//寿命カウント
		m_nCountSize++;
	}

	// メニュー画面フレーム更新
	if (m_nType == FRAME)
	{
		// エンターが押されたら
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) == true || pGamepad->IsButtonDown(CInput::B) == true)
		{
			// テクスチャ座標変更
			SetTex(1.0f, 0.5f, 2);
		}
	}

	//途中色変更
	if (m_nType == g_nTypeCollarLogo)
	{
		//色を変える
		CScene2D::SetCollar(g_col);
	}

	//途中移動
	if (m_nType == g_nTypeMoveLogo)
	{
		//初期化
		g_nTypeMoveLogo = -1;

		//移動させる
		SetPosition(g_posLogo);

		//更新
		CScene2D::Update();
	}

	//途中破棄
	if (g_nTypeLogo == m_nType)
	{
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CLogo::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// 生成処理
//=============================================================================
CLogo * CLogo::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType)
{
	// ローカル変数宣言
	CLogo *pLogo = NULL;
	pLogo = new CLogo;

	// 初期化
	if (pLogo != NULL)
	{
		pLogo->Init(pos, fSizeX, fSizeY, nType);
	}

	// テクスチャ割り当て
	pLogo->BindTexture(m_apTextureLogo[nType]);

	return pLogo;
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CLogo::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/singouflame_short.png", &m_apTextureLogo[FRAME]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/menu_flame.png", &m_apTextureLogo[GAMEFRAME]);

	// タイトル、メニュー　テクスチャ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_logo000.png", &m_apTextureLogo[TITLE_LOGO]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter.png", &m_apTextureLogo[TITLE_UI_PRESS_ENTER]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gamestart.png", &m_apTextureLogo[TITLE_UI_GAMESTART]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial.png", &m_apTextureLogo[TITLE_UI_TUTORIAL]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/exit.png", &m_apTextureLogo[TITLE_UI_END]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/menu_flame.png", &m_apTextureLogo[MENU_FLAME]);

	// チュートリアル
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/controls_bg.png", &m_apTextureLogo[CONTROLS_BG]);

	// ポーズ テクスチャ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause.png", &m_apTextureLogo[PAUSE01]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/restart.png", &m_apTextureLogo[PAUSE_RESTART]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Menu.png", &m_apTextureLogo[PAUSE_MENU]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/exit.png", &m_apTextureLogo[PAUSE_EXIT]);

	//リスト背景
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/navigate.png", &m_apTextureLogo[LIST]);

	//ゲームTUTORIAL
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/start.jpg", &m_apTextureLogo[START]);

	//ゲームTUTORIAL
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Gaming_telop_black.png", &m_apTextureLogo[RESULT_SCORE]);

	//ゲームTUTORIAL
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Your Score.png", &m_apTextureLogo[MAX_SCORE]);

	//ゲームランキングUI
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/1位.png", &m_apTextureLogo[RANKING_FIRST]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/2位.png", &m_apTextureLogo[RANKING_SECOND]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/3位.png", &m_apTextureLogo[RANKING_THIRD]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/4位.png", &m_apTextureLogo[RANKING_FOUR]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/5位.png", &m_apTextureLogo[RANKING_FIVE]);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg.png", &m_apTextureLogo[RANKING_BG]);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ランキング.png", &m_apTextureLogo[RANKING_LOGO]);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/addscore.png", &m_apTextureLogo[RANKING_ADD]);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/equal-red3.png", &m_apTextureLogo[RANKING_EQUAL]);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rankingtutorial.png", &m_apTextureLogo[RANKING_TUTORIAL]);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CAUTION_frame_yellow.png", &m_apTextureLogo[RANKING_FREAM]);

	//ゲーム中に表示させたいUI
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/説明.jpg", &m_apTextureLogo[GAMR_UI01]);

	//ルール説明
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ルール説明.jpg", &m_apTextureLogo[RULE]);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CLogo::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		if (m_apTextureLogo != NULL)
		{
			m_apTextureLogo[nCnt]->Release();
			m_apTextureLogo[nCnt] = NULL;
		}
	}
}

//=============================================================================
// 破棄したいタイプ
//=============================================================================
void CLogo::TypeUninit(int nType)
{
	g_nTypeLogo = nType;
}

//=============================================================================
// 位置をずらしたいタイプ
//=============================================================================
void CLogo::TypeMove(D3DXVECTOR3 pos, int nTypeMove)
{
	g_posLogo = pos;
	g_nTypeMoveLogo = nTypeMove;
}

//=============================================================================
// 指定したタイプの色を変える
//=============================================================================
void CLogo::TypeChangeCollar(D3DXCOLOR col, int nTypeCollar)
{
	g_nTypeCollarLogo = nTypeCollar;
	g_col = col;
}

//=============================================================================
//
// メイン処理 [logopause.cpp]
// Author : takahashi shota
//
//=============================================================================

//ポーズ画面に表示させたい場合

//=============================================================================
// コンストラクタ
//=============================================================================
CLogoPause::CLogoPause(int nPriority, int nType) : CLogo(nPriority, nType)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CLogoPause::~CLogoPause()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLogoPause::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType)
{
	CScene2D::Init(pos, fSizeX, fSizeY);

	CLogoPause::m_nType = nType;

	if (m_nType == FRAME)
	{
		// テクスチャ座標変更
		SetTex(1.0f, 0.5f, 1);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLogoPause::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLogoPause::Update(void)
{
	// キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// 特殊更新
	// タイトル移動
	if (m_nType == 0)
	{
		// 現在の位置の取得
		D3DXVECTOR3 pos = GetPosition();

		// 移動させる
		SetPosition(pos);

		// 更新
		CScene2D::Update();
	}

	if (m_nType == 9)
	{
		// 色を変える
		CScene2D::SetCollar(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}

	if (m_nType == g_nTypeCollarLogo)
	{
		// 色を変える
		CScene2D::SetCollar(g_col);
	}

	// 特殊更新
	if (m_nType == FRAME)
	{
		// エンターが押されたら
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{// テクスチャ座標変更
			SetTex(1.0f, 0.5f, 2);
		}
	}

	// 途中移動
	if (m_nType == g_nTypeMoveLogo)
	{
		// 初期化
		g_nTypeMoveLogo = -1;

		// 移動させる
		SetPosition(g_posLogo);

		// 更新
		CScene2D::Update();
	}

	// 途中終了
	if (g_nTypeLogo == m_nType)
	{
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CLogoPause::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// 生成処理
//=============================================================================
CLogoPause * CLogoPause::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType)
{
	// ローカル変数宣言
	CLogoPause *pLogoPause = NULL;
	pLogoPause = new CLogoPause;

	// 初期化
	if (pLogoPause != NULL)
	{
		pLogoPause->Init(pos, fSizeX, fSizeY, nType);
	}

	// テクスチャ割り当て
	pLogoPause->BindTexture(m_apTextureLogo[nType]);

	return pLogoPause;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CLogoPause::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		if (m_apTextureLogo != NULL)
		{
			m_apTextureLogo[nCnt]->Release();
			m_apTextureLogo[nCnt] = NULL;
		}
	}
}

//=============================================================================
// 破棄したいタイプ
//=============================================================================
void CLogoPause::TypeUninit(int nType)
{
	g_nTypeLogo = nType;
}

//=============================================================================
// 位置をずらしたいタイプ
//=============================================================================
void CLogoPause::TypeMove(D3DXVECTOR3 pos, int nTypeMove)
{
	g_posLogo = pos;
	g_nTypeMoveLogo = nTypeMove;
}

//=============================================================================
// 色を変えたいタイプ
//=============================================================================
void CLogoPause::TypeChangeCollar(D3DXCOLOR col, int nTypeCollar)
{
	g_nTypeCollarLogo = nTypeCollar;
	g_col = col;
}
