#ifndef TEMPERATUREWATCHER_H_
#define TEMPERATUREWATCHER_H_

#include <stdint.h>

class TemperatureWatcher{
    public:
        typedef enum{
            alarmLow,
            alarmNone,
            alarmHigh
        }TemperatureAlarm_t;

        /**
         * Constructor for class
         * Accepts a pointer to a function that
         * will return an int64_t representing
         * the current temperature.
         * This allows the 'update()' method
         * to be called at the end of the loop 
         * and using its own internal timing
         * methods, will update itself as need be
        **/
        TemperatureWatcher(float (*temperatureGetter)(), unsigned long (*timeGetter)(), float targetTemperature);

        /**
         * Get the current status of the alarm
        **/
        TemperatureAlarm_t alarm();

        void setUpdateDelaySeconds(unsigned long s);
        unsigned long getUpdateDelaySeconds();

        void setUpdateDelayMilliseconds(unsigned long ms);
        unsigned long getUpdateDelayMilliseconds();

        void setUpdateDelayMicroseconds(unsigned long us);
        unsigned long getUpdateDelayMicroseconds();
        
        /**
         * Check current temperature and update
         * alarm status as necessary
        **/
        void update();

    private:
        TemperatureAlarm_t temperatureAlarm;
        float targetTemperature;
        float highTargetTemperature;
        float lowTargetTemperature;

        bool manualThresholdTemperatures;

        float (*getTemperature)(void);
        unsigned long (*getTime)(void);
        unsigned long lastUpdateTime;

        /**
         * Time (in ms) to wait before updating
         * alarm status
        **/
        unsigned long updateDelay;

        /**
         * Return high threshold temperature
        **/
        inline float getHighTemperature(){
            if(manualThresholdTemperatures) return highTargetTemperature;
            else return targetTemperature * 1.1f;
        }

        /**
         * Return low threshold temperature
        **/
        inline float getLowTemperature(){
            if(manualThresholdTemperatures) return lowTargetTemperature;
            else return targetTemperature * 0.9f;
        }
};

#endif