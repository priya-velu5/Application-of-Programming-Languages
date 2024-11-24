package Week8;

import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.regex.*;
import java.util.stream.*;


public class TwentyNine {
    public static void main(String[] args) throws InterruptedException {
        if (args.length < 1) {
            System.err.println("Please provide a file path as an argument.");
            return;
        }

        // Instantiate objects
        WordFrequencyManager wordFreqManager = new WordFrequencyManager();
        StopWordManager stopWordManager = new StopWordManager();
        DataStorageManager storageManager = new DataStorageManager();
        WordFrequencyController wfController = new WordFrequencyController();

        // Initialize objects
        wordFreqManager.start();
        stopWordManager.start();
        storageManager.start();
        wfController.start();

        stopWordManager.postMessage(Arrays.asList("init", wordFreqManager));
        storageManager.postMessage(Arrays.asList("init", args[0], stopWordManager));
        wfController.postMessage(Arrays.asList("run", storageManager));
        
        // Send "die" messages to all objects
        // wordFreqManager.postMessage(Arrays.asList("die"));
        // stopWordManager.postMessage(Arrays.asList("die"));
        // storageManager.postMessage(Arrays.asList("die"));
        

        // Wait for objects to finish
        wordFreqManager.joinObject();
        stopWordManager.joinObject();
        storageManager.joinObject();
        wfController.joinObject();

    }
}
class ActiveWFObject extends Thread {
    private final BlockingQueue<List<Object>> queue = new LinkedBlockingQueue<>();
    private volatile boolean stopMe = false;

    public void postMessage(List<Object> message) {
        queue.add(message);
    }

    public void stopObject() {
        stopMe = true;
    }

    public void joinObject() throws InterruptedException {
        this.join();
    }

    @Override
    public void run() {
        while (!stopMe || !queue.isEmpty()) {
            try {
                List<Object> message = queue.poll(100, TimeUnit.MILLISECONDS);
                if (message != null) {
                    dispatch(message);
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }

    protected void dispatch(List<Object> message) {
        if ("die".equals(message.get(0))) {
            stopObject();
        } else {
            throw new UnsupportedOperationException("Dispatch not implemented");
        }
    }
}

class DataStorageManager extends ActiveWFObject {
    private String data = "";
    private StopWordManager stopWordManager;

    @Override
    protected void dispatch(List<Object> message) {
        String command = (String) message.get(0);
        if ("init".equals(command)) {
            init((String) message.get(1), (StopWordManager) message.get(2));
        } else if ("send_word_freqs".equals(command)) {
            sendWords((WordFrequencyController) message.get(1));
        }else if ("die".equals(command)) {
            // Stop this actor and notify StopWordManager to stop
            stopObject();
            stopWordManager.postMessage(Arrays.asList("die"));
        }
    }

    private void init(String filePath, StopWordManager stopWordManager) {
        this.stopWordManager = stopWordManager;
        try {
            data = Files.readString(Path.of(filePath)).toLowerCase();
            data = data.replaceAll("[\\W_]+", " ");
        } catch (IOException e) {
            throw new RuntimeException("Error reading file: " + filePath, e);
        }
    }

    private void sendWords(WordFrequencyController recipient) {
        for (String word : data.split("\\s+")) {
            stopWordManager.postMessage(Arrays.asList("filter", word, recipient));
        }
        stopWordManager.postMessage(Arrays.asList("top25", recipient));
    }
}

class StopWordManager extends ActiveWFObject {
    private Set<String> stopWords = new HashSet<>();
    private WordFrequencyManager wordFrequencyManager;

    @Override
    protected void dispatch(List<Object> message) {
        String command = (String) message.get(0);
        if ("init".equals(command)) {
            init((WordFrequencyManager) message.get(1));
        } else if ("filter".equals(command)) {
            filter((String) message.get(1), (WordFrequencyController) message.get(2));
        } else if ("top25".equals(command)) {
            wordFrequencyManager.postMessage(message);
        } else if ("die".equals(command)) {
            // Stop this actor and notify WordFrequencyManager to stop
            stopObject();
            wordFrequencyManager.postMessage(Arrays.asList("die"));
        }
    }

    private void init(WordFrequencyManager wordFrequencyManager) {
        this.wordFrequencyManager = wordFrequencyManager;
        try {
            String content = Files.readString(Path.of("../stop_words.txt"));
            stopWords.addAll(Arrays.asList(content.split(",")));
            for (char c = 'a'; c <= 'z'; c++) {
                stopWords.add(String.valueOf(c));
            }
        } catch (IOException e) {
            throw new RuntimeException("Error reading stop words file", e);
        }
    }

    private void filter(String word, WordFrequencyController recipient) {
        if (!stopWords.contains(word)) {
            wordFrequencyManager.postMessage(Arrays.asList("word", word));
        }
    }
}

class WordFrequencyManager extends ActiveWFObject {
    private final Map<String, Integer> wordFreqs = new HashMap<>();

    @Override
    protected void dispatch(List<Object> message) {
        String command = (String) message.get(0);
        if ("word".equals(command)) {
            incrementCount((String) message.get(1));
        } else if ("top25".equals(command)) {
            sendTop25((WordFrequencyController) message.get(1));
        } else if ("die".equals(command)) {
            // Stop this actor
            stopObject();
        }
    }

    private void incrementCount(String word) {
        wordFreqs.put(word, wordFreqs.getOrDefault(word, 0) + 1);
    }

    private void sendTop25(WordFrequencyController recipient) {
        List<Map.Entry<String, Integer>> sortedFreqs = wordFreqs.entrySet().stream()
                .sorted((a, b) -> b.getValue().compareTo(a.getValue()))
                .limit(25)
                .collect(Collectors.toList());
        recipient.postMessage(Arrays.asList("top25", sortedFreqs));
    }
}

class WordFrequencyController extends ActiveWFObject {
    private DataStorageManager storageManager;

    @Override
    protected void dispatch(List<Object> message) {
        String command = (String) message.get(0);
        if ("run".equals(command)) {
            runCommand((DataStorageManager) message.get(1));
        } else if ("top25".equals(command)) {
            display((List<Map.Entry<String, Integer>>) message.get(1));
        }
    }

    private void runCommand(DataStorageManager storageManager) {
        this.storageManager = storageManager;
        storageManager.postMessage(Arrays.asList("send_word_freqs", this));
    }

    private void display(List<Map.Entry<String, Integer>> wordFreqs) {
        for (Map.Entry<String, Integer> entry : wordFreqs) {
            System.out.println(entry.getKey() + " - " + entry.getValue());
        }
        stopObject(); // Stop WordFrequencyController itself
        storageManager.postMessage(Arrays.asList("die"));
        
    }
}
