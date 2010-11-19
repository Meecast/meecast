#include <cxxtest/TestSuite.h>
#include "../core/core.h"
#include <string>

class OmweatherTest : public CxxTest::TestSuite{
    public:
        void testSourceClass(void){
            Core::Source *s = new Core::Source("source.xml");
            TS_ASSERT( 1 + 1 > 1 );
            TS_ASSERT_EQUALS( 1 + 1, 2 );
        }
};
