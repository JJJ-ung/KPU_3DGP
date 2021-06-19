#include "framework.h"
#include "Transform.h"

CTransform::CTransform()
{
}

CTransform::~CTransform()
{
}

VOID CTransform::Move_Forward(const FLOAT t)
{
    XMVECTOR vPos = GetState(POSITION);
    XMVECTOR vLook = GetState(LOOK);

    vPos += vLook * m_fSpeed * t;
    SetState(POSITION, vPos);
}

VOID CTransform::Move_Right(const FLOAT t)
{
	XMVECTOR vPos = GetState(POSITION);
	XMVECTOR vRight = GetState(RIGHT);

	vPos += vRight * m_fSpeed * t;
	SetState(POSITION, vPos);
}

VOID CTransform::Move_Up(const FLOAT t)
{
	XMVECTOR vPos = GetState(POSITION);
	XMVECTOR vUp = GetState(UP);

	vPos += vUp * m_fSpeed * t;
	SetState(POSITION, vPos);
}

VOID CTransform::Move_Direction(XMVECTOR v, const FLOAT t)
{
	XMVECTOR vPos = GetState(POSITION);
    
	vPos += XMVector3Normalize(v) * m_fSpeed * t;
	SetState(POSITION, vPos);
}

VOID CTransform::Move_Target(XMVECTOR vDstPos, const FLOAT t)
{
	XMVECTOR vSrcPos = GetState(POSITION);
	XMVECTOR vDirection = vDstPos - vSrcPos;
	vSrcPos += XMVector3Normalize(vDirection) * m_fSpeed * t;

	SetState(POSITION, vSrcPos);
}

VOID CTransform::Rotate_Axis(XMVECTOR v, const FLOAT t)
{
    XMMATRIX m = XMMatrixRotationAxis(v, m_fRotationSpeed * t);

	XMVECTOR vRight = GetState(RIGHT);
	XMVECTOR vUp = GetState(UP);
	XMVECTOR vLook = GetState(LOOK);

	vRight = XMVectorSetW(XMVector3TransformNormal(vRight, m), 0.f);
	vUp = XMVectorSetW(XMVector3TransformNormal(vUp, m), 0.f);
	vLook = XMVectorSetW(XMVector3TransformNormal(vLook, m), 0.f);

	SetState(RIGHT, vRight);
	SetState(UP, vUp);
	SetState(LOOK, vLook);
}

VOID CTransform::Rotate_LookAt(XMVECTOR vTarget)
{
	XMVECTOR vSrcPos = GetState(POSITION);
	XMVECTOR vDirection = vTarget - vSrcPos;
	XMVECTOR vUp{ 0.f, 1.f, 0.f, 0.f };
	XMVECTOR vRight = XMVector3Cross(vUp, vDirection);
	XMVECTOR vLook = XMVector3Cross(vRight, vUp);
	SetState(RIGHT, XMVector3Normalize(vRight) * m_vScale.x);
	SetState(UP, XMVector3Normalize(vUp) * m_vScale.y);
	SetState(LOOK, XMVector3Normalize(vLook) * m_vScale.z);
}

VOID CTransform::Set_Scale(XMFLOAT3 v)
{
	m_vScale = v;
	SetState(RIGHT, XMVector3Normalize(GetState(RIGHT)) * v.x);
	SetState(UP, XMVector3Normalize(GetState(UP)) * v.y);
	SetState(LOOK, XMVector3Normalize(GetState(LOOK)) * v.z);
}

HRESULT CTransform::Release()
{
    return NOERROR;
}

CTransform* CTransform::Create()
{
    CTransform* pInstance = new CTransform;
    if (FAILED(pInstance->Initialize()))
        SafeDelete(pInstance);
    return pInstance;
}
