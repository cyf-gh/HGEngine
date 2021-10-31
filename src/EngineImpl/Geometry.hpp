#pragma once

#include "../Engine/HGComponent.h"

namespace HGEngine {
namespace V1SDL {

class Geometry : public HG::HGComponent {
public:
	HG_COMPONENT_RENDERABLE
	Geometry() : HG::HGComponent() { 
		nRenderIndex = HG::HGRenderableComponentSeq::GEOMETRY;
	}
	Geometry( const char* strName ) : HG::HGComponent( strName ) {
		nRenderIndex = HG::HGRenderableComponentSeq::GEOMETRY;
	}
	~Geometry() { }
private:

};

}
}