#!/bin/python

class TemperatureWatcher:
    def __init__(self, get_temperature, get_time):
        # Initialize getter functions
        self.__get_temperature = get_temperature
        self.__get_time = get_time

        self.__manual_threshold_temperature = False

        self.__target_temperature = 50.0
        self.__upper_threshold = self.__target_temperature + 1
        self.__lower_threshold = self.__target_temperature - 1

        self.__update_delay = 500
        self.__last_update = 0

        self.__current_temperature = self.__get_temperature()

        self.__alarm = "none"


    def Alarm(self):
        return self.__alarm

    def Update(self):
        if self.__last_update + self.__update_delay <= self.__get_time():
            self.__last_update = self.__get_time()
            self.__current_temperature = self.__get_temperature()

            if(self.__current_temperature > self.GetHighTemperature()):
                self.__alarm = "high"
            
            elif self.__current_temperature < self.GetLowTemperature():
                self.__alarm = "low"

            else:
                self.__alarm = "none"
                

    def GetHighTemperature(self):
        if self.__manual_threshold_temperature: return self.__upper_threshold
        else: return self.__target_temperature * 1.1
    
    def GetLowTemperature(self):
        if(self.__manual_threshold_temperature): return self.__lower_threshold
        else: return self.__target_temperature * 0.9


    def SetUpdateDelaySeconds(self, s):
        self.__update_delay = s * 1000.0

    def  GetUpdateDelaySeconds(self):
        return self.__update_delay / 1000


    def  SetUpdateDelayMilliseconds(self, ms):
        self.__update_delay = ms

    def  GetUpdateDelayMilliseconds(self):
        return self.__update_delay


    def  SetUpdateDelayMicroseconds(self, us):
        self.__update_delay = us / 1000.0

    def  GetUpdateDelayMicroseconds(self):
        return self.__update_delay * 1000.0


    def SetTargetTemperature(self, temp):
        self.__target_temperature = temp

    def SetLowerThreshold(self, temperature):
        self.__lower_threshold = temperature

    def SetUpperThreshold(self, temperature):
        self.__upper_threshold = temperature

    def GetTargetTemperature(self):
        return self.__target_temperature

    def GetCurrentTemperature(self):
        return self.__current_temperature

