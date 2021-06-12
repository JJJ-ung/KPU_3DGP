#include "framework.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceFrequency(&m_CpuTick);
}

CTimeMgr::~CTimeMgr()
{
	Release();
}

DOUBLE CTimeMgr::UpdateTime()
{
	QueryPerformanceCounter(&m_FrameTime);

	m_TimeDelta = DOUBLE(m_FrameTime.QuadPart - m_FixTime.QuadPart) / DOUBLE(m_CpuTick.QuadPart);

	m_FixTime.QuadPart = m_FrameTime.QuadPart;

	if ((m_FrameTime.QuadPart - m_LastTime.QuadPart) > m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_LastTime = m_FrameTime;
	}

	return DOUBLE(m_TimeDelta);
}

BOOL CTimeMgr::FrameLimit(DOUBLE dFrameLimit)
{
	m_TimeAcc += m_TimeDelta;

	if (m_TimeAcc > dFrameLimit)
	{
		++m_iRenderCall;
		UpdateFrame();
		return TRUE;
	}

	return FALSE;
}

VOID CTimeMgr::UpdateFrame()
{
	if (1.0 <= m_TimeAcc)
	{
		wsprintf(m_szFPS, L"FPS:%d", m_iRenderCall);
		m_iRenderCall = 0;
		m_TimeAcc = 0.0;
	}
	SetWindowText(g_hWnd, m_szFPS);
}

VOID CTimeMgr::Release()
{
}
