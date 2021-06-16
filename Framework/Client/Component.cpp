#include "framework.h"
#include "Component.h"

CComponent::CComponent()
{
}

CComponent::~CComponent()
{
}

HRESULT CComponent::Initialize()
{
	return NOERROR;
}

INT CComponent::Update(const FLOAT& fTimeDelta)
{
	return 0;
}

INT CComponent::LateUpdate(const FLOAT& fTimeDelta)
{
	return 0;
}

VOID CComponent::Render()
{
	return VOID();
}

HRESULT CComponent::Release()
{
	return NOERROR;
}
