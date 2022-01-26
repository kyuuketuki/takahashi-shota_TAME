//=============================================================================
//
// レンダラー処理 [renderer.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _RENDERE_H_
#define _RENDERE_H_

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"

//前方宣言
class CDebugProc;

//=============================================================================
// レンダラークラスの定義
//=============================================================================
class CRenderer
{
public:
	CRenderer();								//コンストラクタ
	~CRenderer();								//デストラクタ
	HRESULT Init(HWND hWnd, bool bWindow);						//初期化処理
	void Uninit(void);											//終了処理
	void Update(void);											//更新処理
	void Draw(void);											//描画処理
	void Draw2(void);											//描画処理(残像あり)

	//デバイス情報
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

	//マルチターゲット関数
	void ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU);
	LPDIRECT3DTEXTURE9 GetTextureMT(void) { return m_pTextureMT[0]; }

private:
	void DrawFPS();

	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffMT;				//バッファ
	LPDIRECT3D9					m_pD3D = NULL;				// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9			m_pD3DDevice = NULL;		// Deviceオブジェクト(描画に必要)
	LPD3DXFONT					m_pFont = NULL;				// フォントへのポインタ

	//マルチターゲット変数
	LPDIRECT3DTEXTURE9 m_pTextureMT[2];
	LPDIRECT3DSURFACE9 m_pRenderMT[2];
	LPDIRECT3DSURFACE9 m_pBuffMT[2];
	D3DVIEWPORT9 m_ViewportMT;

	//静的メンバ変数
	static CDebugProc *m_pDebug;							//デバック表示のメンバ変数
};

#endif
