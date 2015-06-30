'''
• 1. Zeile: Anzahl der Atome im Molek ̈ul N
• 2. Zeile: Kommentarzeile, ggf. leer
• 3. bis N + 2te Zeile: Atomkoordinatenangaben Atomtyp, x, y, z
'''
import re
from Geokonverter.Entity.Molecule import Molecule
from Geokonverter.Entity.MoleculeAtom import MoleculeAtom


class EncoderXYZ():
    def __init__(self):
        self.__first_pattern = '^\s*(\d+)\s*$'
        self.__second_pattern = '^(.*)$'
        self.__third_pattern = '^\s+(\w+)\s+(-?\d+\.\d+)\s+(-?\d+\.\d+)\s+(-?\d+\.\d+)\s+$'


    def can_encode(self, format):
        return format in ('XYZ', 'xyz,', 'x')


    def encode(self, entity):
        atoms = ""
        for atom in entity.atoms:
            atoms += (" %s\t%.14f\t%.14f\t%.14f\n" % (atom.t.upper(), atom.x, atom.y, atom.z) )
        return "\t%s\n\n%s" % (entity.atom_count, atoms)


    def decode(self, filename):

        entity = Molecule()

        first = False
        second = False

        with open(filename) as file:
            for num, line in enumerate(file, 1):
                if 1 == num and not first and not second:
                    match = re.search(self.__first_pattern, line)
                    if match and 1 == len(match.groups()):
                        first = True
                        entity.atom_count = match.group(1)

                elif num > 1 and first and not second:
                    match = re.search(self.__second_pattern, line)
                    if match and 1 == len(match.groups()):
                        second = True

                elif num > 2 and first and second:
                    match = re.search(self.__third_pattern, line)
                    if match and 4 == len(match.groups()):
                        entity.add_atom(MoleculeAtom(
                            match.group(1),
                            float(match.group(2)),
                            float(match.group(3)),
                            float(match.group(4))
                        ))

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

                elif num > 1 and first and not second:
                    match = re.search(self.__second_pattern, line)
                    if match and 1 == len(match.groups()):
                        second = True

                elif num > 2 and first and second:
                    match = re.search(self.__third_pattern, line)
                    if match and 4 == len(match.groups()):
                        third = True
                        break

        return (first and second and third)