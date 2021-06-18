#include "framework.h"
#include "GameMgr.h"

#include "Obj.h"
#include "Scene.h"

IMPLEMENT_SINGLETON(CGameMgr)

CGameMgr::CGameMgr()
{
}

CGameMgr::~CGameMgr()
{
	SafeDelete(m_pCurrScene);
}

INT CGameMgr::Update(const float& fTimeDelta)
{
	ObjectUpdate(fTimeDelta);
	ObjectLateUpdate(fTimeDelta);
	return m_pCurrScene->Update(fTimeDelta);
}

INT CGameMgr::Render()
{
	for (auto p : m_lstRender)
		p->Render();
	m_lstRender.clear();
	return 0;
}

INT CGameMgr::ObjectUpdate(const float& fTimeDelta)
{
	for (int i = 0; i < END; ++i)
	{
		auto iter = m_lstGameObj[i].begin();
		for (; iter != m_lstGameObj[i].end(); )
		{
			if ((*iter)->Update(fTimeDelta) == OBJDEAD)
			{
				SafeDelete(*iter);
				iter = m_lstGameObj[i].erase(iter);
			}
			++iter;
		}
	}

	return 0;
}

INT CGameMgr::ObjectLateUpdate(const float& fTimeDelta)
{
	for (int i = 0; i < END; ++i)
	{
		auto iter = m_lstGameObj[i].begin();
		for (; iter != m_lstGameObj[i].end(); )
		{
			if ((*iter)->LateUpdate(fTimeDelta) == OBJDEAD)
			{
				SafeDelete(*iter);
				iter = m_lstGameObj[i].erase(iter);
			}
			++iter;
		}
	}

	return 0;
}

INT CGameMgr::SceneUpdate(const float& fTimeDelta)
{
	return m_pCurrScene->Update(fTimeDelta);
}

HRESULT CGameMgr::Set_MatrixInfo(XMMATRIX proj, XMMATRIX view)
{
	m_matProj = proj;
	m_matView = view;
	return NOERROR;
}

HRESULT CGameMgr::Add_RenderList(CObj* pObj)
{
	if (!pObj) return E_FAIL;
	m_lstRender.emplace_back(pObj);
	return NOERROR;
}

HRESULT CGameMgr::Add_GameObj(ObjID eID, CObj* pObj)
{
	if (!pObj) return E_FAIL;
	m_lstGameObj[eID].emplace_back(pObj);
	return NOERROR;
}

HRESULT CGameMgr::Set_CurrScene(CScene* pScene)
{
	if (!pScene) return E_FAIL;
	SafeDelete(m_pCurrScene);
	m_pCurrScene = pScene;
	//Clear_ObjList();
	return NOERROR;
}

HRESULT CGameMgr::Clear_ObjList()
{
	for (int i = 0; i < END; ++i)
	{
		for_each(m_lstGameObj[i].begin(), m_lstGameObj[i].end(), [](CObj* p) { SafeDelete(p); });
		m_lstGameObj[i].clear();
	}
	m_lstRender.clear();

	return NOERROR;
}
