#include "framework.h"
#include "FollowCamera.h"
#include "Transform.h"

CFollowCamera::CFollowCamera()
{
}

CFollowCamera::~CFollowCamera()
{
}

HRESULT CFollowCamera::Initialize(CTransform* pTarget, XMVECTOR vOffset, FLOAT fDistance)
{
	if (FAILED(CCamera::Initialize()))
		return E_FAIL;

	if (!pTarget)
		return E_FAIL;
	m_pTarget = pTarget;

	m_vOffset = vOffset;
	m_fDistance = fDistance;

	Setup_CameraInfo();

	return NOERROR;
}

INT CFollowCamera::Update(const FLOAT& fTimeDelta)
{
	Setup_CameraInfo();

	POINT		ptCursorPos = { g_nWinCX >> 1, g_nWinCY >> 1 };

	ClientToScreen(g_hWnd, &ptCursorPos);

	SetCursorPos(ptCursorPos.x, ptCursorPos.y);

	return 0;
}

INT CFollowCamera::LateUpdate(const FLOAT& fTimeDelta)
{
	CCamera::LateUpdate(fTimeDelta);

	return 0;
}

VOID CFollowCamera::Render()
{
	return VOID();
}

VOID CFollowCamera::Setup_CameraInfo()
{
	XMVECTOR vTargetPos = m_pTarget->GetState(CTransform::POSITION);
	XMVECTOR vTargetDir = m_pTarget->GetState(CTransform::LOOK);

	XMVECTOR vDir = XMVectorSetW(XMVector3Normalize(vTargetDir * -1.f), 0.f);

	m_vAt = vTargetPos;
	m_vEye = XMVectorSetW(vTargetPos + vDir * m_fDistance + m_vOffset, 1.f);

	m_pTransform->SetState(CTransform::POSITION, m_vEye);

	m_pTransform->Rotate_LookAt(vTargetPos);

	m_matView = XMMatrixInverse(nullptr, m_pTransform->GetWorld());
}

VOID CFollowCamera::Release()
{
	return VOID();
}

CFollowCamera* CFollowCamera::Create(CTransform* pTarget, XMVECTOR vOffset, FLOAT fDistance)
{
	CFollowCamera* pInstance = new CFollowCamera;
	if (FAILED(pInstance->Initialize(pTarget, vOffset, fDistance)))
		SafeDelete(pInstance);
	return pInstance;
}
