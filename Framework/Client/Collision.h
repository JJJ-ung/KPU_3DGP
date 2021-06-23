#pragma once
#include "Obj.h"
class CTransform;
class CStaticMesh;
class CCollision : public CComponent
{
public:
	explicit CCollision();
	virtual ~CCollision();

public:
	CTransform* Get_Transform() { return m_pTransform; }

public:
	virtual HRESULT Initialize(CTransform* pOwnerTransform, FLOAT fSize, XMVECTOR vOffset);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual VOID Render();

public:
	BOOL Collision_Sphere(CCollision* pTarget);
	XMVECTOR Get_Position();

private:
	BOOL m_bColl = FALSE;
	FLOAT m_fRadius = 0.f;
	XMVECTOR m_vOffset = {};

private:
	CStaticMesh* m_pMesh = nullptr;
	CTransform* m_pTransform = nullptr;
	CTransform* m_pOwnerTransform = nullptr;

private:
	BOOL m_bRender = FALSE;
	OBJINFO m_tObjInfo = {};
	CONSTANTINFO m_tConstInfo = {};
	ID3D12PipelineState* m_pPipelineState = nullptr;
	ID3D12RootSignature* m_pRootSignature = nullptr;

public:
	virtual HRESULT Release();
	static CCollision* Create(CTransform* pOwnerTransform, FLOAT fSize, XMVECTOR vOffset = XMVectorSet(0.f, 0.f, 0.f, 0.f));
};

