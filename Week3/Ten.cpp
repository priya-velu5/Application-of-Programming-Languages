#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <vector>
#include <functional>

using namespace std;
//this class serves as an abstraction to encapsulate the states and operations. 
// this keyword is used to implement function chaining
class WordFrequencyAnalyzer {
public:
    // Constructor
    WordFrequencyAnalyzer(const string& stopWordsFile, const string& textFile)
        : stopWordsFile(stopWordsFile), textFile(textFile) {}

    // Function to wrap the input values and load stop words
    WordFrequencyAnalyzer& loadStopWords() {
        ifstream inputFile(stopWordsFile);
        if (!inputFile.is_open()) {
            cout << "Stop words file not found." << endl;
            return *this;
        }

        string line;
        while (getline(inputFile, line)) {
            stringstream linestream(line);
            string value;
            while (getline(linestream, value, ',')) {
                stopWords.insert(value);
            }
        }
        return *this;
    }

    // Function to tokenize the text file
    WordFrequencyAnalyzer& tokenize() {
        ifstream inputFile(textFile);
        if (!inputFile.is_open()) {
            cout << "Text file not found." << endl;
            return *this;
        }

        regex word_regex("[a-z]{2,}");
        string line;
        while (getline(inputFile, line)) {
            transform(line.begin(), line.end(), line.begin(), ::tolower);
            for (sregex_iterator it(line.begin(), line.end(), word_regex), end; it != end; ++it) {
                string word = it->str();
                if (stopWords.find(word) == stopWords.end()) {  // Check if the word is not a stop word
                    tokens.push_back(word);
                }
            }
        }
        return *this;
    }

    // Function to count word frequencies
    WordFrequencyAnalyzer& countFreq() {
        for (const auto& word : tokens) {
            wordFreq[word]++;
        }
        return *this;
    }
    
    bool static sortByVal(const pair<string, int> &a, const pair<string, int> &b) {
        return (a.second > b.second);
    }
    // Function to sort word frequencies
    WordFrequencyAnalyzer& sortFreq() {
        for (const auto& entry : wordFreq) {
            sortedFreq.push_back(make_pair(entry.first, entry.second));
        }
        sort(sortedFreq.begin(), sortedFreq.end(),sortByVal);  // Sort by frequency in descending order
        
        return *this;
    }

    // Function to get the top frequencies as a result
    vector<pair<string, int> > getTopFreq(int n = 25) const {
        vector<pair<string, int> > result;
        for (int i = 0; i < min(n, static_cast<int>(sortedFreq.size())); i++) {
            result.push_back(sortedFreq[i]);
        }
        return result;
    }

    // Function to print the top frequencies
    void printTopFreq(int n = 25) const {
        auto topFreq = getTopFreq(n);
        for (const auto& freq : topFreq) {
            cout << freq.first << " - " << freq.second << endl;
        }
    }

private:
    string stopWordsFile;
    string textFile;
    set<string> stopWords;
    vector<string> tokens;
    map<string, int> wordFreq;
    vector<pair<string, int> > sortedFreq;
};

// Main function
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << "<text_file>" << endl;
        return 1;
    }

    WordFrequencyAnalyzer analyzer("stop_words.txt", argv[1]);
    analyzer.loadStopWords()
           .tokenize()
           .countFreq()
           .sortFreq()
           .printTopFreq(); // Print final results

    return 0;
}
