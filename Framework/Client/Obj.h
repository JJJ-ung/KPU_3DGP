#pragma once

class CInputMgr;
class CObj
{
public:
	explicit CObj();
	virtual ~CObj();

public:
	virtual HRESULT Initialize();
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual INT LateUpdate(const FLOAT& fTimeDelta);
	virtual VOID Render();

protected:
	CInputMgr* m_pInputMgr = nullptr;

protected:
	//list<Component*> m_lstComponent;

public:
	virtual VOID Release();
};

