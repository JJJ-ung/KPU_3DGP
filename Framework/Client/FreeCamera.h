#pragma once
#include "Camera.h"
class CFreeCamera : public CCamera
{
public:
	explicit CFreeCamera();
	virtual ~CFreeCamera();

public:
	virtual HRESULT Initialize();
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual INT LateUpdate(const FLOAT& fTimeDelta);
	virtual VOID Render();

public:
	virtual VOID Release();
	static CFreeCamera* Create();
};

