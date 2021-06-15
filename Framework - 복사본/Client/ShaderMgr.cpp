#include "framework.h"
#include "ShaderMgr.h"

#include "DeviceMgr.h"
#include "Shader.h"

IMPLEMENT_SINGLETON(CShaderMgr)

CShaderMgr::CShaderMgr()
{
}

CShaderMgr::~CShaderMgr()
{
	Release();
}

CShader* CShaderMgr::Add_Shader(const wstring& strKey, const wstring& strPath, const D3D_SHADER_MACRO* defines, const string& strEntry, const string& strTarget)
{
	if (Find_Shader(strKey))
		return nullptr;

	CShader* pInstance = CShader::Create(strPath, defines, strEntry, strTarget);
	m_mapShader[strKey] = pInstance;

	return pInstance;
}

CShader* CShaderMgr::Find_Shader(wstring strKey)
{
	auto iter = m_mapShader.find(strKey);
	if (iter == m_mapShader.end())
		return nullptr;
	return (*iter).second;
}

HRESULT CShaderMgr::Set_RootSignature(UINT iParam, D3D12_ROOT_PARAMETER* pParam, UINT iDesc, D3D12_STATIC_SAMPLER_DESC* pDesc)
{
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(
		iParam, pParam, iDesc, pDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	if (FAILED(CDeviceMgr::GetInstance()->Set_RootParameter(rootSigDesc)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CShaderMgr::Add_PipelineState(const wstring& strKey, const wstring& strVSKey, const wstring& strPSKey)
{
	CShader* pVS = Find_Shader(strVSKey);
	CShader* pPS = Find_Shader(strPSKey);
	if (!pVS | !pPS)
		return E_FAIL;

	if (m_mapPipelineState.find(strKey) != m_mapPipelineState.end())
		return E_FAIL;

	m_mapPipelineState.insert(make_pair(strKey, nullptr));

	D3D12_GRAPHICS_PIPELINE_STATE_DESC tDesc = CDeviceMgr::GetInstance()->Create_PipelineDesc(pVS, pPS);

	if (FAILED(CDeviceMgr::GetInstance()->Get_Device()->CreateGraphicsPipelineState(&tDesc, IID_PPV_ARGS(&m_mapPipelineState[strKey]))))
		return E_FAIL;

	return NOERROR;
}

VOID CShaderMgr::Release()
{
	for_each(m_mapShader.begin(), m_mapShader.end(), [](pair<wstring, CShader*> P) { SafeDelete(P.second); });
	m_mapShader.clear();
}
