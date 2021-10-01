#pragma once

#include "HGObject.h"

namespace __HGImpl { namespace V1SDL { class GameObject; } }

namespace HG {
namespace V1SDL {
class HGComponent : public HGObject<HGComponent> {
protected:
	__HGImpl::V1SDL::GameObject *m_pGameObject;
public:
	__HGImpl::V1SDL::GameObject *GetGameObject() const { return m_pGameObject; }
	void SetGameObject( __HGImpl::V1SDL::GameObject *pGameObject ) { m_pGameObject = pGameObject; }
	explicit HGComponent( const char* strName ) : HGObject<HGComponent>( strName ) { }
	virtual ~HGComponent() = default;
};
}
}