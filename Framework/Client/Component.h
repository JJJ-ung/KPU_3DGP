#pragma once
class CGameMgr;
class CDeviceMgr;
class CShaderMgr;
class CComponent
{
public:
	explicit CComponent();
	virtual ~CComponent();

public:
	virtual HRESULT Initialize();
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual INT LateUpdate(const FLOAT& fTimeDelta);
	virtual VOID Render();

protected:
	CGameMgr* m_pGameMgr = nullptr;
	CDeviceMgr* m_pDeviceMgr = nullptr;
	CShaderMgr* m_pShaderMgr = nullptr;

public:
	virtual HRESULT Release();
};

