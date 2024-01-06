#pragma once

#include <vector>
#include <Math.hpp>
#include "../GameObject.h"

#ifdef HGENGINE_GUI
namespace HGEngine {
namespace V1SDL {
namespace UI {
class UILayout {
public:
	HG::Math::HGRect Rect;
	std::vector<GameObject*> m_vecObjs;
};

class SingleUILayout : public UILayout {

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
#endif