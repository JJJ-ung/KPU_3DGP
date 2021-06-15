#include "framework.h"
#include "MainGame.h"

#include "DeviceMgr.h"
#include "ShaderMgr.h"

#include "Mesh.h"
#include "Shader.h"

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

	CMesh::Create("../Binary/Resources/YoshiCart/torokko_yoshi_body01a.obj");

	return NOERROR;
}

INT CMainGame::Update(const float& fTimeDelta)
{
	return 0;
}

INT CMainGame::Render()
{
	m_pDeviceMgr->RenderBegin();
	// Render

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
	
	return NOERROR;
}

HRESULT CMainGame::Ready_Device()
{
	if (FAILED(m_pDeviceMgr->Init_GraphicDevice(VEC4(0.1f, 0.1f, 0.1f, 1.f))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainGame::Ready_Shader()
{
	// 루트 시그니처 생성
	// 1. 슬롯 추가
	vector< CD3DX12_ROOT_PARAMETER> vecParam;
	CD3DX12_DESCRIPTOR_RANGE cbvTable;
	cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	vecParam.push_back({});
	vecParam[0].InitAsDescriptorTable(1, &cbvTable);
	// 2. 샘플러 추가
	vector<CD3DX12_STATIC_SAMPLER_DESC> vecDesc;
	// 3. 루트 시그니처 생성
	if (FAILED(m_pShaderMgr->Set_RootSignature((UINT)vecParam.size(), vecParam.data(), (UINT)vecDesc.size(), vecDesc.data())))
		return E_FAIL;

	// 쉐이더 추가 및 컴파일
	CShader* pShader = nullptr;
	pShader = m_pShaderMgr->Add_Shader(L"VS", L"../Binary/Shaders/color.hlsl", nullptr, "VS", "vs_5_1");
	if (!pShader) return E_FAIL;
	pShader->Add_InputElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, sizeof(float) * 3);

	pShader = m_pShaderMgr->Add_Shader(L"PS", L"../Binary/Shaders/color.hlsl", nullptr, "PS", "ps_5_1");
	if (!pShader) return E_FAIL;

	// 파이프라인 생성
	if (FAILED(m_pShaderMgr->Add_PipelineState(L"Test", L"VS", L"PS")))
		return E_FAIL;

	return NOERROR;
}

VOID CMainGame::Release()
{
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
