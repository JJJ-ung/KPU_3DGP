#pragma once
#include "Obj.h"
class CCamera : public CObj
{
public:
	explicit CCamera();
	virtual ~CCamera();

public:
	virtual HRESULT Initialize(CObj* pOwner);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual INT LateUpdate(const FLOAT& fTimeDelta);
	virtual VOID Render();

private:
	XMVECTOR m_vEye{}, m_vAt{}, m_vUp{};
	FLOAT m_fFovY = 0.f, m_fAspect = 0.f, m_fNear = 0.f, m_fFar = 0.f;
	FLOAT m_fDistance = 1.f;

private:
	XMMATRIX m_matView{};
	XMMATRIX m_matProj{};

private:
	CObj* m_pOwner = nullptr;

public:
	virtual VOID Release();
	static CCamera* Create(CObj* pOwner);
};

