#pragma once
class CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr)

private:
	CTimeMgr();
	~CTimeMgr();

public:
	DOUBLE TimeDelta() const { return m_TimeDelta; }

public:
	DOUBLE UpdateTime();
	BOOL FrameLimit(DOUBLE dFrameLimit);
	VOID UpdateFrame();
	
private:
	DOUBLE m_TimeDelta = 0.0;
	DOUBLE m_TimeAcc = 0.0;

private:
	LARGE_INTEGER				m_FrameTime;
	LARGE_INTEGER				m_FixTime;
	LARGE_INTEGER				m_LastTime;
	LARGE_INTEGER				m_CpuTick;

private:
	UINT						m_iRenderCall = 0;
	TCHAR					m_szFPS[MAX_PATH] = L"";

private:
	VOID Release();
};

