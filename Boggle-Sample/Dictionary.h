// dictionary.h defines the structure for the object Dictionary.
// A Dictionary is a type of Trie that stores words, which gives users the
// option to insert words, check to see if a word is in the dictionary, and
// check to see if a prefix is in a word in the dictionary.
//
// Created by: Gabrielle Cella
// Created on: 3/26/13
// Last modified: 4/2/13

#ifndef DICT_H
#define DICT_H

#include <string>
#include <cctype>
#include "trienode.h"
using namespace std;

class Dictionary
{
public:

    //default constructor
    Dictionary();

    //insert a string into the dictionary
    bool insert(string s);

    //test a string to see if it is a word in the dictionary
    bool isWord(string s);

    //test a string to see if it is a prefix in the dictionary
    bool isPrefix(string s);

private:

    TrieNode* root;

    /*FUNCTIONS*/

    //insert function that uses recursion to put a string in the trie
    bool putInTree(TrieNode* r, string s);

    //returns false if there are alphabetical characters somewhere in the strng
    bool noAlpha(string s);

    //boolean function that returns true if it can scale to the end of a string
    //in a trie
    bool scaleToEnd(TrieNode* r, string s);

    //boolean function that returns true if it can scale to the end of a string
    //in a trie and that word is considered a word
    bool wordAtEnd(TrieNode* r, string s);

    //converts a char to its corresponding integer value (e.g. a to 0, b to 1)
    int charToInt(char c);
};


#endif
