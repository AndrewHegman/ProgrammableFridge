#include <Timer.h>

Timer::Timer(){
    elapsedTime = 0;
    timeout = -1;
    startTime = 0;
    stopTime = 0;
    running = false;
}

void Timer::Start(){
    startTime = millis();
    running = true;
}


void Timer::Stop(){
    running = false;
    stopTime = millis();
}

bool Timer::Expired(){
    if(timeout == -1){
        return true;
    }

    return ElapsedTime() > timeout;
}

bool Timer::Running(){
    return running;
}

long Timer::ElapsedTime(){
    if (running) return millis() - startTime;
    else return stopTime - startTime;
} 

void Timer::SetTimeout(long timeout){
    this->timeout = timeout; 
}

long Timer::GetTimeout(){
    return timeout;
}