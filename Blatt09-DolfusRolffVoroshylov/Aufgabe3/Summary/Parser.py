# ============================================================================
# Name        : Parser.py
# Author      : Dolfus, Rollf, Voroshylov
# Version     :
# Copyright   : 2015
# Description : Blatt 09, Aufgabe 3
# ============================================================================
import re, sys
from Summary.Entity.Simulation import Simulation


class Parser():
    """ Just a file constructor, try to read and
    parse file direct in constructor
    """

    def __init__(self, filename):

        self.__list = []

        try:

            file = open(filename, 'r')
            # parse file, try to find
            # required information and
            # fill list with this objects
            for element in self.__parse(file):
                self.__list.append(element)
            file.close()

        except IOError:
            print("Can not open file: ", filename)


    @property
    def list(self):
        return self.__list


    """ Parser function, read file, try to find required data
    like temperature, pressure and so on, put this data to
    Simulation model object and use yield to return this to
    """

    def __parse(self, file):

        cache = None
        for line in file.readlines():

            if 'Simulation' in line:
                # first match has no cache yet
                # we do not have to return a
                # simulation cache object
                if None != cache:
                    # return previous element
                    # than create a new one
                    yield cache

                cache = Simulation()
                cache.index = re.findall('\d+', line)[0]

            if 'temperature' in line:
                cache.temperature = float(re.findall('\d+\.\d+', line)[0])

            if 'pressure' in line:
                cache.pressure = float(re.findall('\d+\.\d+', line)[0])

            if 'E(intermolecular)' in line:
                cache.energy = float(re.findall('\d+\.\d+', line)[0])

            if 'molecule' in line:
                if None != re.search("yes", line):
                    cache.out_range_append(1)

        # no iterations more
        # needs to return a last element
        yield cache
