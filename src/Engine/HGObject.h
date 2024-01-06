//
// Created by cyf-m on 2020/12/8.
//

#ifndef HONEYGAME_OBJECT_H
#define HONEYGAME_OBJECT_H

#include <string>
#include <format>
#include <unordered_map>
#include <Type.h>
#include <Error.h>
#include <Random.h>
#include <Math.hpp>

namespace HG {

template<class T> class HGObject {
private:
	void EnsureUni( const char* strName ) {
		HG_ASSERT( umTheseOnesById.find( UID ) == umTheseOnesById.end() );
		if( umTheseOnes.find( strName ) != umTheseOnes.end() ) {
			mStrName = std::format( "{}_{}", strName, std::to_string( UID ) );
		}
	}
protected:
	std::string mStrName;
	HGObject* pParent;

public:
	const un32 UID;
	std::vector<HGObject*> Children;

public:
	typedef T obj_type;

	/// \brief all instances of this object
	static std::unordered_map<std::string, T*> umTheseOnes;
	static std::unordered_map<un32, T*> umTheseOnesById;

	static T* Find( const char* strName ) { return umTheseOnes.count( strName ) == 0 ? nullptr : umTheseOnes[strName]; }
	static T* FindById( const un32 unId ) { return umTheseOnesById.count( unId ) == 0 ? nullptr : umTheseOnesById[unId]; }

public:
	bool HasParent() const { return pParent != nullptr; }
	void SetParent( HGObject* pp ) {
		if( pParent != nullptr ) {
			if( pp != nullptr ) {
				pp->Children.push_back( this );
			} else {
				HG_ERASE_IN_VEC( this, pParent->Children );
			}
		}
		pParent = pp;
	}
	HGObject* GetParent() const { return pParent; }

	const char* GetName() const { return mStrName.c_str(); }

	const void SetName( const char* strName ) {
		auto node = umTheseOnes.extract( GetName() );
		if( node.empty() == false ) {
			node.key() = strName;
			umTheseOnes.insert( std::move( node ) );
		}
		mStrName = strName;
	}

	explicit HGObject( const char* strName, HGObject* pp = nullptr ) : pParent( pp ), mStrName( strName ), UID( HG::Random::RandomXORSHIFT::Random.GetRandUInt() ) {
		EnsureUni( strName );
		HGObject<T>::umTheseOnes[mStrName.c_str()] = static_cast< T* >( this );
		HGObject<T>::umTheseOnesById[UID] = static_cast< T* >( this );
	}
	explicit HGObject() : pParent( nullptr ), UID( HG::Random::RandomXORSHIFT::Random.GetRandUInt() ) {
		mStrName = std::to_string( UID );
		EnsureUni( mStrName.c_str() );
		umTheseOnes[mStrName.c_str()] = static_cast< T* >( this );
		umTheseOnesById[UID] = static_cast< T* >( this );
	}
	virtual ~HGObject() {
		umTheseOnes[GetName()] = nullptr;
		umTheseOnesById[UID] = nullptr;
	}
};

template<class T> std::unordered_map<std::string, T*>  HGObject<T>::umTheseOnes = std::unordered_map<std::string, T*>();
template<class T> std::unordered_map<un32, T*>  HGObject<T>::umTheseOnesById = std::unordered_map<un32, T*>();
}

#endif //HONEYGAME_OBJECT_H
