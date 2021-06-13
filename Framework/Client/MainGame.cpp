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
	if (FAILED(m_pDeviceMgr->Init_GraphicDevice(VEC4(1.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	return NOERROR;
}

INT CMainGame::Update(const float& fTimeDelta)
{
	return 0;
}

INT CMainGame::Render()
{
	m_pDeviceMgr->RenderBegin();
	// Render

	m_pDeviceMgr->RenderEnd();

	return 0;
}

VOID CMainGame::Release()
{
	m_pDeviceMgr->DestroyInstance();
}

CMainGame* CMainGame::Create()
{
	CMainGame* pInstance = new CMainGame;
	if (FAILED(pInstance->Initialize()))
		SafeDelete(pInstance);
	return pInstance;
}
