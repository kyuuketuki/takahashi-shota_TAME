//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
// インクルードファイル読み込み
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene3D.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define DATA_LENGTH (64)

//=============================================================================
// パーティクルクラスの定義
//=============================================================================
class CParticle : public CScene3D
{
public:
	CParticle(int nPriority = 0, int nType = 0);								//コンストラクタ
	virtual ~CParticle();														//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fX, float fY, float fSpeed, D3DXCOLOR col, int nLife, float fGravity);			//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理

	static CParticle *Create(D3DXVECTOR3 pos, float fX, float fY, float fSpeed, D3DXCOLOR col, int nLife, float fGravity);//生成処理

	static void Load(void);                                                 //テクスチャロード処理
	void SetParticle(D3DXVECTOR3 pos, float fX, float fY, float fSpeed, D3DXCOLOR col, int nLife, float fGravity);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャ情報
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_move;
	float m_fAngle;
	int m_nType;

	//テキストから読み込み用メンバ変数
	int m_nCountText;                  //パーティクルの数
	float m_SizeX;                     //パーティクルのサイズ
	float m_SizeY;                     //パーティクルのサイズ
	float m_fSpeed;                    //パーティクルのスピード
	D3DXCOLOR m_col;                   //パーティクルのカラー
	int m_nLife;                       //パーティクルの寿命
	float m_fGravity;                  //パーティクルの重力
	int m_nLifeGravity;                //パーティクルの重力発生用変数
};
#endif
