#include "framework.h"
#include "CShaderMgr.h"
#include "DeviceMgr.h"

IMPLEMENT_SINGLETON(CShaderMgr)

CShaderMgr::CShaderMgr()
{
	m_tDefaultDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	m_tDefaultDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	m_tDefaultDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	m_tDefaultDesc.SampleMask = UINT_MAX;
	m_tDefaultDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	m_tDefaultDesc.NumRenderTargets = 1;
	m_tDefaultDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_tDefaultDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_tDefaultDesc.SampleDesc.Count = 1;
	m_tDefaultDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
}

CShaderMgr::~CShaderMgr()
{
	for_each(m_mapPipelineState.begin(), m_mapPipelineState.end(), [](pair<wstring, ID3D12PipelineState*> p) { SafeRelease(p.second); });
}

HRESULT CShaderMgr::Set_RootSignature(UINT iParam, D3D12_ROOT_PARAMETER* pParam, UINT iDesc, D3D12_STATIC_SAMPLER_DESC* pDesc)
{
	CD3DX12_ROOT_SIGNATURE_DESC tRootDesc(
		iParam, pParam, iDesc, pDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	if (FAILED(D3D12SerializeRootSignature(&tRootDesc, D3D_ROOT_SIGNATURE_VERSION_1, serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf())))
		return E_FAIL;

	if (FAILED(CDeviceMgr::GetInstance()->Get_Device()->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_pRootSignature))))
		return E_FAIL;

	return NOERROR;
}

ComPtr<ID3DBlob> CompileShader(
	const std::wstring& filename,
	const D3D_SHADER_MACRO* defines,
	const std::string& entrypoint,
	const std::string& target)
{
	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = S_OK;

	ID3DBlob* byteCode = nullptr;
	ID3DBlob* errors;
	hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

	if (errors != nullptr)
		OutputDebugStringA((char*)errors->GetBufferPointer());

	return byteCode;
}

HRESULT CShaderMgr::Add_PipelineState(const wstring strKey, 
	const wstring& strVSPath, const wstring& strPSPath, 
	const string& strVSEntry, const string& strPSEntry, 
	const string& strVSTarget, const string& strPSTarget, 
	VEC_INPUTELEMENT* vecInputLayout,
	D3D12_PRIMITIVE_TOPOLOGY_TYPE ePrimitiveTopologyType, 
	D3D12_RASTERIZER_DESC tRasterizerDesc, 
	const D3D_SHADER_MACRO* pDefines)
{
	if (Get_PipelineState(strKey))
		return E_FAIL;

	m_mapPipelineState[strKey] = nullptr;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC tDesc = m_tDefaultDesc;
	tDesc.pRootSignature = m_pRootSignature;
	tDesc.InputLayout = { (*vecInputLayout).data(), (UINT)(*vecInputLayout).size() };
	tDesc.VS = Compile_Shader(strVSPath, pDefines, strVSEntry, strVSTarget);
	tDesc.PS = Compile_Shader(strPSPath, pDefines, strPSEntry, strPSTarget);
	tDesc.PrimitiveTopologyType = ePrimitiveTopologyType;
	tDesc.RasterizerState = tRasterizerDesc;

	if (FAILED(CDeviceMgr::GetInstance()->Get_Device()->CreateGraphicsPipelineState(&tDesc, IID_PPV_ARGS(&m_mapPipelineState[strKey]))))
		return E_FAIL;

	return NOERROR;
}

ID3D12PipelineState* CShaderMgr::Get_PipelineState(const wstring& strKey)
{
	auto iter = m_mapPipelineState.find(strKey);
	if (iter == m_mapPipelineState.end())
		return nullptr;
	return iter->second;
}

D3D12_SHADER_BYTECODE CShaderMgr::Compile_Shader(const wstring& strPath, const D3D_SHADER_MACRO* defines, const string& strEntry, const string& strTarget)
{
	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* errors;
	ID3DBlob* byteCode = nullptr;
	if (FAILED(D3DCompileFromFile(strPath.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		strEntry.c_str(), strTarget.c_str(), compileFlags, 0, &byteCode, &errors)))
	{
		if (errors != nullptr)
			OutputDebugStringA((char*)errors->GetBufferPointer());
		return D3D12_SHADER_BYTECODE();
	}

	D3D12_SHADER_BYTECODE tReturn
	{
		reinterpret_cast<BYTE*>(byteCode->GetBufferPointer()),
		byteCode->GetBufferSize()
	};

	return tReturn;
}
