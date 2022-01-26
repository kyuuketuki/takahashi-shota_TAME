//=============================================================================
//
// 敵処理 [enemy.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "player.h"
#include "scene2D.h"

//マクロ定義
#define ENEMY_SIZE (70)

//=============================================================================
// 敵クラスの定義
//=============================================================================
class CEnemy : public CScene2D
{
public:
	CEnemy(int nPriority = 1);				//コンストラクタ
	virtual ~CEnemy();						//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fmove, D3DXVECTOR3 objective, int nLife, float fSizeX, float fSizeY, int nTime, int nType, int nMoveType);	   //初期化処理
	void Uninit(void);																																   //終了処理
	void Update(void);																																   //更新処理
	void Draw(void);																																   //描画処理

	void Collision(void);		//当たり判定	

	void Attack(void);
	void Move(void);
																																					   
	//生成処理																																				   																															   //テクスチャ破棄
	static CEnemy *Create(D3DXVECTOR3 pos, float fmove, D3DXVECTOR3 objective, int nLife, float fSizeX, float fSizeY, int nTime, int nType, int nMoveType);

	//特殊更新
	static void NextObjective(D3DXVECTOR3 pos, float fMove, int nType);
	static void NextObjectiveAttack(D3DXVECTOR3 pos, float fMove, int nType);

	static void NextAttack(int NextAttack);
	static void NextAttack2(int NextAttack);
	static void NextAttack3(int NextAttack);

	static void NextMove(D3DXVECTOR3 pos, float fMove, int nType);
	static void MoveClear(void);

	//テクスチャ関連
	static HRESULT Load(void);
	static void Unload(void);

private:
	//静的メンバ変数
	static CPlayer				*m_pPlayer;				//プレイヤーへのポインタ	ホーミング時に使用
	static LPDIRECT3DTEXTURE9	m_pTexture[ENEMY_MAX];	//テクスチャへのポインタ
														
	//メンバ変数
	D3DXVECTOR3					m_pos;					//位置
	D3DXVECTOR3					m_objective;			//ゴール地点
	D3DXCOLOR					m_col;					//色情報
	int							m_nLife;				//寿命
	int							m_nAttack;				//攻撃手段
	int							m_nTime;				//行動を管理する時間
	int							m_nType;				//敵の種類をタイプで管理
	int							m_nMoveType;			//動きを統一させるタイプ
	float						m_fMove;				//移動スピード
	float						m_fSizeX;				//サイズ
	float						m_fSizeY;				//サイズ
	float						m_fMoveEnemy;			//敵の移動速度

	int							m_nAttackCnt;			//攻撃するタイミング
	int							m_nAttackCnt2;			//攻撃するタイミング
	int							m_nAttackCnt3;			//攻撃するタイミング

	//角度計算用
	float						m_fAngleEnemyBullet = 0.0f;
	float						m_fAngleEnemyBullet2 = 0.0f;
	float						m_fAngleEnemyBullet3 = 0.0f;

	bool						m_bAttackPattern;		//攻撃パターン用
	bool						m_bMovEnemy;			//移動するか判定
	bool						m_bAngleEnemy = false;	//弾の発射用

	//敵の無敵時間
	int							m_nHitTime;
	bool						m_bInvincibility = false;
};

#endif