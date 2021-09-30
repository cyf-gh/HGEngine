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
            std::string mStrName;

        public:
            /// \brief all instances of this object
            static std::unordered_map<std::string, T*> umTheseOnes;
            static T* Find( const char* strName ) { return umTheseOnes.count( strName ) == 0 ? nullptr : umTheseOnes[strName]; }

            const un32 UID;
            const char* GetName() const { return mStrName.c_str(); }

            explicit Object( const char * strName ) : mStrName( strName ), UID( HGCore::RandomXORSHIFT::Random.GetRandUInt() ) {
                umTheseOnes[ strName ] = static_cast<T*>( this );
            }
            virtual ~Object() {
                umTheseOnes[ GetName() ] = nullptr;
            }
        };
        template<class T> std::unordered_map<std::string, T*>  Object<T>::umTheseOnes = std::unordered_map<std::string, T*>();
} }

#endif //HONEYGAME_OBJECT_H
