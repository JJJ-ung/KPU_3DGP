#include "framework.h"
#include "Obj.h"

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

	return NOERROR;
}

INT CObj::Update(const FLOAT& fTimeDelta)
{
	return 0;
}

VOID CObj::Render()
{
	return VOID();
}

VOID CObj::Release()
{
	return VOID();
}
