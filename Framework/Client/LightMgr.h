#pragma once
class CShaderMgr;
class CLightMgr
{
	DECLARE_SINGLETON(CLightMgr)

private:
	CLightMgr();
	~CLightMgr();

public:
	HRESULT Add_Directional(UINT iIndex, XMFLOAT4 vDir, XMFLOAT4 vDiffuse, XMFLOAT4 vAmbient, XMFLOAT4 vSpecular);

public:
	VOID Update_CameraPos(XMVECTOR vPos);
	VOID Set_LightCBV(ID3D12GraphicsCommandList* pCommandList);

private:
	LIGHTINFO m_tLightInfo = {};
	CShaderMgr* m_pShaderMgr = nullptr;
};

