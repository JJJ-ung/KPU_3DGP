#pragma once
class CMaterial;
class CDeviceMgr;
class CSubMesh
{
public:
	typedef struct tagVertex
	{
		tagVertex() {}
		tagVertex(XMFLOAT3 p, XMFLOAT2 t, XMFLOAT3 n)
			: vp(p), vt(t), vn(n) {}
		XMFLOAT3 vp;
		XMFLOAT2 vt;
		XMFLOAT3 vn;
	}VERTEX;

public:
	CSubMesh();
	~CSubMesh();

public:
	vector<VERTEX>& Get_Vertices() { return vertices; }

public:
	VOID Render(ID3D12GraphicsCommandList* pCommandList);

public:
	HRESULT Init_BufferView();
	HRESULT Add_Vertex(XMFLOAT3 p, XMFLOAT2 t, XMFLOAT3 n);
	HRESULT Add_Material(CMaterial* pMat);

private:
	vector<VERTEX> vertices{};
	D3D12_VERTEX_BUFFER_VIEW vbv;

private:
	CMaterial* m_pMaterial = nullptr;

private:
	CDeviceMgr* m_pDeviceMgr = nullptr;

public:
	virtual HRESULT Release();
	static CSubMesh* Create();
};
