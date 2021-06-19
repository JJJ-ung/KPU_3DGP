#include "framework.h"
#include "StaticMesh.h"
#include "DeviceMgr.h"
#include "CShaderMgr.h"
#include "SubMesh.h"
#include "Material.h"

CStaticMesh::CStaticMesh()
{
}

CStaticMesh::~CStaticMesh()
{
	Release();
}

HRESULT CStaticMesh::Initialize(const string& strPath)
{
	if (FAILED(Load_MeshFromFile(strPath)))
		return E_FAIL;

	m_pDeviceMgr = CDeviceMgr::GetInstance();
	m_pShaderMgr = CShaderMgr::GetInstance();

	Setup_BufferView();

	return NOERROR;
}

INT CStaticMesh::Update(const FLOAT& fTimeDelta)
{
	return 0;
}

INT CStaticMesh::LateUpdate(const FLOAT& fTimeDelta)
{
	return 0;
}

VOID CStaticMesh::Render()
{
	auto commandList = m_pDeviceMgr->Get_CommandLst();
	for (auto p : m_mapSubMesh)
		p.second->Render(commandList);
}

HRESULT CStaticMesh::Load_MeshFromFile(const string& strPath)
{
	ifstream fin{ strPath };
	if (!fin) return E_FAIL;

	string s;
	CSubMesh* submesh = nullptr;
	vector<XMFLOAT3> vp;
	vector<XMFLOAT2> vt;
	vector<XMFLOAT3> vn;
	while (!fin.eof())
	{
		fin >> s;

		if (s == "mtllib")
		{
			fin >> s;
			if (FAILED(Load_MaterialFromFile(s)))
				return E_FAIL;
		}

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
			submesh = CSubMesh::Create();
			m_mapSubMesh[s] = submesh;
		}

		if (s == "usemtl")
		{
			fin >> s;
			auto iter = m_mapMaterial.find(s);
			if (iter == m_mapMaterial.end())
				return E_FAIL;
			if (FAILED(submesh->Add_Material(iter->second)))
				return E_FAIL;
		}

		if (s == "f")
		{
			char tmp;
			INT iv, ivt, ivn;
			for (int i = 0; i < 3; ++i)
			{
				fin >> iv >> tmp >> ivt >> tmp >> ivn;
				//submesh->Add_Vertex(vp[iv - 1], vt[ivt - 1], vn[ivn - 1]);
				submesh->Get_Vertices().push_back(CSubMesh::VERTEX(vp[iv - 1], vt[ivt - 1], vn[ivn - 1]));
			}
		}
	}

	return NOERROR;
}

HRESULT CStaticMesh::Load_MaterialFromFile(const string& strPath)
{
	ifstream fin{ strPath };
	if (!fin) return E_FAIL;
	string s = "";
	CMaterial* pMat = nullptr;

	while (!fin.eof())
	{
		fin >> s;
		
		if (s == "newmtl")
		{
			fin >> s;
			pMat = new CMaterial;
			m_mapMaterial[s] = pMat;
		}
		if (s == "Ns")
			fin >> pMat->Get_Material().Ns;
		if (s == "Ni")
			fin >> pMat->Get_Material().Ni;
		if (s == "d")
			fin >> pMat->Get_Material().d;
		if (s == "Tr")
			fin >> pMat->Get_Material().Tr;
		if (s == "Tf")
		{
			for(int i = 0; i < 3; ++i)
				fin >> pMat->Get_Material().Tf[i];
		}
		if (s == "illum")
			fin >> pMat->Get_Material().illum;
		if (s == "Ka")
		{
			for (int i = 0; i < 3; ++i)
				fin >> pMat->Get_Material().Ka[i];
		}
		if (s == "Kd")
		{
			for (int i = 0; i < 3; ++i)
				fin >> pMat->Get_Material().Kd[i];
		}
		if (s == "Ks")
		{
			for (int i = 0; i < 3; ++i)
				fin >> pMat->Get_Material().Ks[i];
		}
		if (s == "Ke")
		{
			for (int i = 0; i < 3; ++i)
				fin >> pMat->Get_Material().Ke[i];
		}
		if (s == "map_Ka")
		{
			fin >> s;
			pMat->Get_Material().map_Ka.assign(s.begin(), s.end());
		}
		if (s == "map_Kd")
		{
			fin >> s;
			pMat->Get_Material().map_Kd.assign(s.begin(), s.end());
		}
	}

	for (auto p : m_mapMaterial)
	{
		if (FAILED(p.second->Load_MaterialInfo(g_iTexIndex)))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT CStaticMesh::Setup_BufferView()
{
	for (auto p : m_mapSubMesh)
	{
		if (FAILED(p.second->Init_BufferView()))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT CStaticMesh::Release()
{
	for_each(m_mapSubMesh.begin(), m_mapSubMesh.end(), [](pair<string, CSubMesh*> p) { SafeDelete(p.second); });
	return NOERROR;
}

CStaticMesh* CStaticMesh::Create(const string& strPath)
{
	CStaticMesh* pInstance = new CStaticMesh;
	if (FAILED(pInstance->Initialize(strPath)))
		SafeDelete(pInstance);
	return pInstance;
}
