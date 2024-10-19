#include <algorithm>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
  // Load stop words
  vector<string> stopWords;
  vector<string> cleanedWords;
  vector<string> inputWords;
  string stopWord;

  // Read stop words from file
  ifstream stopWordsFile("stop_words.txt");
  if (!stopWordsFile.is_open()) {
    cout << "Input File not found" << endl;
    return 1;
  }

  while (getline(stopWordsFile, stopWord, ',')) {
    stopWords.push_back(stopWord);
  }
  stopWordsFile.close();

  // Read input words from file
  ifstream inputFile("pride-and-prejudice.txt");
  if (!inputFile.is_open()) {
    cout << "Input File not found" << endl;
    return 1;
  }

  string word;
  while (inputFile >> word) {
    inputWords.push_back(word);
  }
  inputFile.close();

  // Clean and split words
  for (const string &word : inputWords) {
    string cleanedWord = "";
    for (char ch : word) {
      if (isalpha(ch)) {
        cleanedWord += tolower(ch);
      } else {
        if (!cleanedWord.empty()) {
          cleanedWords.push_back(cleanedWord);
          cleanedWord = "";
        }
      }
    }
    if (!cleanedWord.empty()) {
      cleanedWords.push_back(cleanedWord);
    }
  }

  // Count frequency of cleaned words
  const int maxWords = 10000; // Assuming a maximum number of unique words
  string uniqueWords[maxWords];
  int wordFreq[maxWords] = {0};
  int uniqueCount = 0;

  for (const string &cleanedWord : cleanedWords) {
    // Check if the word is a stop word or has length <= 1
    if (cleanedWord.length() <= 1)
      continue;

    bool isStopWord = false;
    for (const string &stop : stopWords) {
      if (cleanedWord == stop) {
        isStopWord = true;
        break;
      }
    }
    if (isStopWord)
      continue;

    // Count frequency
    bool found = false;
    for (int i = 0; i < uniqueCount; i++) {
      if (uniqueWords[i] == cleanedWord) {
        wordFreq[i]++;
        found = true;
        break;
      }
    }

    if (!found && uniqueCount < maxWords) {
      uniqueWords[uniqueCount] = cleanedWord;
      wordFreq[uniqueCount] = 1;
      uniqueCount++;
    }
  }

  // Sort by frequency
  for (int i = 0; i < uniqueCount - 1; i++) {
    for (int j = i + 1; j < uniqueCount; j++) {
      if (wordFreq[j] > wordFreq[i]) {
        // Swap frequencies
        swap(wordFreq[i], wordFreq[j]);
        // Swap corresponding words
        swap(uniqueWords[i], uniqueWords[j]);
      }
    }
  }

  // Print top 25 most frequent words
  for (int i = 0; i < min(uniqueCount, 25); i++) {
    cout << uniqueWords[i] << " - " << wordFreq[i] << endl;
  }

  return 0;
}
