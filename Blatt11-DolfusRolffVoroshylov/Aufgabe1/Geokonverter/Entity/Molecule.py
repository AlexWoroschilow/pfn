class Molecule():
    def __init__(self):
        self.__name = ""
        self.__atom_count = 0
        self.__description = ""
        self.__spin = None
        self.__charge = None
        self.__atoms = []

    @property
    def name(self):
        return self.__name

    @name.setter
    def name(self, value):
        self.__name = value

    @property
    def spin(self):
        return self.__spin

    @spin.setter
    def spin(self, value):
        self.__spin = value

    @property
    def charge(self):
        return self.__charge

    @charge.setter
    def charge(self, value):
        self.__charge = value

    @property
    def description(self):
        return self.__description

    @description.setter
    def description(self, value):
        self.__description = value

    @property
    def atom_count(self):
        return self.__atom_count

    @atom_count.setter
    def atom_count(self, value):
        self.__atom_count = value

    @property
    def atoms(self):
        return self.__atoms

    def add_atom(self, atom):
        return self.atoms.append(atom)
