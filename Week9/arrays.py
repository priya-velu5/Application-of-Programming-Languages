import sys, string
import numpy as np


leet_dict = {
    'A': '4',
    'B': '8',
    'C': '{',
    'D': '[)',
    'E': '3',
    'F': '|=',
    'G': '6',
    'H': '|-|',
    'I': '1',
    'J': '_|',
    'K': '|<',
    'L': '|_',
    'M': '|\/|',
    'N': '|\|',
    'O': '0',
    'P': '|>',
    'Q': '0,',
    'R': '|2',
    'S': '5',
    'T': '7',
    'U': '|_|',
    'V': '\/',
    'W': '\/\/',
    'X': '><',
    'Y': '`/',
    'Z': '2'
    }

def input_chars(is_leet=False):
    
    # Example input: "Hello  World!" 
    characters = np.array([' ']+list(open(sys.argv[1]).read())+[' '])
    # Result: array([' ', 'H', 'e', 'l', 'l', 'o', ' ', ' ', 
    #           'W', 'o', 'r', 'l', 'd', '!', ' '], dtype='<U1')

    # Normalize
    characters[~np.char.isalpha(characters)] = ' '
    if(is_leet):
        characters = np.char.upper(characters)
    else:
        characters = np.char.lower(characters)
    return characters

def leet_speak(characters):
    ## do the leet speak conversion 
    ##Note: iteration is done over the dictionary keys not the array. 
    for k, v in leet_dict.items():
        characters = np.char.replace(characters, k, v)
    return characters

def process_char(characters):
    ### Split the words by finding the indices of spaces
    sp = np.where(characters == ' ')

    # A little trick: let's double each index, and then take pairs
    sp2 = np.repeat(sp, 2)
    # Get the pairs as a 2D matrix, skip the first and the last
    w_ranges = np.reshape(sp2[1:-1], (-1, 2))

    # Remove the indexing to the spaces themselves
    w_ranges = w_ranges[np.where(w_ranges[:, 1] - w_ranges[:, 0] > 2)]

    # Voila! Words are in between spaces, given as pairs of indices
    words = list(map(lambda r: characters[r[0]:r[1]], w_ranges))
    swords = np.array(list(map(lambda w: ''.join(w).strip(), words)))
    return swords

## create a list of two-grams and count the top 5 occurances. 
def count_2grams(ns_words):
    first_words = ns_words[:-1]  # All words except the last
    second_words = ns_words[1:]  # All words except the first

    # Combine into 2-grams
    two_grams = np.char.add(first_words, ' ')  # Add a space between words
    two_grams = np.char.add(two_grams, second_words)

    ### Finally, count the word occurrences
    uniq, counts = np.unique(two_grams, axis=0, return_counts=True)
    wf_sorted = sorted(zip(uniq, counts), key=lambda t: t[1], reverse=True)

    for w, c in wf_sorted[:5]:
        print(w, '-', c)

## Main

## Normal processing
characters = input_chars()
swords = process_char(characters)
stop_words = np.array(list(set(open('../stop_words.txt').read().split(','))))
ns_words = swords[~np.isin(swords, stop_words)]
count_2grams(ns_words)

## Leet speak processing
print("\nIn Leet Speak: \n")
print("processing...")
characters = input_chars(is_leet=True)
characters_l = leet_speak(characters)
swords_l = process_char(characters_l)
stop_words_l = np.char.upper(stop_words)
stop_words_l = leet_speak(stop_words_l)
ns_words_l = swords_l[~np.isin(swords_l, stop_words_l)]

count_2grams(ns_words_l)
