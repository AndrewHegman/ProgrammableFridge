#!/bin/python

class RelayHandler:
    def __init__(self):
        self.__state = "off"
        
    def On(self):
        self.__state = "on"

    def Off(self):
        self.__state = "off"

    def GetState(self):
        return self.__state
    

if __name__ == "__main__":
    pass