//=============================================================================
//
// シーンX　ポイント　処理 [pointerX.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "pointerX.h"
#include "sceneX.h"
#include "player3D.h"
#include "destination.h"

int g_PointNumber = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CPointer::CPointer(int nPriority, int nType)
{
	g_PointNumber = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPointer::~CPointer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPointer::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	CSceneX::Init(pos, fSizeX, fSizeY);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPointer::Uninit(void)
{
	CSceneX::Uninit();

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPointer::Update(void)
{
	//位置情報を受け取る
	D3DXVECTOR3 p = CPlayer3D::GetPosition();
	m_rot = CSceneX::GetRevolution();

	//位置をプレイヤーの上に設定
	p = D3DXVECTOR3(p.x, 100.0f, p.z);
	
	//目的地を設定
	D3DXVECTOR3 posGoal = CDestination::GetPosition(g_PointNumber);
	
	//2頂点差を計算
	D3DXVECTOR3 a = p - posGoal;

	//角度を計算
	float nAtan2 = (float)atan2(a.z, a.x);
	m_rot.y = -nAtan2;

	//位置の更新
	CSceneX::SetPosition(p);
	CSceneX::SetRevolution(m_rot);
}

//=============================================================================
// 描画処理
//=============================================================================
void CPointer::Draw(void)
{
	CSceneX::Draw();
}

//=============================================================================
// 生成処理
//=============================================================================
CPointer * CPointer::Create(D3DXVECTOR3 pos, char * TextName)
{
	// ローカル変数宣言
	CPointer *pPointer = NULL;
	pPointer = new CPointer;

	pPointer->BindTextName(TextName);

	// 初期化
	if (pPointer != NULL)
	{
		pPointer->Init(pos, 0, 0);
	}

	return pPointer;
}

void CPointer::SetPointNumber(int nNumber)
{
	g_PointNumber = nNumber;
}
