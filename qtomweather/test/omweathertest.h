#include <cxxtest/TestSuite.h>
#include "../core/core.h"
#include <string>

class OmweatherTest : public CxxTest::TestSuite{
    public:
        void testSourceClass(void){
            Core::Source *s = NULL;
//            TS_ASSERT_THROWS_ANYTHING(s = new Core::Source("null_source.xml", "../core/data/source.xsd") );
//            TS_ASSERT_THROWS_ANYTHING(s = new Core::Source("source.xml", "../core/data/null_source.xsd") );
            TS_ASSERT_THROWS_NOTHING(s = new Core::Source("source.xml", "../core/data/source.xsd") );
            if(!s){
                TS_TRACE("Invalid source.");
                return;
            }
            TS_ASSERT(s->name().compare("weather.com") == 0);
            TS_ASSERT(s->logo().compare("weather.com.png") == 0);
        }
        void testConfigClass(void){
            Core::Config *c = NULL;
            TS_ASSERT_THROWS_NOTHING(c = new Core::Config("config.xml", "../core/data/config.xsd"));
            if(!c)
                return;
            TS_TRACE(c->prefix());
            c->TemperatureUnit("F");
            TS_ASSERT(c->iconSet().compare("Funny") == 0);
            TS_ASSERT(c->TemperatureUnit().compare("F")== 0);
            TS_ASSERT(c->FontColor().compare("#00ff00")== 0);
        }
        void testDataClass(void){
            Core::Data *d = NULL;
            TS_ASSERT_THROWS_NOTHING(d = create_and_fill_class_data_for_day_forecast());
        }
        void testDataListClass(void){
            Core::DataList *dl = NULL;
            TS_ASSERT_THROWS_NOTHING(dl = new Core::DataList());
            Core::Data *d1 = create_and_fill_class_data_for_hours_forecast();
            Core::Data *d2 = create_and_fill_class_data_for_day_forecast();
            dl->push_back(d1);
            dl->push_back(d2);
            TS_ASSERT_EQUALS( dl->size(), 2);
        }
        void testDataParser(void){
            Core::DataParser *p = NULL;
//            TS_ASSERT_THROWS_ANYTHING(p = new Core::DataParser("data.xml", "../core/data/null_data.xsd"));
            TS_ASSERT_THROWS_NOTHING(p = new Core::DataParser("data.xml", "../core/data/data.xsd"));
        }
    private:
        //////////////////////////////////////////////////////////////////////////////
        Core::Data *
        create_and_fill_class_data_for_hours_forecast()
        {
            Core::Data *wdata = new Core::Data;
            wdata->StartTime(time(NULL) - 3600);
            wdata->EndTime(time(NULL) + 3600);
            wdata->temperature_hi().value(23.0);
            wdata->temperature_low().value(15.0);
            wdata->Flike().value(18.0);
            wdata->WindSpeed(3.0);
            wdata->WindGust(4.0);
            wdata->WindDirection(std::string("NNW"));
            wdata->Icon(3);
            wdata->Text(std::string("Clear"));
            return wdata;
        }
        //////////////////////////////////////////////////////////////////////////////
        Core::Data *
        create_and_fill_class_data_for_day_forecast()
        {
            time_t      current_time, begin_current_day, end_current_day;
            struct tm   *tm = NULL;
            int         year, current_month;

            Core::Data *wdata = new Core::Data;
            /* Create current day */
            current_time = time(NULL);
            begin_current_day = current_time;
            tm = localtime(&begin_current_day);
            year = 1900 + tm->tm_year;
            current_month = tm->tm_mon;
            tm->tm_sec = 0; tm->tm_min = 0; tm->tm_hour = 0;
            tm->tm_isdst = 1;
            begin_current_day = mktime(tm);
            tm->tm_sec = 0; tm->tm_min = 59; tm->tm_hour = 23;
            end_current_day = mktime(tm);
            wdata->StartTime(begin_current_day);
            wdata->EndTime(end_current_day);
            wdata->temperature_low().value(20.0);
            wdata->temperature_hi().value(10.0);
            wdata->Flike().value(17.0);
            wdata->WindGust(2.8);
            wdata->WindSpeed(2.4);
            wdata->Humidity(845);
            wdata->Icon(4);
            wdata->Text(std::string("Cloudy"));
            return wdata;
        }
        //////////////////////////////////////////////////////////////////////////////
};
