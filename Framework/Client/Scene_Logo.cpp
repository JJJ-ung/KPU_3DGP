#include "framework.h"
#include "Scene_Logo.h"
#include "Scene_Stage.h"

#include "Player.h"
#include "RacingMap.h"
#include "FollowCamera.h"
#include "FreeCamera.h"

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

	if (FAILED(m_pGameMgr->Add_GameObj(CGameMgr::MAP, CRacingMap::Create())))
		return E_FAIL;

	CObj* pObj = CPlayer::Create();
	if (FAILED(m_pGameMgr->Add_GameObj(CGameMgr::PLAYER, pObj)))
		return E_FAIL;

	//if (FAILED(m_pGameMgr->Add_GameObj(CGameMgr::BASE, CFollowCamera::Create(pObj->Get_Transform(), XMVECTOR{0.f, 50.f, -30.f, 0.f}))))
	//	return E_FAIL;

	//if (FAILED(m_pGameMgr->Add_GameObj(CGameMgr::BASE, CFreeCamera::Create())))
	//	return E_FAIL;
	if (FAILED(m_pGameMgr->Add_GameObj(CGameMgr::CAMERA, CFollowCamera::Create(pObj->Get_Transform(), XMVectorSet(0.f, 10.f, 0.f, 0.f), 50.f))))
		return E_FAIL;

	m_pLightMgr->Add_Directional(0, XMFLOAT4{0.f, -1.f, 1.f, 0.f}, XMFLOAT4{0.5f, 0.5f, 0.5f, 1.f}, XMFLOAT4{0.1f, 0.1f, 0.1f, 1.f}, XMFLOAT4{1.f, 1.f, 1.f, 1.f});

	m_pDeviceMgr->Close_CommandLst();
	m_pDeviceMgr->Wait_GPU();

	return NOERROR;
}

INT CScene_Logo::Update(const FLOAT& fTimeDelta)
{
	//if (m_pInputMgr->KeyDown(KEY_ENTER))
	//	m_pGameMgr->Set_CurrScene(CScene_Stage::Create());

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
