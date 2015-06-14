# ============================================================================
# Name        : Simulation.py
# Author      : Dolfus, Rollf, Voroshylov
# Version     :
# Copyright   : 2015
# Description : Blatt 09, Aufgabe 3
# ============================================================================
__author__ = 'sensey'


class Simulation():
    def __init__(self):
        self.__index = None
        self.__temperature = None
        self.__pressure = None
        self.__energy = None
        self.__out_range = 0

    @property
    def index(self):
        return self.__index

    @index.setter
    def index(self, value):
        self.__index = value

    @property
    def temperature(self):
        return self.__temperature

    @temperature.setter
    def temperature(self, value):
        self.__temperature = value

    @property
    def pressure(self):
        return self.__pressure

    @pressure.setter
    def pressure(self, value):
        self.__pressure = value

    @property
    def energy(self):
        return self.__energy

    @energy.setter
    def energy(self, value):
        self.__energy = value

    @property
    def out_range(self):
        return self.__out_range

    def out_range_append(self, value):
        self.__out_range += value

    def __str__(self):
        return "Id: %s, Temperature: %s, Pressure: %s, Energie: %s, Out: %s" % (
            self.index, self.temperature, self.pressure, self.energy, self.out_range)