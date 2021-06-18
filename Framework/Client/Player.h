#pragma once
#include "Obj.h"

struct TEST
{
	XMFLOAT4X4 mat;
	XMFLOAT4 color1;
	XMFLOAT4 color2;
};

class CPlayer : public CObj
{
public:
	explicit CPlayer();
	virtual ~CPlayer();

public:
	virtual HRESULT Initialize();
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual INT LateUpdate(const FLOAT& fTimeDelta);
	virtual VOID Render();

private:
	float m_fAngle = 0.f;
	ComPtr<ID3D12Resource> m_pTexture = nullptr;
	ComPtr<ID3D12Resource> m_pUploadHeap = nullptr;

public:
	virtual VOID Release();
	static CPlayer* Create();
};

