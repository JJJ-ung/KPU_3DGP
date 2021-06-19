#include "framework.h"
#include "Player.h"
#include "StaticMesh.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Initialize()
{
	if (FAILED(CObj::Initialize()))
		return E_FAIL;

	m_pRootSignature = m_pShaderMgr->Get_RootSignature();
	m_pPipelineState = m_pShaderMgr->Get_PipelineState(L"Default");

	m_pTransform = CTransform::Create();

	if (FAILED(Add_Component(CStaticMesh::Create("../Binary/Resources/YoshiCart/torokko_yoshi_body01a.obj"))))
		return E_FAIL;

	//if (FAILED(Add_Component(CStaticMesh::Create("../Binary/Resources/1Ho/1Ho.obj"))))
	//	return E_FAIL;
	//if (FAILED(Add_Component(CStaticMesh::Create("../Binary/Resources/RainbowRoad/smkds_rainbowroad.obj"))))
	//	return E_FAIL;

	return NOERROR;
}

INT CPlayer::Update(const FLOAT& fTimeDelta)
{
	if (m_pInputMgr->KeyPressing(KEY_W))
		m_pTransform->Move_Forward(fTimeDelta);
	if (m_pInputMgr->KeyPressing(KEY_S))
		m_pTransform->Move_Forward(-fTimeDelta);
	if (m_pInputMgr->KeyPressing(KEY_D))
		m_pTransform->Move_Right(fTimeDelta);
	if (m_pInputMgr->KeyPressing(KEY_A))
		m_pTransform->Move_Right(-fTimeDelta);

	LONG	MouseMove = 0;
	if (MouseMove = m_pInputMgr->GetMouseMove(CInputMgr::DIMS_X))
		m_pTransform->Rotate_Axis(XMVECTOR{ 0.f, 1.f, 0.f, 0.f }, XMConvertToRadians(MouseMove * fTimeDelta));

	CObj::Update(fTimeDelta);

	return 0;
}

INT CPlayer::LateUpdate(const FLOAT& fTimeDelta)
{
	CObj::LateUpdate(fTimeDelta);

	m_pGameMgr->Add_RenderList(this);

	return 0;
}

VOID CPlayer::Render()
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

VOID CPlayer::Release()
{
	CObj::Release();
}

CPlayer* CPlayer::Create()
{
	CPlayer* pInstance = new CPlayer;
	if (FAILED(pInstance->Initialize()))
		SafeDelete(pInstance);
	return pInstance;
}
