#pragma once
#include "Obj.h"
class CRacingMap : public CObj
{
public:
	explicit CRacingMap();
	virtual ~CRacingMap();

public:
	virtual HRESULT Initialize();
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual INT LateUpdate(const FLOAT& fTimeDelta);
	virtual VOID Render();

public:
	virtual VOID Release();
	static CRacingMap* Create();
};

