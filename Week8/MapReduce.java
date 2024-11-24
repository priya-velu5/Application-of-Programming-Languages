// map reduce

package Week8;

import java.io.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.regex.*;
import java.util.stream.Collectors;

public class MapReduce {
    // Set of stopwords
    private static final Set<String> stopwords = new HashSet<>();

    public static void main(String[] args) throws IOException, InterruptedException, ExecutionException {
        // Read input data and divide into blocks
        
        if (args.length < 1) {
            System.out.println("Usage: java Thirty.java <input_file> <stop_words_file>");
            return;
        }
        // Load stop words
        loadStopWords("../stop_words.txt");

        String content = readFile(args[0]);
        List<String> blocks = splitIntoBlocks(content, 1000); // Blocks of 1000 words each

        // Map phase - Apply the worker function to each block
        ExecutorService executor = Executors.newFixedThreadPool(5); // Parallelize with 5 threads
        List<Future<Map<String, Integer>>> mapResults = new ArrayList<>();
        for (String block : blocks) {
            mapResults.add(executor.submit(() -> workerFunction(block)));
        }

        // Collect map results
        List<Map<String, Integer>> intermediateResults = new ArrayList<>();
        for (Future<Map<String, Integer>> result : mapResults) {
            intermediateResults.add(result.get());
        }

        // Reduce phase - takes the results of the many worker functions and recombines them into a coherent output
        Map<String, Integer> finalResult = reduce(intermediateResults);

        // Print top 25 results
        finalResult.entrySet().stream()
                .sorted((a, b) -> b.getValue().compareTo(a.getValue()))
                .limit(25)
                .forEach(entry -> System.out.println(entry.getKey() + " - " + entry.getValue()));

        // Shutdown executor
        executor.shutdown();
    }

    // Load stop words into a Set
    private static void loadStopWords(String stopWordsFile) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(stopWordsFile))) {
            String content = reader.readLine();
            if (content != null) {
                stopwords.addAll(Arrays.asList(content.split(",")));
            }
        }
    }

    // Read input data from file
    private static String readFile(String inputFile) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(inputFile))) {
            return reader.lines().collect(Collectors.joining(" "));
        }
    }

    // Divide input data into blocks
    private static List<String> splitIntoBlocks(String content, int blockSize) {
        String[] words = content.toLowerCase().split("\\W+");
        
        List<String> blocks = new ArrayList<>();
        for (int i = 0; i < words.length; i += blockSize) {
            blocks.add(String.join(" ", Arrays.copyOfRange(words, i, Math.min(words.length, i + blockSize))));
        }
        return blocks;
    }

    // Worker function applied to each block (Map step)
    private static Map<String, Integer> workerFunction(String block) {
        Map<String, Integer> wordCounts = new HashMap<>();
        Matcher matcher = Pattern.compile("[a-z]{2,}").matcher(block);
        while (matcher.find()) {
            String word = matcher.group();
            if (!stopwords.contains(word)){
                wordCounts.put(word, wordCounts.getOrDefault(word, 0) + 1);
            }
            
        }
        return wordCounts;
    }

    // Reduce function combines intermediate results
    private static Map<String, Integer> reduce(List<Map<String, Integer>> intermediateResults) {
        Map<String, Integer> finalResult = new HashMap<>();
        for (Map<String, Integer> partialResult : intermediateResults) {
            for (Map.Entry<String, Integer> entry : partialResult.entrySet()) {
                finalResult.merge(entry.getKey(), entry.getValue(), Integer::sum);
            }
        }
        return finalResult;
    }
}
