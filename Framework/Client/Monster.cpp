#include "framework.h"
#include "Monster.h"
#include "StaticMesh.h"

CMonster::CMonster()
{
}

CMonster::~CMonster()
{
	Release();
}

HRESULT CMonster::Initialize(XMVECTOR vPos, CTransform* pPlayerTransform)
{
	if (!pPlayerTransform)
		return E_FAIL;

	if (FAILED(CObj::Initialize()))
		return E_FAIL;

	m_pRootSignature = m_pShaderMgr->Get_RootSignature();
	m_pPipelineState = m_pShaderMgr->Get_PipelineState(L"Default");

	m_pTransform = CTransform::Create();
	m_pTransform->SetState(CTransform::POSITION, vPos);
	m_pTransform->Set_Scale({0.5f, 0.5f, 0.5f});

	m_pPlayerTransform = pPlayerTransform;

	if (FAILED(Add_Component(CStaticMesh::Create("../Binary/Resources/Monster/Boo.obj"))))
		return E_FAIL;

	m_pCollision = CCollision::Create(m_pTransform, 13.f, XMVectorSet(0.f, -3.f, 0.f, 0.f));
	if (FAILED(Add_Component(m_pCollision)))
		return E_FAIL;
	m_pGameMgr->Add_Collision(CGameMgr::MONSTER, m_pCollision);

	return NOERROR;
}

INT CMonster::Update(const FLOAT& fTimeDelta)
{
	//m_pTransform->Rotate_LookAt(m_pPlayerTransform->GetState(CTransform::POSITION));

	if (m_bDead)
	{
		CObj::Update(fTimeDelta);
		return NOEVENT;
	}

	XMVECTOR vPos = m_pTransform->GetState(CTransform::POSITION);

	m_fDelta += fTimeDelta * 180.f;
	if (m_fDelta >= 360.f)
		m_fDelta = 0.f;
	vPos = XMVectorSetY(vPos, sinf(XMConvertToRadians(m_fDelta)) + 15.f);
	m_pTransform->SetState(CTransform::POSITION, vPos);

	XMVECTOR vTarget = m_pPlayerTransform->GetState(CTransform::POSITION);
	m_pTransform->Rotate_LookAt(vTarget);

	CObj::Update(fTimeDelta);

	return 0;
}

INT CMonster::LateUpdate(const FLOAT& fTimeDelta)
{
	if (m_bDead)
		return Update_Dead(fTimeDelta);

	for (auto p : m_pGameMgr->Get_CollisionList(CGameMgr::BULLET))
	{
		if (m_pCollision->Collision_Sphere(p))
			m_bDead = TRUE;
	}

	CObj::LateUpdate(fTimeDelta);

	m_pGameMgr->Add_RenderList(this);

	return 0;
}

VOID CMonster::Render()
{
	auto commandList = m_pDeviceMgr->Get_CommandLst();
	if (!commandList) return;

	commandList->SetGraphicsRootSignature(m_pShaderMgr->Get_RootSignature());
	commandList->SetPipelineState(m_pPipelineState);

	XMMATRIX matView = m_pGameMgr->Get_View();
	XMMATRIX matProj = m_pGameMgr->Get_Proj();
	XMMATRIX matWorld = m_pTransform->GetWorld();

	XMStoreFloat4x4(&m_tConstInfo.World, XMMatrixTranspose(matWorld));
	XMStoreFloat4x4(&m_tObjInfo.View, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&m_tObjInfo.Proj, XMMatrixTranspose(matProj));

	m_pShaderMgr->Get_ObjCBV()->CopyData(0, m_tObjInfo);

	auto objCB = m_pShaderMgr->Get_ObjCBV()->Resource();
	commandList->SetGraphicsRootConstantBufferView(RootParam::OBJ, objCB->GetGPUVirtualAddress());

	commandList->SetGraphicsRoot32BitConstants(RootParam::CONSTANT, 20, &m_tConstInfo, 0);

	m_pLightMgr->Set_LightCBV(commandList);

	for (auto p : m_lstComponent)
		p->Render();
}

INT CMonster::Update_Dead(const FLOAT& fTimeDelta)
{
	m_fTime += fTimeDelta;
	if (m_fTime > 0.7f)
	{
		if (FAILED(m_pGameMgr->Delete_Collision(CGameMgr::MONSTER, m_pCollision)))
			return NOEVENT;
		return OBJDEAD;
	}

	m_pTransform->Rotate_Axis(m_pTransform->GetState(CTransform::LOOK), fTimeDelta * 25.f);
	CObj::LateUpdate(fTimeDelta);
	m_pGameMgr->Add_RenderList(this);

	return 0;
}

VOID CMonster::Release()
{
	CObj::Release();
}

CMonster* CMonster::Create(XMVECTOR vPos, CTransform* pPlayerTransform)
{
	CMonster* pInstance = new CMonster;
	if (FAILED(pInstance->Initialize(vPos, pPlayerTransform)))
		SafeDelete(pInstance);
	return pInstance;
}
