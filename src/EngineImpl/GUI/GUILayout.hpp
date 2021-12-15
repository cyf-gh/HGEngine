#pragma once

#include <vector>
#include <Math.hpp>
#include "../GameObject.h"

namespace HGEngine {
namespace V1SDL {
namespace UI {
class UILayout {
public:
	HG::Math::HGRect Rect;
	std::vector<GameObject*> m_vecObjs;
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