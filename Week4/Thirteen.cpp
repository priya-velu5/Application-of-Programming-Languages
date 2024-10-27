// class without a class - use maps and lambda functions 
//to create class-like object, because everything is an object, including functions.
// javascrtipt like model
// github code - only 13.1?

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <regex>
#include <string>
#include <set>
#include <cctype>
#include <any> // using this as c++ does not have a built-in way to store any type of data

using namespace std;
// Data Storage Object
map<string, any> data_storage_obj;

// Stop Words Object
map<string, any> stop_words_obj;

// Word Frequency Object
map<string, any> word_freqs_obj;

void extract_words(map<string, any>& obj, const string& path_to_file) {
    /*
        Extracts words from a file, cleans it
        and stores them in the data object
    */
    ifstream file(path_to_file);
    if (!file.is_open()) {
        throw runtime_error("Could not open file: " + path_to_file);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    string data = buffer.str();

    // Cleaning and lowering the case of the data
    regex pattern("[\\W_]+");
    data = regex_replace(data, pattern, " ");
    transform(data.begin(), data.end(), data.begin(), ::tolower);

    // Splitting words
    istringstream iss(data);
    vector<string> words;
    string word;
    while (iss >> word) {
        words.push_back(word);
    }
    obj["data"] = words;
}

void load_stop_words(map<string, any>& obj) {
    /*
        Load stop words and store them 
        in the stop words object
    */
    set<string> stop_words;
    ifstream file("../stop_words.txt");
    if (!file.is_open()) {
        throw runtime_error("Could not open stop words file");
    }
    string line;
    while (getline(file, line, ',')) {
        stop_words.insert(line);
    }
    for (char ch = 'a'; ch <= 'z'; ++ch) {
        stop_words.insert(string(1, ch));
    }
    obj["stop_words"] = stop_words;
}

// Function to simulate the increment_count functionality
void increment_count(map<string, any>& obj, const string& word) {
    auto& freqs = any_cast<unordered_map<string, int>&>(obj["freqs"]);
    ++freqs[word];
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Please provide a file path as an argument." << endl;
        return 1;
    }

    data_storage_obj["data"] = vector<string>();
    data_storage_obj["init"] = std::function<void(const string&)>(
        [&](const string& path_to_file) { extract_words(data_storage_obj, path_to_file); }
    );
    data_storage_obj["words"] = [&]() -> vector<string>& { return any_cast<vector<string>&>(data_storage_obj["data"]); };
  
    stop_words_obj["stop_words"] = set<string>();
    stop_words_obj["init"] = [&]() { load_stop_words(stop_words_obj); };
    stop_words_obj["is_stop_word"] = [&](const string& word) -> bool {
        auto& stop_words = any_cast<set<string>&>(stop_words_obj["stop_words"]);
        return stop_words.find(word) != stop_words.end();
    };


    word_freqs_obj["freqs"] = unordered_map<string, int>();
    word_freqs_obj["increment_count"] = [&](const string& word) { increment_count(word_freqs_obj, word); };
    word_freqs_obj["sorted"] = [&]() -> vector<pair<string, int>> {
        auto& freqs = any_cast<unordered_map<string, int>&>(word_freqs_obj["freqs"]);
        vector<pair<string, int>> sorted_freqs(freqs.begin(), freqs.end());
        sort(sorted_freqs.begin(), sorted_freqs.end(), [](const auto& a, const auto& b) {
            return b.second > a.second;
        });
        return sorted_freqs;
    };

    // Initialize and load data
    any_cast<function<void(const string&)>>(data_storage_obj["init"])(argv[1]);
    any_cast<function<void()>>(stop_words_obj["init"])();

    // Process words
    for (const auto& word : any_cast<function<vector<string>&()>>(data_storage_obj["words"])()) {
        if (!any_cast<function<bool(const string&)>>(stop_words_obj["is_stop_word"])(word)) {
            any_cast<function<void(const string&)>>(word_freqs_obj["increment_count"])(word);
        }
    }

    // Sort and print top 25 words
    auto word_freqs = any_cast<function<vector<pair<string, int>>()>>(word_freqs_obj["sorted"])();
    for (size_t i = 0; i < min(word_freqs.size(), size_t(25)); ++i) {
        cout << word_freqs[i].first << " - " << word_freqs[i].second << endl;
    }

    return 0;
}
