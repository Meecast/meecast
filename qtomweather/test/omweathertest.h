#include <cxxtest/TestSuite.h>
#include "../core/core.h"
#include <string>

class OmweatherTest : public CxxTest::TestSuite{
    public:
        void testSourceClass(void){
//default            Core::Source *s = new Core::Source("source.xml");
            Core::Source *s = NULL;
            TS_ASSERT_THROWS_ANYTHING(s = new Core::Source("null_source.xml", "../core/data/source.xsd") );
            TS_ASSERT_THROWS_ANYTHING(s = new Core::Source("source.xml", "../core/data/null_source.xsd") );
            TS_ASSERT_THROWS_NOTHING(s = new Core::Source("source.xml", "../core/data/source.xsd") );
            TS_ASSERT(s->name().compare("weather.com") == 0);
            TS_ASSERT(s->logo().compare("weather.com.png") == 0);
        }
        void testConfigClass(void){
            Core::Config *c = NULL;
            TS_ASSERT_THROWS_NOTHING(c =  new Core::Config());
            c->Base_Icons_Path(std::string("../omweather/data/icons"));
            c->Iconset(std::string("Glance"));
            c->TemperatureUnit("F");
            TS_ASSERT(c->Base_Icons_Path().compare("../omweather/data/icons") == 0);
            TS_ASSERT(c->Iconset().compare("Glance") == 0);
            TS_ASSERT(c->TemperatureUnit().compare("F")== 0);
            TS_ASSERT(c->FontColor().compare("#00ff00")== 0);
        }
};
