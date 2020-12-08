//
// Created by cyf-m on 2020/12/8.
//

#include "GameObject.h"
#include "../Core/Log.h"

using namespace __HGImpl::V1;
using namespace HGCore;


GameObject::GameObject( const char* strName ) : Object<GameObject>( strName ) {
    HG_LOG_INFO( std::string("game object [").append(GetName()).append("] constructed").c_str() );
}

GameObject::~GameObject() {
    HG_LOG_INFO( std::string("game object [").append(GetName()).append("] destructed").c_str() );
}
