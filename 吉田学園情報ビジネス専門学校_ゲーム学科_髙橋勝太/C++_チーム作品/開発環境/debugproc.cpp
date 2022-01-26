//=============================================================================
//
//デバック表示処理[debugproc.cpp]
// Author; Miura　Yudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "debugproc.h"
#include "manager.h"
#include "Renderer.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPD3DXFONT CDebugProc::m_pFont = NULL;
char CDebugProc::m_aStr[7][1024] = {};

//=============================================================================
//コンストラクタ
//=============================================================================
CDebugProc::CDebugProc()
{

}

//=============================================================================
//デストラクタ
//=============================================================================
CDebugProc::~CDebugProc()
{

}

//=============================================================================
//デバックの初期化処理
//=============================================================================
void CDebugProc::Init(void)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(pDevice, 35, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);

	//バッファのクリア
	memset(&m_aStr[0], 0, sizeof(m_aStr));
}

//=============================================================================
//デバックの終了処理
//=============================================================================
void CDebugProc::Uninit(void)
{

}

//=============================================================================
//デバックの可変引数処理
//=============================================================================
void CDebugProc::Print(int nCnt2, const char * fmt, ...)
{
	char aStar[256] = {};
	int nCnt = 0;

	va_list ap;

	va_start(ap, fmt);

	while (*fmt)
	{
		if (*fmt == '%')
		{
			//カウントアップ
			fmt++;
			switch (*fmt)
			{
			case 's':
				nCnt += wsprintf(&aStar[nCnt], "%s", va_arg(ap, char *));
				break;
			case 'd':
				nCnt += wsprintf(&aStar[nCnt], "%d", va_arg(ap, int));
				break;
			case 'c':
				nCnt += wsprintf(&aStar[nCnt], "%c", va_arg(ap, char));
				break;
			case 'f':
				nCnt += sprintf(&aStar[nCnt], "%.2f", va_arg(ap, double));
				break;
			default:
				break;
			}
		}
		else
		{
			nCnt += wsprintf(&aStar[nCnt], "%c", *fmt);
		}

		//カウントアップ
		fmt++;
	}

	//終了
	va_end(ap);

	//テキストの描画
	strcat(m_aStr[nCnt2], aStar);
}

//=============================================================================
//デバックの描画処理
//=============================================================================
void CDebugProc::Draw(void)
{
	RECT rect0 = { 0,50,SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rect1 = { 0,300,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect2 = { 1640,820,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect3 = { 1740,820,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect4 = { 0, 600, SCREEN_WIDTH, SCREEN_HEIGHT };
	//RECT rect5 = { 1400,100,SCREEN_WIDTH,SCREEN_HEIGHT };
	//RECT rect6 = { 1400,350,SCREEN_WIDTH,SCREEN_HEIGHT };

	// テキスト描画
	m_pFont->DrawText(NULL, m_aStr[0], -1, &rect0, DT_LEFT, D3DCOLOR_ARGB(0xff, 255, 255, 15));
	m_pFont->DrawText(NULL, m_aStr[1], -1, &rect1, DT_LEFT, D3DCOLOR_ARGB(0xff, 255, 255, 15));
	m_pFont->DrawText(NULL, m_aStr[2], -1, &rect2, DT_LEFT, D3DCOLOR_ARGB(0xff, 0, 0, 15));
	m_pFont->DrawText(NULL, m_aStr[3], -1, &rect3, DT_LEFT, D3DCOLOR_ARGB(0xff, 0, 0, 15));
	m_pFont->DrawText(NULL, m_aStr[4], -1, &rect4, DT_LEFT, D3DCOLOR_ARGB(0xff, 255, 255, 15));
	//m_pFont->DrawText(NULL, m_aStr[5], -1, &rect5, DT_LEFT, D3DCOLOR_ARGB(0xff, 255, 255, 15));
	//m_pFont->DrawText(NULL, m_aStr[6], -1, &rect6, DT_LEFT, D3DCOLOR_ARGB(0xff, 255, 255, 15));

	//バッファのクリア
	memset(&m_aStr[0], 0, sizeof(m_aStr));
}
