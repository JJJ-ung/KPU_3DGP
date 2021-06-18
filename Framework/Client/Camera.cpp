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
	//if (!pOwner)
	//	return E_FAIL;

	if (FAILED(CObj::Initialize()))
		return E_FAIL;

	m_pOwner = pOwner;

	m_fFovY = XMConvertToRadians(60.f);
	m_fAspect = float(g_nWinCX) / float(g_nWinCY);
	m_fNear = 0.1f;
	m_fFar = 1000.f;
	m_matProj = XMMatrixPerspectiveFovLH(m_fFovY, m_fAspect, m_fNear, m_fFar);

	m_vEye = { 0.f, 0.f, -30.f, 1.f };
	m_vAt = { 0.f, 0.f, 0.f, 1.f };
	m_vUp = { 0.f, 1.f, 0.f, 0.f };
	m_matView = XMMatrixLookAtLH(m_vEye, m_vAt, m_vUp);

	m_pGameMgr->Set_MatrixInfo(m_matProj, m_matView);

	return NOERROR;
}

INT CCamera::Update(const FLOAT& fTimeDelta)
{
	if (CInputMgr::GetInstance()->KeyPressing(KEY_W))
	{
		m_vEye += XMVECTOR{ 0.f, 1.f * fTimeDelta, 0.f, 0.f };
		m_vAt += XMVECTOR{ 0.f, 1.f * fTimeDelta, 0.f, 0.f };
	}

	if (CInputMgr::GetInstance()->KeyPressing(KEY_S))
	{
		m_vEye += XMVECTOR{ 0.f, -1.f * fTimeDelta, 0.f, 0.f };
		m_vAt += XMVECTOR{ 0.f, -1.f * fTimeDelta, 0.f, 0.f };
	}

	if (CInputMgr::GetInstance()->KeyPressing(KEY_A))
	{
		m_vEye += XMVECTOR{ 0.f, 0.f, 1.f * fTimeDelta, 0.f };
		m_vAt += XMVECTOR{ 0.f, 0.f, 1.f * fTimeDelta, 0.f };
	}

	if (CInputMgr::GetInstance()->KeyPressing(KEY_D))
	{
		m_vEye += XMVECTOR{ 0.f, 0.f, -1.f * fTimeDelta, 0.f };
		m_vAt += XMVECTOR{ 0.f, 0.f, -1.f * fTimeDelta, 0.f };
	}

	return 0;
}

INT CCamera::LateUpdate(const FLOAT& fTimeDelta)
{
	m_matView = XMMatrixLookAtLH(m_vEye, m_vAt, m_vUp);

	m_pGameMgr->Set_MatrixInfo(m_matProj, m_matView);

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
