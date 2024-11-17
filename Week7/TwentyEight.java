package Week7;
import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.regex.*;

public class TwentyEight {

    // Iterator for reading words from a file
    public static Iterator<String> wordsIterator(String filename) throws IOException {
        BufferedReader reader = new BufferedReader(new FileReader(filename));
        return new Iterator<>() {
            String nextLine = null;
            Iterator<String> currentLineWords = Collections.emptyIterator();

            @Override
            public boolean hasNext() {
                try {
                    while (!currentLineWords.hasNext() && (nextLine = reader.readLine()) != null) {
                        currentLineWords = Arrays.stream(nextLine.toLowerCase()
                                                .replaceAll("[\\W_]+", " ")
                                                .split(" "))
                                                // .split("\\W+"))
                                                 .filter(word -> word.length() > 1)
                                                 .iterator();
                    }
                } catch (IOException e) {
                    throw new UncheckedIOException(e);
                }
                return currentLineWords.hasNext();
            }

            @Override
            public String next() {
                if (hasNext()) {
                    return currentLineWords.next();
                }
                throw new NoSuchElementException();
            }
        };
    }

    // Iterator for filtering non-stop words
    public static Iterator<String> nonStopWordsIterator(String filename, Set<String> stopWords) throws IOException {
        Iterator<String> wordIterator = wordsIterator(filename);
        return new Iterator<>() {
            String nextWord = null;

            @Override
            public boolean hasNext() {
                while (nextWord == null && wordIterator.hasNext()) {
                    String candidate = wordIterator.next();
                    if (!stopWords.contains(candidate)) {
                        nextWord = candidate;
                    }
                }
                return nextWord != null;
            }

            @Override
            public String next() {
                if (hasNext()) {
                    String result = nextWord;
                    nextWord = null;
                    return result;
                }
                throw new NoSuchElementException();
            }
        };
    }

    // Iterator for counting and sorting word frequencies
    public static Iterator<Map.Entry<String, Integer>> countAndSortIterator(String filename, Set<String> stopWords) throws IOException {
        Iterator<String> nonStopWordIterator = nonStopWordsIterator(filename, stopWords);
        Map<String, Integer> wordFreqs = new HashMap<>();
        final int[] wordCount = {0}; // Keeps track of the total words processed
    
        return new Iterator<>() {
            List<Map.Entry<String, Integer>> sortedEntries = new ArrayList<>();
            Iterator<Map.Entry<String, Integer>> currentBatch = Collections.emptyIterator();
    
            @Override
            public boolean hasNext() {
                while (!currentBatch.hasNext()) {
                    if (!nonStopWordIterator.hasNext()) {
                        // If no more words are available, finalize the sorted batch
                        if (!wordFreqs.isEmpty()) {
                            sortedEntries = new ArrayList<>(wordFreqs.entrySet());
                            sortedEntries.sort(Map.Entry.<String, Integer>comparingByValue().reversed());
                            currentBatch = sortedEntries.iterator();
                        }
                        return currentBatch.hasNext();
                    }
    
                    // Process the next word
                    String word = nonStopWordIterator.next();
                    wordFreqs.put(word, wordFreqs.getOrDefault(word, 0) + 1);
                    wordCount[0]++;
    
                    // Emit sorted entries after every 5000 words
                    if (wordCount[0] % 5000 == 0) {
                        // Sort and emit the current batch
                        sortedEntries = new ArrayList<>(wordFreqs.entrySet());
                        sortedEntries.sort(Map.Entry.<String, Integer>comparingByValue().reversed());
                        currentBatch = sortedEntries.iterator();
                        currentBatch = Collections.emptyIterator();
                    }
                }
                return currentBatch.hasNext() || nonStopWordIterator.hasNext();
            }
    
            @Override
            public Map.Entry<String, Integer> next() {
                if (hasNext()) {
                    return currentBatch.next();
                }
                throw new NoSuchElementException();
            }
        };
    }
    

    // Main function
    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java WordFrequencyCounter <input_file> <stop_words_file>");
            return;
        }

        String inputFile = args[0];
        String stopWordsFile = "../stop_words.txt";

        try {
            // Load stop words into a Set (comma-separated)
            String stopWordsContent = new String(Files.readAllBytes(Paths.get(stopWordsFile)));
            Set<String> stopWords = new HashSet<>(Arrays.asList(stopWordsContent.split(",")));

            // Get the sorted word-frequency pairs
            Iterator<Map.Entry<String, Integer>> sortedWordFreqs = countAndSortIterator(inputFile, stopWords);

            // Print top 25 words from the final output
            System.out.println("-----------------------------");
            int count = 0;
            while (sortedWordFreqs.hasNext() && count < 25) {
                Map.Entry<String, Integer> entry = sortedWordFreqs.next();
                System.out.println(entry.getKey() + " - " + entry.getValue());
                count++;
            }

        } catch (IOException e) {
            System.err.println("Error processing files: " + e.getMessage());
        }
    }
}
