#ifndef DATA_H
#define DATA_H
////////////////////////////////////////////////////////////////////////////////
#include "parser.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    /* Weather data for interval from _start_time to _end_time */
    class WeatherIntervalData {
        time_t _start_time;
        time_t _end_time;
        float _temperature;
        float _flike;
        float _windSpeed;
        std::string _windDirection;
        float _windGust;
        int _humidity;
        int _pressure;
        std::string _text;
        int _icon;

        public:
            WeatherIntervalData();
            WeatherIntervalData(const WeatherIntervalData& data);
            WeatherIntervalData& operator=(const WeatherIntervalData& data);
            virtual ~WeatherIntervalData(){};
    };

    /* All WeatherIntervalData */
    class Data {
        std::vector <WeatherIntervalData> data_array;

        public:
            Data();
            virtual ~Data(){};
    };

} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // DATA_H
