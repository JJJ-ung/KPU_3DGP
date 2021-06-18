#include "framework.h"
#include "Material.h"
#include "DeviceMgr.h"
#include "DDSTextureLoader.h"

CMaterial::CMaterial()
{
	m_pDeviceMgr = CDeviceMgr::GetInstance();
}

CMaterial::~CMaterial()
{
}

HRESULT CMaterial::Load_MaterialInfo(UINT iIndex)
{
	m_iTexIndex = iIndex;

	if (FAILED(CreateDDSTextureFromFile12(m_pDeviceMgr->Get_Device(), m_pDeviceMgr->Get_CommandLst(),
		m_tMaterialInfo.map_Ka.c_str(), m_pTexture, m_pUploadHeap)))
		return E_FAIL;

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor;
	hDescriptor.InitOffsetted(m_pDeviceMgr->Get_SrvHeap()->GetCPUDescriptorHandleForHeapStart(), iIndex, m_pDeviceMgr->Get_SrvSize());

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = m_pTexture->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = m_pTexture->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	m_pDeviceMgr->Get_Device()->CreateShaderResourceView(m_pTexture.Get(), &srvDesc, hDescriptor);

	return NOERROR;
}

VOID CMaterial::Render(ID3D12GraphicsCommandList* pCommandList)
{
	if (!pCommandList) return;
	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(m_pDeviceMgr->Get_SrvHeap()->GetGPUDescriptorHandleForHeapStart());
	tex.Offset(m_iTexIndex, m_pDeviceMgr->Get_SrvSize());
	pCommandList->SetGraphicsRootDescriptorTable(0, tex);
}
