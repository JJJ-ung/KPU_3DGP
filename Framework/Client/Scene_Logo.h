#pragma once
#include "Scene.h"
class CScene_Logo : public CScene
{
public:
	explicit CScene_Logo();
	virtual ~CScene_Logo();

public:
	virtual HRESULT Initialize();
	virtual INT Update(const FLOAT& fTimeDelta);

public:
	virtual VOID Release();
	static CScene_Logo* Create();
};

