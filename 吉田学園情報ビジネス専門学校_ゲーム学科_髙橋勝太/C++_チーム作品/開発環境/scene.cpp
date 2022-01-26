//=============================================================================
//
// リスト処理 [scene.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "camera.h"

// 静的メンバ関数
int CScene::m_NumAll = 0;
CScene *CScene::m_pTop[6][3];
CScene *CScene::m_pCur[6][3];

//=============================================================================
// コンストラクタ
//=============================================================================
CScene::CScene(int nPriority, int nType)
{
	m_nPriority = nPriority;
	m_nType = nType;

	// Topに何も入ってないとき
	if (m_pTop[nPriority][nType] == NULL)		// NULLの時
	{
		m_pTop[nPriority][nType] = this;		// Topにアドレスを入れる
		this->m_pNext = NULL;
		this->m_pPrev = NULL;
	}

	else // 最後尾
	{
		// リストへ追加
		m_pCur[nPriority][nType]->m_pNext = this;	// 最後尾の次が自分			気づいた
		this->m_pPrev = m_pCur[nPriority][nType];	// 自分の前を最後尾にする	認識し合う
		this->m_pNext = NULL;
	}

	m_pCur[nPriority][nType] = this;		// 最後尾ですよ

											// 死亡フラグの初期化
	m_bDeath = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// すべての破棄
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCntType = 0; nCntType < 3; nCntType++)
	{
		for (int nCntPriority = 0; nCntPriority < 6; nCntPriority++)
		{
			CScene *pScene = m_pTop[nCntPriority][nCntType];

			while (pScene)
			{
				// ポインタを記憶させる
				CScene *pSceneNext = pScene->m_pNext;

				// 更新処理
				pScene->Uninit();

				// 記憶した情報を受け渡す
				pScene = pSceneNext;
			}
		}
	}
}

//=============================================================================
// すべての更新処理　継承先
//=============================================================================
void CScene::UpdateAll(void)
{
	// ゲーム画面
	if (CManager::GetPause() == false)
	{
		for (int nCntType = 0; nCntType < 2; nCntType++)
		{
			// ゲーム画面のみを描画
			{
				for (int nCntPriority = 0; nCntPriority < 6; nCntPriority++)
				{
					CScene *pScene = m_pTop[nCntPriority][nCntType];

					while (pScene)
					{
						// ポインタを記憶させる
						CScene *pSceneNext = pScene->m_pNext;

						if (pScene->m_bDeath == false)
						{
							// 更新処理
							pScene->Update();
						}

						// 記憶した情報を受け渡す
						pScene = pSceneNext;
					}
				}
			}
		}
	}

	// ポーズ画面
	if (CManager::GetPause() == true)
	{
		for (int nCntType = 0; nCntType < 3; nCntType++)
		{
			// ポーズ画面とフェイド画面の場合
			if (nCntType == 1 || nCntType == 2)
			{
				for (int nCntPriority = 0; nCntPriority < 6; nCntPriority++)
				{
					CScene *pScene = m_pTop[nCntPriority][nCntType];

					while (pScene)
					{
						// ポインタを記憶させる
						CScene *pSceneNext = pScene->m_pNext;

						if (pScene->m_bDeath == false)
						{
							// 更新処理
							pScene->Update();
						}

						//記憶した情報を受け渡す
						pScene = pSceneNext;
					}
				}
			}
		}
	}

	// 死亡フラグをチェックしてオブジェクトを破棄
	for (int nCntType = 0; nCntType < 3; nCntType++)
	{
		for (int nCntPriority = 0; nCntPriority < 6; nCntPriority++)
		{
			// 先頭オブジェクト情報の取得
			CScene *pScene = m_pTop[nCntPriority][nCntType];

			// 最後尾までループ
			while (pScene)
			{
				// 次の情報を取得
				CScene *pSceneNext = pScene->m_pNext;

				if (pScene->m_bDeath == true)
				{
					// 前後に自分が消えたことを知らせる
					if (pScene->m_pPrev != NULL)
					{
						pScene->m_pPrev->m_pNext = pScene->m_pNext;
					}

					if (pScene->m_pNext != NULL)
					{
						pScene->m_pNext->m_pPrev = pScene->m_pPrev;
					}

					if (pScene == m_pCur[nCntPriority][nCntType])
					{
						m_pCur[nCntPriority][nCntType] = pScene->m_pPrev;
					}

					if (pScene == m_pTop[nCntPriority][nCntType])
					{
						m_pTop[nCntPriority][nCntType] = pScene->m_pNext;
					}

					// 情報を消す
					delete pScene;
					pScene = NULL;
				}

				// 次の情報にする
				pScene = pSceneNext;
			}
		}
	}
}

//=============================================================================
// すべての描画処理　継承先
//=============================================================================
void CScene::DrawAll(void)
{
	// ゲーム画面
	if (CManager::GetPause() == false)
	{
		for (int nCntType = 0; nCntType < 2; nCntType++)
		{
			for (int nCntPriority = 0; nCntPriority < 6; nCntPriority++)
			{
				CScene *pScene = m_pTop[nCntPriority][nCntType];

				while (pScene)
				{
					// ポインタを記憶させる
					CScene *pSceneNext = pScene->m_pNext;

					// 更新処理
					pScene->Draw();

					// 次の情報にする
					pScene = pSceneNext;
				}
			}
		}
	}

	// ポーズ画面
	if (CManager::GetPause() == true)
	{
		for (int nCntType = 0; nCntType < 3; nCntType++)
			{
		for (int nCntPriority = 0; nCntPriority < 6; nCntPriority++)
		{
			
				CScene *pScene = m_pTop[nCntPriority][nCntType];

				while (pScene)
				{
					// ポインタを記憶させる
					CScene *pSceneNext = pScene->m_pNext;

					// 更新処理
					pScene->Draw();

					// 次の情報にする
					pScene = pSceneNext;
				}
			}
		}
	}
}

//=============================================================================
// Get関数
//=============================================================================
CScene * CScene::GetScene(int nIdx, int nPriority)
{
	// 先頭オブジェクト情報の取得
	CScene *pScene = m_pTop[nPriority][0];

	int nCnt = 0;	// 計算用

					// 抽出したいポインタまでループ
	while (pScene)
	{
		// ポインタを記憶させる
		CScene *pSceneNext = pScene->m_pNext;

		// 抽出したいポインタの場合
		if (nIdx == nCnt)
		{
			break;			// ループから抜け出す
		}

		// 次のポインタに移す
		pScene = pSceneNext;

		// ずらす
		nCnt++;
	}

	return pScene;
}

//=============================================================================
// 位置管理
//=============================================================================
D3DXVECTOR3 CScene::GetPosition(void)
{
	return D3DXVECTOR3();
}

//=============================================================================
// 破棄
//=============================================================================
void CScene::Release(void)
{
	// 死亡フラグ
	this->m_bDeath = true;
}
