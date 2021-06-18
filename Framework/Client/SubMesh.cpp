#include "framework.h"
#include "SubMesh.h"
#include "DeviceMgr.h"
#include "DDSTextureLoader.h"
#include "Material.h"

CSubMesh::CSubMesh()
{
	m_pDeviceMgr = CDeviceMgr::GetInstance();
}

CSubMesh::~CSubMesh()
{
	Release();
}

VOID CSubMesh::Render(ID3D12GraphicsCommandList* pCommandList)
{
	if (!pCommandList) return;

	if (m_pMaterial)
		m_pMaterial->Render(pCommandList);

	pCommandList->IASetVertexBuffers(0, 1, &vbv);
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pCommandList->DrawInstanced((UINT)vertices.size(), 1, 0, 0);
}

HRESULT CSubMesh::Init_BufferView()
{
	const UINT64 iByteSize = vertices.size() * sizeof(VERTEX);
	ID3D12Resource* pVertexBufferGPU = nullptr;
	ID3D12Resource* pVertexBufferUploader = nullptr;
	pVertexBufferGPU = m_pDeviceMgr->Create_DefaultBuffer(vertices.data(), iByteSize, pVertexBufferUploader);
	if (!pVertexBufferGPU) return E_FAIL;
	vbv.BufferLocation = pVertexBufferGPU->GetGPUVirtualAddress();
	vbv.StrideInBytes = sizeof(VERTEX);													// 한 정점 원소의 크기
	vbv.SizeInBytes = (UINT)vertices.size() * sizeof(VERTEX);			// 버퍼의 총 크기
	return NOERROR;
}

HRESULT CSubMesh::Add_Vertex(XMFLOAT3 p, XMFLOAT2 t, XMFLOAT3 n)
{
	VERTEX tmp;
	tmp.vp = p;
	tmp.vt = t;
	tmp.vn = n;
	vertices.push_back(tmp);
	return NOERROR;
}

HRESULT CSubMesh::Add_Material(CMaterial* pMat)
{
	if (!pMat) return E_FAIL;
	m_pMaterial = pMat;

	return NOERROR;
}

HRESULT CSubMesh::Release()
{
	vertices.clear();
	return NOERROR;
}

CSubMesh* CSubMesh::Create()
{
	CSubMesh* pInstance = new CSubMesh;
	return pInstance;
}
