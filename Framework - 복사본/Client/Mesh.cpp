#include "framework.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "Material.h"
#include "DeviceMgr.h"
#include "CShaderMgr.h"
#include "InputMgr.h"

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
	Release();
}

HRESULT CMesh::Initialize(const string& strPath, const wstring& strPipelineKey)
{
	if (FAILED(Load_Mesh(strPath.c_str())))
		return E_FAIL;

	m_pDeviceMgr = CDeviceMgr::GetInstance();
	m_pShaderMgr = CShaderMgr::GetInstance();

	m_pPipelineState = m_pShaderMgr->Get_PipelineState(strPipelineKey);
	if (!m_pPipelineState)
		return E_FAIL;

	//const UINT64 iByteSize = vp.size() * sizeof(VEC3);
	//ID3D12Resource* pVertexBufferGPU = nullptr;
	//ID3D12Resource* pVertexBufferUploader = nullptr;
	//pVertexBufferGPU = m_pDeviceMgr->Create_DefaultBuffer(vp.data(), iByteSize, pVertexBufferUploader);
	//vbv.BufferLocation = pVertexBufferGPU->GetGPUVirtualAddress();
	//vbv.StrideInBytes = sizeof(VEC3);								// 한 정점 원소의 크기
	//vbv.SizeInBytes = (UINT)vp.size() * sizeof(VEC3);			// 버퍼의 총 크기

	//const UINT64 iByteSize2 = idx.size() * sizeof(uint16_t);
	//ID3D12Resource* pIndexBufferGPU = nullptr;
	//ID3D12Resource* pIndexBufferUploader = nullptr;
	//pIndexBufferGPU = m_pDeviceMgr->Create_DefaultBuffer(idx.data(), iByteSize2, pIndexBufferUploader);
	//ibv.BufferLocation = pIndexBufferGPU->GetGPUVirtualAddress();
	//ibv.Format = DXGI_FORMAT_R16_UINT;
	//ibv.SizeInBytes = iByteSize2;

	const UINT64 iByteSize = vtx.size() * sizeof(VEC3);
	ID3D12Resource* pVertexBufferGPU = nullptr;
	ID3D12Resource* pVertexBufferUploader = nullptr;
	pVertexBufferGPU = m_pDeviceMgr->Create_DefaultBuffer(vtx.data(), iByteSize, pVertexBufferUploader);
	vbv.BufferLocation = pVertexBufferGPU->GetGPUVirtualAddress();
	vbv.StrideInBytes = sizeof(VEC3);								// 한 정점 원소의 크기
	vbv.SizeInBytes = (UINT)vtx.size() * sizeof(VEC3);			// 버퍼의 총 크기

	m_fFovY = XMConvertToRadians(45.f);
	m_fAspect = float(g_nWinCX) / g_nWinCY;
	m_fNear = 0.1f;
	m_fFar = 1000.f;
	m_matProj = XMMatrixPerspectiveFovLH(m_fFovY, m_fAspect, m_fNear, m_fFar);

	m_vEye = { 0.f, 0.f, -30.f, 1.f };
	m_vAt = { 0.f, 0.f, 0.f, 1.f };
	m_vUp = { 0.f, 1.f, 0.f, 0.f };
	m_matView = XMMatrixLookAtLH(m_vEye, m_vAt, m_vUp);

	m_matWorld = XMMatrixIdentity();

	return NOERROR;
}

INT CMesh::Update(const FLOAT& fTimeDelta)
{
	if (CInputMgr::GetInstance()->KeyPressing(KEY_W))
	{
		m_vEye += XMVECTOR{0.f, 1.f * fTimeDelta, 0.f, 0.f};
		m_vAt += XMVECTOR{ 0.f, 1.f * fTimeDelta, 0.f, 0.f };
	}

	if (CInputMgr::GetInstance()->KeyPressing(KEY_S))
	{
		m_vEye += XMVECTOR{ 0.f, -1.f * fTimeDelta, 0.f, 0.f };
		m_vAt += XMVECTOR{ 0.f, -1.f * fTimeDelta, 0.f, 0.f };
	}

	if (CInputMgr::GetInstance()->KeyPressing(KEY_ENTER))
	{
		angle += fTimeDelta * 180.f;
		m_matWorld = XMMatrixScaling(1, 1, 1) * XMMatrixRotationX(XMConvertToRadians(angle)) * XMMatrixIdentity();
	}

	if (CInputMgr::GetInstance()->KeyPressing(KEY_A))
	{
		angle += fTimeDelta * 180.f;
		m_matWorld = XMMatrixScaling(1, 1, 1) * XMMatrixRotationY(XMConvertToRadians(angle)) * XMMatrixIdentity();
	}

	if (CInputMgr::GetInstance()->KeyPressing(KEY_D))
	{
		angle += fTimeDelta * -180.f;
		m_matWorld = XMMatrixScaling(1, 1, 1) * XMMatrixRotationY(XMConvertToRadians(angle)) * XMMatrixIdentity();
	}

	return 0;
}

VOID CMesh::Render()
{
	auto commandList = m_pDeviceMgr->Get_CommandLst();

	commandList->SetGraphicsRootSignature(m_pShaderMgr->Get_RootSignature());
	commandList->SetPipelineState(m_pPipelineState);


	m_matView = XMMatrixLookAtLH(m_vEye, m_vAt, m_vUp);

	// 여기에 넣어야댐 (확실치는 않대)
	//XMFLOAT4 color[2] = { { 1.f, 0.f, 0.f, 1.f }, { 1.f, 1.f, 0.f, 1.f } };
	XMMATRIX mat = m_matWorld * m_matView * m_matProj;
	TEST tmp = { {}, { 1.f, 0.f, 0.f, 1.f }, { 1.f, 1.f, 0.f, 1.f } };
	XMStoreFloat4x4(&tmp.mat, XMMatrixTranspose(mat));
	commandList->SetGraphicsRoot32BitConstants(0, 24, &tmp, 0);
	// root parameter index가 버퍼 인덱스가 아니라 배열의 인덱스!!!

	// 입력값 넣기 (Vertex Input)
	//commandList->IASetVertexBuffers()
	//commandList->IASetIndexBuffer()

	commandList->IASetVertexBuffers(0, 1, &vbv);
	//commandList->IASetIndexBuffer(&ibv);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->DrawInstanced(vtx.size(), 1, 0, 0);
	//commandList->DrawIndexedInstanced(idx.size(), 1, 0, 0, 0);
}

HRESULT CMesh::Load_Mesh(const string& strPath)
{
	ifstream fin{ strPath };
	if (!fin) return E_FAIL;

	string s;
	CSubMesh* submesh = nullptr;
	while (!fin.eof())
	{
		fin >> s;

		if (s == "v")
		{
			VEC3 v;
			fin >> v.x >> v.y >> v.z;
			vp.push_back(v);
		}

		if (s == "vn")
		{
			VEC3 v;
			fin >> v.x >> v.y >> v.z;
			vn.push_back(v);
		}

		if (s == "vt")
		{
			VEC2 v;
			fin >> v.x >> v.y;
			vt.push_back(v);
		}

		if (s == "g")
		{
			fin >> s;
			submesh = new CSubMesh;
			m_mapMesh[s] = submesh;
		}

		if (s == "usemtl")
		{
			fin >> s;
			//submesh->material = new CMaterial;
			//submesh->material->name = s;
		}

		if (s == "f")
		{
			char tmp;
			INT iv, ivt, ivn;
			for (int i = 0; i < 3; ++i)
			{
				// 1/1/1
				fin >> iv >> tmp >> ivt >> tmp >> ivn;
				submesh->vertices.push_back(Vertex(vp[iv - 1], vt[ivt - 1], vn[ivn - 1]));
				submesh->indices.push_back(Face(iv - 1, ivt - 1, ivn - 1));
				idx.push_back(iv - 1);
				vtx.push_back(vp[iv - 1]);
			}
		}
	}

	return NOERROR;
}

HRESULT CMesh::Release()
{
	for_each(m_mapMesh.begin(), m_mapMesh.end(), [](pair<string, CSubMesh*> p) { SafeDelete(p.second); });
	return NOERROR;
}

CMesh* CMesh::Create(const string& strPath, const wstring& strPipelineKey)
{
	CMesh* pInstance = new CMesh;
	if (FAILED(pInstance->Initialize(strPath, strPipelineKey)))
		SafeDelete(pInstance);
	return pInstance;
}
