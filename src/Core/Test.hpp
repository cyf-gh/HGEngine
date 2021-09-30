#include "./Log.h"

namespace HGCore {
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
	HGTest( const char* strName ):Name( strName ) { }
	~HGTest() {}

private:

};
}
}

#define HG_TEST_START( TEST_NAME ) \
	HGCore::Test::HGTest t( TEST_NAME ) \

#define HG_TEST_ASSERT_TRUE( isTrue ) 
	
