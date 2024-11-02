// create a class framework that executes each entity through callbacks

#include <algorithm>
#include <boost/signals2.hpp>
#include <cctype>
#include <fstream>
#include <iostream>
#include <locale>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std; // Use the standard namespace

// The "I'll call you back" Word Frequency Framework in C++

class WordFrequencyFramework {
public:
  boost::signals2::signal<void(const string &)> load_event;
  boost::signals2::signal<void()> dowork_event;
  boost::signals2::signal<void()> end_event;

  void run(const string &path_to_file) {
    load_event(path_to_file);
    dowork_event();
    end_event();
  }
};

class StopWordFilter {
public:
  StopWordFilter(WordFrequencyFramework &wfapp) {
    wfapp.load_event.connect(
        [this](const string &ignore) { load_stop_words(); });
  }

  bool is_stop_word(const string &word) {
    return this->stop_words.find(word) != this->stop_words.end();
  }

private:
  unordered_set<string> stop_words;

  void load_stop_words() {
    ifstream file("stop_words.txt");
    string line;
    if (file.is_open()) {
      while (getline(file, line, ',')) {
        this->stop_words.insert(line);
      }
      file.close();
    }
    // Add single-letter words (a-z)
    for (char ch = 'a'; ch <= 'z'; ++ch) {
      this->stop_words.insert(string(1, ch));
    }
  }
};

class DataStorage {
public:
  boost::signals2::signal<void(const string &)> word_event;

  DataStorage(WordFrequencyFramework &wfapp, StopWordFilter &stop_word_filter)
      : stop_word_filter(stop_word_filter) {
    wfapp.load_event.connect([this](const string &path) { load_data(path); });
    wfapp.dowork_event.connect([this]() { produce_words(); });
  }

private:
  string data;
  StopWordFilter &stop_word_filter;

  void load_data(const string &path_to_file) {
    ifstream file(path_to_file);
    string content((istreambuf_iterator<char>(file)),
                   (istreambuf_iterator<char>()));
    file.close();
    regex re("[^a-zA-Z0-9]+");
    data = regex_replace(content, re, " ");
    transform(data.begin(), data.end(), data.begin(), ::tolower);
  }

  void produce_words() {
    istringstream iss(data);
    string word;
    while (iss >> word) {
      if (!stop_word_filter.is_stop_word(word)) {
        word_event(word);
      }
    }
  }
};

class WordFrequencyCounter {
public:
  WordFrequencyCounter(WordFrequencyFramework &wfapp,
                       DataStorage &data_storage) {
    data_storage.word_event.connect(
        [this](const string &word) { increment_count(word); });
    wfapp.end_event.connect([this]() { print_frequencies(); });
  }

private:
  unordered_map<string, int> word_freqs;

  void increment_count(const string &word) { word_freqs[word]++; }

  void print_frequencies() {
    vector<pair<string, int>> freqs(word_freqs.begin(), word_freqs.end());
    sort(freqs.begin(), freqs.end(),
         [](const auto &a, const auto &b) { return b.second < a.second; });

    for (size_t i = 0; i < 25 && i < freqs.size(); ++i) {
      cout << freqs[i].first << " - " << freqs[i].second << endl;
    }
  }
};

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <path_to_file>" << endl;
    return 1;
  }

  WordFrequencyFramework wfapp;
  StopWordFilter stop_word_filter(wfapp);
  DataStorage data_storage(wfapp, stop_word_filter);
  WordFrequencyCounter word_freq_counter(wfapp, data_storage);

  wfapp.run(argv[1]);

  return 0;
}
