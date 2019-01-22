#ifndef TIMER_H_
#define TIMER_H_

#include <Arduino.h>

class Timer{
    public:
        Timer();

        /**
         * Start timer running. Sets 'running'
         */
        void Start();

        /**
         * Stop timer. Clears 'running'
         */
        void Stop();

        /**
         * Returns true if time since
         * Start() has been called exceeds
         * 'elapsedTime'. If 'elapsedTime'
         * has not been set (=-1), always returns
         * true. Otherwise, returns false;
         */ 
        bool Expired();

        /**
         * Set 'timeout' value. Expired() will
         * return true when 'elapsedTime' > 
         * 'timeout'.
         */
        void SetTimeout(long timeout);

        /**
         * Return 'timeout' value
         */
        long GetTimeout();

        /**
         * Returns 'running' status
         */
        bool Running();

        long ElapsedTime();
    private:
        long timeout;
        long elapsedTime;
        long startTime;
        long stopTime;
        bool running;
};

#endif