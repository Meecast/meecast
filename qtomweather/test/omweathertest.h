#include <cxxtest/TestSuite.h>
#include "../core/core.h"
#include <string>

class OmweatherTest : public CxxTest::TestSuite{
    public:
        void testSourceClass(void){
//default            Core::Source *s = new Core::Source("source.xml");
            TS_ASSERT_THROWS_NOTHING( new Core::Source("source.xml", "../core/data/source.xsd") );
        }
};
