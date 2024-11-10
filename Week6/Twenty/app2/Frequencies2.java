package Twenty.app2;


import Twenty.framework.FrequencyCounter;
import java.util.*;
import java.util.stream.Collectors;

public class Frequencies2 implements FrequencyCounter {
    @Override
    public Map<String, Integer> top25(List<String> words) {
        return words.stream()
                .collect(Collectors.groupingBy(word -> word, Collectors.counting()))
                .entrySet().stream()
                .sorted(Map.Entry.<String, Long>comparingByValue().reversed())
                .limit(25)
                .collect(Collectors.toMap(Map.Entry::getKey, entry -> entry.getValue().intValue(),
                        (e1, e2) -> e1, LinkedHashMap::new));
    }
}
