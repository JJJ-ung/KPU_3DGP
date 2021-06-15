#pragma once
#include "Component.h"
class CSubMesh;
class CMesh : public CComponent
{
public:
	explicit CMesh();
	virtual ~CMesh();

public:
	virtual HRESULT Initialize(const string& strPath);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual VOID Render();

private:
	HRESULT Load_Mesh(const string& strPath);

private:
	vector<VEC3> vp;	
	vector<VEC2> vt;	
	vector<VEC3> vn;

private:
	map<string, CSubMesh*> m_mapMesh;

public:
	virtual HRESULT Release();

public:
	static CMesh* Create(const string& strPath);
};

