'''
• 1. Zeile: Signalwort $coord
• 2. bis N + 1te Zeile: Atomkoordinatenangaben x, y, z, Atomtyp
• N + 2te Zeile: beginnt mit (irgend-)einem Signalwort (eingeleitet durch $), diese und alle
weiteren Zeilen k ̈onnen im Input ignoriert werden, im Output soll $end genutzt werden ̊
Die Atomkoordinaten werden in Bohr angegeben (1 Bohr = 0,529177 Angstr ̈
om).
'''
import re
from Geokonverter.Entity.Molecule import Molecule
from Geokonverter.Entity.MoleculeAtom import MoleculeAtom


class EncoderTURBOMOLE():
    def __init__(self):
        self.__first_pattern = '^.*(\$coord).*$'
        self.__second_pattern = '^\s+(-?\d+\.\d+)\s+(-?\d+\.\d+)\s+(-?\d+\.\d+)\s+(\w+)\s+$'
        self.__third_pattern = '^.*(\$end).*$'

    def can_encode(self, format):
        return format in ('TURBOMOLE', 'turbomole,', 't')

    def encode(self, entity):
        atoms = ""
        for atom in entity.atoms:
            atoms += ("\t%.14f\t%.14f\t%.14f\t%s\n" % (
                        self.to_bohr(atom.x),
                        self.to_bohr(atom.y),
                        self.to_bohr(atom.z),
                        atom.t.lower())
                      )
        return "$coord\n%s$end" % atoms

    def from_bohr(self, value):
        return float(value) * 0.529177

    def to_bohr(self, value):
        return value / 0.529177

    def decode(self, filename):

        entity = Molecule()

        first = False
        second = False
        third = False

        with open(filename) as file:
            for num, line in enumerate(file, 1):
                if 1 == num and not first and not second:
                    match = re.search(self.__first_pattern, line)
                    if match and 1 == len(match.groups()):
                        first = True

                elif num > 1 and first and not third:
                    match = re.search(self.__second_pattern, line)
                    if match and 4 == len(match.groups()):
                        second = True
                        entity.atom_count += 1
                        entity.add_atom(MoleculeAtom(
                            match.group(4),
                            self.from_bohr(match.group(1)),
                            self.from_bohr(match.group(2)),
                            self.from_bohr(match.group(3))
                        ))

                elif num > 3 and first and second:
                    match = re.search(self.__third_pattern, line)
                    if match and 1 == len(match.groups()):
                        third = True

        return entity


    def can_decode(self, filename):

        first = False
        second = False
        third = False

        with open(filename) as file:
            for num, line in enumerate(file, 1):
                if 1 == num and not first and not second:
                    match = re.search(self.__first_pattern, line)
                    if match and 1 == len(match.groups()):
                        first = True
                elif num > 2 and first and not second:
                    match = re.search(self.__second_pattern, line)
                    if match and 4 == len(match.groups()):
                        second = True
                elif num > 3 and first and second:
                    match = re.search(self.__third_pattern, line)
                    if match and 1 == len(match.groups()):
                        third = True
                        break

        return (first and second and third)