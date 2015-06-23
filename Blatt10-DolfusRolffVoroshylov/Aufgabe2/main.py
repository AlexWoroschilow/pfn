import sys
import lgs
import string

def main():
  koeffmat = list()
  help = 'Gib die Koeffizientenmatrix des zu\
  lösenden LGS zeienweise ein!'
  print (help)
  print('Eine leere Zeile beendet die Eingabe!'.center(len(help)))
  line = input().strip()  
  if line:
    koeffmat.append(line)
    while line:
      line = input().strip()
      if line:
        koeffmat.append(line)

  print (lgs.lgs(koeffmat))

if __name__ == "__main__":
    try:
      main()
    except Exception as e:
      print (e)
