#pragma once
#include "Component.h"
class CMaterial;
class CSubMesh;
class CDeviceMgr;
class CShaderMgr;
class CStaticMesh : public CComponent
{
public:
	explicit CStaticMesh();
	virtual ~CStaticMesh();

public:
	virtual HRESULT Initialize(const string& strPath);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual INT LateUpdate(const FLOAT& fTimeDelta);
	virtual VOID Render();

private:
	HRESULT Load_MeshFromFile(const string& strPath);
	HRESULT Load_MaterialFromFile(const string& strPath);
	HRESULT Setup_BufferView();

private:
	map<string, CMaterial*> m_mapMaterial;
	map<string, CSubMesh*> m_mapSubMesh;

private:
	D3D12_VERTEX_BUFFER_VIEW vbv{};

public:
	virtual HRESULT Release();
	static CStaticMesh* Create(const string& strPath);
};

