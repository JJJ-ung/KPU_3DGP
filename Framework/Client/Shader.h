#pragma once
class CShader
{
public:
	CShader();
	~CShader();

public:
	D3D12_SHADER_BYTECODE Get_ByteCode() const { return m_tByteCode; }
	VEC_INPUTELEMENT& Get_InputLayout() { return m_vecInputLayout; }

public:
	HRESULT				Initialize(const wstring& strPath, const D3D_SHADER_MACRO* defines, const string& strEntry, const string& strTarget);
	HRESULT				Add_InputElement(const string& strKey, const DXGI_FORMAT& eFormat, const UINT& iSize, 
																		const D3D12_INPUT_CLASSIFICATION& eClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA);

private:
	UINT													m_iInputByteCnt = 0;
	VEC_INPUTELEMENT						m_vecInputLayout;
	D3D12_SHADER_BYTECODE		m_tByteCode = {};

private:
	VOID						Release();

public:
	static CShader*	Create(const wstring& strPath, const D3D_SHADER_MACRO* defines, const string& strEntry, const string& strTarget);
};

