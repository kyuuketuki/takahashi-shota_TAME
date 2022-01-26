#include "map.h"
#include "city.h"
#include "manager.h"

//静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CMap::m_pTexture[MAX_MAP_TEXTURE] = {};

//==============================================================================
// コンストラクタ
//==============================================================================
CMap::CMap(int nPriority, int nType)
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CMap::~CMap()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CMap::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//モデルを生成
	char *TextName[MAX_MAP_OPBJECT];

	TextName[0] = "data/MODEL/map/map01.x";
	TextName[1] = "data/MODEL/map/map02.x";
	TextName[2] = "data/MODEL/map/map03.x";
	TextName[3] = "data/MODEL/map/map04.x";
	TextName[4] = "data/MODEL/map/map05.x";
	TextName[5] = "data/MODEL/map/map06.x";
	TextName[6] = "data/MODEL/map/map07.x";
	TextName[7] = "data/MODEL/map/map08.x";
	TextName[8] = "data/MODEL/map/map09.x";
	TextName[9] = "data/MODEL/map/map10.x";
	TextName[10] = "data/MODEL/map/map11.x";
	TextName[11] = "data/MODEL/map/map12.x";
	TextName[12] = "data/MODEL/map/map13.x";
	TextName[13] = "data/MODEL/map/map14.x";
	TextName[14] = "data/MODEL/map/map15.x";
	TextName[15] = "data/MODEL/map/map16.x";
	TextName[16] = "data/MODEL/map/map17.x";
	TextName[17] = "data/MODEL/map/map18.x";
	TextName[18] = "data/MODEL/map/map19.x";
	TextName[19] = "data/MODEL/map/map20.x";
	TextName[20] = "data/MODEL/map/map21.x";
	TextName[21] = "data/MODEL/map/map22.x";
	TextName[22] = "data/MODEL/map/map23.x";
	TextName[23] = "data/MODEL/map/map24.x";
	TextName[24] = "data/MODEL/map/map25.x";
	TextName[25] = "data/MODEL/map/map26.x";
	TextName[26] = "data/MODEL/map/map27.x"; 
	TextName[27] = "data/MODEL/map/map28.x";
	TextName[28] = "data/MODEL/map/map29.x";

	for (int nOj = 0; nOj < MAX_MAP_OPBJECT; nOj++)
	{
		m_apMap[nOj] = CCity::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), TextName[nOj]);
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ローカル変数宣言
	D3DXMATERIAL *pMat;		// マテリアルへのポインタ

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_apMap[0]->GetBuff()->GetBufferPointer();

	// テクスチャの読み込み
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/LM_Basketball.png       ", &m_pTexture[0]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/City_Props.png          ", &m_pTexture[1]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/City_Props.png          ", &m_pTexture[2]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/LM_Clinic.png           ", &m_pTexture[3]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Filler_Buildings_1.png  ", &m_pTexture[4]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Filler_Buildings_1.png  ", &m_pTexture[5]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/LM_FishFactory.png      ", &m_pTexture[6]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/LM_FishFactory.png      ", &m_pTexture[7]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/noname.png              ", &m_pTexture[8]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/LM_Projects.png         ", &m_pTexture[9]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/LM_Projects.png         ", &m_pTexture[10]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/LM_Laundrymat.png       ", &m_pTexture[11]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Paramount.png           ", &m_pTexture[12]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Paramount.png           ", &m_pTexture[13]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/LM_Pawnshop.png         ", &m_pTexture[14]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/LM_Pawnshop.png         ", &m_pTexture[15]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/noname.png              ", &m_pTexture[16]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Roads_Grounds000.png    ", &m_pTexture[17]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Roads_Grounds.png       ", &m_pTexture[18]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Lotus_wheel_large.png   ", &m_pTexture[19]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/noanme.png              ", &m_pTexture[20]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/noname.png              ", &m_pTexture[21]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Lotus_Yellow.png        ", &m_pTexture[22]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/LOWRISECONDO1_d.png     ", &m_pTexture[23]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/45building_d.png        ", &m_pTexture[24]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building001.png         ", &m_pTexture[25]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building002.png         ", &m_pTexture[26]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/noname.png              ", &m_pTexture[27]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building003_2.jpg       ", &m_pTexture[28]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building003_1.jpg       ", &m_pTexture[29]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building004.jpg         ", &m_pTexture[30]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building005.jpg         ", &m_pTexture[31]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building006.png         ", &m_pTexture[32]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building007.png         ", &m_pTexture[33]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building008.png         ", &m_pTexture[34]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building009.png         ", &m_pTexture[35]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building010.png         ", &m_pTexture[36]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building011.png         ", &m_pTexture[37]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building012.png         ", &m_pTexture[38]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building013.png         ", &m_pTexture[39]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building014.png         ", &m_pTexture[40]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building015.png         ", &m_pTexture[41]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building016.png         ", &m_pTexture[42]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building017.png         ", &m_pTexture[43]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building018.png         ", &m_pTexture[44]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building019.png         ", &m_pTexture[45]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building020.png         ", &m_pTexture[46]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building021.png         ", &m_pTexture[47]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building022.png         ", &m_pTexture[48]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building023.png         ", &m_pTexture[49]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building024.png         ", &m_pTexture[50]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building025.jpg         ", &m_pTexture[51]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/building017.png         ", &m_pTexture[52]);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Grass000.jpg            ", &m_pTexture[53]);
	}

	for (int nOj = 0; nOj < MAX_MAP_OPBJECT; nOj++)
	{
		//テクスチャ情報を受け渡す
		for (int n = 0; n < MAX_MAP_TEXTURE; n++)
		{
			//テクスチャ受け渡し
			m_apMap[nOj]->SetTexture(n, m_pTexture[n]);
		}
	}

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CMap::Uninit(void)
{
	for (int nCntObject = 0; nCntObject < MAX_MAP_OPBJECT; nCntObject++)
	{
		if (m_apMap[nCntObject] != NULL)
		{
			//オブジェクト破棄
			m_apMap[nCntObject]->Uninit();
			delete m_apMap[nCntObject];
		}
	}

	//オブジェクトの破棄
	Release();
}

//==============================================================================
// 更新処理
//==============================================================================
void CMap::Update(void)
{

}

//==============================================================================
// 描画処理
//==============================================================================
void CMap::Draw(void)
{
	for (int n = 0; n < MAX_MAP_OPBJECT; n++)
	{
		if (m_apMap[n] != NULL)
		{
			m_apMap[n]->Draw();
		}
	}
}

//==============================================================================
// 生成処理
//==============================================================================
CMap * CMap::Create()
{
	//プレイヤーを生成
	CMap *pMap = NULL;
	pMap = new CMap;

	//NULLチェック
	if (pMap != NULL)
	{
		pMap->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
	}

	return pMap;
}