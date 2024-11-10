// This code was modified from https://github.com/crista/exercises-in-programming-style/blob/master/11-things/tf_10.java

import java.io.*;
import java.lang.reflect.*;
import java.util.*;

public class Seventeen {
    /*
     * The main function
     */
    public static void main(String[] args) throws IOException {
        //prompt users for a class name and call a function that prints all the information about the class using reflection.
        //no hardcoded conditionals
        if (args.length == 0) {
            System.out.println("No file path provided.");
            return;
        }
        
        System.out.println("Enter a class to inspect: ");
        Scanner in =  new Scanner(System.in);
        String name = in.nextLine();
        System.out.println("Getting information about " + name);
        
        Class getInfo = null;
        try{
            getInfo = Class.forName(name);
        } catch (ClassNotFoundException e) {
            System.out.println("Class not found.");
            return;
        }
        if (getInfo !=null){
            // get interfaces
            Class[] interfaces = getInfo.getInterfaces();
            for (Class iface: interfaces)
                    System.out.println("Interface name: " + iface.getName());
            // get superclass
            Class superclass = getInfo.getSuperclass();
            if (superclass != null) {
                System.out.println("Superclass name: " + superclass.getName());
            }
            // get methods
            Method[] methods = getInfo.getDeclaredMethods();
            for (Method m: methods){
                System.out.println("Method name: \t"+m);
                System.out.println("Method return type : \t"+m.getReturnType());
                System.out.println("Method parameter count: \t"+m.getParameterCount());
                System.out.println();
                Parameter[] parameters = m.getParameters();
                for(Parameter parameter : parameters) {
                    System.out.println("Method's Parameter : "+parameter);
                }
                System.out.println();
                }

            // get fields
            Field[] fields = getInfo.getDeclaredFields();
            for (Field field : fields) {
                System.out.println("Field name: " + field.getName());
                System.out.println("Field type: " + field.getType());
                System.out.println("Field modifiers: " + Modifier.toString(field.getModifiers()));
                System.out.println();
            }
        }
        
        new WordFrequencyController(args[0]).run();

    }
}

/*
 * The classes
 */

abstract class TFExercise {
    public String getInfo() {
        return this.getClass().getName();
    }
}

class WordFrequencyController extends TFExercise {
    private DataStorageManager storageManager;
    private StopWordManager stopWordManager;
    private WordFrequencyManager wordFreqManager;
    
    public WordFrequencyController(String pathToFile) throws IOException {
        this.storageManager = new DataStorageManager(pathToFile);
        this.stopWordManager = new StopWordManager();
        this.wordFreqManager = new WordFrequencyManager();
    }

    public void run() {
        // invoke the methods of the DataStorageManager, 
        // StopWordManager, and WordFrequencyCounter objects using reflection.
        
        Method getword;
        Method isStop;
        Method increment;
        try {
            getword = storageManager.getClass().getMethod("getWords");
            List<String> words = (List<String>) getword.invoke(storageManager);
            isStop = stopWordManager.getClass().getMethod("isStopWord", String.class); 
            increment = wordFreqManager.getClass().getMethod("incrementCount", String.class);
            
            for (String word : words) {
                boolean isStopWord = (boolean) isStop.invoke(stopWordManager, word);
                if (!isStopWord) {
                    increment.invoke(wordFreqManager ,word);
                }
            }

            Method sortedMethod = wordFreqManager.getClass().getMethod("sorted");
            List<WordFrequencyPair> sortedWords = (List<WordFrequencyPair>) sortedMethod.invoke(wordFreqManager);

            int numWordsPrinted = 0;
            for (WordFrequencyPair pair : sortedWords) {
                System.out.println(pair.getWord() + " - " + pair.getFrequency());
                
                numWordsPrinted++;
                if (numWordsPrinted >= 25) {
                    break;
                }
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    
    }
}

/** Models the contents of the file. */
class DataStorageManager extends TFExercise {
    private List<String> words;

    public DataStorageManager(String pathToFile) throws IOException {
        this.words = new ArrayList<String>();

        Scanner f = new Scanner(new File(pathToFile), "UTF-8");
        try {
            f.useDelimiter("[\\W_]+");
            while (f.hasNext()) {
                this.words.add(f.next().toLowerCase());
            }
        } finally {
            f.close();
        }
    }

    public List<String> getWords() {
        return this.words;
    }

    public String getInfo() {
        return super.getInfo() + ": My major data structure is a " + this.words.getClass().getName();
    }
}

/** Models the stop word filter. */
class StopWordManager extends TFExercise {
    private Set<String> stopWords;

    public StopWordManager() throws IOException {
        this.stopWords = new HashSet<String>();

        Scanner f = new Scanner(new File("../stop_words.txt"), "UTF-8");
        try {
            f.useDelimiter(",");
            while (f.hasNext()) {
                this.stopWords.add(f.next());
            }
        } finally {
            f.close();
        }

        // Add single-letter words
        for (char c = 'a'; c <= 'z'; c++) {
            this.stopWords.add("" + c);
        }
    }

    public boolean isStopWord(String word) {
        return this.stopWords.contains(word);
    }

    public String getInfo() {
        return super.getInfo() + ": My major data structure is a " + this.stopWords.getClass().getName();
    }
}

/** Keeps the word frequency data. */
class WordFrequencyManager extends TFExercise {
    private Map<String, MutableInteger> wordFreqs;

    public WordFrequencyManager() {
        this.wordFreqs = new HashMap<String, MutableInteger>();
    }

    public void incrementCount(String word) {
        MutableInteger count = this.wordFreqs.get(word);
        if (count == null) {
            this.wordFreqs.put(word, new MutableInteger(1));
        } else {
            count.setValue(count.getValue() + 1);
        }
    }

    public List<WordFrequencyPair> sorted() {
        List<WordFrequencyPair> pairs = new ArrayList<WordFrequencyPair>();
        for (Map.Entry<String, MutableInteger> entry : wordFreqs.entrySet()) {
            pairs.add(new WordFrequencyPair(entry.getKey(), entry.getValue().getValue()));
        }
        Collections.sort(pairs);
        Collections.reverse(pairs);
        return pairs;
    }

    public String getInfo() {
        return super.getInfo() + ": My major data structure is a " + this.wordFreqs.getClass().getName();
    }
}

class MutableInteger {
    private int value;

    public MutableInteger(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }

    public void setValue(int value) {
        this.value = value;
    }
}

class WordFrequencyPair implements Comparable<WordFrequencyPair> {
    private String word;
    private int frequency;

    public WordFrequencyPair(String word, int frequency) {
        this.word = word;
        this.frequency = frequency;
    }

    public String getWord() {
        return word;
    }

    public int getFrequency() {
        return frequency;
    }

    public int compareTo(WordFrequencyPair other) {
        return this.frequency - other.frequency;
    }
}