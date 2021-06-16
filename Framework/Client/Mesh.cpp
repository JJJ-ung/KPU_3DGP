#include "framework.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "Material.h"
#include "DeviceMgr.h"
#include "CShaderMgr.h"

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
	Release();
}

HRESULT CMesh::Initialize(const string& strPath, const wstring& strPipelineKey)
{
	//if (FAILED(Load_Mesh(strPath.c_str())))
	//	return E_FAIL;

	m_pDeviceMgr = CDeviceMgr::GetInstance();
	m_pShaderMgr = CShaderMgr::GetInstance();

	m_pPipelineState = m_pShaderMgr->Get_PipelineState(strPipelineKey);
	if (!m_pPipelineState)
		return E_FAIL;

	return NOERROR;
}

INT CMesh::Update(const FLOAT& fTimeDelta)
{
	return 0;
}

VOID CMesh::Render()
{
	auto commandList = m_pDeviceMgr->Get_CommandLst();

	commandList->SetGraphicsRootSignature(m_pShaderMgr->Get_RootSignature());
	commandList->SetPipelineState(m_pPipelineState);

	// 여기에 넣어야댐 (확실치는 않대)
	XMFLOAT4 color[2] = { { 1.f, 0.f, 0.f, 1.f }, { 1.f, 1.f, 0.f, 1.f } };
	commandList->SetGraphicsRoot32BitConstants(0, 8, &color, 0);
	// root parameter index가 버퍼 인덱스가 아니라 배열의 인덱스!!!

	// 입력값 넣긴
	//commandList->IASetVertexBuffers()
	//commandList->IASetIndexBuffer()
	// 인덱스 쓰면 인덱스로

	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->DrawInstanced(3, 1, 0, 0);
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
			}
		}
	}

	return NOERROR;
}

HRESULT CMesh::Release()
{
	return NOERROR;
}

CMesh* CMesh::Create(const string& strPath, const wstring& strPipelineKey)
{
	CMesh* pInstance = new CMesh;
	if (FAILED(pInstance->Initialize(strPath, strPipelineKey)))
		SafeDelete(pInstance);
	return pInstance;
}
