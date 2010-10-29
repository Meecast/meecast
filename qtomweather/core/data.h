#ifndef DATA_H
#define DATA_H
////////////////////////////////////////////////////////////////////////////////
#include <time.h>
#include <vector>
#include <string>
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    /* Weather data for interval from _start_time to _end_time */
    class Data {
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
            Data();
            Data(const Data& data);
            Data& operator=(const Data& data);
            void SetStartTime(time_t start_time);
            void SetEndTime(time_t end_time);
            void SetTemperature(float temperat);
            float GetTemperature();
            virtual ~Data();
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // DATA_H
