#pragma once
#include "Obj.h"
class CCamera : public CObj
{
public:
	explicit CCamera();
	virtual ~CCamera();

public:
	virtual HRESULT Initialize();
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual INT LateUpdate(const FLOAT& fTimeDelta);
	virtual VOID Render();

protected:
	XMVECTOR m_vEye{}, m_vAt{}, m_vUp{};
	FLOAT m_fFovY = 0.f, m_fAspect = 0.f, m_fNear = 0.f, m_fFar = 0.f;

protected:
	XMMATRIX m_matView{};
	XMMATRIX m_matProj{};

public:
	virtual VOID Release();
	static CCamera* Create();
};

