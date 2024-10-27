// create a class framework that executes each entity through callbacks 

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <boost/signals2.hpp>
#include <regex>
#include <cctype>
#include <locale>

// The "I'll call you back" Word Frequency Framework in C++

class WordFrequencyFramework {
public:
    boost::signals2::signal<void(const std::string&)> load_event;
    boost::signals2::signal<void()> dowork_event;
    boost::signals2::signal<void()> end_event;

    void run(const std::string& path_to_file) {
        load_event(path_to_file);
        dowork_event();
        end_event();
    }
};

class StopWordFilter {
public:
    StopWordFilter(WordFrequencyFramework& wfapp) {
        wfapp.load_event.connect([this](const std::string& ignore) { load_stop_words(); });
    }

    bool is_stop_word(const std::string& word) {
        return stop_words.find(word) != stop_words.end();
    }

private:
    std::unordered_set<std::string> stop_words;

    void load_stop_words() {
        std::ifstream file("../stop_words.txt");
        std::string line;
        if (file.is_open()) {
            while (getline(file, line, ',')) {
                stop_words.insert(line);
            }
            file.close();
        }
        // Add single-letter words (a-z)
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            stop_words.insert(std::string(1, ch));
        }
    }
};

class DataStorage {
public:
    boost::signals2::signal<void(const std::string&)> word_event;

    DataStorage(WordFrequencyFramework& wfapp, StopWordFilter& stop_word_filter)
        : stop_word_filter(stop_word_filter) {
        wfapp.load_event.connect([this](const std::string& path) { load_data(path); });
        wfapp.dowork_event.connect([this]() { produce_words(); });
    }

private:
    std::string data;
    StopWordFilter& stop_word_filter;

    void load_data(const std::string& path_to_file) {
        std::ifstream file(path_to_file);
        std::string content((std::istreambuf_iterator<char>(file)),
                            (std::istreambuf_iterator<char>()));
        file.close();
        std::regex re("[^a-zA-Z0-9]+");
        data = std::regex_replace(content, re, " ");
        std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    }

    void produce_words() {
        std::istringstream iss(data);
        std::string word;
        while (iss >> word) {
            if (!stop_word_filter.is_stop_word(word)) {
                word_event(word);
            }
        }
    }
};

class WordFrequencyCounter {
public:
    WordFrequencyCounter(WordFrequencyFramework& wfapp, DataStorage& data_storage) {
        data_storage.word_event.connect([this](const std::string& word) { increment_count(word); });
        wfapp.end_event.connect([this]() { print_frequencies(); });
    }

private:
    std::unordered_map<std::string, int> word_freqs;

    void increment_count(const std::string& word) {
        word_freqs[word]++;
    }

    void print_frequencies() {
        std::vector<std::pair<std::string, int>> freqs(word_freqs.begin(), word_freqs.end());
        std::sort(freqs.begin(), freqs.end(),
                  [](const auto& a, const auto& b) { return b.second < a.second; });

        for (size_t i = 0; i < 25 && i < freqs.size(); ++i) {
            std::cout << freqs[i].first << " - " << freqs[i].second << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_file>" << std::endl;
        return 1;
    }

    WordFrequencyFramework wfapp;
    StopWordFilter stop_word_filter(wfapp);
    DataStorage data_storage(wfapp, stop_word_filter);
    WordFrequencyCounter word_freq_counter(wfapp, data_storage);

    wfapp.run(argv[1]);

    return 0;
}