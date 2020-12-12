//
// Created by cyf-m on 2020/12/9.
//

#ifndef HONEYGAME_GAMEOBJECT2D_H
#define HONEYGAME_GAMEOBJECT2D_H

#include "../Core/Math.h"
#include "GameObject.h"

namespace __HGImpl { namespace V1 {
    class GameObject2D : public GameObject {
    private:
        SDL_Texture *pTexture;
        const std::string strFileName;

    public:
        HGCore::Math::HGPos tPosition;
        HGCore::Math::HGSize tRect;

        void Update( void* pEvent ) override;
        void Render( HGCore::Renderer *pRenderer ) override;
        void OnAttach() override;
        void OnEnable() override;
        void OnDisable() override;
        SDL_Texture * GetTexture() const { return pTexture; }
        explicit GameObject2D( const char*strObjectName, const char* strFileName="");
        virtual ~GameObject2D();
    };

}   }

#endif //HONEYGAME_GAMEOBJECT2D_H
