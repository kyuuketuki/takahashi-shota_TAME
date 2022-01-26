//=============================================================================
//
//フェード処理[fade.h]
// Author; takahashi
//
//=============================================================================

#ifndef _FADE_H_
#define _FADE_H_

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "Scene.h"
#include "manager.h"

//=============================================================================
//フェードクラスの定義
//=============================================================================
class CFade : public CScene
{
public:
	//フェードの状態
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_MAX
	}FADE;

	CFade(int nPriority = 4, int nType = 1);				//コンストラクタ
	~CFade();												//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);  //初期化処理
	void Uninit(void);											//終了処理
	void Update(void);											//更新処理
	void Draw(void);											//描画処理

	//生成処理
	static CFade *Create(CManager::MODE modenext);
	
	//受け渡し
	D3DXVECTOR3		GetAllPosition(void) { return D3DXVECTOR3(); }
	FADE			GetFade(void) { return m_fade; }
	static void		SetFade(FADE fade, CManager::MODE modeNext);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							//頂点情報
	D3DXVECTOR3				m_pos;								//位置情報
	D3DXCOLOR				m_col;								//カラー情報

	//静的メンバ変数
	static CManager::MODE m_modenext;							//次のモード情報を記憶
	static FADE			m_fade;									//モード情報
	
};
#endif
