#pragma once
#include "Obj.h"
class CBullet : public CObj
{
public:
	explicit CBullet();
	virtual ~CBullet();

public:
	virtual HRESULT Initialize(CTransform* pPlayerTransform);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual INT LateUpdate(const FLOAT& fTimeDelta);
	virtual VOID Render();

private:
	XMVECTOR m_vDir = {};
	FLOAT m_fSpeed = 10.f;
	CTransform* m_pPlayerTransform = nullptr;

public:
	virtual VOID Release();
	static CBullet* Create(CTransform* pPlayerTransform);
};

