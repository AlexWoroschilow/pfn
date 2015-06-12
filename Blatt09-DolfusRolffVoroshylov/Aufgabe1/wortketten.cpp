#include <stdlib.h>

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <map>

typedef std::pair<std::string, std::string> PathPoints;
typedef std::pair<PathPoints, std::vector<std::string> > Path;
typedef std::vector<Path> Paths;
typedef std::vector<std::string> Wordlist;
typedef std::vector<std::pair<std::string, std::string> > WordPairlist;
typedef std::map<std::string, Wordlist> Adjacencemap;
typedef std::map<std::string, std::string> PreviousWords;

class WordGraph {
  public:
    /** Berechnet einen Graphen aus einer gegeben liste von Wörtern.
        Hierbei haben zwei verbundene Knoten nur einen einzigen
        unterschiedlichen Buchstaben.
        @param filename Der Dateiname mit den Wörtern für den Graphen
    */
    WordGraph(const std::string& filename) {
      buildAdjacence(loadWords(filename));
    }

    /** Gibt alle verbundenen Knoten zurück, welche mit dem gegebenen Knoten
        verbunden sind.
        @param word Das Wort, für welches die Nachbarknoten zurückgegeben werden
                    sollen.
        @return Die mit dem Knoten benachbarten Wörter
    */
    Wordlist getAdjacence(const std::string& word) const {
      Adjacencemap::const_iterator found = adjacence.find(word);
      if (found != adjacence.end()) {
        return found->second;
      }
      return Wordlist();
    }

  private:
    /** Läd alle Wörter aus einer Datei und gibt die gelesenen Wörter zurück
        @param filename Der Dateiname mit den zu lesenen Wörtern
        @return die gelesenen Wörter
    */
    Wordlist loadWords(const std::string& filename) {
      Wordlist words;
      std::string line;
      std::ifstream file(filename.c_str());
      if (file.bad()) {
        throw std::runtime_error("Could not open file " + filename);
      }

      while (std::getline(file, line)) {
        words.push_back(line);
      }
      return words;
    }

    /** Berechnet die benachbarten Knoten.
        Hierbei sind zwei Knoten benachbart wenn sie nur ein Buchstabe
        unterscheidet.
        @param words Die liste mit allen wörten die den Graph bilden sollen.
    */
    void buildAdjacence(const Wordlist& words) {
      Wordlist::const_iterator it1, it2;
      for (it1 = words.begin(); it1 != words.end(); ++it1) {
        Wordlist& currentWordlist = adjacence[*it1];
        std::string currentWord = *it1;

        for (it2 = words.begin(); it2 != words.end(); ++it2) {
          if (it1 != it2) {
            std::string::size_type diff;
            std::string otherWord = *it2;
            //Unterschiede der beiden strings berechnen
            diff = difference(currentWord.begin(), currentWord.end(),
                              otherWord.begin(), otherWord.end());
            if (diff == 1) {
              currentWordlist.push_back(*it2);
            }
          }
        }
      }
    }

    /** Berechnet die unterschiede zwischen zwei gegeben Objekten.
        @param it1_start der Start des ersten Objekts
        @param it1_end das Ende des ersten Objekts
        @param it2_start der Start des ersten Objekts
        @param it2_end das Ende des zweiten Objekts
        @return die Anzahl der unterschiede in den beiden Objekten
    */
    template <typename InputIterator>
    typename std::iterator_traits<InputIterator>::difference_type
      difference(InputIterator it1_start, InputIterator it1_end,
                 InputIterator it2_start, InputIterator it2_end) {
        InputIterator it1, it2;
        typename std::iterator_traits<InputIterator>::difference_type diff = 0;
        for (it1 = it1_start, it2 = it2_start;
             it1 != it1_end && it2 != it2_end;
             ++it1, ++it2) {
            diff = (*it1 != *it2) ? diff + 1 : diff;
        }
        return diff;
    }

    Adjacencemap adjacence;
};

class Pathfinder {
  friend class std::basic_ostream<char>;
  public:
    /** Berechnet alle Pfade aus die in der gegeben Datei stehen.
        @param g Der Graph zu dem die Wege berechnet werden sollen.
        @param filename Der Dateiname mit start und ende des Pfades
    */
    Pathfinder(const WordGraph& g, const std::string& filename)
      : graph(g) {
      computePaths(loadPairs(filename));
    }

  private:
    /** Gibt den Pfad auf den gegeben stream aus. Hierbei wird für cerr
        die printCerr funktion aufgerufen und für alle anderen ostream
        die print funktion.
    */
    friend std::ostream& operator << (std::ostream& stream,
                                     const Pathfinder& path);

    /** Läd alle Start- und Endpunkte aus der gegeben Datei
        @param filename Die Datei mit den Start und Endpunkten der Pfade
        @return Die Start und Endpaare
    */
    WordPairlist loadPairs(const std::string& filename) {
      WordPairlist pairs;
      std::string line;
      std::ifstream file(filename.c_str());
      if (file.bad()) {
        throw std::runtime_error("Could not open file " + filename);
      }

      while (std::getline(file, line)) {
        std::string first, second;
        std::string::size_type pos = line.find('\t');
        if (pos == std::string::npos) {
          throw std::runtime_error("Could not parse string: " + line);
        }

        first = line.substr(0, pos);
        second = line.substr(pos + 1, line.size() - pos - 1);
        pairs.push_back(std::make_pair(first, second));
      }

      return pairs;
    }

    /** Berechnet alle Pfade aus einer gegeben Liste mit start und Endpunkten
        der zu berechnenden Pfade
        @param pairs Die Liste der Paare mit den Start und Endpunkten
    */
    void computePaths(const WordPairlist& pairs) {
      WordPairlist::const_iterator it;
      for (it = pairs.begin(); it != pairs.end(); ++it) {
        paths.push_back(computePath(it->first, it->second));
      }
    }

    /** Berechnet einen Pfad vom start zum ende. Hierbei wird Breitensuche
        benutzt um die Pfade zu berechnen.
        @param start der Startpunkt des Pfades
        @param end das Ende des Pfades
        @return Der gefundene Pfad
    */
    Path computePath(const std::string& start, const std::string& end) {
      std::map<std::string, bool> visited;
      std::queue<std::string> queue;
      std::string currentWord;
      PreviousWords prev;
      Path path;
      path.first = std::make_pair(start, end);

      queue.push(start);
      visited[start] = true;

      while (!queue.empty()) {
        std::string top = queue.front();
        queue.pop();
        Wordlist::const_iterator it;
        const Wordlist words = graph.getAdjacence(top);
        for (it = words.begin(); it != words.end(); ++it) {
          if (!visited[*it]) {
            visited[*it] = true;
            queue.push(*it);
            prev[*it] = top;
          }
        }
      }

      if (prev.find(end) != prev.end()) {
        currentWord = end;
        path.second.push_back(end);
        while (prev[currentWord] != start) {
          path.second.push_back(prev[currentWord]);
          currentWord = prev[currentWord];
        }
        path.second.push_back(start);
        /* Da der Pfad umgekeht gebaut wurde für die richtige Reihenfolge
           den Pfad wieder umdrehen */
        std::reverse(path.second.begin(), path.second.end());
      }

      return path;
    }

    /** Gibt die berechneten Pfade auf den ostream aus.
    */
    std::ostream& print(std::ostream& stream) const {
      Paths::size_type i;
      for (i = 0; i < paths.size(); ++i) {
        if (paths[i].second.empty()) {
          stream << "no chain found for words: "
                 << paths[i].first.first
                 << " and "
                 << paths[i].first.second;
        } else {
          stream << "found chain of "
                 << paths[i].second.size()
                 << " words for words: "
                 << paths[i].first.first
                 << " and "
                 << paths[i].first.second;
        }
        if (i < paths.size()) {
          stream << std::endl;
        }
      }

      return stream;
    }

    /** Gibt stderr auf den stream aus
    */
    std::ostream& printCerr(std::ostream& stream) const {
      Paths::const_iterator it;
      for (it = paths.begin(); it != paths.end(); ++it) {
        std::vector<std::string>::size_type i;
        for (i = 0; i < it->second.size(); ++i) {
          if (i < it->second.size() - 1) {
            stream << it->second[i] << "-->";
          } else {
            stream << it->second[i];
          }
        }
      }

      return stream;
    }

    Paths paths;
    const WordGraph& graph;
};

std::ostream& operator << (std::ostream& stream, const Pathfinder& path) {
  if (stream.rdbuf() == std::cerr.rdbuf()) {
    return path.printCerr(stream);
  }

  return path.print(stream);
}

int main(int argc, char* argv[]) {
  if (argc == 3) {
    WordGraph parser(argv[1]);
    Pathfinder pathfinder(parser, argv[2]);
    // Für stderr aus kommentieren
    // std::cerr << pathfinder;
    std::cout << pathfinder;
  } else {
    std::cout << "Usage: " << argv[0] << " <wordfile> <wordpairs>" << std::endl;
  }


  return EXIT_SUCCESS;
}