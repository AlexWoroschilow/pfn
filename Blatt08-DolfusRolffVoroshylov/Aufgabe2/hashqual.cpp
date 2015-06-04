#include <stdlib.h>

#include <map>
#include <cmath>
#include <limits>
#include <iomanip>
#include <iostream>
#include <algorithm>

#include "hashqual.hpp"
#include "tokenizer.hpp"
#include "hashqual-functions.h"

/** Definition für die Ergebnisse der Token */
typedef std::map<unsigned int, std::vector<std::string> > ComputeSingleStore;
/** Definition für ein paar aus ComputeSingleStore und string */
typedef std::pair<std::string, ComputeSingleStore > StringStorePair;
/** Definition für alle Ergebnisse der Hash funktionen */
typedef std::vector<StringStorePair> ComputeStore;

bool HashQuality::QualityOrder::operator ()(const QualityStore::value_type& a,
                                            const QualityStore::value_type& b) {
  /* Aufgrund von ungenauigkeiten beim Vergleichen mit
     epsilon auf gleichheit prüfen */
  if (std::abs(a.second - b.second) <
      std::numeric_limits<double>::epsilon()) {
    return a.first < b.first;
  }

  return a.second < b.second;
}

HashQuality::HashQuality() {
  tokenizer(HashQuality::tokenHandler, this);
  compute();
}

void HashQuality::compute() {
  unsigned long i;
  ComputeStore::iterator it;
  ComputeStore computeStore;
  const unsigned int hashCount = hashfunction_number();

  // Alle Hash funktionen durchlaufen
  for (i = 0; i < hashCount; ++i) {
    StringSet::iterator it;
    ComputeSingleStore singleStore;
    const Hashfunction* func = hashfunction_get(i);
    /* und in den jeweiligen singlestore der Funktion doppelte Werte
       in den vektor des stores einfügen */
    for (it = tokenStore.begin(); it != tokenStore.end(); ++it) {
      const unsigned int hashValue = func->func(it->c_str());
      singleStore[hashValue].push_back(*it);
    }

    computeStore.push_back(ComputeStore::value_type(func->name, singleStore));
  }

  for (it = computeStore.begin(); it != computeStore.end(); ++it) {
    // hashqual(h,T) = Sum f(h, T, i)^2 berechnen
    double result;
    std::vector<std::string>::size_type sum = 0;
    ComputeSingleStore::iterator single;
    for (single = it->second.begin(); single != it->second.end(); ++single) {
      sum += std::pow(single->second.size(), 2); //f(h, T, i)^2
    }
    // durch |H(h, T)| teilen
    if (!it->second.empty()) {
      result = static_cast<double>(sum) /
               static_cast<double>(it->second.size());
    }
    else {
      /* Da kein Ergeniss gefunden wurde kann auch keine
         Qualität berechnet werden */
      result = std::numeric_limits<double>::quiet_NaN();
    }
    // und Ergebniss speichern
    qualityStore.push_back(QualityStore::value_type(it->first, result));
  }
  // Sortieren der Objekte nach ihrer Qualität und dann nach dem Namen
  std::sort(qualityStore.begin(), qualityStore.end(), QualityOrder());
}

void HashQuality::tokenHandler (const char* token, void* data) {
  static_cast<HashQuality*>(data)->addToken(token);
}

std::ostream& operator << (std::ostream& stream,
                           const HashQuality& qual) {
  /* Default Output für einen stream um
     code duplikation zu vermeiden
  */
  #define defaultOutput     \
       it->first            \
    << ": "                 \
    << std::fixed           \
    << std::setprecision(8) \
    << it->second

  HashQuality::QualityStore::const_iterator it;
  stream << "# different words in input: "
         << qual.tokenStore.size()
         << std::endl;

  for (it = qual.qualityStore.begin();
       it != qual.qualityStore.end() - 1;
       ++it) {
    stream << defaultOutput << std::endl;
  }

  return stream << defaultOutput;

#undef defaultOutput
}

int main() {
  std::cout << HashQuality() << std::endl;
  return EXIT_SUCCESS;
}