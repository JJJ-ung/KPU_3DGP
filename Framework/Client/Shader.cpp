#include "framework.h"
#include "Shader.h"

CShader::CShader()
{
}

CShader::~CShader()
{
	Release();
}

HRESULT CShader::Initialize(const wstring& strPath, const D3D_SHADER_MACRO* defines, const string& strEntry, const string& strTarget)
{
	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> errors;
	ComPtr<ID3DBlob> byteCode = nullptr;
	if (FAILED(D3DCompileFromFile(strPath.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		strEntry.c_str(), strTarget.c_str(), compileFlags, 0, &byteCode, &errors)))
	{
		if (errors != nullptr)
			OutputDebugStringA((char*)errors->GetBufferPointer());
		return E_FAIL;
	}

	m_tByteCode.BytecodeLength = byteCode->GetBufferSize();
	m_tByteCode.pShaderBytecode = byteCode->GetBufferPointer();

	return NOERROR;
}

HRESULT CShader::Add_InputElement(const string& strKey, const DXGI_FORMAT& eFormat, const UINT& iSize, const D3D12_INPUT_CLASSIFICATION& eClass)
{
	if (strKey.empty() || iSize == 0)
		return E_FAIL;

	m_vecInputLayout.push_back(D3D12_INPUT_ELEMENT_DESC{strKey.c_str(), 0, eFormat, 0, m_iInputByteCnt, eClass, 0});
	m_iInputByteCnt += iSize;

	return NOERROR;
}

VOID CShader::Release()
{
}

CShader* CShader::Create(const wstring& strPath, const D3D_SHADER_MACRO* defines, const string& strEntry, const string& strTarget)
{
	CShader* pInstance = new CShader;
	if (FAILED(pInstance->Initialize(strPath, defines, strEntry, strTarget)))
		SafeDelete(pInstance);
	return pInstance;
}
