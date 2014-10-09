// BogBoard.h contains the declaration of the class BogBoard. BogBoard is a
// self-sufficient board that is used to house the board with which the user
// is playing boggle. This board has the capacity to solve itself and look
// to see if it contains certain words. In order to do so, the board must 
// occasionally confer with the dictionary of approved word, so it contains 
// an instance of the class Dictionary, as well. 
// This board is used by both the BogSolver class and the BogValidator class.
//
//Created by: Gabrielle Cella
//Created on: 20 April 2013
//Last Modified: 30 April 2013

#ifndef BOGBOARD
#define BOGBOARD

#include "BogWordList.h"
#include "Dictionary.h"
#include <iostream>
#include <string>
using namespace std;

//LNode is used by the functions that go to solve the board. An LNode is a
//linked list of BogLetts, which is useful as an easy-to-pass parameter
//and useful because it contains all of the data for a BogLett
struct LNode{
    BogLett data;
    LNode* next;
};

/*CLASS DEFINITION*/

class BogBoard
{
public:
    //default constructor
    BogBoard();

    //destructor
    ~BogBoard();

    //declares a board under the private data member BogLett** board, based on 
    //the user's input number of rows and columns
    //initializes the indicies of each BogLett based on location in board
    void setBoards(int rowNum, int colNum);

    //inserts the charaters in the board as the user inputs each row of chars
    void boardInsertByRow(int rowNum, string rowletters);

    //inserts into dictionary of accepted boggle words
    bool dictInsert(string s);

    //public function that determines if a word can be found in the board by 
    //calling the helperfunction bool isWordHere()
    bool isInBoard(string s);

    //runs the lookup function in the dictionary on an input word
    bool isInDict(string s);

    //public function that solves the board by calling the helperfunction
    //goThruBoard(). goThruBoard() will build the BogWordList* boardWords
    //and this function returns that list at the end.
    BogWordList* solve();

    //function that creates and returns a deep copy of a given BogWordList
    BogWordList* getWordListCopy(BogWordList* original);

private:

    /*SOLUTION FUNCTIONS*/

    //private helperfunction for solve()
    //uses recursion to go through the board and test every possible letter
    //combination to see if it is an acceptable word. if so, it adds the word
    //to BogWordList* boardWords (the solved list of words in the board)
    void goThruBoard(int rNum, int cNum, LNode* list);

    //reads in a linked list of bogLetts and returns the string spelled from
    //its chars
    string listToString(LNode* s);

    //looks through a linked list of bogLetts and determines if it contains a
    //'Q'
    bool qExists(LNode* s);

    //private helper functoin for isInBoard()
    //uses recursion to go through the board and test if the string s is there
    bool isWordHere(int rNum, int cNum, const string& s, unsigned sIndex);

    /*DICTIONARIES*/

    //there are two dictionaries used in this program
    //dict is a dictionary of acceptable solution words
    Dictionary dict;

    //solvedDict is a dictionary of all words that have been added to the
    //solution list boardWords
    Dictionary solvedDict;

    /*BOARD*/

    //the private data below is the board of BogLetts established by the 
    //program and its demensions
    int row;
    int col;
    BogLett** board;

    //spotsUsed is a 2D array of bools. This array is initially marked all   
    //false and is checked to see if a letter has already been used in a 
    //word being developed by isWordHere() and goThruBoard() 
    bool** spotsUsed;

    //this function declares spotsUsed to the same dimensions as the BogBoard
    //when it declares the BogBoard
    void declareSpotsUsed();

    //function initializes all elements in spotsUsed as false
    void initializeAllFalse();

    /*BOGWORDLIST*/

    //list of words found in the board when the solve function gathers them
    BogWordList* boardWords;

    //capacity of BogWord element of BogWordList
    int wordListCapac;

    //adds a linked list of BogLetts to the BogWordList
    void addToWordList(LNode* list);

    //expands the BogWord array in BogWordList
    void expandWordList();

    //private helperfunction to the destructor that deletes the dynamic memory
    //in boardWords
    void deleteBoardWords();
};

#endif
