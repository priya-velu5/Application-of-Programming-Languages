#include <iostream>
#include <fstream>
#include <unordered_set>
#include <map>
#include <vector>
#include <regex>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <file>" << endl;
        return 1;
    }

    // Load stopwords
    unordered_set<string> stopwords;
    ifstream("../stop_words.txt") >> noskipws >> [&, stop = string()] {
        while (getline(cin, stop, ',')) stopwords.insert(stop);
    };

    // Open and process the input file
    ifstream input_file(argv[1]);
    string content((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
    transform(content.begin(), content.end(), content.begin(), ::tolower);

    // Extract words and count occurrences (excluding stopwords)
    map<string, int> word_count;
    for (sregex_iterator it(content.begin(), content.end(), regex("[a-z]{2,}")), end; it != end; ++it) {
        string word = it->str();
        if (stopwords.find(word) == stopwords.end()) word_count[word]++;
    }

    // Sort and print the top 25 words
    vector<pair<string, int>> sorted_words(word_count.begin(), word_count.end());
    partial_sort(sorted_words.begin(), sorted_words.begin() + min(25, (int)sorted_words.size()), sorted_words.end(), [](const auto& a, const auto& b) { return b.second < a.second; });

    for (int i = 0; i < min(25, (int)sorted_words.size()); ++i) {
        cout << sorted_words[i].first << " - " << sorted_words[i].second << endl;
    }

    return 0;
}
