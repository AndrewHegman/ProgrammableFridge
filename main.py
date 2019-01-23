#!/usr/bin/python

from TemperatureWatcher import TemperatureWatcher
from RelayHandler import RelayHandler
from gpiozero import DigitalOutputDevice, DigitalInputDevice
import time

def get_temperature(method="sensor"):
    pass

def main():
    temperature_monitor = TemperatureWatcher(get_temperature, time.time)
    fridge_relay = RelayHandler()

    while(True):
        temperature_monitor.Update()

        if temperature_monitor.Alarm() == "high":
            fridge_relay.On()

        elif temperature_monitor.Alarm() == "low":
            fridge_relay.Off()

        elif temperature_monitor.Alarm() == "none":
            pass

        else:
            # Create an error case..
            pass

if __name__ == "__main__":
    main()