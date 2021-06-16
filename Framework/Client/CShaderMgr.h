#pragma once
class CShaderMgr
{
	DECLARE_SINGLETON(CShaderMgr)

private:
	CShaderMgr();
	~CShaderMgr();

public:
	HRESULT Set_RootSignature(UINT iParam, D3D12_ROOT_PARAMETER* pParam, UINT iDesc, D3D12_STATIC_SAMPLER_DESC* pDesc);

	HRESULT Add_PipelineState(const wstring strKey, const wstring& strVSPath, const wstring& strPSPath,
														const string& strVSEntry, const string& strPSEntry, const string& strVSTarget, const string& strPSTarget,
														VEC_INPUTELEMENT* vecInputLayout,
														D3D12_PRIMITIVE_TOPOLOGY_TYPE ePrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
														D3D12_RASTERIZER_DESC tRasterizerDesc = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT),
														const D3D_SHADER_MACRO* pDefines = nullptr);

	ID3D12RootSignature* Get_RootSignature() { return m_pRootSignature; }
	ID3D12PipelineState* Get_PipelineState(const wstring& strKey);

	D3D12_SHADER_BYTECODE Compile_Shader(const wstring& strPath, const D3D_SHADER_MACRO* defines, const string& strEntry, const string& strTarget);

private:
	ID3D12RootSignature* m_pRootSignature = nullptr;
	map<wstring, ID3D12PipelineState*> m_mapPipelineState;

private:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_tDefaultDesc{};
};
