import math
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt

class daten_analyse:
  def __init__(self, file):
    self.file = file
    self.temperatures = np.array([])
    self.flowRates = np.array([])
    self.measurement = np.array([])
    self.idx = np.array([])
    self.tests = np.array([0])
    
    # Lese Datei und Daten ein
    with open(file) as f:
      for line in f:
        line = line.strip().split()
        if "Temperatur:" in line:
          self.temperatures = np.append(self.temperatures,
              float(line[line.index("Temperatur:") + 1]))
        elif "Fliessgeschwindigkeit:" in line:
          self.flowRates = np.append(self.flowRates,
              float(line[line.index("Fliessgeschwindigkeit:") + 1]))
        elif "Nein" in line:
          self.tests[-1] +=  1
        elif "Ja" in line:
          self.tests[-1] += 1
          self.tests = np.append(self.tests, 0)
        elif "Messwert:" in line:
          self.measurement = np.append(self.measurement,
              float(line[line.index("Messwert:") + 1]))
        elif "Messung" in line:
          self.idx = np.append(self.idx, int(line[
              line.index("Messung") - 1][:-1]))

    # Entferne letztes element, da dieses nur zum
    # erzeugen des arrays gebraucht wrd.
    self.tests = np.delete(self.tests, -1)

  # Berechne mittelwert der Temperatur mit Numpy
  @property
  def mean_T(self):
    return np.mean(self.temperatures)

  # Berechne mittelwert der Fliessgeschwidigkeit
  @property
  def mean_v(self):
    return np.mean(self.flowRates)

  # Berechne mittelwert der Anzahl der notwendigen Tests
  @property
  def mean_n(self):
    return np.mean(self.tests)

  # Berechne mittelwert der Messwerte
  @property
  def mean_w(self):
    return np.mean(self.measurement)

  # Berechne Standardabweichung der Temperaturen
  @property
  def std_T(self):
    return np.std(self.temperatures)

  # Berechne Standardabweichungen der Fliessgeschw.
  @property
  def std_v(self):
    return np.std(self.flowRates)

  # Berechne Standardabweichung der Anzahl der n. Tests
  @property
  def std_n(self):
    return np.std(self.tests)

  # Berechne Standardabweichung der Messwerte
  @property
  def std_w(self):
    return np.std(self.measurement)

  # Berechne Wertebereich der Temperatur
  @property
  def range_T(self):
    return (np.amin(self.temperatures), np.amax(self.temperatures))

  # Berechne Wertebereich der Fliessgeschw.
  @property
  def range_v(self):
    return (np.amin(self.flowRates), np.amax(self.flowRates))

  # Berechne Wertebereich der Anzahl der Tests
  @property
  def range_n(self):
    return (np.amin(self.tests), np.amax(self.tests))

  # Berechne Wertebereich der Werte
  @property
  def range_w(self):
    return (np.amin(self.measurement), np.amax(self.measurement))

  # Erzeuge string aus aktuellen Daten
  def __str__(self):
    s = ""
    s += "=== Analyse der Ergebnisse in %s ===\n" %self.file
    s += " - Temperatur: %.2f +/- %.2f \n" %(self.mean_T, self.std_T)
    s += "   Wertebereich: %.2f - %.2f\n" %(self.range_T[0], self.range_T[1])
    s += " - Fliessgeschwindigkeit: %.3f +/- %.3f\n" %(self.mean_v, self.std_v)
    s += "   Wertebereich: %.3f - %.3f\n" %(self.range_v[0], self.range_v[1])
    s += " - notwendige Tests: %.1f +/- %.1f\n" %(self.mean_n, self.std_n)
    s += "   Wertebereich: %.0f - %.0f\n" %(self.range_n[0], self.range_n[1])
    s += " - Messwert: %.2f +/- %.2f\n" %(self.mean_w, self.std_w)
    s += "   Wertebereich: %.2f - %.2f\n" %(self.range_w[0], self.range_w[1])
    return s

  # Plot Funktion
  def plot_results(self):
    # Berechne temporaere Mittelwerte
    def tmpMean(arr):
      mean = np.array([])
      for i in range(arr.size):
        mean = np.append(mean, np.mean(arr[:i+1]))
      return mean

    # Berechne temporaere Standardabweichung
    def tmpStd(arr):
      std = np.array([])
      for i in range(arr.size):
        std = np.append(std, np.std(arr[:i+1])/math.sqrt(i+1))
      return std

    plots = plt.figure()

    # Zeichne Temperaturen
    temp = plots.add_subplot(3,1,1)
    temp.set_title('Auswertung der Messreihe')
    temp.set_ylabel("Temperatur in [°C]")
    temp.plot(self.idx, self.temperatures, "ko-")
    temp.errorbar(self.idx, tmpMean(self.temperatures),
                  yerr = tmpStd(self.temperatures), fmt='rp')

    # Zeichne fliessgeschw.
    flow = plots.add_subplot(3,1,2)
    flow.set_ylabel("v(Fliess) in cm/s")
    flow.plot(self.idx, self.flowRates, "ks-")
    flow.errorbar(self.idx, tmpMean(self.flowRates),
                  yerr = tmpStd(self.flowRates), fmt='rp')

    # Zeichne Anzahl der Messungen
    meas = plots.add_subplot(3,1,3)
    meas.set_ylabel("Messwert")
    meas.plot(self.idx, self.measurement, "k^-")
    meas.errorbar(self.idx, tmpMean(self.measurement),
                  yerr = tmpStd(self.measurement), fmt='rp')
    plots.savefig("Abb1.pdf")

    # Zeichne histogram
    hists = plt.figure()
    tests = hists.add_subplot(1,1,1)
    tests.set_title("Auswertung der Messreihe fuer die Anzahl der Tests")
    tests.set_xlabel("Anzahl der Messungen")
    tests.set_ylabel("Haeufigkeit")
    tests.axis([0, 6, 0, 70])
    tests.hist(self.tests, color="0.75", bins=[0, 1, 2, 3, 4, 5, 6], align="left")

    hists.tight_layout()
    hists.savefig("Abb2.pdf")

if __name__ == '__main__':
  try:
    input = input("Dateinamen der Datei mit Messwerten\n")
    da = daten_analyse(input)
    print(da)
    da.plot_results()
  except Exception as e:
    print (e)
