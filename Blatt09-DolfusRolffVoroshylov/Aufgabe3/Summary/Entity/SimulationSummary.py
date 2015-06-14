# ============================================================================
# Name        : SimulationSummary.py
# Author      : Dolfus, Rollf, Voroshylov
# Version     :
# Copyright   : 2015
# Description : Blatt 09, Aufgabe 3
# ============================================================================
import math


class SimulationSummary():
    """ Initialize a simulation summary object
    calculate all values in constructor """

    def __init__(self, list):
        self.__energy = 0
        self.__energy_max = 0
        self.__energy_min = 0
        self.__energy_dev = 0
        self.__pressure = 0
        self.__pressure_max = 0
        self.__pressure_min = 0
        self.__pressure_dev = 0
        self.__temperature = 0
        self.__temperature_max = 0
        self.__temperature_min = 0
        self.__temperature_dev = 0
        self.__out_range = 0
        self.__out_range_max = 0
        self.__out_range_min = 0
        self.__out_range_dev = 0

        for element in list:
            self.__energy += element.energy
            if element.energy > self.__energy_max:
                self.__energy_max = element.energy
            if element.energy < self.__energy_min:
                self.__energy_min = element.energy

            self.__pressure += element.pressure
            if element.pressure > self.__pressure_max:
                self.__pressure_max = element.pressure
            if element.pressure < self.__pressure_min:
                self.__pressure_min = element.pressure

            self.__temperature += element.temperature
            if element.temperature > self.__temperature_max:
                self.__temperature_max = element.temperature
            if element.temperature < self.__temperature_min:
                self.__temperature_min = element.temperature

            self.__out_range += element.out_range
            if element.out_range > self.__out_range_max:
                self.__out_range_max = element.out_range
            if element.out_range < self.__out_range_min:
                self.__out_range_min = element.out_range

        self.__energy /= len(list)
        self.__pressure /= len(list)
        self.__temperature /= len(list)
        self.__out_range /= len(list)

        for element in list:
            self.__energy_dev += math.pow((element.energy - self.__energy), 2)
            self.__pressure_dev += math.pow((element.pressure - self.__pressure), 2)
            self.__out_range_dev += math.pow((element.out_range - self.__out_range), 2)
            self.__temperature_dev += math.pow((element.temperature - self.__temperature), 2)

        self.__energy_dev = math.sqrt(self.__energy_dev / len(list))
        self.__pressure_dev = math.sqrt(self.__pressure_dev / len(list))
        self.__out_range_dev = math.sqrt(self.__out_range_dev / len(list))
        self.__temperature_dev = math.sqrt(self.__temperature_dev / len(list))

    @property
    def temperature(self):
        return self.__temperature

    @property
    def temperature_max(self):
        return self.__temperature_max

    @property
    def temperature_min(self):
        return self.__temperature_min

    @property
    def temperature_dev(self):
        return self.__temperature_dev

    @property
    def pressure(self):
        return self.__pressure

    @property
    def pressure_max(self):
        return self.__pressure_max

    @property
    def pressure_min(self):
        return self.__pressure_min

    @property
    def pressure_dev(self):
        return self.__pressure_dev

    @property
    def energy(self):
        return self.__energy

    @property
    def energy_min(self):
        return self.__energy_min

    @property
    def energy_max(self):
        return self.__energy_max

    @property
    def energy_dev(self):
        return self.__energy_dev

    @property
    def out_range(self):
        return self.__out_range

    @property
    def out_range_min(self):
        return self.__out_range_min

    @property
    def out_range_max(self):
        return self.__out_range_max

    @property
    def out_range_dev(self):
        return self.__out_range_dev


    def __str__(self):
        return "=== ERGEBNISSE ===\n" \
               "- Temperatur: %.3f +/- %.3f\n" \
               "  Wertebereich: %.3f - %.3f\n" \
               "- Druck: %.3f +/- %.3f\n" \
               "  Wertebereich: %.3f - %.3f\n" \
               "- intermolekulare Energie: %.3f +/- %.3f\n" \
               "  Wertebereich: %.3f - %.3f\n" \
               "- Molekuele ausser Reichweite: %.3f +/- %.3f\n" \
               "  Wertebereich: %.3f - %.3f\n" % (
                   self.temperature, self.temperature_dev, self.temperature_min, self.temperature_max,
                   self.pressure, self.pressure_dev, self.pressure_min, self.pressure_max,
                   self.energy, self.energy_dev, self.energy_min, self.energy_max,
                   self.out_range, self.out_range_dev, self.out_range_min, self.out_range_max)
