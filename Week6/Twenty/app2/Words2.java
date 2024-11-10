package Twenty.app2;

import Twenty.framework.WordExtractor;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

public class Words2 implements WordExtractor {
    @Override
    public List<String> extractWords(String pathToFile) {
        List<String> words = new ArrayList<>();
        // // Step 1: Read stop words and store them in a Set
        // Set<String> stopWords = new HashSet<>();
        // try (BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream("../../stop_words.txt"), StandardCharsets.UTF_8))) {
        //     String line;
        //     while ((line = reader.readLine()) != null) {
        //         stopWords.add(line.trim().toLowerCase());  // Convert stop words to lowercase
        //     }
        // }catch(IOException e){
        //     e.printStackTrace();
        // }

        String content = "";
        try {
            content = new String(Files.readAllBytes(Paths.get((String) pathToFile)));
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        words = Arrays.asList(content.replaceAll("[\\W_]+", " ").toLowerCase().split(" "));
        String stopWordsContent = "";
        try {
            stopWordsContent = new String(Files.readAllBytes(Paths.get("../../stop_words.txt")));
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        Set<String> stopWords = new HashSet<>(Arrays.asList(stopWordsContent.split(",")));
        stopWords.addAll("abcdefghijklmnopqrstuvwxyz".chars().mapToObj(c -> String.valueOf((char) c)).collect(Collectors.toSet()));

        // Step 3: Remove stop words from the list
        List<String> filteredData = new ArrayList<>();
        for (String word : words) {
            if (!stopWords.contains(word)) {  // Add to filtered list if not a stop word
                filteredData.add(word);
            }
        }

        return filteredData;  

    }
}
