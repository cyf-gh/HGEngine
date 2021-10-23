#pragma once

#include <typeinfo>
#include "HGObject.h"

namespace HGEngine { namespace V1SDL { class GameObject; } }

namespace HG {
/// \brief 
/// 组件基类，其集合由GameObject拥有<br>
/// Component base class, the set of components is owned by GameObject
/// \see HGEngine::V1SDL::GameObject
class HGComponent : public HGObject<HGComponent> {
protected:
	HGEngine::V1SDL::GameObject *m_pGameObject;
	
public:
	virtual bool IsOneOnlyPerGameObject() { return true; }
	HGEngine::V1SDL::GameObject *GetGameObject() const { return m_pGameObject; }
	void SetGameObject( HGEngine::V1SDL::GameObject *pGameObject ) { m_pGameObject = pGameObject; }
	explicit HGComponent( const char* strName ) : m_pGameObject( nullptr ), HGObject<HGComponent>( strName ) { }
	explicit HGComponent() : m_pGameObject( nullptr ), HGObject<HGComponent>() { }
	virtual ~HGComponent() = default;
};
}