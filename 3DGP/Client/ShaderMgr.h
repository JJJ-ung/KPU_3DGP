#pragma once

class CShader;
class CDeviceMgr;
class CShaderMgr
{
	DECLARE_SINGLETON(CShaderMgr)

private:
	CShaderMgr();
	~CShaderMgr();

public:
	HRESULT Init_ShaderMgr(ID3D12Device* pDevice, string strProfile);
	HRESULT Set_RootSignature(vector<D3D12_ROOT_PARAMETER>& vecParam);
	HRESULT Release_RootSignature();
	HRESULT AddShader(wstring strKey, wstring strPath, string strVertexShader, BYTE flag, string strPixelShader);

private:
	string m_strShaderProfile = "";
	map<wstring, CShader*> m_mapShader;

private:
	ID3D12Device* m_pDevice = nullptr;
	ID3D12RootSignature* m_pd3dRootSignature = nullptr;
};

