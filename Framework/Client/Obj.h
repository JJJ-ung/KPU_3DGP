#pragma once
#include "InputMgr.h"
#include "GameMgr.h"
#include "DeviceMgr.h"
#include "CShaderMgr.h"
class CComponent;
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

public:
	HRESULT Add_Component(CComponent* pComponent);

protected:
	CInputMgr* m_pInputMgr = nullptr;
	CGameMgr* m_pGameMgr = nullptr;
	CDeviceMgr* m_pDeviceMgr = nullptr;
	CShaderMgr* m_pShaderMgr = nullptr;

protected:
	list<CComponent*> m_lstComponent;

protected:
	ID3D12PipelineState* m_pPipelineState = nullptr;
	ID3D12RootSignature* m_pRootSignature = nullptr;

public:
	virtual VOID Release();
};

