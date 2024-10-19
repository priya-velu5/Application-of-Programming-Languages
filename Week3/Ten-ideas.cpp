#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <vector>
#include <sstream>

using namespace std;

class TheOne {
public:
    explicit TheOne(const string& v) : value(v) {}

    TheOne& bind(function<string(const string&)> func) {
        try {
            value = func(value);
        } catch (const exception& e) {
            cerr << "Error during function call: " << e.what() << endl;
            exit(1);
        }
        return *this;
    }

    void print() const {
        cout << value << endl;
    }

private:
    string value;
};

// Function Definitions - takes an input and returns an output



vector<string> tokenize(string fileName) {
  /*
    Reads a txt file line by line, extracts cleaned tokens
    and stores them in a global vector var.
  */

 set<string> stopWords;
  ifstream stopFile("stop_words.txt");
  if (!stopFile.is_open()) {
    cout << "Input File not found" << endl;
    return;
  }

  string line;
  while (getline(stopFile, line)) {
    stringstream linestream(line);
    string value;

    while (getline(linestream, value, ',')) {
      stopWords.insert(value);
    }
  }

  ifstream inputFile(fileName);

  if (!inputFile.is_open()) {
    cout << "Input File not found" << endl;
    return;
  }

  vector<string> tokens;
  regex word_regex("[a-z]{2,}");
  string line;
  while (getline(inputFile, line)) {
    transform(line.begin(), line.end(), line.begin(), ::tolower);

    regex word_regex("[a-z]{2,}");

    map<string, int> wordFreq;
    for (sregex_iterator it(line.begin(), line.end(), word_regex), end;
         it != end; ++it) {
      string word = it->str();
      if (stopWords.find(word) != stopWords.end()) {
        continue;
      } else {
        tokens.push_back(word);
      }
    }
  }
  return tokens; 
}

map<string, int> countFreq(vector<string> tokens) {
  /*
  Takes tokens as input and
  stores the word-frequency pairs in a map
*/
  map<string, int> wordFreq;
  for (auto word : tokens) {
    wordFreq[word]++;
  }
}

bool sortByVal(const pair<string, int> &a, const pair<string, int> &b) {
  return (a.second > b.second);
}

vector<pair<string, int> > sortFreq(map<string, int> wordFreq ) {
  vector<pair<string, int>> sortedFreq;
  // copy key-value pairs from the map to the vector
  map<string, int>::iterator it;
  for (it = wordFreq.begin(); it != wordFreq.end(); it++) {
    sortedFreq.push_back(make_pair(it->first, it->second));
  }

  sort(sortedFreq.begin(), sortedFreq.end(), sortByVal);
}
string printTopFreq(vector<pair<string, int> > sortedFreq) {
  stringstream result;
  for (int i = 0; i < 25; i++) {
    result << sortedFreq[i].first << " - " << sortedFreq[i].second << endl;
  }
  return result.str();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <path_to_file>" << endl;
        return 1;
    }

    TheOne(argv[1])
        .bind(tokenize)
        .bind(countFreq)
        .bind(sortFreq)
        .bind(printTopFreq)
        .print();

    return 0;
}
