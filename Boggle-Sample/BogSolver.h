// BogSolver.h contains the declaration of the class BogSolver. BogSolver is a 
// method for solving boggle board. It produces a list of all acceptable words
// found within the boggle board and allows the user to print or otherwise
// obtain information about the set of solved words. 
//
//Created by: Gabrielle Cella
//Created on: 20 April 2013
//Last Modified: 30 April 2013

#ifndef BOGSOLV
#define BOGSOLV

#include <iostream>
#include "BogWordList.h"
#include "BogBoard.h"

class BogSolver
{
public:

    //default constructor
    BogSolver();

    //destructor
    ~BogSolver();

    //reads in the words of the dictionary
    bool readDict();

    //reads in the boggle board itself
    bool readBoard();

    //commands the board to solve itself
    bool solve();

    //returns the number of words that are in the solution set
    int numWords();

    //returns the number of words of a specified length in solution set
    int numWords(int len);

    //returns a deep copy of wordlist--the BogWordList of solved words
    BogWordList* getWords();

    //returns a BogWordList of solved words that are of the specified length
    BogWordList* getWords(int len);

    //prints the words in wordlist -- in HBF
    void printWords();

    //prints the words of a specified length in the wordlist -- in HBF
    void printWords(int len);

    //prints the words in the wordlist
    void listWords();

    //prints teh words of a specified length in the wordlist
    void listWords(int len);

private:

    //the data below pertain to the demensions of the boggle board
    unsigned row;
    unsigned col;

    //the boggle board (instance of class BogBoard, which contains the board
    //itself and methods for producing solutions to the board)
    BogBoard board;

    //a list of the solved words that exist in the BogBoard
    BogWordList* wordlist;

    //takes in a BogWord and a length and determines if the word is of the 
    //specified length by taking into account whether or not it contains a Q
    bool appropLen(BogWord* w, int len);

    //returns a string without any Us after Qs
    string removeQU(string s);

    //looks at a BogWord and returns true if there is a Q in it
    bool QinWord(BogWord* w);

    //private helperfunction for the destructor that deletes the BogWordList
    void deletewordlist();
};

#endif
