// ------------------13.1 + 13.2------------------

// #!/usr/bin/env node
// const fs = require("fs");
// const path = require("path");

// // Auxiliary functions
// function extractWords(obj, pathToFile) {
//     const data = fs.readFileSync(pathToFile, "utf-8");
//     const dataStr = data.replace(/[\W_]+/g, " ").toLowerCase();
//     obj.data = dataStr.split(/\s+/);
// }

// function loadStopWords(obj) {
//     const stopWordsData = fs.readFileSync(
//         path.join(__dirname, "../stop_words.txt"),
//         "utf-8",
//     );
//     obj.stopWords = stopWordsData.split(",");
//     // add single-letter words
//     obj.stopWords.push(...Array.from("abcdefghijklmnopqrstuvwxyz"));
// }

// function incrementCount(obj, w) {
//     obj.freqs[w] = (obj.freqs[w] || 0) + 1;
// }

// // Data objects
// const dataStorageObj = {
//     data: [],
//     init: (pathToFile) => extractWords(dataStorageObj, pathToFile),
//     words: () => dataStorageObj.data,
// };

// const stopWordsObj = {
//     stopWords: [],
//     init: () => loadStopWords(stopWordsObj),
//     isStopWord: (word) => stopWordsObj.stopWords.includes(word),
// };

// const wordFreqsObj = {
//     freqs: {},
//     incrementCount: (w) => incrementCount(wordFreqsObj, w),
//     sorted: () =>
//         Object.entries(wordFreqsObj.freqs).sort(([, a], [, b]) => b - a),
// };

// // Initialization
// if (process.argv.length < 3) {
//     console.error("Please provide a path to the text file.");
//     process.exit(1);
// }

// dataStorageObj.init(process.argv[2]);
// stopWordsObj.init();

// // Word counting
// dataStorageObj.words().forEach((w) => {
//     if (!stopWordsObj.isStopWord(w)) {
//         wordFreqsObj.incrementCount(w);
//     }
// });

// // Output the top 25 most frequent words
// const wordFreqs = wordFreqsObj.sorted();
// for (const [w, c] of wordFreqs.slice(0, 25)) {
//     console.log(`${w} - ${c}`);
// }


// 13.2 - Add a new method to the wordFreqsObj object
//
// wordFreqsObj.top25 = function () {
//     console.log("----- printing for 13.2-----")
//     const sortedFreqs = this.sorted();
//     sortedFreqs.slice(0, 25).forEach(([w, c]) => {
//         console.log(`${w} - ${c}`);
//     });
// };

// // Call the new top25 method
// wordFreqsObj.top25();

//-------------------------13.3-------------------------


const fs = require("fs");
const path = require("path");

// Auxiliary functions
function extractWords(obj, pathToFile) {
    const data = fs.readFileSync(pathToFile, "utf-8");
    const dataStr = data.replace(/[\W_]+/g, " ").toLowerCase();
    obj.data = dataStr.split(/\s+/);
}

function loadStopWords(obj) {
    const stopWordsData = fs.readFileSync(
        path.join(__dirname, "../stop_words.txt"),
        "utf-8"
    );
    obj.stopWords = stopWordsData.split(",");
    // add single-letter words
    obj.stopWords.push(...Array.from("abcdefghijklmnopqrstuvwxyz"));
}

function incrementCount(obj, w) {
    obj.freqs[w] = (obj.freqs[w] || 0) + 1;
}

// Data objects with 'this' reference
const dataStorageObj = {
    data: [],
    init(pathToFile) {
        extractWords(this, pathToFile);
    },
    words() {
        return this.data;
    },
};

const stopWordsObj = {
    stopWords: [],
    init() {
        loadStopWords(this);
    },
    isStopWord(word) {
        return this.stopWords.includes(word);
    },
};

const wordFreqsObj = {
    freqs: {},
    incrementCount(w) {
        incrementCount(this, w);
    },
    sorted() {
        return Object.entries(this.freqs).sort(([, a], [, b]) => b - a);
    },
};

// Initialization
if (process.argv.length < 3) {
    console.error("Give input file path");
    process.exit(1);
}

dataStorageObj.init(process.argv[2]);
stopWordsObj.init();

// Word counting
dataStorageObj.words().forEach((w) => {
    if (!stopWordsObj.isStopWord(w)) {
        wordFreqsObj.incrementCount(w);
    }
});

// Additions for the top25 method (after line #46)
wordFreqsObj.top25 = function () {
    const sortedFreqs = this.sorted();
    sortedFreqs.slice(0, 25).forEach(([w, c]) => {
        console.log(`${w} - ${c}`);
    });
};

// Call the new top25 method
wordFreqsObj.top25();
