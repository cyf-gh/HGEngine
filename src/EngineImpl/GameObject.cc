//
// Created by cyf-m on 2020/12/8.
//

#include "Log.hpp"
#include "../Engine/HGBehaviour.h"
#include "GameObject.h"
#include "EngineImpl.h"
#include "Renderer2D.h"
#include "Transform.hpp"
#include "Camera.h"
#include "Scene.h"

using namespace HGEngine;
using namespace HGEngine::V1SDL;
using namespace HG;
using namespace HG::Math;

bool GameObject::IsInCameraView() {
    auto pScene = EngineImpl::GetEngine()->GetCurrentScene();
    if (nullptr == pScene) { return false; }
    auto pCam = pScene->GetMainCamera();
    if (pCam == nullptr) {
        return false;
    }
    auto* pCamTransform = pCam->GetComponent<Transform>();
    auto* pT = GetComponent<Transform>();
    auto& rect1 = pT->ToHGRectGlobal();
    auto& rect2 = pCamTransform->ToHGRectGlobal();
    if (IsEqual(pT->f64Angle, (double)0)) {
        return !((!rect1.IsOverlap(rect2)) && !rect1.IsIn(rect2) && !rect2.IsIn(rect1));
    }
    else {
        HGPolygon<double> s;
        HGVec2<double> c;
        rect1.ToShape(s);
        c.X = pT->tRotateCenter.X + pT->tPosition.X;
        c.Y = pT->tRotateCenter.Y + pT->tPosition.Y;
        s.Rotate(pT->f64Angle, c);
        return rect2.IsOverlap(s.GetCircumscribedCircle());
    }
}

void GameObject::renderCameraView( Renderer2D* pRenderer ) {
    auto pTransform = GetComponent<Transform>();
    if( pTransform == nullptr ) {
        return;
    }
    auto& tSrcRect = pTransform->ToSDLRectLocal();
    auto& tDestRect = pTransform->ToSDLRectGlobal();
    auto& tCenterPt = pTransform->ToSDLPoint();
    if( m_pScene != nullptr ) {
        auto pCam = m_pScene->GetMainCamera();
        if( pCam != nullptr ) {
            auto pCamTransform = pCam->GetComponent<Transform>();
            if( !IsFixedToCamera() ) {
                if( pCam->RenderInViewOnly && !this->IsInCameraView() ) {
                    return;
                }
                tDestRect.x -= pCamTransform->tPosition.X;
                tDestRect.y -= pCamTransform->tPosition.Y;
            }
        }
    }
    pRenderer->CopyEx( this, pTransform->IsZeroLocal() ? nullptr : &tSrcRect, pTransform->IsZeroGlobal() ? nullptr : &tDestRect, pTransform->f64Angle, &tCenterPt, SDL_FLIP_NONE );
}

void HGEngine::V1SDL::GameObject::Update( void* pEvent ) { 
    HG_EVENT_CALL( OnFixedUpdate, pEvent, this );
}

void HGEngine::V1SDL::GameObject::Render( void* pRenderer ) { 
    auto pRd2D = static_cast< Renderer2D* >( pRenderer );
    HG_EVENT_CALL( OnRender, pRd2D, this );
    renderCameraView( pRd2D );
}

std::vector<HG::HGComponent*> HGEngine::V1SDL::GameObject::GetRenderableComponentsSorted() const {
    std::vector<HG::HGComponent*> vecRC;
    for (auto& c : m_vecComponents) {
        if (c->IsRenderable()) {
            vecRC.push_back(c);
        }
    }
    std::sort(vecRC.begin(), vecRC.end(), [](HG::HGComponent* c1, HG::HGComponent* c2) {
        return c1->nRenderIndex < c2->nRenderIndex;
        });
    return vecRC;
}

GameObject::GameObject( const char* strName, Scene* pScene, bool isFixed2Camera, bool isGui )
: HGObject<GameObject>( strName ), m_pScene( pScene ), m_vecComponents(), m_pLayer( nullptr ), m_bIsFixedToCamera( isFixed2Camera ), m_bIsGUI( isGui ) {
    AddComponent( new HGBehaviour(  "Behaviour" ) );
	AddComponent( new Transform( "Transform"  ) );
    HG_EVENT_CALL( OnBeforeConstruct, nullptr, this );
    HG_LOG_INFO( std::format("GameObject[{}] !Constructed", GetName() ).c_str() );
    if ( m_pScene == nullptr ) {
        m_pScene = EngineImpl::GetEngine()->GetCurrentScene();
        if ( m_pScene != nullptr ) {
            m_pScene->AttachGameObject( this );
        }
    }
}

HGEngine::V1SDL::GameObject::GameObject() : HGObject<GameObject>(), m_pScene( nullptr ), m_vecComponents(), m_pLayer( nullptr ), m_bIsFixedToCamera( false ), m_bIsGUI( false ) {
    AddComponent( new HGBehaviour( "Behaviour" ) );
    HG_LOG_INFO( std::format( "GameObject[{}] !Constructed Default", UID ).c_str() );
}

GameObject::~GameObject() {
    for( auto& c : m_vecComponents ) {
        if( c != nullptr ) {
            --c->nRefCount;
            if( c->nRefCount == 0 ) {
                HG_LOG_INFO( std::format( "GameObject[{}]=>Component[{}] ~Destructed", GetName(), c->GetName() ).c_str() );
                HG_SAFE_DEL( c );
            } else {
                HG_LOG_INFO( std::format( "GameObject[{}]=>Component[{}] ~Ref-- == [{}]", GetName(), c->GetName(), c->nRefCount ).c_str() );
                c = nullptr;
            }
        }
    }
    HG_LOG_INFO( std::format( "GameObject[{}] ~Destructed", GetName() ).c_str() );
}
