package Twenty.framework;

import java.util.List;
import java.util.Map;

public interface FrequencyCounter {
    Map<String, Integer> top25(List<String> words);
}