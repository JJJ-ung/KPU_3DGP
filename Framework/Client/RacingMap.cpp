#include "framework.h"
#include "RacingMap.h"
#include "StaticMesh.h"

CRacingMap::CRacingMap()
{
}

CRacingMap::~CRacingMap()
{
}

HRESULT CRacingMap::Initialize()
{
	if (FAILED(CObj::Initialize()))
		return E_FAIL;

	m_pRootSignature = m_pShaderMgr->Get_RootSignature();
	m_pPipelineState = m_pShaderMgr->Get_PipelineState(L"Default");

	m_pTransform = CTransform::Create();

	if (FAILED(Add_Component(CStaticMesh::Create("../Binary/Resources/Map/Map.obj"))))
		return E_FAIL;

	m_pTransform->Set_Scale(XMFLOAT3(0.1f, 0.1f, 0.1f));
	m_pTransform->SetState(CTransform::POSITION, XMVectorSet(0.f, -39.5f, 0.f, 1.f));

	return NOERROR;
}

INT CRacingMap::Update(const FLOAT& fTimeDelta)
{
	CObj::Update(fTimeDelta);

	return 0;
}

INT CRacingMap::LateUpdate(const FLOAT& fTimeDelta)
{
	CObj::LateUpdate(fTimeDelta);

	m_pGameMgr->Add_RenderList(this);

	return 0;
}

VOID CRacingMap::Render()
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

VOID CRacingMap::Release()
{
	CObj::Release();
}

CRacingMap* CRacingMap::Create()
{
	CRacingMap* pInstance = new CRacingMap;
	if (FAILED(pInstance->Initialize()))
		SafeDelete(pInstance);
	return pInstance;
}
