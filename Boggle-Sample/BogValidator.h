// BogValidator.h contains the declaration of the class BogValidator.
// This class allows the user to check if a given word or a given set of words 
// exists in the boggle board.
//
//Created by: Gabrielle Cella
//Created on: 20 April 2013
//Last Modified: 30 April 2013

#ifndef B_VALID
#define B_VALID

#include <iostream>
#include "BogBoard.h"
using namespace std;

class BogValidator
{
public:

    //default constructor
    BogValidator();

    //destructor
    ~BogValidator();

    //reads in the words for the boggle dictionary
    bool readDict();

    //reads in the boggle board
    bool readBoard();

    //returns true if a single string is a valid word and is in the BogBoard
    bool isValid(string s);

    //reads in the list of words to be checked and prints a report based on 
    //their validity
    void checkWords();

private:

    //returns a string with all Us after Qs deleted
    string removeQU(string s);

    //determines if a string has already been checked--cannot grant OK to 
    //the same word twice
    bool alreadyUsed(string s, int pos);

    //the data below pertian to the board and its demensions
    unsigned row;
    unsigned col;
    BogBoard board;

    //the data below pertian to the dynamic array of words to be checked
    string* words;
    int wordCount;
    int wordCapacity;
    //this is a function to expand the dynamic array of words to be checked
    void expandWords();
};

#endif
