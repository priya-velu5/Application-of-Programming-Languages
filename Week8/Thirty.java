package Week8;
import java.io.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.regex.*;
import java.util.stream.Collectors;

public class Thirty {
    // Two data spaces
    private static final BlockingQueue<String> word_space = new LinkedBlockingQueue<>();
    private static final BlockingQueue<Map<String, Integer>> freq_space = new LinkedBlockingQueue<>();

    // Set of stopwords
    private static final Set<String> stopwords = new HashSet<>();

    public static void main(String[] args) throws InterruptedException, IOException {
        // Load stop words
        loadStopWords("../stop_words.txt");
        if (args.length < 1) {
            System.out.println("Usage: java Thirty.java <input_file>");
            return;
        }
        // Read input file and populate word_space
        populateWordSpace(args[0]);

        // Create and start worker threads
        List<Thread> workers = new ArrayList<>();
        for (int i = 0; i < 5; i++) {
            Thread worker = new Thread(Thirty::processWords);
            workers.add(worker);
            worker.start();
        }

        // Wait for worker threads to finish
        for (Thread worker : workers) {
            worker.join();
        }

        // Merge partial results from freq_space
        Map<String, Integer> wordFreqs = mergeFrequencies();

        // Print top 25 frequencies
        wordFreqs.entrySet().stream()
                .sorted((a, b) -> b.getValue().compareTo(a.getValue()))
                .limit(25)
                .forEach(entry -> System.out.println(entry.getKey() + " - " + entry.getValue()));
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

    // Read input file and populate word_space
    private static void populateWordSpace(String inputFile) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(inputFile))) {
            String content = reader.lines().collect(Collectors.joining(" "));
            Matcher matcher = Pattern.compile("[a-z]{2,}").matcher(content.toLowerCase());
            while (matcher.find()) {
                word_space.put(matcher.group());
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    // Worker function to process words and populate freq_space
    private static void processWords() {
        Map<String, Integer> wordFreqs = new HashMap<>();
        try {
            while (true) {
                String word = word_space.poll(1, TimeUnit.SECONDS); // Wait for words
                if (word == null) break; // No more words to process
                if (!stopwords.contains(word)) {
                    wordFreqs.put(word, wordFreqs.getOrDefault(word, 0) + 1);
                }
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        // Add the results to freq_space
        try {
            freq_space.put(wordFreqs);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    // Merge partial results from freq_space
    private static Map<String, Integer> mergeFrequencies() {
        Map<String, Integer> wordFreqs = new HashMap<>();
        while (!freq_space.isEmpty()) {
            try {
                Map<String, Integer> partialFreqs = freq_space.poll(1, TimeUnit.SECONDS);
                if (partialFreqs == null) break; // No more results
                for (Map.Entry<String, Integer> entry : partialFreqs.entrySet()) {
                    wordFreqs.merge(entry.getKey(), entry.getValue(), Integer::sum);
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        return wordFreqs;
    }
}
