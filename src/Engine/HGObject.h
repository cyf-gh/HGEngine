//
// Created by cyf-m on 2020/12/8.
//

#ifndef HONEYGAME_OBJECT_H
#define HONEYGAME_OBJECT_H

#include <string>
#include <format>
#include <unordered_map>
#include <algorithm>
#include <mutex>
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
	/// \brief mutex for thread-safe access to static maps
	static std::mutex s_Mutex;

	static T* Find( const char* strName ) { 
		std::lock_guard<std::mutex> lock( s_Mutex );
		return umTheseOnes.count( strName ) == 0 ? nullptr : umTheseOnes[strName]; 
	}
	static T* FindById( const un32 unId ) { 
		std::lock_guard<std::mutex> lock( s_Mutex );
		return umTheseOnesById.count( unId ) == 0 ? nullptr : umTheseOnesById[unId]; 
	}

public:
	bool HasParent() const { return pParent != nullptr; }
	void SetParent( HGObject* pp ) {
		if( pParent != nullptr ) {
			HG_ERASE_IN_VEC( this, pParent->Children );
		}
		if( pp != nullptr ) {
			auto it = std::find( pp->Children.begin(), pp->Children.end(), this );
			if( it == pp->Children.end() ) {
				pp->Children.push_back( this );
			}
		}
		pParent = pp;
	}
	HGObject* GetParent() const { return pParent; }

	const char* GetName() const { return mStrName.c_str(); }

	void SetName( const char* strName ) {
		std::lock_guard<std::mutex> lock( s_Mutex );
		auto node = umTheseOnes.extract( GetName() );
		if( node.empty() == false ) {
			node.key() = strName;
			umTheseOnes.insert( std::move( node ) );
		}
		mStrName = strName;
	}

	explicit HGObject( const char* strName, HGObject* pp = nullptr ) : pParent( pp ), mStrName( strName ), UID( HG::Random::RandomXORSHIFT::Random.GetRandUInt() ) {
		EnsureUni( strName );
		{
			std::lock_guard<std::mutex> lock( s_Mutex );
			HGObject<T>::umTheseOnes[mStrName.c_str()] = static_cast< T* >( this );
			HGObject<T>::umTheseOnesById[UID] = static_cast< T* >( this );
		}
	}
	explicit HGObject() : pParent( nullptr ), UID( HG::Random::RandomXORSHIFT::Random.GetRandUInt() ) {
		mStrName = std::to_string( UID );
		EnsureUni( mStrName.c_str() );
		{
			std::lock_guard<std::mutex> lock( s_Mutex );
			umTheseOnes[mStrName.c_str()] = static_cast< T* >( this );
			umTheseOnesById[UID] = static_cast< T* >( this );
		}
	}
	virtual ~HGObject() {
		std::lock_guard<std::mutex> lock( s_Mutex );
		umTheseOnes.erase( GetName() );
		umTheseOnesById.erase( UID );
	}
};

template<class T> std::unordered_map<std::string, T*>  HGObject<T>::umTheseOnes = std::unordered_map<std::string, T*>();
template<class T> std::unordered_map<un32, T*>  HGObject<T>::umTheseOnesById = std::unordered_map<un32, T*>();
template<class T> std::mutex HGObject<T>::s_Mutex;
}

#endif //HONEYGAME_OBJECT_H
