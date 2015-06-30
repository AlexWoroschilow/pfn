'''
• 1. Zeile: wird durch ! eingeleitet und kann verschiedene Keyw ̈orter enthalten, bei der Ausgabe
in ORCA-Format soll die Zeile nach ! leer bleiben
• weitere Zeilen: optional k ̈onnen weitere Zeilen mit zus ̈atzlichen Informationen vorhanden
sein
• Geometrieblock: eingeleitet durch ∗xyz <int1> <int2>, wobei <int1> die Ladung
des Molek ̈uls, <int2> die Spinmultiplizit ̈at der Wellenfunktion beschreibt; bei der Ausgabe
in ORCA-Format soll von einem neutralen Molek ̈ul (Ladung = 0) ausgegangen werden, die
Multiplizit ̈at soll entsprechend 1 sein, wenn die Anzahl der Elektronen gerade ist, oder 2
andernfalls.
• anschließend f ̈ur jedes Atom:
– Atomtyp, x, y, z
• abschließend ∗
 ̊
Die Atomkoordinaten werden in Angstr ̈
om angegeben.
'''
import re
from Geokonverter.Entity.Molecule import Molecule
from Geokonverter.Entity.MoleculeAtom import MoleculeAtom


class EncoderORCA():
    def __init__(self):
        self.__first_pattern = '^!\s*(.+)$'
        self.__second_pattern = '^(\*xyz)\s+(\w+)\s+(\w+)\s+$'
        self.__third_pattern = '^\s+(\w+)\s+(-?\d+\.\d+)\s+(-?\d+\.\d+)\s+(-?\d+\.\d+)\s+$'


    def can_encode(self, format):
        return format in ('ORCA', 'orca', 'o')


    def encode(self, entity):

        atoms = ""
        for atom in entity.atoms:
            atoms += (" %s\t%.14f\t%.14f\t%.14f\n" % (atom.t.upper(), atom.x, atom.y, atom.z) )
        return "! \n*xyz %s %s\n%s*" % (entity.charge, entity.spin, atoms)


    def decode(self, filename):

        first = False
        second = False
        third = False

        entity = Molecule()

        with open(filename) as file:
            for num, line in enumerate(file, 1):

                if num > 3 and first and second:
                    match_third = re.search(self.__third_pattern, line)
                    if match_third and 4 == len(match_third.groups()):
                        entity.atom_count += 1
                        entity.add_atom(MoleculeAtom(
                            match_third.group(1),
                            float(match_third.group(2)),
                            float(match_third.group(3)),
                            float(match_third.group(4)),
                        ))

                elif num > 2 and first and not second:
                    match_second = re.search(self.__second_pattern, line)
                    if match_second and 3 == len(match_second.groups()):
                        entity.spin = match_second.group(3)
                        entity.charge = match_second.group(2)
                        second = True

                if first and not second and not third:
                    entity.description = entity.description + line.strip("\n")

                elif 1 == num and not first and not second:
                    match_first = re.search(self.__first_pattern, line)
                    if match_first and 1 == len(match_first.groups()):
                        entity.name = match_first.group(1)
                        first = True

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
                    if match and 3 == len(match.groups()):
                        second = True

                elif num > 3 and first and second:
                    match = re.search(self.__third_pattern, line)
                    if match and 4 == len(match.groups()):
                        third = True
                        break

        return (first and second and third)