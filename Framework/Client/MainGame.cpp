#include "framework.h"
#include "MainGame.h"

#include "DeviceMgr.h"
#include "ShaderMgr.h"

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

	//if (FAILED(Ready_Shader()))
	//	return E_FAIL;

	CMesh::Create("../Binary/1Ho.obj");

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
	// ��Ʈ �ñ״�ó ����
	// 1. ���� �߰�
	vector< CD3DX12_ROOT_PARAMETER> vecParam;
	// 2. ���÷� �߰�
	vector<CD3DX12_STATIC_SAMPLER_DESC> vecDesc;
	// 3. ��Ʈ �ñ״�ó ����
	if (FAILED(m_pShaderMgr->Set_RootSignature((UINT)vecParam.size(), vecParam.data(), (UINT)vecDesc.size(), vecDesc.data())))
		return E_FAIL;

	// ���̴� �߰� �� ������

	// ���������� ����
	if (FAILED(m_pShaderMgr->Add_PipelineState(L"", L"", L"")))
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
