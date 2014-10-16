// dictionary.cpp contains the definitions for the functions declared in
// dictionary.h. This class defines the type Dictionary, which is a Trie that
// stores words.
// Users have the option to store words in the dictionary, check to see if 
// a word is in the dictoinary, and check to see a prefix can be found in a
// word in the dictionary.
//
// Created by: Gabrielle Cella
// Created on: 3/26/13
// Last modified: 4/2/13

#include <string>
#include <cctype>
#include "Dictionary.h"
using namespace std;

//default constructor
Dictionary::Dictionary()
{
    root = NULL;
}

//public insertion function
//arguments: string
//returns: bool -- did the insertion work correctly
//calls private func putInTree(). These functions must be separated because 
//putInTree() takes in TrieNode as a parameter and is initially passed the root
bool Dictionary::insert(string s)
{
    if(root == NULL)		//being tree
	root = new TrieNode;

    return putInTree(root, s);
}

//private insert function
//arguments: TrieNode* r, string s
//returns: bool -- did the insertion work correctly
//recursive function
bool Dictionary::putInTree(TrieNode* r, string s)
{
    if(r == NULL)		//if we run out of room, new returns NULL
	return false;		//and we're given NULL to work with.

    int len = s.length();

    if(noAlpha(s)){		//done or empty string is inserted
	r->set_isWord(true);
	return true;
    }

    int i = 0;
    while(i< len && !isalpha(s[i]))	//scale to first real letter
	i++;

    char current = tolower(s[i]);	//because capitals (store first letter)
    string truncated = s.erase(0, i+1);		//truncate stored letter
						//and non-letters before it

    if(r->pointToChild(charToInt(current)) == NULL)
	r->makeNodeAt(charToInt(current));		//make node at letter
    return putInTree(r->pointToChild(charToInt(current)), truncated);
}

//private boolean function--are there no alphabetical characters in a string?
//arguments: string s
//returns: bool
bool Dictionary::noAlpha(string s)
{
    int len = s.length();
    int i = 0;

    while(i < len){
	if(isalpha(s[i]))
	    return false;	//return false if you find an alpha!
	i++;
    }
    return true;	//either len == 0, or isalpha() failed for every char
}

//private function
//arguments: char
//retunrs: corresponding int
int Dictionary::charToInt(char c)
{
    return (c - 'a');
}

//public prefix func -- is the input string the prefix to a word in the tree?
//arguments: string
//returns: bool
//uses an aux private function that works recursively (TrieNode parameter)
bool Dictionary::isPrefix(string s)
{
    if(root == NULL)		//no prefixes in empty trees
	return false;
    else
	return scaleToEnd(root, s);
}

//private prefix func -- can we use our tree to scale to the end of the word?
//arguments: TrieNode, string
//returns: bool
//uses recursion to determine if the given string can be found in the tree
bool Dictionary::scaleToEnd(TrieNode* r, string s)
{
    int len = s.length();

    if(noAlpha(s))		//either empty string or we finished scaling
	return true;

    int i = 0;
    while(i < len && !isalpha(s[i]))
	i++;

    char current = tolower(s[i]);
    string truncated = s.erase(0, i+1);

    if(r->pointToChild(charToInt(current)) != NULL)
	return scaleToEnd(r->pointToChild(charToInt(current)), truncated);
    else
	return false;
}

//public isWord func -- is the input string a word in the tree?
//arguments: string
//returns: bool
//uses aux private function that works recursively (TrieNode parameter)
bool Dictionary::isWord(string s)
{
    if(root == NULL)		//no words in empty trees
	return false;
    else
	return wordAtEnd(root, s);
}

//private isWord func -- can we use our tree to scale to the end of the word,
//and if so is that last node considered a word? (isWord == true) at last node?
//arguments: TrieNode, string
//returns: bool
//uses recursion to scale to the end of the word, and when it reaches there,
//it evaluates bool based on final node's isWord member
bool Dictionary::wordAtEnd(TrieNode* r, string s)
{
    int len = s.length();

    if(noAlpha(s)){		//either empty string or we finished scaling
	if(r->get_isWord())
	    return true;	//truth value stored at node for final char
	else
	    return false;
    }

    int i = 0;
    while(i < len && !isalpha(s[i]))
	i++;

    char current = tolower(s[i]);
    string truncated = s.erase(0, i+1);

    if(r->pointToChild(charToInt(current)) != NULL)
	return wordAtEnd(r->pointToChild(charToInt(current)), truncated);
    else
	return false;
}
