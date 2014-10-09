// BogValidator.cpp contains the definitions of the functions declared in the
// class BogValidator
// This class allows the user to check if a given word or a given set of words
// exist in the Boggle Board.
//
//Created by: Gabrielle Cella
//Created on: 20 April 2013
//Last Modified: 30 April 2013

#include <iostream>
#include <string>
#include <cctype>
#include "BogBoard.h"
#include "BogValidator.h"

const int INIT_SIZE = 5;	//initial size of words dynamic array
const string SENTINEL = ".";	//sentinel value to end reading in dictionary

//default constructor
BogValidator::BogValidator()
{
    words = new string [INIT_SIZE];	//declare words dynamic array
    wordCount = 0;
    wordCapacity = INIT_SIZE;
}

//public function
//boolean
//reads in and stores the dictionary
bool BogValidator::readDict()
{
    string input;

    while(1){
	cin >> input;
	if(input == SENTINEL)	//sentinel = "."
	    break;

	//NB. words are stored in the dictionary without the QU to make
	//processes in the board easier
	if(!board.dictInsert(removeQU(input)))
	    return false;
    }
    return true;
}

//private function
//returns a string with all of the Us after Qs deleted
string BogValidator::removeQU(string s)
{
    string rvalue = "";
    for(unsigned i = 0; i < s.length(); i++){
	rvalue+=s[i];
	if(s[i] == 'Q' || s[i] == 'q')	//if it's Q, the following letter
	    i++;			//will be U, so do an extra update i++
    }
    return rvalue;
}

//public function
//boolean
//read in the board dimensions, and the letters in the board
bool BogValidator::readBoard()
{
    string rowletters;

    cin >> row >> col;

    board.setBoards(row, col);	//function declares board member of BogBoard

    for(unsigned i = 0; i < row; i++){
	cin >> rowletters;

	if(rowletters.length() != col)	//returns false if the wrong number of
	    return false;		//letters per row is inserted

	//this loop ensures that all rowletters are uppercase
	for(unsigned j = 0; j < rowletters.length(); j++)
	    rowletters[j] = toupper(rowletters[j]);

	board.boardInsertByRow(i, rowletters);	//insert the rowletters
    }						//in their appropriate row

    return true;
}

//public function
//bool
//checks to make sure a single string is valid
bool BogValidator::isValid(string s)
{
    if(s.length() < 3)	//don't bother making it go through the function
	return false;
    else
	return (board.isInDict(removeQU(s)) && board.isInBoard(removeQU(s)));
}

//public function
//void
//reads in a list of user words and prints whether or not they are valid
void BogValidator::checkWords()
{
    string temp;

    while(cin >> temp){			//read in all remaining words in file
	if(wordCount == wordCapacity)
	    expandWords();

	//ensure that all words are uppercase
	for(unsigned i = 0; i < temp.length(); i++)
	    temp[i] = toupper(temp[i]);

	words[wordCount] = temp;
	wordCount++;
    }

    //this loop prints the report for each word
    for(int i = 0; i < wordCount; i++){
	if(!alreadyUsed(words[i], i) && isValid(words[i]))
	    cout << "OK " << words[i] << endl;
	else
	    cout << "NO " << words[i] << endl;
    }
}

//private function
//boolean
//that goes through array until the current index and returns true if the word
//is in the array until then. If this is the case, the word has already been 
//checked and cannot be counted twice
bool BogValidator::alreadyUsed(string s, int pos)
{
    for(int i = 0; i < pos; i++)
	if(words[i] == s)
	    return true;
    return false;
}

//private function
//void
//expand dynamic array of strings that holds user words to be checked
void BogValidator::expandWords()
{
    string* temp = new string [wordCapacity*2];

    for(int i = 0; i < wordCount; i++){
	temp[i] = words[i];
    }
    wordCapacity*=2;

    delete [] words;
    words = temp;
}

//destructor
BogValidator::~BogValidator()
{
    delete [] words;
}
