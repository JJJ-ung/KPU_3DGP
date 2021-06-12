#include "framework.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
{
}

CTimeMgr::~CTimeMgr()
{
	Release();
}

FLOAT CTimeMgr::ComputeTime()
{
	QueryPerformanceCounter(&m_CurTime);

	if (m_CurTime.QuadPart - m_OldTime.QuadPart > m_CpuTick.QuadPart)
	{
		QueryPerformanceCounter(&m_CurTime);
		QueryPerformanceCounter(&m_OldTime);
		QueryPerformanceFrequency(&m_CpuTick);
	}

	m_TimeDelta = float(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CpuTick.QuadPart;

	m_OldTime.QuadPart = m_CurTime.QuadPart;

	m_RenderAcc += m_TimeDelta;
	++m_iRenderCall;

	if (m_RenderAcc >= 1.f)
	{
		wsprintf(m_szFrameRate, L"Client | FPS:%d", m_iRenderCall);
		m_iRenderCall = 0;
		m_RenderAcc = 0.f;
	}
	SetWindowText(g_hWnd, m_szFrameRate);

	return m_TimeDelta;
}

BOOL CTimeMgr::FrameLimit(FLOAT Limit)
{
	QueryPerformanceCounter(&m_FrameCurTime);

	if (m_FrameCurTime.QuadPart - m_FrameOldTime.QuadPart > m_FrameCpuTick.QuadPart)
	{
		QueryPerformanceCounter(&m_FrameCurTime);
		QueryPerformanceCounter(&m_FrameOldTime);
		QueryPerformanceFrequency(&m_FrameCpuTick);
	}

	m_TimeAcc += float(m_FrameCurTime.QuadPart - m_FrameOldTime.QuadPart) / m_FrameCpuTick.QuadPart;
	m_FrameOldTime.QuadPart = m_FrameCurTime.QuadPart;

	if ((1.f / Limit) < m_TimeAcc)
	{
		m_TimeAcc = 0.0;
		return TRUE;
	}

	return FALSE;
}

VOID CTimeMgr::Release()
{
}
