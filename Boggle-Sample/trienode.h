// trienode.h defines the object TrieNode. Each TrieNode has a boolean that 
// determines whether or not it marks the end of a word.
//
// Created by: Gabrielle Cella
// Created on: 3/26/13
// Last modified: 4/2/13

#ifndef NODE_H
#define NODE_H

#include <string>
using namespace std;

const int ALPHABET = 26;	//number of elements in children array

class TrieNode
{
public:

    //default constructor
    TrieNode();

    //makes node at element in children array at given index
    void makeNodeAt(int index);

    //points to element in children array at given index
    TrieNode* pointToChild(int index);

    //sets private member isWord
    void set_isWord(bool value);

    //gets private member isWord
    bool get_isWord();

private:
    bool isWord;
    TrieNode* children[ALPHABET];
};

#endif
