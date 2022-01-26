//==============================================================================
//
// 荷物を届ける処理処理[deliver.h]
// Author; takahashi syota
//
//==============================================================================

#ifndef _DELIVERY_H_
#define _DELIVERY_H_
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//==============================================================================
//インクルード読み込み
//==============================================================================
#include "main.h"
#include "scene3D.h"

//==============================================================================
//荷物クラスの定義
//==============================================================================
class CDelivery : public CScene3D
{
public:
	CDelivery();                                                            //コンストラクタ
	~CDelivery();                                                           //デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);              //初期化処理
	void Uninit(void);												        //終了処理
	void Update(void);												        //更新処理
	void Draw(void);												        //描画処理

	static CDelivery *Create(D3DXVECTOR3 pos);								//生成処理

private:
	D3DXVECTOR3 m_pos;						                                //位置情報
	D3DXVECTOR3 m_move;														//移動量
	D3DXVECTOR3 m_rot;						                                //回転情報
	D3DXVECTOR3 m_rotDest;						                            //回転情報
	D3DXCOLOR	m_col;						                                //カラー設定
	int			m_nLife;													//寿命
	bool		m_bUseAction;												//描画許可

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9	m_pTexture;									//テクスチャへのポインタ
	static int m_nNumber;													//届け番号
};
#endif