#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <vector>

using namespace std;

int main() {

  // Load stopwords
  set<string> stopWords;
  ifstream stopwords_file("stop_words.txt");

  string line;
  while (getline(stopwords_file, line)) {
    stringstream linestream(line);
    string value;

    while (getline(linestream, value, ',')) {
      stopWords.insert(value);
    }
  }

  ifstream input_file("pride-and-prejudice.txt");
  // inefficient but the shortest way to read a txt file and store it in a
  // string
  string content((istreambuf_iterator<char>(input_file)),
                 istreambuf_iterator<char>());
  transform(content.begin(), content.end(), content.begin(), ::tolower);

  regex word_regex("[a-z]{2,}");

  map<string, int> wordFreq;
  for (sregex_iterator it(content.begin(), content.end(), word_regex), end;
       it != end; ++it) {
    string word = it->str();
    if (stopWords.find(word) != stopWords.end() || word.length() <= 1) {
      continue;
    } else {
      wordFreq[word]++;
    }
  }

  vector<pair<string, int>> sorted_words(wordFreq.begin(), wordFreq.end());
  partial_sort(sorted_words.begin(),
               sorted_words.begin() + min(25, (int)sorted_words.size()),
               sorted_words.end(), [](const auto &a, const auto &b) {
                 return b.second < a.second;
               });

  for (int i = 0; i < min(25, (int)sorted_words.size()); ++i) {
    cout << sorted_words[i].first << " - " << sorted_words[i].second << endl;
  }

  return 0;
}
