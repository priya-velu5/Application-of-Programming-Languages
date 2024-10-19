#include <algorithm>
#include <bits/stdc++.h>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
set<string> stopWords;
vector<string> cleanedWords;
vector<string> inputWords;
map<string, int> wordFreq;

// function to read and store stop words in a set
void loadStopWords(string fileName) {
  ifstream inputFile(fileName);
  if (!inputFile.is_open()) {
    cout << "Input File not found" << endl;
    return;
  }

  string line;
  while (getline(inputFile, line)) {
    std::stringstream linestream(line);
    std::string value;

    while (getline(linestream, value, ',')) {
      // stopWords.push_back(value);
      stopWords.insert(value);
    }
  }
  // for (string word : stopWords) {
  //   cout << word << endl;
  // }
  inputFile.close();
}

void cleanAndSplitWords() {
  for (const string &word : inputWords) {
    string cleanedWord = "";
    string temp = "";

    for (char ch : word) {
      if (isalpha(ch)) {
        temp += tolower(ch);
      } else {
        if (!temp.empty()) {
          cleanedWords.push_back(temp);
          temp = "";
        }
      }
    }

    if (!temp.empty()) {
      cleanedWords.push_back(temp);
    }
  }
}

// utility comparator function to pass to the sort() module
bool sortByVal(const pair<string, int> &a, const pair<string, int> &b) {
  return (a.second > b.second);
}

void printTopFreq(map<string, int> freq) {
  // create a empty vector of pairs
  vector<pair<string, int>> temp;

  // copy key-value pairs from the map to the vector
  map<string, int>::iterator it;
  for (it = freq.begin(); it != freq.end(); it++) {
    temp.push_back(make_pair(it->first, it->second));
  }

  sort(temp.begin(), temp.end(), sortByVal);

  for (int i = 0; i < 25; i++) {
    cout << temp[i].first << " - " << temp[i].second << endl;
  }
}

void countFreq(string fileName) {
  // read the file
  ifstream inputFile(fileName);

  if (!inputFile.is_open()) {
    cout << "Input File not found" << endl;
    return;
  }

  map<string, int> wordFreq;
  
  string word;
  while (inputFile >> word) {
    inputWords.push_back(word);
  }
  cleanAndSplitWords();

  for (auto word : cleanedWords) {
    if (stopWords.find(word) != stopWords.end() || word.length() <= 1) {
      continue;
    } else {
      wordFreq[word] = wordFreq[word] + 1;
    }
  }

  // print the top 25 words
  printTopFreq(wordFreq);

  inputFile.close();
}

int main() {
  loadStopWords("stop_words.txt");
  countFreq("pride-and-prejudice.txt");

  return 0;
}