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
	HRESULT Set_RootSignature(ID3D12RootSignature* pRootSig) { if (!pRootSig) return E_FAIL; m_pRootSignature = pRootSig; }

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
	// 그리기 명령을 제출하기 전에 파이프라인에 묶어야 할 자원들이 무엇이고 그 자원들이 쉐이더 입력 레지스터들에 어떻게 대응되는지를 정의한다
	HRESULT Set_RootParameter(D3D12_ROOT_SIGNATURE_DESC tRootDesc);
	D3D12_GRAPHICS_PIPELINE_STATE_DESC Create_PipelineDesc(CShader* pVertexShader, CShader* pPixelShader);

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
	D3D12_RESOURCE_BARRIER			m_tResourceBarr = {};
	ID3D12RootSignature*						m_pRootSignature = nullptr;

private:
	FLOAT													m_vClearCol[4] = {};
	ID3D12Debug*										m_pDebugController = nullptr;

private:
	VOID Release();
};

