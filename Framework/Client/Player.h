#pragma once
#include "Obj.h"
class CPlayer : public CObj
{
public:
	explicit CPlayer();
	virtual ~CPlayer();

public:
	virtual HRESULT Initialize();
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual INT LateUpdate(const FLOAT& fTimeDelta);
	virtual VOID Render();

public:
	VOID KeyInput(const FLOAT& fTimeDelta);

private:
	BOOL m_bKnockBack;
	XMVECTOR m_vBeginPos;

public:
	virtual VOID Release();
	static CPlayer* Create();
};

