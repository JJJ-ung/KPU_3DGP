#pragma once
#include "Scene.h"
class CScene_Stage : public CScene
{
public:
	explicit CScene_Stage();
	virtual ~CScene_Stage();

public:
	virtual HRESULT Initialize();
	virtual INT Update(const FLOAT& fTimeDelta);

public:
	virtual VOID Release();
	static CScene_Stage* Create();
};

