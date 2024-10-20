#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <vector>
#include <sstream>

using namespace std;

set<string> stopWords;
vector<string> tokens;
map<string, int> wordFreq;
vector<pair<string, int> > sortedFreq;

/*
This program uses a recursive approach to sort the word-frequency pairs
*/

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

// Function to find the index of the maximum element (based on pair's second value)
int findMaxIndex(const vector<pair<string, int> >& arr, int start) {
    int maxIndex = start;
    for (int i = start + 1; i < arr.size(); i++) {
        if (arr[i].second > arr[maxIndex].second) {  // Compare by second element
            maxIndex = i;
        }
    }
    return maxIndex;
}

// Recursive function for selection sort on vector<pair<string, int> >
void selectionSort(vector<pair<string, int> >& arr, int start) {
    // Base case: if we have reached the end of the vector
    if (start >= arr.size() - 1) {
        return;
    }

    // Find the maximum element in the unsorted portion of the array
    int maxIndex = findMaxIndex(arr, start);

    // Swap the found maximum element with the current start element
    if (maxIndex != start) {
        swap(arr[start], arr[maxIndex]);
    }

    // Recursively sort the remaining portion of the array
    selectionSort(arr, start + 1);
}

void sortFreq() {

  // copy key-value pairs from the map to the vector
  map<string, int>::iterator it;
  for (it = wordFreq.begin(); it != wordFreq.end(); it++) {
    sortedFreq.push_back(make_pair(it->first, it->second));
  }

  selectionSort(sortedFreq, 0);
}

void printTopFreq() {

  for (int i = 0; i < 25; i++) {
    cout << sortedFreq[i].first << " - " << sortedFreq[i].second << endl;
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
        cout << "Usage: " << argv[0] << " <text_file>" << endl;
        return 1;
    }
    string textFile = argv[1];
  loadStopWords("stop_words.txt");
  tokenize(textFile);
  countFreq();
  sortFreq();
  printTopFreq();

  return 0;
}