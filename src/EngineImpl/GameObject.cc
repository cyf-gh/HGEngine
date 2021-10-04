//
// Created by cyf-m on 2020/12/8.
//

#include "../Core/Log.h"
#include "../Engine/HGBehaviour.h"
#include "GameObject.h"
#include "EngineImpl.h"
#include "Scene.h"

using namespace __HGImpl;
using namespace __HGImpl::V1SDL;
using namespace HG;
using namespace HG::Math;

bool __HGImpl::V1SDL::GameObject::IsInCameraView() {
    auto pScene = EngineImpl::GetEngine()->GetCurrentScene();
    if ( nullptr == pScene ) { return false; }
    auto pCam =  pScene->GetMainCamera();
    if( pCam == nullptr ) {
        return false;
    }
    auto pCamTransform =pCam->GetComponent<Transform>();
    auto pT = this->GetComponent<Transform>();
    auto rect1 = pT->ToHGRectGlobal();
    auto rect2 = pCamTransform->ToHGRectGlobal();
    if( pT->f64Angle == 0 ) {
        return !( ( !rect1.IsOverlap( rect2 ) ) && !rect1.IsIn( rect2 ) && !rect2.IsIn( rect1 ) );
    } else {
        HGShape<double> s;
        rect1.ToShape( s );
        return rect2.IsIntersect( s.GetCircumscribedCircle() );
    }
}

GameObject::GameObject( const char* strName, Scene* pScene )
: HGObject<GameObject>( strName ), m_pScene( pScene ), m_vecComponents(), Layer( 0 ) {
    AddComponent( new HGBehaviour( "Behaviour" ) );
    HG_EVENT_CALL( OnBeforeConstruct, nullptr, this );
    if ( m_pScene == nullptr ) {
        m_pScene = EngineImpl::GetEngine()->GetCurrentScene();
    }
    if ( m_pScene != nullptr ) {
        HG_LOG_INFO( std::string("game object [").append(GetName()).append("] constructed").c_str() );
        m_pScene->AttachGameObject( this );
    }
}

GameObject::~GameObject() {
    for( auto& c : m_vecComponents ) {
        HG_LOG_INFO( (std::string("game object [").append(GetName()) + "] component [" + c->GetName() + "] destructed").c_str() );
        HG_SAFE_DEL( c );
    }
    HG_LOG_INFO( std::string("game object [").append(GetName()).append("] destructed").c_str() );
}
