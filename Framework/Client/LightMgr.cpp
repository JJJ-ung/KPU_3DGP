#include "framework.h"
#include "LightMgr.h"
#include "CShaderMgr.h"

IMPLEMENT_SINGLETON(CLightMgr)

CLightMgr::CLightMgr()
{
	m_pShaderMgr = CShaderMgr::GetInstance();
}

CLightMgr::~CLightMgr()
{
}

HRESULT CLightMgr::Add_Directional(UINT iIndex, XMFLOAT4 vDir, XMFLOAT4 vDiffuse, XMFLOAT4 vAmbient, XMFLOAT4 vSpecular)
{
	if (iIndex > 2)
		return E_FAIL;

	m_tLightInfo.LightDir[iIndex] = vDir;
	m_tLightInfo.LightDiffuse[iIndex] = vDiffuse;
	m_tLightInfo.LightAmbient[iIndex] = vAmbient;
	m_tLightInfo.LightSpecular[iIndex] = vSpecular;

	return NOERROR;
}

VOID CLightMgr::Update_CameraPos(XMVECTOR vPos)
{
	XMStoreFloat4(&m_tLightInfo.CameraPos, vPos);
}

VOID CLightMgr::Set_LightCBV(ID3D12GraphicsCommandList* pCommandList)
{
	m_pShaderMgr->Get_LightCBV()->CopyData(0, m_tLightInfo);

	auto lightCB = m_pShaderMgr->Get_LightCBV()->Resource();
	pCommandList->SetGraphicsRootConstantBufferView(RootParam::LIGHT, lightCB->GetGPUVirtualAddress());
}
