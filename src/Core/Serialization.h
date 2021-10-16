#pragma once

// https://charon-cheung.github.io/2019/06/18/C++/Boost/Boost%E6%95%99%E7%A8%8B%EF%BC%88%E5%9B%9B%EF%BC%89%E8%AF%BB%E5%86%99JSON/#%E4%BF%AE%E6%94%B9%E6%9F%90%E4%B8%AAJSON%E5%80%BC

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <nameof.hpp>
#include <Type.h>
#include <Math.hpp>
using boost::property_tree::ptree;

#define HG_BOOST_PTREEOUT( PROP ) out.put( std::string( NAMEOF( PROP ) ), PROP )
#define HG_BOOST_PTREEOUT_CLASS( PROP ) out.put_child( std::string( NAMEOF( PROP ) ), HG::Serialization::Marshal( PROP, tmp ) ); tmp = boost::property_tree::ptree()
#define HG_BOOST_PTREEOUT_START boost::property_tree::ptree tmp

#define HG_BOOST_PTREEREADC( PROP, T ) PROP = in.get<T>( std::string( NAMEOF( PROP ) ) )

#define HG_MARSHAL_FULL_SPEC( T )	template<> HG_INLINE boost::property_tree::ptree Marshal<T>( const T& t, boost::property_tree::ptree& out )
#define HG_UNMARSHAL_FULL_SPEC( T ) template<> HG_INLINE T* Unmarshal( T* t, const boost::property_tree::ptree& in )

namespace HG {
namespace Serialization {

template<typename T>
HG_INLINE ptree Marshal( const T& t, ptree& out ) {
	HG_BOOST_PTREEOUT( t );
	return out;
}

template<typename T>
HG_INLINE T* Unmarshal( T* t, const ptree& in ) {
	return t;
}

template<typename T>
HG_INLINE ptree Marshal( const std::vector<T>& t, ptree& out ) {
	ptree c;
	for( auto& it : t ) {
		c.put( "", *it );
		out.push_back( std::make_pair( "", c ) );
	}
	return out;
}

// TODO: 完成类的marshal全特化

HG_MARSHAL_FULL_SPEC( Math::HGVec2<float> ) {
	HG_BOOST_PTREEOUT( t.X );
	HG_BOOST_PTREEOUT( t.Y );
	return out;
}

HG_UNMARSHAL_FULL_SPEC( Math::HGVec2<float> ) {
	HG_BOOST_PTREEREADC( t->X, float );
	HG_BOOST_PTREEREADC( t->Y, float );
	return t;
}


HG_MARSHAL_FULL_SPEC( Math::HGSize<un32> ) {
	HG_BOOST_PTREEOUT( t.W );
	HG_BOOST_PTREEOUT( t.H );
	return out;
}

HG_MARSHAL_FULL_SPEC( Math::HGPos ) {
	HG_BOOST_PTREEOUT( t.X );
	HG_BOOST_PTREEOUT( t.Y );
	return out;
}
}
}
