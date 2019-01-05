#include "TemperatureWatcher.h"

TemperatureWatcher::TemperatureWatcher(float (*temperatureGetter)(void), unsigned long (*timeGetter)(void)){
    getTemperature = temperatureGetter;
    getTime = timeGetter;

    targetTemperature = 50.0f;
    updateDelay = 500;

    manualThresholdTemperatures = false;

    lastUpdateTime = 0;

    currentTemperature = getTemperature();
}

TemperatureWatcher::TemperatureAlarm_t TemperatureWatcher::alarm(){
    return temperatureAlarm;
}

TemperatureWatcher::TemperatureAlarm_t TemperatureWatcher::update(){
    if(lastUpdateTime + updateDelay <=  getTime()){
        lastUpdateTime = getTime();
        currentTemperature = getTemperature();
        if(currentTemperature > getHighTemperature()){
            temperatureAlarm = TemperatureAlarm_t::alarmHigh;
        }
        else if(currentTemperature < getLowTemperature()){
            temperatureAlarm = TemperatureAlarm_t::alarmLow;
        }
        else{
            temperatureAlarm = TemperatureAlarm_t::alarmNone;
        }
    }
    return temperatureAlarm;
}

float TemperatureWatcher::getCurrentTemperature(){
    return currentTemperature;
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

void TemperatureWatcher::setTargetTemperature(float temp){
    targetTemperature = temp;
}