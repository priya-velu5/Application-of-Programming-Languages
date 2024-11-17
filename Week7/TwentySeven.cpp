// create an excel like implementation
//every class represents a column. It has data element and a function to update the data element
// when the data element is updated, it should update the data element of all the columns that depend on it

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <vector>
#include <sstream>
using namespace std;

class allWordsColumn{

public:
    vector<string> tokens;
    void updateTokens(string fileName){
        fstream inputFile(fileName);

        if (!inputFile.is_open()) {
            cout << "Input File not found" << endl;
            return;
        }
        regex word_regex("[a-z]{2,}");
        string line;
        while (getline(inputFile, line)) {
            transform(line.begin(), line.end(), line.begin(), ::tolower);
            for (sregex_iterator it(line.begin(), line.end(), word_regex), end;
                it != end; ++it) {
            string word = it->str();
            tokens.push_back(word);
            
            }
        }
    }

};

class stopWordsColumn{
public:
    set<string> stopWords;
    void updateStopWords(string fileName){
        ifstream inputFile(fileName);
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
    }
};

class cleanedTokensColumn{
public:
    vector<string> cleanedTokens;
    void updateCleanedTokens(allWordsColumn tokenObj, stopWordsColumn stopObj){
        for (auto word : tokenObj.tokens) {
            if (stopObj.stopWords.find(word) != stopObj.stopWords.end()) {
                continue;
            } else {
                cleanedTokens.push_back(word);
            }
        }
    }
};

class uniqueWordsColumn{
public:
    vector<string> uniqueWords;
    void updateUniqueWords(cleanedTokensColumn cleanedObj){
        set<string> temp;
        for (auto word : cleanedObj.cleanedTokens) {
            temp.insert(word);
        }

        for(auto word: temp){
            uniqueWords.push_back(word);
        }
    }   
};

class countsColumn{
    //store the frequency of 
public:
    vector<int> counts;
    void updateWordFreq(cleanedTokensColumn cleanedObj, uniqueWordsColumn uniqueObj){
        
        map<string, int> wordFreq;
        //find the counts
        for (auto word : cleanedObj.cleanedTokens) {
            wordFreq[word]++;
        }

        //update counts in the same order as unique words
        for (auto word : uniqueObj.uniqueWords) {
            counts.push_back(wordFreq[word]);
        }
    }
};

class sortFreqFunction{

    public:
        vector<pair<string, int> > sortedFreq;

        static bool sortByVal(const pair<string, int> &a, const pair<string, int> &b) {
            return (a.second > b.second);
        }

        void sortFreq(uniqueWordsColumn uniqueObj, countsColumn countObj){ 
            // copy key-value pairs from the map to the vector
            vector<string>::iterator it;
            
            for (int i = 0; i < uniqueObj.uniqueWords.size(); i++) {
                sortedFreq.push_back(make_pair(uniqueObj.uniqueWords[i], countObj.counts[i]));
            }

            sort(sortedFreq.begin(), sortedFreq.end(), sortByVal);
        }
    
        void printTopFreq() {
            for (int i = 0; i < 25; i++) {
                cout << sortedFreq[i].first << " - " << sortedFreq[i].second << endl;
            }
        }
};

class spreadsheet{
    public:
        allWordsColumn allWords;
        stopWordsColumn stopWords;
        cleanedTokensColumn cleanedTokens;
        uniqueWordsColumn uniqueWords;
        countsColumn counts;
        sortFreqFunction sort;

        void updateSpreadsheet(string fileName, string stopWordsFile){
            allWords.updateTokens(fileName);
            stopWords.updateStopWords(stopWordsFile);
            cleanedTokens.updateCleanedTokens(allWords, stopWords);
            uniqueWords.updateUniqueWords(cleanedTokens);
            counts.updateWordFreq(cleanedTokens, uniqueWords);
            sort.sortFreq(uniqueWords, counts);
        }
};

int main(int argc, char *argv[]){
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <text_file>" << endl;
        return 1;
    }
    string textFile = argv[1];
    spreadsheet sheet;
    sheet.updateSpreadsheet(textFile, "../stop_words.txt");
    sheet.sort.printTopFreq();
    return 0;
}