#include "framework.h"
#include "Player.h"
#include "StaticMesh.h"

#include "DDSTextureLoader.h"

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

	//if (FAILED(Add_Component(CStaticMesh::Create("../Binary/Resources/test.obj"))))
	//	return E_FAIL;
	//if (FAILED(Add_Component(CStaticMesh::Create("../Binary/Resources/Apple/Apple.obj"))))
	//	return E_FAIL;
	if (FAILED(Add_Component(CStaticMesh::Create("../Binary/Resources/YoshiCart/torokko_yoshi_body01a.obj"))))
		return E_FAIL;
	//if (FAILED(Add_Component(CStaticMesh::Create("../Binary/Resources/1Ho/1Ho.obj"))))
	//	return E_FAIL;

	//if (FAILED(CreateDDSTextureFromFile12(m_pDeviceMgr->Get_Device(), m_pDeviceMgr->Get_CommandLst(),
	//	L"../Binary/Resources/1Ho/mBody_Alb.dds", m_pTexture, m_pUploadHeap)))
	//	return E_FAIL;

	//CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(m_pDeviceMgr->Get_SrvHeap()->GetCPUDescriptorHandleForHeapStart());

	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.Format = m_pTexture->GetDesc().Format;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	//srvDesc.Texture2D.MostDetailedMip = 0;
	//srvDesc.Texture2D.MipLevels = m_pTexture->GetDesc().MipLevels;
	//srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	//m_pDeviceMgr->Get_Device()->CreateShaderResourceView(m_pTexture.Get(), &srvDesc, hDescriptor);

	return NOERROR;
}

INT CPlayer::Update(const FLOAT& fTimeDelta)
{
	CObj::Update(fTimeDelta);

	m_fAngle += fTimeDelta * 100.f;

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
	XMMATRIX matWorld = XMMatrixRotationY(XMConvertToRadians(m_fAngle));

	OBJINFO tmp = {};
	XMStoreFloat4x4(&tmp.World, XMMatrixTranspose(matWorld));
	XMStoreFloat4x4(&tmp.View, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tmp.Proj, XMMatrixTranspose(matProj));

	m_pShaderMgr->Get_ObjCBV()->CopyData(0, tmp);

	auto objCB = m_pShaderMgr->Get_ObjCBV()->Resource();
	commandList->SetGraphicsRootConstantBufferView(RootParam::OBJ, objCB->GetGPUVirtualAddress());

	//commandList->SetGraphicsRoot32BitConstants(RootParam::OBJ, BufferSlotSize[RootParam::OBJ], &tmp, 0);

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
