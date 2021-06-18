#include "framework.h"
#include "Camera.h"

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
	Release();
}

HRESULT CCamera::Initialize(CObj* pOwner)
{
	if (!pOwner)
		return E_FAIL;

	m_pOwner = pOwner;

	m_fFovY = XMConvertToRadians(60.f);
	m_fAspect = float(g_nWinCX / g_nWinCY);
	m_fNear = 0.1f;
	m_fFar = 1000.f;
	m_matProj = XMMatrixPerspectiveFovLH(m_fFovY, m_fAspect, m_fNear, m_fFar);

	m_vEye = { 0.f, 1.f, -1.f, 1.f };
	m_vAt = { 0.f, 0.f, 0.f, 1.f };
	m_vUp = { 0.f, 1.f, 0.f, 0.f };
	m_matView = XMMatrixLookAtLH(m_vEye, m_vAt, m_vUp);

	return NOERROR;
}

INT CCamera::Update(const FLOAT& fTimeDelta)
{

	return 0;
}

INT CCamera::LateUpdate(const FLOAT& fTimeDelta)
{
	return 0;
}

VOID CCamera::Render()
{
}

VOID CCamera::Release()
{
}

CCamera* CCamera::Create(CObj* pOwner)
{
	CCamera* pInstance = new CCamera;
	if (FAILED(pInstance->Initialize(pOwner)))
		SafeDelete(pInstance);
	return pInstance;
}
