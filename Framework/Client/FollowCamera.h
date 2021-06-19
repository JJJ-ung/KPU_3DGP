#pragma once
#include "Camera.h"
class CFollowCamera : public CCamera
{
public:
	explicit CFollowCamera();
	virtual ~CFollowCamera();

public:
	virtual HRESULT Initialize(CTransform* pTarget, XMVECTOR vOffset, FLOAT fDistance);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual INT LateUpdate(const FLOAT& fTimeDelta);
	virtual VOID Render();

private:
	VOID Setup_CameraInfo();

private:
	XMVECTOR m_vOffset{};
	FLOAT m_fDistance = 1.f;
	CTransform* m_pTarget = nullptr;

public:
	virtual VOID Release();
	static CFollowCamera* Create(CTransform* pTarget, XMVECTOR vOffset, FLOAT fDistance);
};

