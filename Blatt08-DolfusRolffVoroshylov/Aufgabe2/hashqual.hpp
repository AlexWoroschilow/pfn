#pragma once

#include <string>
#include <set>
#include <map>
#include <vector>
#include <ostream>

#include "hashqual-functions.h"

class HashQuality
{
  friend class std::basic_ostream<char>;
  public:
    /** Konstruktor, dieser erstellt eine Liste mit allen möglichen
        Hashfunction's aus hashqual-functions.h und ruft compute auf.
        Die Ausgabe erfolgt über den standard output operator
    */
    HashQuality();
   
  private:
    /** Definition für den Container der die Token hält */
    typedef std::set<std::string> StringSet;
    /** Definition für den Container welcher die berechnete
        Qualität der Hash Funktion beinhaltet. */
    typedef std::vector<std::pair<std::string, double> > QualityStore;
    
    /** Nested struct um eine Sortierordnung nach Qualität und Namen
        zu gewährleisten. */
    struct QualityOrder {
      bool operator () (const QualityStore::value_type& a,
                        const QualityStore::value_type& b);
    };
    
    /** Schreibt die berechnete Hash qualität in einen ostream */
    friend std::ostream& operator << (std::ostream& stream,
                                      const HashQuality& qual);
                                      
    /** Der Tokenhandler, welcher an den tokenizer übergeben wird
        um den gegeben input zu tokenizen.
        Hierbei wird erwartet, dass data ein Pointer auf eine Instanz
        von HashQuality ist. */
    static void tokenHandler (const char* token, void* data);
                                      
    /** Berechnet die Hash qualität der gegebenen Hash funktionen. */
    void compute();
    
    /** Fügt einen Token in den token store ein. */
    void addToken(const std::string& tok);
  
    /** str_set nach vorlesung */
    StringSet tokenStore;
    /** Store für die berechnete Qualität */
    QualityStore qualityStore;
};

inline void HashQuality::addToken(const std::string& tok) { 
  /* Funktion in die Headerdatei einlagern, damit jede
     Übersetzungseinheit die Möglichkeit hat diese
     Funktion zu inlinen. */
  tokenStore.insert(tok);
}