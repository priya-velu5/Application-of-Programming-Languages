package Week10_redo;

import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.function.Function;
import java.util.stream.*;

public class Ten {
    private Object value;
    private List<Function<Object, Object>> functions;

    // Constructor to wrap the initial value
    public Ten(Object value) {
        this.value = value;
    }

    // Bind function to apply a transformation
    public Ten bind(Function<Object, Object> func) {
        this.value = func.apply(this.value);
        return this;
    }

    // Method to unwrap and print the final value
    public void printMe() {
        System.out.println(this.value);
    }

    // Main function to run the program
    public static void main(String[] args) {
        if (args.length < 1) {
            System.err.println("Usage: java Ten <path to text file>");
            System.exit(1);
        }

        new Ten(args[0])
            .bind(Ten::tokenize)
            .bind(Ten::removeStopWords)
            .bind(Ten::countFreq)
            .bind(Ten::sortFreq)
            .bind(Ten::top25Freq)
            .printMe();
    }

    // Functions representing the pipeline steps
    public static Object tokenize(Object pathToFile) {
        try {
            String content = new String(Files.readAllBytes(Paths.get((String) pathToFile)));
            return Arrays.asList(content.replaceAll("[\\W_]+", " ").toLowerCase().split(" "));
        } catch (IOException e) {
            throw new RuntimeException("Error reading file: " + pathToFile, e);
        }
    }

    public static Object removeStopWords(Object wordList) {
        try {
            List<String> words = (List<String>) wordList;
            String stopWordsContent = new String(Files.readAllBytes(Paths.get("../stop_words.txt")));
            Set<String> stopWords = new HashSet<>(Arrays.asList(stopWordsContent.split(",")));
            stopWords.addAll("abcdefghijklmnopqrstuvwxyz".chars().mapToObj(c -> String.valueOf((char) c)).collect(Collectors.toSet()));

            return words.stream()
                        .filter(word -> !stopWords.contains(word))
                        .collect(Collectors.toList());
        } catch (IOException e) {
            throw new RuntimeException("Error reading stop words file.", e);
        }
    }

    public static Object countFreq(Object wordList) {
        List<String> words = (List<String>) wordList;
        Map<String, Integer> wordFreqs = new HashMap<>();

        for (String word : words) {
            wordFreqs.put(word, wordFreqs.getOrDefault(word, 0) + 1);
        }

        return wordFreqs;
    }

    public static Object sortFreq(Object wordFreq) {
        Map<String, Integer> wordFreqMap = (Map<String, Integer>) wordFreq;
        return wordFreqMap.entrySet()
                          .stream()
                          .sorted((e1, e2) -> e2.getValue().compareTo(e1.getValue()))
                          .collect(Collectors.toList());
    }

    public static Object top25Freq(Object sortedWordFreqs) {
        List<Map.Entry<String, Integer>> sortedEntries = (List<Map.Entry<String, Integer>>) sortedWordFreqs;
        StringBuilder result = new StringBuilder();
        sortedEntries.stream()
                     .limit(25)
                     .forEach(entry -> result.append(entry.getKey()).append(" - ").append(entry.getValue()).append("\n"));
        return result.toString();
    }
}
