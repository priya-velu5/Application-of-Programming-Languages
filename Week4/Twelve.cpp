// basically classes, dont expose data outside of class - private data members, 
// public member functions to access data
// dispatch function - to call member functions
// dont use constructors but use public init functions

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <regex>
#include <string>
#include <cctype>
#include <set>

using namespace std;

class DataStorageManager {
    string data; //stores the entire content of the file

public:
    void dispatch(const string& message, const string& path = "") {
        if (message == "init") {
            init(path);
        } else if (message == "words") {
            // No operation needed here for words dispatch
        } else {
            throw runtime_error("Message not understood: " + message);
        }
    }

    void init(const string& path) {
        ifstream file(path);
        if (!file.is_open()) {
            throw runtime_error("Could not open file: " + path);
        }
        stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        data = buffer.str();
        regex pattern("[\\W_]+");
        data = regex_replace(data, pattern, " ");
        transform(data.begin(), data.end(), data.begin(), ::tolower);
    }

    vector<string> words() {
        istringstream iss(data);
        vector<string> words;
        string word;
        while (iss >> word) {
            words.push_back(word);
        }
        return words;
    }
};

class StopWordManager {
    set<string> stopWords;

public:
    void dispatch(const string& message, const string& word = "") {
        if (message == "init") {
            init();
        } else if (message == "is_stop_word") {
            // No operation needed here for is_stop_word dispatch
        } else {
            throw runtime_error("Message not understood: " + message);
        }
    }

    void init() {
        ifstream file("stop_words.txt");
        if (!file.is_open()) {
            throw runtime_error("Could not open stop words file");
        }
        string line;
        while (getline(file, line, ',')) {
            stopWords.insert(line);
        }
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            stopWords.insert(string(1, ch));
        }
        file.close();
    }

    bool is_stop_word(const string& word) {
        return stopWords.find(word) != stopWords.end();
    }
};

class WordFrequencyManager {
    unordered_map<string, int> wordFreq;

public:
    void dispatch(const string& message, const string& word = "") {
        if (message == "increment_count") {
            _increment_count(word);
        } else if (message == "sorted") {
            // No operation needed here for sorted dispatch
        } else {
            throw runtime_error("Message not understood: " + message);
        }
    }

    void _increment_count(const string& word) {
        ++wordFreq[word];
    }

    static bool sortByVal(const pair<string, int>& a, const pair<string, int>& b) {
    return (a.second > b.second);
    }

    vector<pair<string, int> > sorted() {
        vector<pair<string, int> > freqs(wordFreq.begin(), wordFreq.end());
        sort(freqs.begin(), freqs.end(), sortByVal);
        return freqs;
    }
};

class WordFrequencyController {
    DataStorageManager storage_manager;
    StopWordManager stop_word_manager;
    WordFrequencyManager word_freq_manager;

public:
    void dispatch(const string& message, const string& path = "") {
        if (message == "init") {
            init(path);
        } else if (message == "run") {
            _run();
        } else {
            throw runtime_error("Message not understood: " + message);
        }
    }

    void init(const string& path) {
        storage_manager.dispatch("init", path);
        stop_word_manager.dispatch("init");
    }

    void _run() {
        for (const auto& word : storage_manager.words()) {
            if (!stop_word_manager.is_stop_word(word)) {
                word_freq_manager.dispatch("increment_count", word);
            }
        }

        auto word_freqs = word_freq_manager.sorted();
        for (size_t i = 0; i < min(word_freqs.size(), size_t(25)); ++i) {
            cout << word_freqs[i].first << " - " << word_freqs[i].second << endl;
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Please provide a file path as an argument." << endl;
        return 1;
    }

    WordFrequencyController wfcontroller;
    wfcontroller.dispatch("init", argv[1]);
    wfcontroller.dispatch("run");

    return 0;
}
