#include "framework.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "Material.h"

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
	Release();
}

HRESULT CMesh::Initialize(const string& strPath)
{
	if (FAILED(Load_Mesh(strPath.c_str())))
		return E_FAIL;

	return NOERROR;
}

INT CMesh::Update(const FLOAT& fTimeDelta)
{
	return 0;
}

VOID CMesh::Render()
{
	return VOID();
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

CMesh* CMesh::Create(const string& strPath)
{
	CMesh* pInstance = new CMesh;
	if (FAILED(pInstance->Initialize(strPath)))
		SafeDelete(pInstance);
	return pInstance;
}
