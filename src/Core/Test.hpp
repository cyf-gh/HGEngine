#pragma once
#include "./Log.h"

namespace HG {
namespace Test {
class HGTest {
public:
	std::string Name;
	void Assert( bool isSuc, const char* info ) {
		if( !isSuc ) {
			HG_LOG_TEST_ASSERT_FAILED( info );
		} else {
			HG_LOG_TEST_ASSERT_SUCCESS( info );
		}
	}
	HGTest( const char* strName ) : Name( strName ) { 
		HG_LOG_INFO( ( "[TEST]\t" + std::string( strName ) ).c_str() );
	}
	~HGTest() {}
private:

};
}
}

#define HG_TEST_START( TEST_NAME ) \
	static __HGImpl::Test::HGTest tTest( TEST_NAME )

#define HG_TEST_ASSERT_TRUE( EXPRESSION, INFO ) \
	tTest.Assert( ( EXPRESSION ), INFO );
	
#define HG_TEST_ASSERT_FALSE( EXPRESSION, INFO ) \
	tTest.Assert( !( EXPRESSION ), INFO );