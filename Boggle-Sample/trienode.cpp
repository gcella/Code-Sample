// trienode.cpp contains the definitions for the functions declared in node.h.
// This class defines the object TrieNode, which contains a static array of 
// pointers to TrieNodes and a boolean that determines if the TrieNode marks
// the end of a word.
//
// Created by: Gabrielle Cella
// Created on: 3/26/13
// Last modified: 4/2/13

#include <string>
#include "trienode.h"
using namespace std;

//default constructor
//sets isWord to false and sets all pointers in the children array to NULL
TrieNode::TrieNode()
{
    isWord = false;
    for(int i = 0; i < ALPHABET; i++){
	children[i] = NULL;
    }
}

//makes a node in the children array at the given index
void TrieNode::makeNodeAt(int index)
{
    children[index] = new TrieNode;
}

//returns a pointer to the node in the children array at the given index
TrieNode* TrieNode::pointToChild(int index)
{
    return children[index];
}

//sets private member isWord
void TrieNode::set_isWord(bool value)
{
    isWord = value;
}

//gets private member isWord
bool TrieNode::get_isWord()
{
    return isWord;
}
