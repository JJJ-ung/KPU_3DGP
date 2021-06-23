#include "framework.h"
#include "Bullet.h"
#include "StaticMesh.h"
#include "Player.h"

CBullet::CBullet()
{
}

CBullet::~CBullet()
{
	Release();
}

HRESULT CBullet::Initialize(CTransform* pPlayerTransform)
{
	if (FAILED(CObj::Initialize()))
		return E_FAIL;

	if (!pPlayerTransform)
		return E_FAIL;

	m_pRootSignature = m_pShaderMgr->Get_RootSignature();
	m_pPipelineState = m_pShaderMgr->Get_PipelineState(L"Default");

	m_pTransform = CTransform::Create();
	m_pPlayerTransform = pPlayerTransform;

	XMVECTOR vPlayerPos = m_pPlayerTransform->GetState(CTransform::POSITION);
	XMVECTOR vPlayerLook = m_pPlayerTransform->GetState(CTransform::LOOK);
	m_pTransform->SetState(CTransform::POSITION, vPlayerPos + XMVectorSet(0.f, 10.f, 0.f, 0.f));
	m_pTransform->Rotate_LookAt(vPlayerPos + vPlayerLook);

	m_vDir = m_pTransform->GetState(CTransform::LOOK);

	m_fSpeed = 20.f;

	if (FAILED(Add_Component(CStaticMesh::Create("../Binary/Resources/Apple/Apple.obj"))))
		return E_FAIL;

	m_pCollision = CCollision::Create(m_pTransform, 7.f, XMVectorSet(0.f, 5.f, 0.f, 0.f));
	if (FAILED(Add_Component(m_pCollision)))
		return E_FAIL;
	m_pGameMgr->Add_Collision(CGameMgr::BULLET, m_pCollision);

	return NOERROR;
}

INT CBullet::Update(const FLOAT& fTimeDelta)
{
	m_pTransform->Move_Direction(m_vDir, fTimeDelta * 8.f);
	m_pTransform->Rotate_Axis(m_pTransform->GetState(CTransform::RIGHT), fTimeDelta * m_fSpeed);

	CObj::Update(fTimeDelta);

	XMVECTOR vDistance = m_pPlayerTransform->GetState(CTransform::POSITION) - m_pTransform->GetState(CTransform::POSITION);
	FLOAT f = XMVectorGetX(XMVector3Length(vDistance));
	if (f >= 1000.f)
	{
		if (FAILED(m_pGameMgr->Delete_Collision(CGameMgr::BULLET, m_pCollision)))
			return NOEVENT;
		return OBJDEAD;
	}

	return 0;
}

INT CBullet::LateUpdate(const FLOAT& fTimeDelta)
{
	CObj::LateUpdate(fTimeDelta);

	m_pGameMgr->Add_RenderList(this);

	return 0;
}

VOID CBullet::Render()
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

VOID CBullet::Release()
{
	CObj::Release();
}

CBullet* CBullet::Create(CTransform* pPlayerTransform)
{
	CBullet* pInstance = new CBullet;
	if (FAILED(pInstance->Initialize(pPlayerTransform)))
		SafeDelete(pInstance);
	return pInstance;
}
