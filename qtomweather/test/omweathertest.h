#include <cxxtest/TestSuite.h>
#include "../core/core.h"
#include <string>

class OmweatherTest : public CxxTest::TestSuite{
    public:
        void testSourceClass(void){
//default            Core::Source *s = new Core::Source("source.xml");
            Core::Source *s = NULL;
            TS_ASSERT_THROWS_NOTHING(s = new Core::Source("source.xml", "../core/data/source.xsd") );
            TS_ASSERT(s->name().compare("weather.com") == 0);
            TS_ASSERT(s->logo().compare("weather.com.png") == 0);
        }
};
