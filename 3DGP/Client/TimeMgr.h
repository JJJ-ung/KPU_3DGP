#pragma once
class CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr)

private:
	CTimeMgr();
	~CTimeMgr();

public:
	const FLOAT& GetDelta() const { return m_TimeDelta; }

public:
	FLOAT ComputeTime();
	BOOL FrameLimit(FLOAT Limit);

private:
	FLOAT								m_TimeDelta = 0.f;
	FLOAT								m_TimeAcc = 0.f;
	FLOAT								m_RenderAcc = 1.f;
	INT										m_iRenderCall = 0;
	TCHAR								m_szFrameRate[MAX_PATH] = L"";

private:
	LARGE_INTEGER	m_CurTime;
	LARGE_INTEGER	m_OldTime;
	LARGE_INTEGER	m_CpuTick;

private:
	LARGE_INTEGER	m_FrameCurTime;
	LARGE_INTEGER	m_FrameOldTime;
	LARGE_INTEGER	m_FrameCpuTick;

private:
	VOID Release();
};

