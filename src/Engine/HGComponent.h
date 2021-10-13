#pragma once

#include <typeinfo>
#include "HGObject.h"

namespace __HGImpl { namespace V1SDL { class GameObject; } }

namespace HG {
/// \brief 
/// 组件基类，其集合由GameObject拥有<br>
/// Component base class, the set of components is owned by GameObject
/// \see __HGImpl::V1SDL::GameObject
class HGComponent : public HGObject<HGComponent> {
protected:
	__HGImpl::V1SDL::GameObject *m_pGameObject;
	
public:
	__HGImpl::V1SDL::GameObject *GetGameObject() const { return m_pGameObject; }
	void SetGameObject( __HGImpl::V1SDL::GameObject *pGameObject ) { m_pGameObject = pGameObject; }
	explicit HGComponent( const char* strName ) : m_pGameObject( nullptr ), HGObject<HGComponent>( strName ) { }
	virtual ~HGComponent() = default;
};
}