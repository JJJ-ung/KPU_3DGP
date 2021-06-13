#include "framework.h"
#include "ShaderMgr.h"
#include "Shader.h"

CShaderMgr::CShaderMgr()
{
}

CShaderMgr::~CShaderMgr()
{
}

HRESULT CShaderMgr::Init_ShaderMgr(ID3D12Device* pDevice, string strProfile)
{
	m_strShaderProfile = strProfile;
	m_pDevice = pDevice;

	return NOERROR;
}

HRESULT CShaderMgr::Set_RootSignature(vector<D3D12_ROOT_PARAMETER>& vecParam)
{
	ID3D12RootSignature* pd3dGraphicsRootSignature = NULL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = vecParam.size();
	d3dRootSignatureDesc.pParameters = vecParam.data();
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob* pd3dSignatureBlob = NULL;
	ID3DBlob* pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	m_pDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	m_pd3dRootSignature = pd3dGraphicsRootSignature;

	if (!m_pd3dRootSignature)
		return E_FAIL;

	return NOERROR;
}

HRESULT CShaderMgr::Release_RootSignature()
{
	if (m_pd3dRootSignature) m_pd3dRootSignature->Release();

	return NOERROR;
}

HRESULT CShaderMgr::AddShader(wstring strKey, wstring strPath, string strVertexShader, BYTE flag, string strPixelShader)
{
	auto iter_find = m_mapShader.find(strKey);
	if (iter_find != m_mapShader.end())
		return E_FAIL;

	CShader* pShader = CShader::Create(strPath, strVertexShader, strPixelShader, m_strShaderProfile, flag, m_pd3dRootSignature);
	if (!pShader)
		return E_FAIL;

	m_mapShader.insert(make_pair(strKey, pShader));

	return NOERROR;
}
