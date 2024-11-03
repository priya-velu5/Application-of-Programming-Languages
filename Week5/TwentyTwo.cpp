#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <vector>
#include <sstream>
#include <cassert>
#include <typeinfo>

using namespace std;

void assertMsg(bool condition, string msg){
    if(!condition){
        cout << msg << endl;
        assert(condition);
    }
}

set<string> loadStopWords(string fileName) {
  /*
  Takes the path of the file as an argument;
  read and store in a set and return it.
  Checks:
  if input file is found. 
  if not found, quit the program
  */

 // assert if filename is a string
    assertMsg(typeid(fileName)== typeid(string), "Error: File Name is not a string");

 // assert if fileName is correct
    assertMsg(fileName == "stop_words.txt", "Error: File Name is not stop_words.txt");

 // assert if fileName is not empty
    assertMsg(!fileName.empty(), "Error: File Name is empty");

 set<string> stopWords;
 ifstream inputFile(fileName);


 try{
  if (!inputFile.is_open()) {
    throw runtime_error("Input File not found"); 
  }else{
    string line;
    while (getline(inputFile, line)) {
        stringstream linestream(line);
        string value;

        while (getline(linestream, value, ',')) {
            stopWords.insert(value);
        }
    }
  }
 } catch (const runtime_error& e) {
    cout << "Error: " << e.what() << endl;
    return stopWords; 
  }

  return stopWords;
  
}

vector<string> tokenize(string fileName, set<string> stopWords) {
  /*
    Reads a txt file line by line, extracts cleaned tokens
    and stores them in a vector and returns it.

    1. Checks if input file is found
    2. Checks if stopWords is not empty
    if the above conditions are not met, return an empty vector
  */


 // assert if filename is a string
    assertMsg(typeid(fileName)== typeid(string), "Error: File Name is not a string");

 // assert if fileName is correct
    assertMsg(fileName == "pride-and-prejudice.txt", "Error: File Name is not pride-and-prejudice.txt");

 // assert if fileName is not empty
    assertMsg(!fileName.empty(), "Error: File Name is empty");

    // assert if stopWords is not empty
    assertMsg(!stopWords.empty(), "Error: Stop Words not found");

    // assert if stopWords is a set
    assertMsg(typeid(stopWords)== typeid(set<string>), "Error: Stop Words is not a set");

  ifstream inputFile(fileName);
  vector<string> tokens;

 try{
    if (!inputFile.is_open()) {
        throw runtime_error("Input File not found");
  }
 }catch(const runtime_error& e){
    cout << "Error: " << e.what() << endl;
    return tokens;
 }

 // check if stopWords is not empty
 try{
    if (stopWords.empty()) {
        throw runtime_error("Stop Words not found");
    }
 }catch(const exception& e){
    cout << "Error: " << e.what() << endl;
    return tokens;
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
  return tokens;
}

map<string, int> countFreq(vector<string> tokens) {
  /*
  Takes tokens as input and
  returns the word-frequency pairs in a map. 
  1. Checks if tokens is not empty, if empty return an empty map
*/

// assert if tokens is not empty
    assertMsg(!tokens.empty(), "Error: Tokens not found");
// assert if tokens is a vector
    assertMsg(typeid(tokens)== typeid(vector<string>), "Error: I need a vector of strings!!!!");

map<string, int> wordFreq;
try{
  if (tokens.empty()) {
    throw runtime_error("Tokens not found");
  }
}catch(const exception& e){
    cout << "Error: " << e.what() << endl;
    return wordFreq;
  }

  for (auto word : tokens) {
    wordFreq[word]++;
  }
  return wordFreq;
}

bool sortByVal(const pair<string, int> &a, const pair<string, int> &b) {
  return (a.second > b.second);
}

vector<pair<string, int> > sortFreq(map<string, int> wordFreq) {
/*
Takes the word-frequency pairs in a map and
returns the sorted word-frequency pairs in a vector.
Checks if wordFreq is not empty; if empty return an empty vector
*/
// assert if wordFreq is not empty
    assertMsg(!wordFreq.empty(), "Error: Word Frequency not found");
// assert if wordFreq is a map
    assertMsg(typeid(wordFreq)== typeid(map<string, int>), "Error: I need a map of strings and integers!!!!");

    vector<pair<string, int> > sortedFreq;
    try{
        if (wordFreq.empty()) {
            throw runtime_error("Word Frequency not found");
        }
    }catch(const exception& e){
        cout << "Error: " << e.what() << endl;
        return sortedFreq;
    }

  // copy key-value pairs from the map to the vector
  map<string, int>::iterator it;
  for (it = wordFreq.begin(); it != wordFreq.end(); it++) {
    sortedFreq.push_back(make_pair(it->first, it->second));
  }

  sort(sortedFreq.begin(), sortedFreq.end(), sortByVal);\
  return sortedFreq;
}
void printTopFreq(vector<pair<string, int> > sortedFreq) {
    /*
    Takes the sorted word-frequency pairs in a vector and
    prints the top 25 words with their frequencies.
    checks if sortedFreq is not empty; if empty print error msg
    */
   // assert if sortedFreq is not empty
    assertMsg(!sortedFreq.empty(), "Error: Sorted Frequency not found");
// assert if sortedFreq is a vector
    assertMsg(typeid(sortedFreq)== typeid(vector<pair<string, int> >), "Error: I need a vector of pairs!!!!");
    try{
        if (sortedFreq.empty()) {
            throw runtime_error("Sorted Frequency not found");
        }
    }catch(const exception& e){
        cout << "Error: " << e.what() << endl;
        return;
    }

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
  printTopFreq(
    sortFreq(
        countFreq(
            tokenize(textFile , loadStopWords("stop_words.txt"))
            )
        )
    );

  return 0;
}
