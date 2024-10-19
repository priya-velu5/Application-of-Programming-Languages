#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <vector>

using namespace std;

set<string> stopWords;
vector<string> tokens;
map<string, int> wordFreq;
vector<pair<string, int>> sortedFreq;

void loadStopWords(string fileName) {
  /*
  Takes the path of the file as an argument;
  read and store stop words in a global set var.
  */

  ifstream inputFile(fileName);
  if (!inputFile.is_open()) {
    cout << "Input File not found" << endl;
    return;
  }

  string line;
  while (getline(inputFile, line)) {
    stringstream linestream(line);
    string value;

    while (getline(linestream, value, ',')) {
      stopWords.insert(value);
    }
  }
}

void tokenize(string fileName) {
  /*
    Reads a txt file line by line, extracts cleaned tokens
    and stores them in a global vector var.
  */

  ifstream inputFile(fileName);

  if (!inputFile.is_open()) {
    cout << "Input File not found" << endl;
    return;
  }

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
}

void countFreq() {
  /*
  Takes tokens as input and
  stores the word-frequency pairs in a map
*/
  for (auto word : tokens) {
    wordFreq[word]++;
  }
}

bool sortByVal(const pair<string, int> &a, const pair<string, int> &b) {
  return (a.second > b.second);
}

void sortFreq() {

  // copy key-value pairs from the map to the vector
  map<string, int>::iterator it;
  for (it = wordFreq.begin(); it != wordFreq.end(); it++) {
    sortedFreq.push_back(make_pair(it->first, it->second));
  }

  sort(sortedFreq.begin(), sortedFreq.end(), sortByVal);
}
void printTopFreq() {

  for (int i = 0; i < 25; i++) {
    cout << sortedFreq[i].first << " - " << sortedFreq[i].second << endl;
  }
}

int main(int argc, char *argv[]) {
  loadStopWords("stop_words.txt");
  tokenize("pride-and-prejudice.txt");
  countFreq();
  sortFreq();
  printTopFreq();

  return 0;
}