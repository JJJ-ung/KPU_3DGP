#include "framework.h"
#include "MainGame.h"

#include "DeviceMgr.h"

CMainGame::CMainGame()
{
}

CMainGame::~CMainGame()
{
	Release();
}

HRESULT CMainGame::Initialize()
{
	m_pDeviceMgr = CDeviceMgr::GetInstance();
	if (FAILED(m_pDeviceMgr->Init_GraphicDevice()))
		return E_FAIL;

	m_pInputMgr = CInputMgr::GetInstance();
	if (FAILED(m_pInputMgr->Init_InputDev()))
		return E_FAIL;

	return NOERROR;
}

INT CMainGame::Update(const float& fTimeDelta)
{
	m_pInputMgr->UpdateKey();

	return 0;
}

INT CMainGame::Render()
{
	if (FAILED(m_pDeviceMgr->RenderBegin()))
		return -1;

	// Render

	if (FAILED(m_pDeviceMgr->RenderEnd()))
		return -1;

	return 0;
}

VOID CMainGame::Release()
{
	CDeviceMgr::GetInstance()->DestroyInstance();
}

CMainGame* CMainGame::Create()
{
	CMainGame* pInstance = new CMainGame;
	if (FAILED(pInstance->Initialize()))
		SafeDelete(pInstance);
	return pInstance;
}
