#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  // Stop words array
  vector<string> stopWords;
  // Word frequency array
  vector<vector<string>> wordFreqs; // Each entry will be [word, frequency]

  // Load stop words
  ifstream stopWordsFile("stop_words.txt");
  if (!stopWordsFile.is_open()) {
    cout << "Stop Words File not found" << endl;
    return 1;
  }

  string stopWord;
  while (getline(stopWordsFile, stopWord, ',')) {
    stopWords.push_back(stopWord);
  }
  stopWords.push_back(
      ""); // Add an empty string to manage single character checks
  for (char c = 'a'; c <= 'z'; c++) {
    stopWords.push_back(string(1, c)); // Add all lowercase letters
  }
  stopWordsFile.close();

  // Tokenize and clean text
  ifstream inputFile("pride-and-prejudice.txt");
  if (!inputFile.is_open()) {
    cout << "Input File not found" << endl;
    return 1;
  }

  string line;
  while (getline(inputFile, line)) {
    int startChar = -1; // Start index of the current word
    for (int i = 0; i < line.length(); i++) {
      char c = line[i];

      if (startChar == -1) { // Not currently in a word
        if (isalnum(c)) {    // Found the start of a word
          startChar = i;
        }
      } else {             // Currently in a word
        if (!isalnum(c)) { // Found the end of a word
          string word = line.substr(startChar, i - startChar);
          // Convert to lowercase
          for (int j = 0; j < word.length(); j++) {
            word[j] = tolower(word[j]);
          }

          // Ignore stop words
          bool found = false;
          for (const auto &stopWord : stopWords) {
            if (word == stopWord) {
              found = true;
              break;
            }
          }

          if (!found) {
            // Check if the word already exists in wordFreqs
            found = false;
            for (int j = 0; j < wordFreqs.size(); j++) {
              if (wordFreqs[j][0] == word) {
                int freq = stoi(wordFreqs[j][1]);
                wordFreqs[j][1] = to_string(freq + 1); // Increment frequency
                found = true;

                // Reorder if necessary
                for (int n = j;
                     n > 0 && stoi(wordFreqs[n][1]) > stoi(wordFreqs[n - 1][1]);
                     n--) {
                  swap(wordFreqs[n], wordFreqs[n - 1]);
                }
                break;
              }
            }
            if (!found) {
              // Add new word with frequency 1
              wordFreqs.push_back({word, "1"});
            }
          }
          startChar = -1; // Reset start index
        }
      }
    }
    // Check if there is a word at the end of the line
    if (startChar != -1) {
      string word = line.substr(startChar, line.length() - startChar);
      for (int j = 0; j < word.length(); j++) {
        word[j] = tolower(word[j]);
      }
      bool found = false;
      for (const auto &stopWord : stopWords) {
        if (word == stopWord) {
          found = true;
          break;
        }
      }
      if (!found) {
        found = false;
        for (int j = 0; j < wordFreqs.size(); j++) {
          if (wordFreqs[j][0] == word) {
            int freq = stoi(wordFreqs[j][1]);
            wordFreqs[j][1] = to_string(freq + 1);
            found = true;

            for (int n = j;
                 n > 0 && stoi(wordFreqs[n][1]) > stoi(wordFreqs[n - 1][1]);
                 n--) {
              swap(wordFreqs[n], wordFreqs[n - 1]);
            }
            break;
          }
        }
        if (!found) {
          wordFreqs.push_back({word, "1"});
        }
      }
    }
  }
  inputFile.close();

  // Print the top 25 words
  for (int i = 0; i < 25 && i < wordFreqs.size(); i++) {
    cout << wordFreqs[i][0] << " - " << wordFreqs[i][1] << endl;
  }

  return 0;
}
