#pragma once

#undef GetObject
#include <string>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <nameof.hpp>
#include "Log.h"
#include "Type.h"
#include "Math.hpp"

#define HG_MARSHAL_START						out.StartObject();
#define HG_MARSHAL_END							out.EndObject();

/// \brief 设置键名
/// \note 
/// * 当strName为空字符串时，不会设置键名
#define HG_MARSHAL_SETKEY						if ( strcmp( strName, "" ) != 0 ) { out.Key( strName ); }

/// \brief 宣告开始构建对象序列化
/// \note 
/// * 当构建对象序列化时，该对象会以 "ObjName" : { ... } 的形式被附加于 out 中
/// * 存在将亡 JSON 字符串储存器 buffer 与 Writer writer
#define HG_MARSHAL_OBJECT_START					rapidjson::StringBuffer buffer; rapidjson::Writer<rapidjson::StringBuffer> writer(buffer); writer.StartObject()
#define HG_MARSHAL_OBJECT_SETPROP( PROP )		HG::Serialization::Marshal( PROP, std::string( NAMEOF( PROP ) ).c_str(), writer )
#define HG_MARSHAL_OBJECT_END					 writer.EndObject(); HG_MARSHAL_SETKEY; out.RawValue( buffer.GetString(), strlen( buffer.GetString() ), rapidjson::kObjectType ); return out;
#define HG_MARSHAL_SETOBJ( PROP )				HG::Serialization::Marshal( PROP, std::string( NAMEOF( PROP ) ).c_str(), out )

#define HG_UNMARSHAL_OBJECT_START				rapidjson::Document d; d.CopyFrom( in, rd.GetAllocator() )
#define HG_UNMARSHAL_OBJECT_END					END: return t
#define HG_UNMARSHAL_GETOBJ( PROP )				Unmarshal( PROP, std::string( NAMEOF( PROP ) ).c_str(), d[std::string( NAMEOF( PROP ) ).c_str()], rd )

#define HG_DEBUG_UNMARSHAL_OBJECT				for( auto& m : d.GetObject() ) { HG_LOG_INFO( m.name.GetString() ); }

#define HG_MARSHAL_FULLSPEC( T )				template<> HG_INLINE rapidjson::Writer<rapidjson::StringBuffer>& Marshal<T>( T& t, const char* strName, rapidjson::Writer<rapidjson::StringBuffer>& out )
#define HG_UNMARSHAL_FULLSPEC( T )				template<> HG_INLINE T& Unmarshal( T& t, const char* strName, const rapidjson::Value& in, rapidjson::Document& rd )

namespace HG {
/// \brief 对象序列化
/// \note 
/// * 当对象的属性被暴露时，应当重写该对象类型的 Marshal Unmarshal 全特化模板函数
namespace Serialization {

template<typename T>
HG_INLINE rapidjson::Writer<rapidjson::StringBuffer>& Marshal( T& t, const char* strName, rapidjson::Writer<rapidjson::StringBuffer>& out ) {
	HG_LOG_WARNNING( std::format( "In default Marshal. Node[{}] escaped", strName ).c_str() );
	return out;
}

template<typename T>
HG_INLINE rapidjson::Writer<rapidjson::StringBuffer>& Marshal( std::vector<T>& t, const char* strName, rapidjson::Writer<rapidjson::StringBuffer>& out ) {
	HG_MARSHAL_SETKEY;
	out.StartArray();
	for( auto& i : t ) {
		Marshal( i, "", out );
	}
	out.EndArray();
	return out;
}

template<typename T>
HG_INLINE T& Unmarshal( T& t, const char* strName, const rapidjson::Value& in, rapidjson::Document& rd ) {
	HG_LOG_WARNNING( std::format( "In default Unmarshal. Node[{}] escaped", strName ).c_str() );
	return t;
}

template<typename T, typename A>
HG_INLINE std::vector<T*, A>& Unmarshal( std::vector<T*, A>& t, const char* strName, const rapidjson::Value& in, rapidjson::Document& rd ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_ASSERT( in.IsArray() );
	for( auto& it : in.GetArray() ) {
		T* _t = nullptr;
		HG_ASSERT( it.IsObject() );
		_t = Unmarshal( ( _t ), strName, it.GetObject(), rd );
		if( _t != nullptr ) {
			t.push_back( _t );
		}
	}
	return t;
}

template<typename T, typename A>
HG_INLINE std::vector<T, A>& Unmarshal( std::vector<T, A>& t, const char* strName, const rapidjson::Value& in, rapidjson::Document& rd ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_ASSERT( in.IsArray() );
	for( auto& it : in.GetArray() ) {
		T _t;
		HG_ASSERT( it.IsObject() );
		Unmarshal( ( _t ), strName, it.GetObject(), rd );
		t.push_back( _t );
	}
	return t;
}

template<typename T, typename A>
HG_INLINE std::vector<std::vector<T, A>>& Unmarshal( std::vector<std::vector<T, A>>& t, const char* strName, const rapidjson::Value& in, rapidjson::Document& rd ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_ASSERT( in.IsArray() );
	for( auto& it : in.GetArray() ) {
		std::vector<T, A> _t;
		HG_ASSERT( it.IsArray() );
		Unmarshal( ( _t ), strName, it, rd );
		t.push_back( ( _t ) );
	}
	return t;
}

HG_MARSHAL_FULLSPEC( un32 ) {
	HG_MARSHAL_SETKEY;
	out.Int( static_cast< int >( t ) );
	return out;
}

HG_UNMARSHAL_FULLSPEC( un32 ) {
	t = static_cast< un32 >( in.GetInt() );
	return t;
}

HG_MARSHAL_FULLSPEC( n32 ) {
	HG_MARSHAL_SETKEY;
	out.Int( static_cast< int >( t ) );
	return out;
}

HG_UNMARSHAL_FULLSPEC( n32 ) {
	t = static_cast< n32 >( in.GetInt() );
	return t;
}

HG_MARSHAL_FULLSPEC( float ) {
	HG_MARSHAL_SETKEY;
	out.Double( static_cast< double >( t ) );
	return out;
}

HG_UNMARSHAL_FULLSPEC( float ) {
	t = static_cast< float >( in.GetDouble() );
	return t;
}

HG_MARSHAL_FULLSPEC( bool ) {
	HG_MARSHAL_SETKEY;
	out.Bool( t );
	return out;
}

HG_UNMARSHAL_FULLSPEC( bool ) {
	t = ( in.GetBool() );
	return t;
}

HG_MARSHAL_FULLSPEC( double ) {
	HG_MARSHAL_SETKEY;
	out.Double( t );
	return out;
}

HG_UNMARSHAL_FULLSPEC( double ) {
	t = in.GetDouble();
	return t;
}

HG_MARSHAL_FULLSPEC( std::string ) {
	HG_MARSHAL_SETKEY;
	out.String( t.c_str() );
	return out;
}

HG_UNMARSHAL_FULLSPEC( std::string ) {
	t = in.GetString();
	return t;
}


////////////////////////////////////////////////////////////////////////////////////////////////

HG_MARSHAL_FULLSPEC( Math::HGVec2<float> ) {
	HG_MARSHAL_OBJECT_START;
	HG_MARSHAL_OBJECT_SETPROP( t.X );
	HG_MARSHAL_OBJECT_SETPROP( t.Y );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( Math::HGVec2<float> ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_UNMARSHAL_GETOBJ( t.X );
	HG_UNMARSHAL_GETOBJ( t.Y );
	HG_UNMARSHAL_OBJECT_END;
}

HG_MARSHAL_FULLSPEC( Math::HGSize<un32> ) {
	HG_MARSHAL_OBJECT_START;
	HG_MARSHAL_OBJECT_SETPROP( t.W );
	HG_MARSHAL_OBJECT_SETPROP( t.H );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( Math::HGSize<un32> ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_UNMARSHAL_GETOBJ( t.W );
	HG_UNMARSHAL_GETOBJ( t.H );
	HG_UNMARSHAL_OBJECT_END;
}

HG_MARSHAL_FULLSPEC( Math::HGPos ) {
	HG_MARSHAL_OBJECT_START;
	HG_MARSHAL_OBJECT_SETPROP( t.X );
	HG_MARSHAL_OBJECT_SETPROP( t.Y );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( Math::HGPos ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_UNMARSHAL_GETOBJ( t.X );
	HG_UNMARSHAL_GETOBJ( t.Y );
	HG_UNMARSHAL_OBJECT_END;
}

HG_MARSHAL_FULLSPEC( Math::HGRect ) {
	HG_MARSHAL_OBJECT_START;
	HG_MARSHAL_OBJECT_SETPROP( t.X );
	HG_MARSHAL_OBJECT_SETPROP( t.Y );
	HG_MARSHAL_OBJECT_SETPROP( t.W );
	HG_MARSHAL_OBJECT_SETPROP( t.H );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( Math::HGRect ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_UNMARSHAL_GETOBJ( t.X );
	HG_UNMARSHAL_GETOBJ( t.Y );
	HG_UNMARSHAL_GETOBJ( t.W );
	HG_UNMARSHAL_GETOBJ( t.H );
	HG_UNMARSHAL_OBJECT_END;
}


}

}
