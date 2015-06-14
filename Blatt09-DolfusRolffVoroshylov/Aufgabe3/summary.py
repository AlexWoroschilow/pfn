#!/usr/bin/env python3
# ============================================================================
# Name        : summary.py
# Author      : Dolfus, Rollf, Voroshylov
# Version     :
# Copyright   : 2015
# Description : Blatt 09, Aufgabe 3
# ============================================================================

import sys
from Summary.Entity.SimulationSummary import SimulationSummary
from Summary.Parser import Parser


def main():
    parser = Parser('Messwerte.txt')
    print(SimulationSummary(parser.list))


if __name__ == "__main__":
    sys.exit(main())

