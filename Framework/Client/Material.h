#pragma once
class CDeviceMgr;
class CMaterial
{
public:
	typedef struct tagMaterial
	{
		INT illum;
		wstring map_Kd = L"";
		wstring map_Ka = L"";
		FLOAT Ns;
		FLOAT Ni;
		FLOAT d;
		FLOAT Tr;
		FLOAT Ka[3];
		FLOAT Kd[3];
		FLOAT Ks[3];
		FLOAT Ke[3];
		FLOAT Tf[3];
	}MATERIAL;

public:
	CMaterial();
	~CMaterial();

public:
	MATERIAL& Get_Material() { return m_tMaterialInfo; }

public:
	HRESULT Load_MaterialInfo(UINT iIndex);
	VOID Render(ID3D12GraphicsCommandList* pCommandList);

private:
	UINT m_iTexIndex = 0;
	CDeviceMgr* m_pDeviceMgr = nullptr;

private:
	MATERIAL m_tMaterialInfo = {};
	ComPtr<ID3D12Resource> m_pTexture = nullptr;
	ComPtr<ID3D12Resource> m_pUploadHeap = nullptr;
};

