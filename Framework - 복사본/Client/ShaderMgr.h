#pragma once
class CShader;
class CShaderMgr
{
	DECLARE_SINGLETON(CShaderMgr)

private:
	CShaderMgr();
	~CShaderMgr();

public:
	CShader*			Add_Shader(const wstring& strKey, const wstring& strPath, const D3D_SHADER_MACRO* defines, const string& strEntry, const string& strTarget);
	CShader*			Find_Shader(wstring strKey);

public:
	HRESULT			Set_RootSignature(UINT iParam, D3D12_ROOT_PARAMETER* pParam,
																	UINT iDesc, D3D12_STATIC_SAMPLER_DESC* pDesc);
	HRESULT			Add_PipelineState(const wstring& strKey, const wstring& strVSKey, const wstring& strPSKey);

private:
	map<wstring, CShader*> m_mapShader;
	map<wstring, ID3D12PipelineState*> m_mapPipelineState;

private:
	VOID					Release();
};

