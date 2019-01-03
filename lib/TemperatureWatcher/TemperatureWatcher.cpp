#include "TemperatureWatcher.h"

TemperatureWatcher::TemperatureWatcher(float (*temperatureGetter)(void), unsigned long (*timeGetter)(void), float targetTemperature){
    getTemperature = temperatureGetter;
    getTime = timeGetter;

    this->targetTemperature = targetTemperature;
    manualThresholdTemperatures = false;

    lastUpdateTime = 0;
}

TemperatureWatcher::TemperatureAlarm_t TemperatureWatcher::alarm(){
    return temperatureAlarm;
}

void TemperatureWatcher::update(){
    if(lastUpdateTime + updateDelay <=  getTime()){
        lastUpdateTime = getTime();
        float temp = getTemperature();
        if(temp > getHighTemperature()){
            temperatureAlarm = TemperatureAlarm_t::alarmHigh;
        }
        else if(temp < getLowTemperature()){
            temperatureAlarm = TemperatureAlarm_t::alarmLow;
        }
        else{
            temperatureAlarm = TemperatureAlarm_t::alarmNone;
        }
    }
}

void TemperatureWatcher::setUpdateDelaySeconds(unsigned long s){
    updateDelay = s * 1000.0L;
}
unsigned long  TemperatureWatcher::getUpdateDelaySeconds(){
    return updateDelay / 1000.0L;
}

void  TemperatureWatcher::setUpdateDelayMilliseconds(unsigned long ms){
    updateDelay = ms;
}
unsigned long  TemperatureWatcher::getUpdateDelayMilliseconds(){
    return updateDelay;
}

void  TemperatureWatcher::setUpdateDelayMicroseconds(unsigned long us){
    updateDelay = us / 1000.0L;
}
unsigned long  TemperatureWatcher::getUpdateDelayMicroseconds(){
    return updateDelay * 1000.0L;
}