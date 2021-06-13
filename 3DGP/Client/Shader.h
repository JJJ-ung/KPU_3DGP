#pragma once
#include "Component.h"

class CDeviceMgr;
class CShader : public CComponent
{
public:
	const BYTE P = 0x00000001;
	const BYTE N = 0x00000002;
	const BYTE UV = 0x00000004;

public:
	explicit CShader();
	virtual ~CShader();

public:
	virtual HRESULT Initialize(wstring strPath, string strVertexShader, string strPixelShader, string strProfile
													, const BYTE flag, ID3D12RootSignature* pd3dRootSignature);
	virtual INT Update(const FLOAT& fTimeDelta);
	virtual VOID Render();

private:
	D3D12_SHADER_BYTECODE			Init_ShaderFile(wstring strPath, string strShader, string strProfile, ID3DBlob** ppd3dShaderBlob);

private:
	D3D12_INPUT_LAYOUT_DESC			Init_InputLayout(const BYTE flag);
	D3D12_RASTERIZER_DESC				Init_RasterizerState();
	D3D12_BLEND_DESC							Init_BlendState();
	D3D12_DEPTH_STENCIL_DESC		Init_DepthStencilState();

private:
	CDeviceMgr* m_pDeviceMgr = nullptr;
	ID3D12PipelineState* m_pd3dPipelineState = NULL;

private:
	virtual HRESULT Release();

public:
	static CShader* Create(wstring strPath, string strVertexShader, string strPixelShader, string strProfile
												, const BYTE flag, ID3D12RootSignature* pd3dRootSignature);
};

