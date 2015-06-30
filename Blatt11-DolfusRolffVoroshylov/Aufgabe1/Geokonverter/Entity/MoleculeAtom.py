class MoleculeAtom():
    def __init__(self, t, x, y, z):
        self.__t = t
        self.__x = x
        self.__y = y
        self.__z = z

    @property
    def t(self):
        return self.__t

    @property
    def x(self):
        return self.__x

    @property
    def y(self):
        return self.__y

    @property
    def z(self):
        return self.__z