#pragma once

#include "../Engine/HGEvent.h"
#include "../Engine/HGComponent.h"
#include "GameObject.h"

namespace __HGImpl {
namespace V1SDL {
/// \brief ��ײ���
class Collision : public HG::HGComponent {
private:
	char getTargetLayer() const { return m_pGameObject->Layer; }

public:
	
	explicit Collision( const char* strName ) : HGComponent( strName ) { }
	virtual ~Collision() {

	}
};
}
}