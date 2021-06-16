#include "framework.h"
#include "DeviceMgr.h"

IMPLEMENT_SINGLETON(CDeviceMgr)

CDeviceMgr::CDeviceMgr()
{
}

CDeviceMgr::~CDeviceMgr()
{
	Release();
}

HRESULT CDeviceMgr::Init_GraphicDevice(VEC4 vBackCol)
{
	if (FAILED(Init_D3DDevice()))
		return E_FAIL;
	if (FAILED(Init_CommandQueue()))
		return E_FAIL;
	if (FAILED(Init_DescriptorHeap()))
		return E_FAIL;
	if (FAILED(Init_SwapChain()))
		return E_FAIL;
	if (FAILED(Init_DepthStencil()))
		return E_FAIL;

	memcpy(&m_vClearCol, &vBackCol, sizeof(VEC4));
	m_tResourceBarr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	m_tResourceBarr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	m_tResourceBarr.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	return NOERROR;
}

HRESULT CDeviceMgr::RenderBegin()
{
	if (FAILED(Reset_CommandAlloc()) || FAILED(Reset_CommandLst()))
		return E_FAIL;

	m_tResourceBarr.Transition.pResource = m_ppSwapChainBuff[m_iSwapChainIdx];
	m_tResourceBarr.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	m_tResourceBarr.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	m_pCommandLst->ResourceBarrier(1, &m_tResourceBarr);

	D3D12_CPU_DESCRIPTOR_HANDLE tRtvDescHwnd = m_pRtvDescHeap->GetCPUDescriptorHandleForHeapStart();
	tRtvDescHwnd.ptr += (m_iSwapChainIdx * m_iRtvDescSize);
	m_pCommandLst->ClearRenderTargetView(tRtvDescHwnd, m_vClearCol, 0, NULL);
	
	D3D12_CPU_DESCRIPTOR_HANDLE tDsvDescHwnd = m_pDsvDescHeap->GetCPUDescriptorHandleForHeapStart();
	m_pCommandLst->ClearDepthStencilView(tDsvDescHwnd, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
	m_pCommandLst->OMSetRenderTargets(1, &tRtvDescHwnd, TRUE, &tDsvDescHwnd);

	m_pCommandLst->RSSetViewports(1, &m_tViewport);
	m_pCommandLst->RSSetScissorRects(1, &m_tScissorRect);

	return NOERROR;
}

HRESULT CDeviceMgr::RenderEnd()
{
	m_tResourceBarr.Transition.pResource = m_ppSwapChainBuff[m_iSwapChainIdx];
	m_tResourceBarr.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	m_tResourceBarr.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	m_pCommandLst->ResourceBarrier(1, &m_tResourceBarr);

	if (FAILED(m_pCommandLst->Close()))
		return E_FAIL;

	ID3D12CommandList* pCmdLst[] = { m_pCommandLst };
	m_pCommandQueue->ExecuteCommandLists(1, pCmdLst);

	Wait_GPU();

	m_pSwapChain->Present(0, 0);
	m_iSwapChainIdx = m_pSwapChain->GetCurrentBackBufferIndex();

	UINT64 iFenceVal = ++m_iFenceVal[m_iSwapChainIdx];
	if (FAILED(m_pCommandQueue->Signal(m_pFence, iFenceVal)))
		return E_FAIL;

	if (m_pFence->GetCompletedValue() < iFenceVal)
	{
		if (FAILED(m_pFence->SetEventOnCompletion(iFenceVal, m_hFenceEvent)))
			return E_FAIL;
		WaitForSingleObject(m_hFenceEvent, INFINITE);
	}

	return NOERROR;
}

HRESULT CDeviceMgr::Init_SwapChain()
{
	DXGI_SWAP_CHAIN_DESC tDesc = {};
	tDesc.BufferCount = m_iSwapChainCnt;
	tDesc.BufferDesc.Width = g_nWinCX;
	tDesc.BufferDesc.Height = g_nWinCY;
	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.BufferDesc.RefreshRate.Numerator = 60;
	tDesc.BufferDesc.RefreshRate.Denominator = 1;
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	tDesc.OutputWindow = g_hWnd;
	tDesc.SampleDesc.Count = (m_bMsaa) ? 4 : 1;
	tDesc.SampleDesc.Quality = (m_bMsaa) ? (m_iMsaaLevel - 1) : 0;
	tDesc.Windowed = TRUE;
	tDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if (FAILED(m_pFactory->CreateSwapChain(m_pCommandQueue, &tDesc, (IDXGISwapChain**)&m_pSwapChain)))
		return E_FAIL;

	m_iSwapChainIdx = m_pSwapChain->GetCurrentBackBufferIndex();

	if (FAILED(m_pFactory->MakeWindowAssociation(g_hWnd, DXGI_MWA_NO_ALT_ENTER)))
		return E_FAIL;

#ifndef _WITH_SWAPCHAIN_FULLSCREEN_STATE
	if (FAILED(Init_RenderTarget()))
		return E_FAIL;
#endif

	return NOERROR;
}

HRESULT CDeviceMgr::Init_D3DDevice()
{
	UINT iFlags = 0;
#ifdef _DEBUG
	ID3D12Debug* pDebug = NULL;
	if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&pDebug))))
		return E_FAIL;
	if (pDebug)
	{
		pDebug->EnableDebugLayer();
		pDebug->Release();
	}
	iFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	if (FAILED(CreateDXGIFactory2(iFlags, IID_PPV_ARGS(&m_pFactory))))
		return E_FAIL;

	IDXGIAdapter1* pAdapter = NULL;

	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != m_pFactory->EnumAdapters1(i, &pAdapter); i++)
	{
		DXGI_ADAPTER_DESC1 tAdapterDesc;
		pAdapter->GetDesc1(&tAdapterDesc);
		if (tAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) 
			continue;
		if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_pDevice))))
			break;
	}

	if (!pAdapter)
	{
		m_pFactory->EnumWarpAdapter(IID_PPV_ARGS(&pAdapter));
		if (FAILED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_pDevice))))
			return E_FAIL;
	}

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS tLevel;
	tLevel.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tLevel.SampleCount = 4;
	tLevel.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	tLevel.NumQualityLevels = 0;
	if (FAILED(m_pDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &tLevel, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS))))
		return E_FAIL;
	m_iMsaaLevel = tLevel.NumQualityLevels;
	m_bMsaa = (m_iMsaaLevel > 1) ? true : false;

	if (FAILED(m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence))))
		return E_FAIL;
	for (UINT i = 0; i < m_iSwapChainCnt; i++)
		m_iFenceVal[i] = 0;

	m_hFenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	SafeRelease(pAdapter);

	return NOERROR;
}

HRESULT CDeviceMgr::Init_CommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC tDesc;
	::ZeroMemory(&tDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	tDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	tDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	if (FAILED(m_pDevice->CreateCommandQueue(&tDesc, IID_PPV_ARGS(&m_pCommandQueue))))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAlloc))))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCommandAlloc, NULL, IID_PPV_ARGS(&m_pCommandLst))))
		return E_FAIL;

	if (FAILED(m_pCommandLst->Close()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CDeviceMgr::Init_DescriptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC tDesc;
	::ZeroMemory(&tDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	tDesc.NumDescriptors = m_iSwapChainCnt;
	tDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	tDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	tDesc.NodeMask = 0;
	if (FAILED(m_pDevice->CreateDescriptorHeap(&tDesc, IID_PPV_ARGS(&m_pRtvDescHeap))))
		return E_FAIL;
	m_iRtvDescSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	tDesc.NumDescriptors = 1;
	tDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	if (FAILED(m_pDevice->CreateDescriptorHeap(&tDesc, IID_PPV_ARGS(&m_pDsvDescHeap))))
		return E_FAIL;
	m_iDsvDescSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	return NOERROR;
}

HRESULT CDeviceMgr::Init_RenderTarget()
{
	D3D12_CPU_DESCRIPTOR_HANDLE tDescHwnd = m_pRtvDescHeap->GetCPUDescriptorHandleForHeapStart();
	for (UINT i = 0; i < m_iSwapChainCnt; i++)
	{
		m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_ppSwapChainBuff[i]));
		m_pDevice->CreateRenderTargetView(m_ppSwapChainBuff[i], NULL, tDescHwnd);
		tDescHwnd.ptr += m_iRtvDescSize;
	}

	return NOERROR;
}

HRESULT CDeviceMgr::Init_DepthStencil()
{
	D3D12_RESOURCE_DESC tResDesc = {};
	tResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	tResDesc.Alignment = 0;
	tResDesc.Width = g_nWinCX;
	tResDesc.Height = g_nWinCY;
	tResDesc.DepthOrArraySize = 1;
	tResDesc.MipLevels = 1;
	tResDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tResDesc.SampleDesc.Count = (m_bMsaa) ? 4 : 1;
	tResDesc.SampleDesc.Quality = (m_bMsaa) ? (m_iMsaaLevel - 1) : 0;
	tResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	tResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_HEAP_PROPERTIES tHeapProp = {};
	tHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	tHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	tHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	tHeapProp.CreationNodeMask = 1;
	tHeapProp.VisibleNodeMask = 1;

	D3D12_CLEAR_VALUE tClearVal = {};
	tClearVal.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tClearVal.DepthStencil.Depth = 1.0f;
	tClearVal.DepthStencil.Stencil = 0;

	m_pDevice->CreateCommittedResource(&tHeapProp, D3D12_HEAP_FLAG_NONE, &tResDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &tClearVal, IID_PPV_ARGS(&m_pDepthStencilBuff));

	D3D12_DEPTH_STENCIL_VIEW_DESC tDSVDesc = {};
	tDSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tDSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	tDSVDesc.Flags = D3D12_DSV_FLAG_NONE;

	D3D12_CPU_DESCRIPTOR_HANDLE tDescHwnd = m_pDsvDescHeap->GetCPUDescriptorHandleForHeapStart();
	m_pDevice->CreateDepthStencilView(m_pDepthStencilBuff, &tDSVDesc, tDescHwnd);

	return NOERROR;
}

HRESULT CDeviceMgr::Wait_GPU()
{
	const UINT64 iFenceVal = ++m_iFenceVal[m_iSwapChainIdx];
	if (FAILED(m_pCommandQueue->Signal(m_pFence, iFenceVal)))
		return E_FAIL;

	if (m_pFence->GetCompletedValue() < iFenceVal)
	{
		if (FAILED(m_pFence->SetEventOnCompletion(iFenceVal, m_hFenceEvent)))
			return E_FAIL;
		WaitForSingleObject(m_hFenceEvent, INFINITE);
	}

	return NOERROR;
}

VOID CDeviceMgr::Release()
{
	CloseHandle(m_hFenceEvent);

	SafeRelease(m_pDepthStencilBuff);
	SafeRelease(m_pDsvDescHeap);

	for (int i = 0; i < m_iSwapChainCnt; i++)
		SafeRelease(m_ppSwapChainBuff[i]);
	
	SafeRelease(m_pRtvDescHeap);
	SafeRelease(m_pCommandAlloc);
	SafeRelease(m_pCommandQueue);
	SafeRelease(m_pCommandLst);

	SafeRelease(m_pFence);

	m_pSwapChain->SetFullscreenState(FALSE, NULL);
	SafeRelease(m_pSwapChain);
	SafeRelease(m_pDevice);
	SafeRelease(m_pFactory);

#ifdef _DEBUG
	IDXGIDebug1* pDebug = NULL;
	DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pDebug));
	if (FAILED(pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL)))
		return;
	SafeRelease(pDebug);
#endif
}
