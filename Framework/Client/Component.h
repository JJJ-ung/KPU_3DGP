#pragma once

class CComponent
{
public:
	explicit CComponent();
	virtual ~CComponent();

public:
	virtual HRESULT Initialize();
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual VOID Render();

public:
	virtual HRESULT Release();
};

