#pragma once
class CShader;
class CDeviceMgr
{
	DECLARE_SINGLETON(CDeviceMgr)

private:
	CDeviceMgr();
	~CDeviceMgr();

public:
	ID3D12Device* Get_Device() const { return m_pDevice; }
	ID3D12CommandAllocator* Get_CommandAlloc() const { return m_pCommandAlloc; }
	ID3D12GraphicsCommandList* Get_CommandLst() const { return m_pCommandLst; }

public:
	HRESULT Reset_CommandAlloc() { return m_pCommandAlloc->Reset(); }
	HRESULT Reset_CommandLst() { return m_pCommandLst->Reset(m_pCommandAlloc, nullptr); }

public:
	HRESULT Init_GraphicDevice(VEC4 vBackCol);
	HRESULT RenderBegin();
	HRESULT RenderEnd();

public:
	HRESULT Init_SwapChain();
	HRESULT Init_D3DDevice();
	HRESULT Init_CommandQueue();
	HRESULT Init_DescriptorHeap();
	HRESULT Init_RenderTarget();
	HRESULT Init_DepthStencil();

public:

private:
	HRESULT Wait_GPU();

private:
	IDXGIFactory4*									m_pFactory = nullptr;
	IDXGISwapChain3*								m_pSwapChain = nullptr;
	ID3D12Device*										m_pDevice = nullptr;

private:
	bool														m_bMsaa = false;
	UINT														m_iMsaaLevel = 0;

private:
	static const UINT									m_iSwapChainCnt = 2;
	UINT														m_iSwapChainIdx = 0;
	ID3D12Resource*								m_ppSwapChainBuff[m_iSwapChainCnt] = {};
	
private:
	ID3D12DescriptorHeap*					m_pRtvDescHeap = nullptr;
	UINT														m_iRtvDescSize = 0;

private:
	ID3D12Resource*								m_pDepthStencilBuff = nullptr;
	ID3D12DescriptorHeap*					m_pDsvDescHeap = nullptr;
	UINT														m_iDsvDescSize = 0;

private:
	ID3D12CommandAllocator*				m_pCommandAlloc = nullptr;
	ID3D12CommandQueue*					m_pCommandQueue = nullptr;
	ID3D12GraphicsCommandList*		m_pCommandLst = nullptr;

private:
	ID3D12Fence*										m_pFence = nullptr;
	UINT64													m_iFenceVal[m_iSwapChainCnt] = {};
	HANDLE													m_hFenceEvent = nullptr;

private:
	D3D12_RECT										m_tScissorRect{ 0, 0, g_nWinCX, g_nWinCY };
	D3D12_VIEWPORT								m_tViewport{ 0, 0, g_nWinCX, g_nWinCY,0, 1 };
	D3D12_RESOURCE_BARRIER			m_tResourceBarr = {};

private:
	FLOAT													m_vClearCol[4] = {};
	ID3D12Debug*										m_pDebugController = nullptr;

private:
	VOID Release();
};

