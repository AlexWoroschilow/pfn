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
    /** Konstruktor, dieser erstellt eine Liste mit allen m�glichen
        Hashfunction's aus hashqual-functions.h und ruft compute auf.
        Die Ausgabe erfolgt �ber den standard output operator
    */
    HashQuality();
   
  private:
    /** Definition f�r den Container der die Token h�lt */
    typedef std::set<std::string> StringSet;
    /** Definition f�r den Container welcher die berechnete
        Qualit�t der Hash Funktion beinhaltet. */
    typedef std::vector<std::pair<std::string, double> > QualityStore;
    
    /** Nested struct um eine Sortierordnung nach Qualit�t und Namen
        zu gew�hrleisten. */
    struct QualityOrder {
      bool operator () (const QualityStore::value_type& a,
                        const QualityStore::value_type& b);
    };
    
    /** Schreibt die berechnete Hash qualit�t in einen ostream */
    friend std::ostream& operator << (std::ostream& stream,
                                      const HashQuality& qual);
                                      
    /** Der Tokenhandler, welcher an den tokenizer �bergeben wird
        um den gegeben input zu tokenizen.
        Hierbei wird erwartet, dass data ein Pointer auf eine Instanz
        von HashQuality ist. */
    static void tokenHandler (const char* token, void* data);
                                      
    /** Berechnet die Hash qualit�t der gegebenen Hash funktionen. */
    void compute();
    
    /** F�gt einen Token in den token store ein. */
    void addToken(const std::string& tok);
  
    /** str_set nach vorlesung */
    StringSet tokenStore;
    /** Store f�r die berechnete Qualit�t */
    QualityStore qualityStore;
};

inline void HashQuality::addToken(const std::string& tok) { 
  /* Funktion in die Headerdatei einlagern, damit jede
     �bersetzungseinheit die M�glichkeit hat diese
     Funktion zu inlinen. */
  tokenStore.insert(tok);
}