package Week8;

import java.io.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.regex.*;
import java.util.stream.Collectors;

public class ThirtyTwo {
    public static void main(String[] args) throws IOException, InterruptedException, ExecutionException {
        
        if (args.length < 1) {
            System.out.println("Usage: java ThirtyTwo.java <input_file> ");
            return;
        }
        // Step 1: Load stop words
        Set<String> stopwords = loadStopWords("../stop_words.txt");

        // Step 2: Read and partition input data
        String content = readFile(args[0]);
        List<String> chunks = partition(content, 10); // Partition into chunks of 10 lines

        // Step 3: First map step - Process each chunk
        ExecutorService executor = Executors.newFixedThreadPool(5);
        List<Future<Map<String, Integer>>> mapResults = new ArrayList<>();
        for (String chunk : chunks) {
            mapResults.add(executor.submit(() -> workerFunction(chunk, stopwords)));
        }

        // Step 4: Collect map results
        List<Map<String, Integer>> intermediateResults = new ArrayList<>();
        for (Future<Map<String, Integer>> result : mapResults) {
            intermediateResults.add(result.get());
        }

        // Step 5: Regroup step - Distribute words into 5 groups
        Map<String, Map<String, Integer>> regrouped = regroup(intermediateResults);

        // Step 6: Second map step - Process each group
        List<Future<Map<String, Integer>>> groupResults = new ArrayList<>();
        for (Map.Entry<String, Map<String, Integer>> entry : regrouped.entrySet()) {
            groupResults.add(executor.submit(() -> reduce(entry.getValue())));
        }

        // Step 7: Collect final results
        Map<String, Integer> finalResult = new HashMap<>();
        for (Future<Map<String, Integer>> result : groupResults) {
            mergeResults(finalResult, result.get());
        }

        // Step 8: Print top 25 results
        finalResult.entrySet().stream()
                .sorted((a, b) -> b.getValue().compareTo(a.getValue()))
                .limit(25)
                .forEach(entry -> System.out.println(entry.getKey() + " - " + entry.getValue()));

        // Shutdown executor
        executor.shutdown();
    }

    // Load stop words from a file
    private static Set<String> loadStopWords(String stopWordsFile) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(stopWordsFile))) {
            String content = reader.readLine();
            return content == null ? new HashSet<>() : new HashSet<>(Arrays.asList(content.split(",")));
        }
    }

    // Read input data from a file
    private static String readFile(String inputFile) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(inputFile))) {
            return reader.lines().collect(Collectors.joining("\n"));
        }
    }

    // Partition input data into chunks of lines
    private static List<String> partition(String content, int numLinesPerChunk) {
        String[] lines = content.split("\n");
        List<String> chunks = new ArrayList<>();
        for (int i = 0; i < lines.length; i += numLinesPerChunk) {
            chunks.add(String.join("\n", Arrays.copyOfRange(lines, i, Math.min(lines.length, i + numLinesPerChunk))));
        }
        return chunks;
    }

    // Worker function: Processes a chunk and excludes stop words
    private static Map<String, Integer> workerFunction(String chunk, Set<String> stopwords) {
        Map<String, Integer> wordCounts = new HashMap<>();
        Matcher matcher = Pattern.compile("[a-z]{2,}").matcher(chunk.toLowerCase());
        while (matcher.find()) {
            String word = matcher.group();
            if (!stopwords.contains(word)) { // Exclude stop words
                wordCounts.put(word, wordCounts.getOrDefault(word, 0) + 1);
            }
        }
        return wordCounts;
    }

    // Regroup function: Redistribute words into 5 groups based on their starting letter
    private static Map<String, Map<String, Integer>> regroup(List<Map<String, Integer>> intermediateResults) {
        Map<String, Map<String, Integer>> groups = new HashMap<>();
        for (String groupKey : Arrays.asList("a-e", "f-j", "k-o", "p-t", "u-z")) {
            groups.put(groupKey, new HashMap<>());
        }

        for (Map<String, Integer> result : intermediateResults) {
            for (Map.Entry<String, Integer> entry : result.entrySet()) {
                String word = entry.getKey();
                int count = entry.getValue();
                String groupKey = getGroupKey(word);
                groups.get(groupKey).merge(word, count, Integer::sum);
            }
        }
        return groups;
    }

    // Determine group based on word's first letter
    private static String getGroupKey(String word) {
        char first = word.charAt(0);
        if (first >= 'a' && first <= 'e') return "a-e";
        if (first >= 'f' && first <= 'j') return "f-j";
        if (first >= 'k' && first <= 'o') return "k-o";
        if (first >= 'p' && first <= 't') return "p-t";
        return "u-z";
    }

    // Reduce function: Combines word counts
    private static Map<String, Integer> reduce(Map<String, Integer> groupedData) {
        return new HashMap<>(groupedData); // Already grouped, just return it
    }

    // Merge results from two maps
    private static void mergeResults(Map<String, Integer> finalResult, Map<String, Integer> partialResult) {
        for (Map.Entry<String, Integer> entry : partialResult.entrySet()) {
            finalResult.merge(entry.getKey(), entry.getValue(), Integer::sum);
        }
    }
}
