#include "framework.h"
#include "Scene_Logo.h"
#include "Scene_Stage.h"

#include "Player.h"
#include "Camera.h"

CScene_Logo::CScene_Logo()
{
}

CScene_Logo::~CScene_Logo()
{
	Release();
}

HRESULT CScene_Logo::Initialize()
{
	if (FAILED(CScene::Initialize())) return E_FAIL;

	cout << "LogoScene" << endl;

	m_pDeviceMgr->Reset_CommandLst();

	if (FAILED(m_pGameMgr->Add_GameObj(CGameMgr::PLAYER, CPlayer::Create())))
		return E_FAIL;

	if (FAILED(m_pGameMgr->Add_GameObj(CGameMgr::BASE, CCamera::Create(nullptr))))
		return E_FAIL;

	m_pDeviceMgr->Close_CommandLst();
	m_pDeviceMgr->Wait_GPU();

	return NOERROR;
}

INT CScene_Logo::Update(const FLOAT& fTimeDelta)
{
	if (m_pInputMgr->KeyDown(KEY_ENTER))
		m_pGameMgr->Set_CurrScene(CScene_Stage::Create());

	return 0;
}

VOID CScene_Logo::Release()
{
	m_pGameMgr->Clear_ObjList();
}

CScene_Logo* CScene_Logo::Create()
{
	CScene_Logo* pInstance = new CScene_Logo;
	if (FAILED(pInstance->Initialize()))
		SafeDelete(pInstance);
	return pInstance;
}
