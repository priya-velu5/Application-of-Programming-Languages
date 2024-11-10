package Twenty.app1;

import Twenty.framework.WordExtractor;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

public class Words1 implements WordExtractor {
    @Override
    public List<String> extractWords(String pathToFile) {
        List<String> words = new ArrayList<>();
        try {
            String content = new String(Files.readAllBytes(Paths.get((String) pathToFile)));
            words = Arrays.asList(content.replaceAll("[\\W_]+", " ").toLowerCase().split(" "));
            String stopWordsContent = new String(Files.readAllBytes(Paths.get("../../stop_words.txt")));
            Set<String> stopWords = new HashSet<>(Arrays.asList(stopWordsContent.split(",")));
            stopWords.addAll("abcdefghijklmnopqrstuvwxyz".chars().mapToObj(c -> String.valueOf((char) c)).collect(Collectors.toSet()));
            return words.stream()
            .filter(word -> !stopWords.contains(word))
            .collect(Collectors.toList());
        } catch (IOException e) {
            e.printStackTrace();
        }
        return words;
    }
}
