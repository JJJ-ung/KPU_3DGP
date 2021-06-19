#include "framework.h"
#include "Camera.h"

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
	Release();
}

HRESULT CCamera::Initialize()
{
	if (FAILED(CObj::Initialize()))
		return E_FAIL;

	m_pTransform = CTransform::Create();
	if (!m_pTransform)
		return E_FAIL;

	m_fFovY = XMConvertToRadians(60.f);
	m_fAspect = float(g_nWinCX) / float(g_nWinCY);
	m_fNear = 0.1f;
	m_fFar = 100000.f;
	m_matProj = XMMatrixPerspectiveFovLH(m_fFovY, m_fAspect, m_fNear, m_fFar);

	return NOERROR;
}

INT CCamera::Update(const FLOAT& fTimeDelta)
{
	return 0;
}

INT CCamera::LateUpdate(const FLOAT& fTimeDelta)
{
	m_matView = XMMatrixInverse(nullptr, m_pTransform->GetWorld());
	m_pGameMgr->Set_MatrixInfo(m_matProj, m_matView);
	m_pLightMgr->Update_CameraPos(m_pTransform->GetState(CTransform::POSITION));

	return 0;
}

VOID CCamera::Render()
{
}

VOID CCamera::Release()
{
}

CCamera* CCamera::Create()
{
	CCamera* pInstance = new CCamera;
	if (FAILED(pInstance->Initialize()))
		SafeDelete(pInstance);
	return pInstance;
}
