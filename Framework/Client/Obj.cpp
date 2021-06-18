#include "framework.h"
#include "Obj.h"

#include "InputMgr.h"
#include "Component.h"

CObj::CObj()
{
}

CObj::~CObj()
{
}

HRESULT CObj::Initialize()
{
	m_pInputMgr = CInputMgr::GetInstance();
	if (!m_pInputMgr)
		return E_FAIL;

	m_pGameMgr = CGameMgr::GetInstance();
	if (!m_pGameMgr)
		return E_FAIL;

	m_pDeviceMgr = CDeviceMgr::GetInstance();
	if (!m_pDeviceMgr)
		return E_FAIL;

	m_pShaderMgr = CShaderMgr::GetInstance();
	if (!m_pShaderMgr)
		return E_FAIL;

	return NOERROR;
}

INT CObj::Update(const FLOAT& fTimeDelta)
{
	for_each(m_lstComponent.begin(), m_lstComponent.end(), [fTimeDelta](CComponent* p) { p->Update(fTimeDelta); });

	return 0;
}

INT CObj::LateUpdate(const FLOAT& fTimeDelta)
{
	for_each(m_lstComponent.begin(), m_lstComponent.end(), [fTimeDelta](CComponent* p) { p->LateUpdate(fTimeDelta); });

	return 0;
}

VOID CObj::Render()
{
	for_each(m_lstComponent.begin(), m_lstComponent.end(), [](CComponent* p) { p->Render(); });

	return VOID();
}

HRESULT CObj::Add_Component(CComponent* pComponent)
{
	if (!pComponent)
		return E_FAIL;

	m_lstComponent.emplace_back(pComponent);

	return NOERROR;
}

VOID CObj::Release()
{
	for_each(m_lstComponent.begin(), m_lstComponent.end(), [](CComponent* p) { SafeDelete(p); });
}
