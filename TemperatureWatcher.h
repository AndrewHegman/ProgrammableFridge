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
        TemperatureWatcher(int64_t (*getTemperature)(void), uint64_t (*getTime)(void));

        /**
         * Get the current status of the alarm
        **/
        TemperatureAlarm_t alarm();

        /**
         * Time (in ms) to wait before updating
         * alarm status
        **/
        uint64_t updateDelay;

        
    private:
        TemperatureAlarm_t alrm;
        int8_t targetTemperature;

        /**
         * Check current temperature and update
         * alarm status as necessary
        **/
        void update();

        int64_t (*getTemperature)(void);

        /**
         * Return high threshold temperature
        **/
        inline int8_t getHighTemperature(){
            return targetTemperature * 1.1f;
        }

        /**
         * Return low threshold temperature
        **/
        inline int8_t getLowTemperature(){
            return targetTemperature * 0.9f;
        }
}