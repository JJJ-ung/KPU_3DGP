#include "framework.h"
#include "FreeCamera.h"

CFreeCamera::CFreeCamera()
{
}

CFreeCamera::~CFreeCamera()
{
}

HRESULT CFreeCamera::Initialize()
{
	if (FAILED(CCamera::Initialize()))
		return E_FAIL;

	m_vEye = { 0.f, 10.f, -30.f, 1.f };
	m_vAt = { 0.f, 0.f, 0.f, 1.f };
	m_vUp = { 0.f, 1.f, 0.f, 0.f };

	m_pTransform->SetState(CTransform::POSITION, m_vEye);
	XMVECTOR vLook = m_vAt - m_vEye;
	m_pTransform->SetState(CTransform::LOOK, XMVector3Normalize(vLook));
	XMVECTOR vRight = XMVectorSetW(XMVector3Cross(m_vUp, vLook), 0.f);
	m_pTransform->SetState(CTransform::RIGHT, XMVector3Normalize(vRight));
	XMVECTOR vUp = XMVectorSetW(XMVector3Cross(vLook, vRight), 0.f);
	m_pTransform->SetState(CTransform::UP, XMVector3Normalize(vUp));

	m_matView = XMMatrixInverse(nullptr, m_pTransform->GetWorld());

	m_pGameMgr->Set_MatrixInfo(m_matProj, m_matView);

	return NOERROR;
}

INT CFreeCamera::Update(const FLOAT& fTimeDelta)
{
	if (m_pInputMgr->KeyPressing(KEY_W))
		m_pTransform->Move_Forward(fTimeDelta);
	if (m_pInputMgr->KeyPressing(KEY_S))
		m_pTransform->Move_Forward(-fTimeDelta);
	if (m_pInputMgr->KeyPressing(KEY_D))
		m_pTransform->Move_Right(fTimeDelta);
	if (m_pInputMgr->KeyPressing(KEY_A))
		m_pTransform->Move_Right(-fTimeDelta);
	if (m_pInputMgr->KeyPressing(KEY_Q))
		m_pTransform->Move_Up(fTimeDelta);
	if (m_pInputMgr->KeyPressing(KEY_E))
		m_pTransform->Move_Up(-fTimeDelta);

	LONG	MouseMove = 0;
	if (MouseMove = m_pInputMgr->GetMouseMove(CInputMgr::DIMS_X))
		m_pTransform->Rotate_Axis(XMVECTOR{0.f, 1.f, 0.f, 0.f}, XMConvertToRadians(MouseMove * fTimeDelta));
	if (MouseMove = m_pInputMgr->GetMouseMove(CInputMgr::DIMS_Y))
		m_pTransform->Rotate_Axis(m_pTransform->GetState(CTransform::RIGHT), XMConvertToRadians(MouseMove * fTimeDelta));

	//POINT		ptCursorPos = { g_nWinCX >> 1, g_nWinCY >> 1 };

	//ClientToScreen(g_hWnd, &ptCursorPos);

	//SetCursorPos(ptCursorPos.x, ptCursorPos.y);

	return 0;
}

INT CFreeCamera::LateUpdate(const FLOAT& fTimeDelta)
{
	CCamera::LateUpdate(fTimeDelta);

	return 0;
}

VOID CFreeCamera::Render()
{
	return VOID();
}

VOID CFreeCamera::Release()
{
	return VOID();
}

CFreeCamera* CFreeCamera::Create()
{
	CFreeCamera* pInstance = new CFreeCamera;
	if (FAILED(pInstance->Initialize()))
		SafeDelete(pInstance);
	return pInstance;
}
