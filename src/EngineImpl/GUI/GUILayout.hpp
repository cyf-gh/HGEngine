#pragma once

#include <Math.hpp>

namespace HGEngine {
namespace V1SDL {
namespace UI {
class UILayout {
public:
	HG::Math::HGRect Rect;
	
};

class VerticalUILayout : public UILayout {

};

class HorizonUILayout : public UILayout {

};

class TableUILayout: public UILayout {

};

}
}
}