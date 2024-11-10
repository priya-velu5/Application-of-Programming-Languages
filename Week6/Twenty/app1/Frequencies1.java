package Twenty.app1;
import Twenty.framework.FrequencyCounter;
import java.util.*;
import java.util.stream.Collectors;

public class Frequencies1 implements FrequencyCounter {
    @Override
    public Map<String, Integer> top25(List<String> words) {
        Map<String, Integer> wordFreqs = new HashMap<>();
        for (String word : words) {
            wordFreqs.put(word, wordFreqs.getOrDefault(word, 0) + 1);
        }
        return wordFreqs.entrySet().stream()
                .sorted(Map.Entry.<String, Integer>comparingByValue().reversed())
                .limit(25)
                .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue,
                        (e1, e2) -> e1, LinkedHashMap::new));
    }
}
