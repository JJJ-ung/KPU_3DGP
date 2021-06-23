#include "framework.h"
#include "Collision.h"
#include "GameMgr.h"
#include "DeviceMgr.h"
#include "CShaderMgr.h"
#include "StaticMesh.h"

CCollision::CCollision()
{
}

CCollision::~CCollision()
{
	Release();
}

HRESULT CCollision::Initialize(CTransform* pOwnerTransform, FLOAT fSize, XMVECTOR vOffset)
{
	if (!pOwnerTransform)
		return E_FAIL;
	m_pOwnerTransform = pOwnerTransform;

	m_pGameMgr = CGameMgr::GetInstance();
	m_pDeviceMgr = CDeviceMgr::GetInstance();
	m_pShaderMgr = CShaderMgr::GetInstance();

	m_pRootSignature = m_pShaderMgr->Get_RootSignature();
	m_pPipelineState = m_pShaderMgr->Get_PipelineState(L"Color");

	m_fRadius = fSize;
	m_vOffset = vOffset;

	m_pTransform = CTransform::Create();
	m_pTransform->Set_Scale({ fSize, fSize, fSize });
	m_pTransform->SetState(CTransform::POSITION, m_pOwnerTransform->GetState(CTransform::POSITION) + vOffset);

	m_pMesh = CStaticMesh::Create("../Binary/Resources/Collider.obj");
	if (!m_pMesh)
		return E_FAIL;
	
	m_tConstInfo.Color = XMFLOAT4(0.f, 1.f, 0.f, 1.f);

	return NOERROR;
}

INT CCollision::Update(const FLOAT& fTimeDelta)
{
	m_bColl = FALSE;
	m_pTransform->SetState(CTransform::POSITION, m_pOwnerTransform->GetState(CTransform::POSITION) + m_vOffset);

	return 0;
}

VOID CCollision::Render()
{
	if (!g_bColliderRender)
		return;

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

	m_pMesh->Render();
}

BOOL CCollision::Collision_Sphere(CCollision* pTarget)
{
	XMVECTOR vSrcCenter = Get_Position();
	XMVECTOR vDstCenter = pTarget->Get_Position();

	XMVECTOR vDist = vDstCenter - vSrcCenter;
	FLOAT fDist = XMVectorGetX(XMVector3Length(vDist));

	if (fDist > m_fRadius + pTarget->m_fRadius)
	{
		if (!m_bColl)
		{
			m_tConstInfo.Color = XMFLOAT4(0.f, 1.f, 0.f, 1.f);
			pTarget->m_tConstInfo.Color = XMFLOAT4(0.f, 1.f, 0.f, 1.f);
		}
		return FALSE;
	}

	m_bColl = TRUE;
	m_tConstInfo.Color = XMFLOAT4(1.f, 0.f, 0.f, 1.f);
	pTarget->m_tConstInfo.Color = XMFLOAT4(1.f, 0.f, 0.f, 1.f);
	return TRUE;
}

XMVECTOR CCollision::Get_Position()
{
	return m_pTransform->GetState(CTransform::POSITION);
}

HRESULT CCollision::Release()
{
	SafeDelete(m_pMesh);
	SafeDelete(m_pTransform);
	return NOERROR;
}

CCollision* CCollision::Create(CTransform* pOwnerTransform, FLOAT fSize, XMVECTOR vOffset)
{
	CCollision* pInstance = new CCollision;
	if (FAILED(pInstance->Initialize(pOwnerTransform, fSize, vOffset)))
		SafeDelete(pInstance);
	return pInstance;
}

