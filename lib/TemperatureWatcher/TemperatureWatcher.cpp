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

TemperatureWatcher::TemperatureAlarm_t TemperatureWatcher::Alarm(){
    return temperatureAlarm;
}

TemperatureWatcher::TemperatureAlarm_t TemperatureWatcher::Update(){
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

void TemperatureWatcher::SetUpdateDelaySeconds(unsigned long s){
    updateDelay = s * 1000.0L;
}
unsigned long  TemperatureWatcher::GetUpdateDelaySeconds(){
    return updateDelay / 1000.0L;
}

void  TemperatureWatcher::SetUpdateDelayMilliseconds(unsigned long ms){
    updateDelay = ms;
}
unsigned long  TemperatureWatcher::GetUpdateDelayMilliseconds(){
    return updateDelay;
}

void  TemperatureWatcher::SetUpdateDelayMicroseconds(unsigned long us){
    updateDelay = us / 1000.0L;
}
unsigned long  TemperatureWatcher::GetUpdateDelayMicroseconds(){
    return updateDelay * 1000.0L;
}

void TemperatureWatcher::SetTargetTemperature(float temp){
    targetTemperature = temp;
}

float TemperatureWatcher::GetTargetTemperature() const {
    return targetTemperature;
}

float* TemperatureWatcher::GetTargetTemperatureInstance(){
    return &targetTemperature;
}

float TemperatureWatcher::GetCurrentTemperature() const {
    return currentTemperature;
}

float* TemperatureWatcher::GetCurrentTemperatureInstance(){
    return &currentTemperature;
}

