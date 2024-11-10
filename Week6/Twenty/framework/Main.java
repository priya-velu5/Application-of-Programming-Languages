package Twenty.framework;
import java.io.FileInputStream;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.List;
import java.util.Map;
import java.util.Properties;

public class Main {
    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java -cp framework.jar:app1.jar Main <file_path>");
            return;
        }
        String filePath = args[0];

        try {
            Properties config = new Properties();
            config.load(new FileInputStream("config.properties"));

            String wordsClass = config.getProperty("words_plugin");
            String frequenciesClass = config.getProperty("frequencies_plugin");

            WordExtractor extractor = (WordExtractor) Class.forName(wordsClass).getDeclaredConstructor().newInstance();
            FrequencyCounter counter = (FrequencyCounter) Class.forName(frequenciesClass).getDeclaredConstructor().newInstance();

            List<String> words = extractor.extractWords(filePath);
            Map<String, Integer> wordFreqs = counter.top25(words);

            wordFreqs.forEach((word, freq) -> System.out.println(word + " - " + freq));

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
