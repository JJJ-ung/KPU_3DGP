#include "framework.h"
#include "MainGame.h"

#include "GameMgr.h"
#include "DeviceMgr.h"
#include "CShaderMgr.h"
#include "Scene_Logo.h"

CMainGame::CMainGame()
{
}

CMainGame::~CMainGame()
{
	Release();
}

HRESULT CMainGame::Initialize()
{
	if (FAILED(Ready_Manager()))
		return E_FAIL;

	if (FAILED(Ready_Device()))
		return E_FAIL;

	if (FAILED(Ready_Shader()))
		return E_FAIL;

	if (FAILED(m_pGameMgr->Set_CurrScene(CScene_Logo::Create())))
		return E_FAIL;

	return NOERROR;
}

INT CMainGame::Update(const float& fTimeDelta)
{
	m_pGameMgr->Update(fTimeDelta);

	return 0;
}

INT CMainGame::Render()
{
	m_pDeviceMgr->RenderBegin();

	// Render
	m_pGameMgr->Render();

	m_pDeviceMgr->RenderEnd();

	return 0;
}

HRESULT CMainGame::Ready_Manager()
{
	m_pDeviceMgr = CDeviceMgr::GetInstance();
	if (!m_pDeviceMgr)
		return E_FAIL;

	m_pShaderMgr = CShaderMgr::GetInstance();
	if (!m_pShaderMgr)
		return E_FAIL;

	m_pGameMgr = CGameMgr::GetInstance();
	if (!m_pGameMgr)
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainGame::Ready_Device()
{
	if (FAILED(m_pDeviceMgr->Init_GraphicDevice(VEC4(0, 0, 1, 1.f))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainGame::Ready_Shader()
{
	// Root Parameter
	CD3DX12_ROOT_PARAMETER slotRootParameter[RootParam::END];

	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	slotRootParameter[RootParam::TEXTABLE].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);
	slotRootParameter[RootParam::OBJ].InitAsConstantBufferView(0);
	slotRootParameter[RootParam::LIGHT].InitAsConstantBufferView(1);
	slotRootParameter[RootParam::CONSTANT].InitAsConstants(20, 2);
	//m_pShaderMgr->Get_CBVResources().push_back(nullptr)
	//slotRootParameter[RootParam::OBJ].InitAsConstants(BufferSlotSize[RootParam::OBJ] = 49, 0);			// ObjInfo
	//slotRootParameter[RootParam::LIGHT].InitAsConstants(BufferSlotSize[RootParam::LIGHT] = 4, 1);			// LightInfo

	auto staticSamplers = Get_StaticSamplers();

	if (FAILED(m_pShaderMgr->Set_RootSignature(_countof(slotRootParameter), slotRootParameter, staticSamplers.size(), staticSamplers.data())))
		return E_FAIL;

	if (FAILED(m_pShaderMgr->Set_CBVUploadBuffer(true)))
		return E_FAIL;

	// Pipeline State
	VEC_INPUTELEMENT tmp =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	//if (FAILED(m_pShaderMgr->Add_PipelineState(L"Color", L"..\\Binary\\Shaders\\Test.hlsl", L"..\\Binary\\Shaders\\color.hlsl", "VS", "PS", "vs_5_0", "ps_5_0", &tmp)))
	//	return E_FAIL;
	if (FAILED(m_pShaderMgr->Add_PipelineState(L"Default", L"..\\Binary\\Shaders\\Default.hlsl", L"..\\Binary\\Shaders\\Default.hlsl", "VS", "PS", "vs_5_0", "ps_5_0", &tmp)))
		return E_FAIL;

	return NOERROR;
}

array<CD3DX12_STATIC_SAMPLER_DESC, 6> CMainGame::Get_StaticSamplers()
{
	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy

	return { pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp };
}

VOID CMainGame::Release()
{
	m_pGameMgr->DestroyInstance();
	m_pShaderMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();
}

CMainGame* CMainGame::Create()
{
	CMainGame* pInstance = new CMainGame;
	if (FAILED(pInstance->Initialize()))
		SafeDelete(pInstance);
	return pInstance;
}
