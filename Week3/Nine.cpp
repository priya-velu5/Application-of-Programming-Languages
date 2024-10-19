#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <vector>
#include <sstream>

using namespace std;
void printTopFreq(vector<pair<string, int> > sortedFreq) {
    for (int i = 0; i < min(25, (int)sortedFreq.size()); i++) {
        cout << sortedFreq[i].first << " - " << sortedFreq[i].second << endl;
    }
}

bool sortByVal(const pair<string, int>& a, const pair<string, int>& b) {
    return (a.second > b.second);
}

void sortFreq(map<string, int> wordFreq, void (*printTopFreq)(vector<pair<string, int> >)) {
    vector<pair<string, int> > sortedFreq;

    // Copy key-value pairs from the map to the vector
    for (const auto& pair : wordFreq) {
        sortedFreq.push_back(make_pair(pair.first, pair.second));
    }

    sort(sortedFreq.begin(), sortedFreq.end(), sortByVal);
    printTopFreq(sortedFreq);
}

void countFreq(vector<string> tokens, void (*sortFreq)(map<string, int>, void (*)(vector<pair<string, int> >)) ) {
    map<string, int> wordFreq;

    for (const auto& word : tokens) {
        wordFreq[word]++;
    }
    sortFreq(wordFreq, printTopFreq);
}

void tokenize(set<string> stopWords, string fileName, void (*countFreq)(vector<string>, void (*)(map<string, int>, void (*)(vector<pair<string, int> >)))) {
    ifstream inputFile(fileName);
    vector<string> tokens;


    if (!inputFile.is_open()) {
        cout << "Input File not found" << endl;
        return;
    }

    regex word_regex("[a-z]{2,}");
    string line;
    while (getline(inputFile, line)) {
        transform(line.begin(), line.end(), line.begin(), ::tolower);
        for (sregex_iterator it(line.begin(), line.end(), word_regex), end; it != end; ++it) {
            string word = it->str();
            if (stopWords.find(word) == stopWords.end()) {
                tokens.push_back(word);
            }
        }
    }
    countFreq(tokens, sortFreq);
}

void loadStopWords(string fileName, string inputName, void (*tokenize)(set<string>, string, void (*)(vector<string>, void (*)(map<string, int>, void (*)(vector<pair<string, int> >)))) ) {
    ifstream inputFile(fileName);
    set<string> stopWords;
    if (!inputFile.is_open()) {
        cout << "Input File not found" << endl;
        return;
    }

    string line;
    while (getline(inputFile, line)) {
        stringstream linestream(line);
        string value;
        while (getline(linestream, value, ',')) {
            stopWords.insert(value);
        }
    }
    tokenize(stopWords, inputName, countFreq);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <text_file>" << endl;
        return 1;
    }
    string textFile = argv[1];
    loadStopWords("stop_words.txt", textFile, tokenize);

    return 0;
}
