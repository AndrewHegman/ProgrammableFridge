/**
 * \class TemperatureWatcher
 * 
 * \brief Monitor temperature readings from
 * a temperature sensor at periodic intervals
 * 
 * This class only monitors the temperature
 * reported by the sensor. It takes no action 
 * based on the current temperature. It does
 * indicate whether the sensor is reporting a 
 * temperature above, below, or inside a custom-
 * defined threshold.
 * 
 * \version $Revision: 0.1 $
 * 
 * \date $Date: 12/30/2018 $
 * 
 */

#ifndef TEMPERATUREWATCHER_H_
#define TEMPERATUREWATCHER_H_

#include <stdint.h>

class TemperatureWatcher{
    public:
        /**
         * \brief Defines the different state that
         * the alarm can be in
         */
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
        TemperatureWatcher(float (*temperatureGetter)(), unsigned long (*timeGetter)());

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
         * Return current temperature from last reading
         * of temperature sensor
         */
        float getCurrentTemperature();

        /**
         * Check current temperature and update
         * alarm status as necessary
        **/
        TemperatureAlarm_t update();

        void setTargetTemperature(float temp);

    private:
        /**
         * \brief Maintains the state of the alarm
         */
        TemperatureAlarm_t temperatureAlarm;

        /**
         * \brief Target temperature as well as the
         * associated high/low threshold values
         */
        float targetTemperature;
        float highTargetTemperature;
        float lowTargetTemperature;

        /**
         * Last reading from temperature sensor
         */
        float currentTemperature;
        /**
         * The user will be given the option
         * to manually set the high/low threshold
         * values. Alternatively, these values can 
         * be set automatically to +/- 10% of the
         * target value, respectively. This is
         * the default mode.
         * 
         * If manualThresholdTemperatures is false,
         * the threshold values are set automatically.
         */
        bool manualThresholdTemperatures;

        /**
         * \brief Function pointer to a function
         * that will return the temperature given
         * by the sensor
         */
        float (*getTemperature)(void);
        
        /**
         * \brief Function pointer to a function
         * that returns the number of milliseconds
         * the system has been running for
         */
        unsigned long (*getTime)(void);
        unsigned long lastUpdateTime;

        /**
         * \brief Time (in ms) to wait before updating
         * alarm status
        **/
        unsigned long updateDelay;

        /**
         * \brief Return high threshold temperature
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