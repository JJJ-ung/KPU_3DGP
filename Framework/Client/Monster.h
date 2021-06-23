#pragma once
#include "Obj.h"
class CMonster : public CObj
{
public:
	explicit CMonster();
	virtual ~CMonster();

public:
	virtual HRESULT Initialize(XMVECTOR vPos, CTransform* pPlayerTransform);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual INT LateUpdate(const FLOAT& fTimeDelta);
	virtual VOID Render();

private:
	INT Update_Dead(const FLOAT& fTimeDelta);

private:
	BOOL m_bDead = FALSE;
	FLOAT m_fTime = 0.f;

private:
	FLOAT m_fDelta = 0.f;
	CTransform* m_pPlayerTransform = nullptr;

public:
	virtual VOID Release();
	static CMonster* Create(XMVECTOR vPos, CTransform* pPlayerTransform);
};

