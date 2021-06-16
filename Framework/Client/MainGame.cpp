#include "framework.h"
#include "MainGame.h"

#include "DeviceMgr.h"
#include "CShaderMgr.h"

#include "Mesh.h"

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

	m_pDeviceMgr->Reset_CommandLst();

	m_pTest = CMesh::Create("../Binary/Resources/YoshiCart/torokko_yoshi_body01a.obj", L"Color");
	//m_pTest = CMesh::Create("../Binary/Resources/Apple/Apple.obj", L"Color");
	//m_pTest = CMesh::Create("../Binary/Resources/cube.obj", L"Color");

	m_pDeviceMgr->Close_CommandLst();
	m_pDeviceMgr->Wait_GPU();

	return NOERROR;
}

INT CMainGame::Update(const float& fTimeDelta)
{
	m_pTest->Update(fTimeDelta);

	return 0;
}

INT CMainGame::Render()
{
	m_pDeviceMgr->RenderBegin();

	// Render
	m_pTest->Render();

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
	if (FAILED(m_pDeviceMgr->Init_GraphicDevice(VEC4(0, 0, 1, 1.f))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainGame::Ready_Shader()
{
	// Root Parameter
	CD3DX12_ROOT_PARAMETER slotRootParameter[1];
	//slotRootParameter[0].InitAsConstantBufferView(0);
	slotRootParameter[0].InitAsConstants(24, 0);
	if (FAILED(m_pShaderMgr->Set_RootSignature(1, slotRootParameter, 0, nullptr)))
		return E_FAIL;

	// Pipeline State
	VEC_INPUTELEMENT tmp =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
		//{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	if (FAILED(m_pShaderMgr->Add_PipelineState(L"Color", L"..\\Binary\\Shaders\\Test.hlsl", L"..\\Binary\\Shaders\\Test.hlsl", "VS", "PS", "vs_5_0", "ps_5_0", &tmp)))
		return E_FAIL;

	return NOERROR;
}

VOID CMainGame::Release()
{
	SafeDelete(m_pTest);
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
