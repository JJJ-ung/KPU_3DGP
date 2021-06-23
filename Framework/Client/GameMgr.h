#pragma once
class CObj;
class CScene;
class CCollision;
class CGameMgr
{
	DECLARE_SINGLETON(CGameMgr)
public:
	enum ObjID { BASE, MAP, PLAYER, BULLET, MONSTER, CAMERA, END };

private:
	CGameMgr();
	~CGameMgr();

public:
	XMMATRIX& Get_View() { return m_matView; }
	XMMATRIX& Get_Proj() { return m_matProj; }

public:
	INT Update(const float& fTimeDelta);
	INT Render();

public:
	INT ObjectUpdate(const float& fTimeDelta);
	INT ObjectLateUpdate(const float& fTimeDelta);

public:
	INT SceneUpdate(const float& fTimeDelta);

public:
	HRESULT Set_MatrixInfo(XMMATRIX proj, XMMATRIX view);
	HRESULT Add_RenderList(CObj* pObj);
	HRESULT Add_GameObj(ObjID eID, CObj* pObj);

public:
	HRESULT Set_CurrScene(CScene* pScene);
	HRESULT Clear_ObjList();

public:
	HRESULT Add_Collision(ObjID eID, CCollision* pCollision);
	HRESULT Delete_Collision(ObjID eID, CCollision* pCollision);
	list<CCollision*>& Get_CollisionList(ObjID eID);

private:
	CScene* m_pCurrScene = nullptr;
	list<CObj*> m_lstGameObj[ObjID::END];
	list<CCollision*> m_lstCollisionObj[ObjID::END];

private:
	list<CObj*> m_lstRender;

private:
	XMMATRIX m_matProj;
	XMMATRIX m_matView;
};