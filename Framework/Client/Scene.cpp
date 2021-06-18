#include "framework.h"
#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
    Release();
}

HRESULT CScene::Initialize()
{
	m_pDeviceMgr = CDeviceMgr::GetInstance();
	if (!m_pDeviceMgr)
		return E_FAIL;

    m_pGameMgr = CGameMgr::GetInstance();
    if (!m_pGameMgr)
        return E_FAIL;

	m_pInputMgr = CInputMgr::GetInstance();
	if (!m_pInputMgr)
		return E_FAIL;

    return NOERROR;
}

INT CScene::Update(const FLOAT& fTimeDelta)
{
    return 0;
}

VOID CScene::Release()
{
    m_pGameMgr->Clear_ObjList();
}
