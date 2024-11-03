import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.function.*;
import java.util.stream.*;

public class TwentyFive {
    private List<Function<Object, Object>> functions;

    public TwentyFive(Function<Object, Object> func) {
        this.functions = new ArrayList<>();
        this.functions.add(func);
    }

    public TwentyFive bind(Function<Object, Object> func) {
        this.functions.add(func);
        return this;
    }

    public void execute() {
        Object value = null;
        for (Function<Object, Object> func : functions) {
            value = func.apply(value);
        }
        System.out.println(value);
    }

    // Main function to run the program
    public static void main(String[] args) {
        if (args.length < 1) {
            System.err.println("Usage: java TwentyFive <path to text file>");
            System.exit(1);
        }
        new TwentyFive((arg) -> getInput(args[0]))
            .bind(TwentyFive::tokenize)
            .bind(TwentyFive::removeStopWords)
            .bind(TwentyFive::countFreq)
            .bind(TwentyFive::sortFreq)
            .bind(TwentyFive::printTopFreq)
            .execute();
    }

    // Functions as per the pipeline
    public static Object getInput(Object arg) {
        // Simply return the argument (the file path) passed in
        return arg;
    }
    

    public static Object tokenize(Object pathToFile) {
        try {
            String content = new String(Files.readAllBytes(Paths.get((String) pathToFile)));
            return Arrays.asList(content.replaceAll("[\\W_]+", " ").toLowerCase().split(" "));
        } catch (IOException e) {
            throw new RuntimeException(e);
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
            throw new RuntimeException(e);
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

    public static Object printTopFreq(Object sortedWordFreqs) {
        List<Map.Entry<String, Integer>> sortedEntries = (List<Map.Entry<String, Integer>>) sortedWordFreqs;
        StringBuilder result = new StringBuilder();
        sortedEntries.stream().limit(25).forEach(entry -> result.append(entry.getKey()).append(" - ").append(entry.getValue()).append("\n"));
        return result.toString();
    }
}
