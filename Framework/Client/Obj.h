#pragma once
#include "LightMgr.h"
#include "InputMgr.h"
#include "GameMgr.h"
#include "DeviceMgr.h"
#include "CShaderMgr.h"
#include "Transform.h"
#include "Collision.h"
class CComponent;
class CObj
{
public:
	explicit CObj();
	virtual ~CObj();

public:
	CTransform* Get_Transform() { return m_pTransform; }

public:
	virtual HRESULT Initialize();
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual INT LateUpdate(const FLOAT& fTimeDelta);
	virtual VOID Render();

public:
	HRESULT Add_Component(CComponent* pComponent);

protected:
	CLightMgr* m_pLightMgr = nullptr;
	CInputMgr* m_pInputMgr = nullptr;
	CGameMgr* m_pGameMgr = nullptr;
	CDeviceMgr* m_pDeviceMgr = nullptr;
	CShaderMgr* m_pShaderMgr = nullptr;

protected:
	OBJINFO m_tObjInfo = {};
	CONSTANTINFO m_tConstInfo = {};
	CTransform* m_pTransform = nullptr;
	list<CComponent*> m_lstComponent;

protected:
	CCollision* m_pCollision = nullptr;

protected:
	ID3D12PipelineState* m_pPipelineState = nullptr;
	ID3D12RootSignature* m_pRootSignature = nullptr;

public:
	virtual VOID Release();
};

