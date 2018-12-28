#include "TemperatureWatcher.h"

TemperatureWatcher::TemperatureWatcher(int64_t (*getTemperature)(void), uint64_t (*getTime)(void)){

}

TemperatureAlarm_t alarm(){
    return alrm;
}

void TemperatureWatcher::update(){
        //TODO: Check if time to update
    if(true){
        int64_t temp = getTemperature();
        if(temp > getHighTemperature()){
            alrm = TemperatureAlarm_t::alarmHigh;
        }
        else if(temp < getLowTemperature()){
            alrm = Temperaturealarm_t::alarmLow;
        }
        else{
            alrm = TemperatureAlarm_t::alarmNone;
        }
    }
}
