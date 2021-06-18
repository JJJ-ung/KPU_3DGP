#pragma once
#include "Component.h"

struct TEST
{
	XMFLOAT4X4 mat;
	XMFLOAT4 color1;
	XMFLOAT4 color2;
};

class CSubMesh;
class CMesh : public CComponent
{
public:
	explicit CMesh();
	virtual ~CMesh();

public:
	virtual HRESULT Initialize(const string& strPath, const wstring& strPipelineKey);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual VOID Render();

private:
	HRESULT Load_Mesh(const string& strPath);

private:
	vector<VEC3> vp;	
	vector<VEC2> vt;	
	vector<VEC3> vn;
	vector<uint16_t> idx;
	vector<VEC3> vtx;

private:
	XMVECTOR m_vEye{}, m_vAt{}, m_vUp{};
	FLOAT m_fFovY = 0.f, m_fAspect = 0.f, m_fNear = 0.f, m_fFar = 0.f;
	FLOAT m_fDistance = 1.f;

private:
	XMMATRIX m_matWorld{};
	XMMATRIX m_matView{};
	XMMATRIX m_matProj{};

	D3D12_VERTEX_BUFFER_VIEW vbv;
	D3D12_INDEX_BUFFER_VIEW ibv;

private:
	map<string, CSubMesh*> m_mapMesh;
	ID3D12PipelineState* m_pPipelineState = nullptr;

	float angle = 0.f;

public:
	virtual HRESULT Release();

public:
	static CMesh* Create(const string& strPath, const wstring& strPipelineKey);
};

