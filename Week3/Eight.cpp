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

bool sortByVal(const pair<string, int> &a, const pair<string, int> &b) {
  return (a.second > b.second);
}

// Return minimum index
int minIndex(int a[], int i, int j)
{
    if (i == j)
        return i;
 
    // Find minimum of remaining elements
    int k = minIndex(a, i + 1, j);
 
    // Return minimum of current and remaining.
    return (a[i] < a[k])? i : k;
}
 
// Recursive selection sort. n is size of a[] and index
// is index of starting element.
void recurSelectionSort(int a[], int n, int index = 0)
{
    // Return when starting and size are same
    if (index == n)
       return;
 
    // calling minimum index function for minimum index
    int k = minIndex(a, index, n-1);
 
    // Swapping when index and minimum index are not same
    if (k != index)
       swap(a[k], a[index]);
 
    // Recursively calling selection sort function
    recurSelectionSort(a, n, index + 1);
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