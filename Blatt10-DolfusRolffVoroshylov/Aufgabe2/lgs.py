import re
import copy
import sys

class lgs():
  # Übernimmt eine Liste von n-Zeilen (Strings) und
  # erzeugt daraus die Koeffizienzenmatrix, die .koeffmat
  # zugewiesen wird; falls eindeutig lösbar, wird .lsgvektor bestimmt
  def __init__(self, koeffmat):
    self.__koeffmat = list()
    self.__lsgvektor = list()

    for row in koeffmat:
      # ermögliche komma als auch leerzeichen als trennzeichen
      splitted = re.sub('\s*,\s*|\s+', ' ', row).split()
      self.__koeffmat.append([float(x) for x in splitted])

    if self.istgueltig:
      self.__gaussmat = self.__computegauss()
      self.bestimme_loesungsvektor()


  # gibt True zurück, wenn es in jeder der n Zeilen genau
  # (n+1) Einträge gibt und mindestens ein Eintrag der
  # ersten n Einträ pro Zeile ungleich Null ist
  @property
  def istgueltig(self):
    if not self.koeffmat:
      return False

    size = len(self.koeffmat[0])
    for row in self.koeffmat:
      if size is not len(row):
        return False
      if len(row) - 1 > 0:
        maximum = max(abs(x) for x in row[0:len(row) - 1])
        if self.__equal(maximum, 0):
          return False
      else:
        # Da hier eine Leerzeile ist kann die Matrix nicht gültig sein
        return False
    return True


  # gibt True zurück, wenn in jeder Zeile das letzte
  # Element gleich Null ist und das Lgs gültig
  @property
  def isthomogen(self):
    if not self.istgueltig:
      return False

    for row in self.koeffmat:
      if not self.__equal(row[len(row) - 1], 0):
        return False
    return True


  # gibt True zurück, wenn keine linearen Abhängigkeiten
  # von Zeilen gefunden werden und das LGS gültig ist;
  # wenn beim Umformen mit dem Gaussalgorithmus eine Nullzeile
  # entsteht, so sind die Zeilen nicht linear Unabhängig
  @property
  def isteindeutig(self):
    if not self.istgueltig or len(self.koeffmat) > len(self.koeffmat[0]):
      return False

    # Nutze aus, dass die Gaussmatrix bereits reduziert wurde und
    # schaue ob eine Zeile 0 ist, dann ist das LGS nicht lin. unabhängig
    for row in self.__gaussmat:
      maximum = max([abs(x) for x in row[0:len(row) - 1]])
      if self.__equal(maximum, 0):
        return False
    return True


  #gibt eine Liste mit den Unbekannten zurück
  def bestimme_loesungsvektor(self):
    #Bestimme den Vektor durch rückwärts einsetzen
    if self.isteindeutig:
      mat = self.__gaussmat
      size = len(mat) - 1
      rowsize = len(mat[0]) - 1
      vec = [0 for x in range(0, len(mat))]
      vec[size] = mat[size][rowsize] / mat[size][rowsize - 1]

      for i in range(size - 1, -1, -1):
        for j in range(i + 1, rowsize):
          vec[i] += mat[i][j] * vec[j]
        vec[i] = float(mat[i][rowsize] - vec[i]) / mat[i][i]
      self.__lsgvektor = vec


  #Berechne den Gauss algorithmus
  def __computegauss(self):
    #kopiere matrix mit allen elementen
    result = copy.deepcopy(self.koeffmat)
    for i in range(0, len(result)):
      for j in range(i + 1, len(result)):
        factor = -1 * result[j][i] / result[i][i]
        for k in range(i, len(result[i])):
          result[j][k] = result[j][k] + factor * result[i][k]
    return result


  # Die koeffizenten Matrix
  @property
  def koeffmat(self):
    return self.__koeffmat


  # Der Lösungsvektor
  @property
  def lsgvektor(self):
    return self.__lsgvektor


  # Prüft ob zwei werte gleich sind, hier float benutzt wird muss auf
  # wird auf ein epsilon geprüft.
  def __equal(self, a, b):
    return abs(a - b) <= sys.float_info.epsilon


  # Erzeugt die String ausgabe
  def __str__(self):
    string = str()
    string += "Das %s LGS mit der Koeffizientenmatrix:\n" \
              % ("homogene" if self.isthomogen else "inhomogene")

    mat = ""
    lineLength = 0
    for row in self.koeffmat:
      line = ""
      if len(row) > 0:
        for i in range(0, len(row) - 1):
          line += "{:>12.6f} ".format(row[i])
        line += "|{:>12.6f}\n".format(row[len(row) - 1])
        mat += line
      lineLength = max(lineLength, len(line))

    vec = ""
    if self.lsgvektor:
      vec += "Das lineare Gleichungssystem hat den Lösungsvektor:\n"
      for elem in self.lsgvektor:
        vec += "{:>12.6f} ".format(elem)
    else:
    	vec += "Das lineare Gleichungssystem hat keinen Lösungsvektor.\n"   

    seperator = '-' * lineLength + "\n"
    return string + seperator + mat + seperator + vec

