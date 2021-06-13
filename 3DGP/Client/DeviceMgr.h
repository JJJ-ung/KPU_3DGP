#pragma once

class CDeviceMgr
{
	DECLARE_SINGLETON(CDeviceMgr)

private:
	CDeviceMgr();
	~CDeviceMgr();

public:
	ID3D12Device* GetDevice() const { return m_pd3dDevice; }
	ID3D12GraphicsCommandList* GetGraphicsCommandList() const { return m_pd3dCommandList; }

public:
	HRESULT Reset_CommandList();

public:
	HRESULT Init_GraphicDevice();
	HRESULT RenderBegin();
	HRESULT RenderEnd();

private:
	HRESULT Init_SwapChain();
	HRESULT Init_D3DDevice();
	HRESULT Init_CommandQueue();
	HRESULT Init_DescriptorHeap();
	HRESULT Init_RenderTarget();
	HRESULT Init_DepthStencil();

private:
	HRESULT Wait_GPUComplete();

private:
	IDXGIFactory4*			m_pdxgiFactory = NULL;
	IDXGISwapChain3*		m_pdxgiSwapChain = NULL;
	ID3D12Device*			m_pd3dDevice = NULL;

	bool								m_bMsaa4xEnable = false;
	UINT								m_nMsaa4xQualityLevels = 0;

	static const UINT			m_nSwapChainBuffers = 2;
	UINT								m_nSwapChainBufferIndex = 0;

	ID3D12Resource*								m_ppd3dSwapChainBackBuffers[m_nSwapChainBuffers] = {};
	ID3D12DescriptorHeap*					m_pd3dRtvDescriptorHeap = NULL;
	UINT														m_nRtvDescriptorIncrementSize = 0;

	ID3D12Resource*								m_pd3dDepthStencilBuffer = NULL;
	ID3D12DescriptorHeap*					m_pd3dDsvDescriptorHeap = NULL;
	UINT														m_nDsvDescriptorIncrementSize = 0;

	ID3D12CommandAllocator*				m_pd3dCommandAllocator = NULL;
	ID3D12CommandQueue*					m_pd3dCommandQueue = NULL;
	ID3D12GraphicsCommandList*		m_pd3dCommandList = NULL;

	ID3D12Fence*										m_pd3dFence = NULL;
	UINT64													m_nFenceValues[m_nSwapChainBuffers] = {};
	HANDLE													m_hFenceEvent = NULL;

	D3D12_RESOURCE_BARRIER			m_d3dResourceBarrier;

#ifdef _DEBUG
	ID3D12Debug* m_pd3dDebugController;
#endif

private:
	VOID Release();
};

