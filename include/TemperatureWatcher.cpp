#include "TemperatureWatcher.h"

TemperatureWatcher::TemperatureWatcher(int64_t (*getTemperature)(void), uint64_t (*getTime)(void)){

}

TemperatureWatcher::TemperatureAlarm_t TemperatureWatcher::alarm(){
    return temperatureAlarm;
}

void TemperatureWatcher::update(){
        //TODO: Check if time to update
    if(true){
        int64_t temp = getTemperature();
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

void TemperatureWatcher::setUpdateDelaySeconds(uint64_t s){
    updateDelay = s * 1000.0L;
}
uint64_t  TemperatureWatcher::getUpdateDelaySeconds(){
    return updateDelay / 1000.0L;
}

void  TemperatureWatcher::setUpdateDelayMilliseconds(uint64_t ms){
    updateDelay = ms;
}
uint64_t  TemperatureWatcher::getUpdateDelayMilliseconds(){
    return updateDelay;
}

void  TemperatureWatcher::setUpdateDelayMicroseconds(uint64_t us){
    updateDelay = us / 1000.0L;
}
uint64_t  TemperatureWatcher::getUpdateDelayMicroseconds(){
    return updateDelay * 1000.0L;
}