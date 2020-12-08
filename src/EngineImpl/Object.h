//
// Created by cyf-m on 2020/12/8.
//

#ifndef HONEYGAME_OBJECT_H
#define HONEYGAME_OBJECT_H

#include <string>
#include <unordered_map>
#include "../Core/Type.h"
#include "../Core/Error.h"
#include "../Core/Renderer.h"
#include "../Core/Math.h"
#include "../Core/Random.h"

namespace __HGImpl { namespace V1 {
        template<class T> class Object {
        protected:
            bool mIsEnable = false;
            std::string mStrName;

        public:
            /// \brief all instances of this object
            static std::unordered_map<const char*, T*> umTheseOnes;
            static T* Find( const char* strName ) { return umTheseOnes; }

            const un32 UID;
            const char* GetName() const { return mStrName.c_str(); }

            /// \brief will be invoked every
            virtual void Update( void* pEvent ) = 0;
            virtual void Render( HGCore::Renderer *pRenderer ) = 0;
            virtual void OnEnable() = 0;
            virtual void OnDisable() = 0;
            void Enable() { mIsEnable = true; OnEnable(); }
            void Disable() { mIsEnable = false; OnDisable(); }
            bool IsEnable() const { return mIsEnable; }
            explicit Object( const char * strName ) : mStrName( strName ), UID( HGCore::RandomXORSHIFT::Random.GetRandUInt() ) {
                umTheseOnes[ strName ] = static_cast<T*>( this );
            }
            virtual ~Object() {
                umTheseOnes[GetName()] = nullptr;
            }
        };
        template<class T> std::unordered_map<const char*, T*>  Object<T>::umTheseOnes = std::unordered_map<const char*, T*>();
} }

#endif //HONEYGAME_OBJECT_H
