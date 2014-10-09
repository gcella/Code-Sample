// BogSolver.cpp contains the definitions of the functions declared in the
// class BogSolver.
// This class allows the user to solve the board and request different ways to 
// print or otherwise obtain information about the set of solved words.
//
//Created by: Gabrielle Cella
//Created on: 20 April 2013
//Last Modified: 30 April 2013

#include <iostream>
#include <string>
#include <cctype>
#include "BogWordList.h"
#include "BogSolver.h"
#include "BogBoard.h"

const string SENTINEL = ".";	//sentinel that marks the end of the dictionary

//default constructor
BogSolver::BogSolver()
{
    wordlist = NULL;
}

//public function
//boolean
//reads in and stores the dictionary
bool BogSolver::readDict()
{
    string input;

    while(1){
	cin >> input;
	if(input == SENTINEL)		//cin until "."
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
string BogSolver::removeQU(string s)
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
bool BogSolver::readBoard()
{
    string rowletters;

    cin >> row >> col;

    board.setBoards(row, col);	//function declares board member of BogBoard

    unsigned i = 0;
    while(cin >> rowletters){
	if(i >= row)
	    return false;		//return false if they try to insert
					//too many rows
	if(rowletters.length() != col)	//or a row has the wrong number of 
	    return false;		//columns

	//this loop ensures that all rowletters are uppercase
	for(unsigned j = 0; j < rowletters.length(); j++)
	    rowletters[j] = toupper(rowletters[j]);

	board.boardInsertByRow(i, rowletters);	//insert the rowletters
	i++;					//in their appropriate row
    }
    if(i < row-1)
	return false;	//return false if they try to insert too few rows

    return true;
}

//public function
//boolean
//function that commands the board to solve itself by producing a list of 
//words and their locations in the board
bool BogSolver::solve()
{
    wordlist = board.solve();
    return true;
}

/*THE FOLLOWING FUNCTIONS ARE CALLED ONLY AFTER THE BOARD HAS BEEN SOLVED*/

//public function
//int
//returns the number of words in the solution set
int BogSolver::numWords()
{
    if(wordlist == NULL)
	return 0;
    else
	return wordlist->numWords;
}

//public function
//int
//returns the number of words of a specified length that exist in the sol. set
int BogSolver::numWords(int len)
{
    if(wordlist == NULL)
	return 0;

    int rvalue = 0;

    for(int i = 0; i < wordlist->numWords; i++){	//loop counts all
	if(appropLen(&wordlist->words[i], len))		//words of the 
	    rvalue++;					//appropriate length
    }
    return rvalue;
}

//public function
//BogWordList*
//returns a BogWordList of solved words
BogWordList* BogSolver::getWords()
{
    if(wordlist == NULL)
	return NULL;
    else
	return board.getWordListCopy(wordlist);		//return deep copy
}

//public function
//BogWordList*
//returns a BogWordList of solved words of the specified length by creating a
//new list with only those words
BogWordList* BogSolver::getWords(int len)
{
    if(wordlist == NULL)
	return NULL;

    BogWordList* rlist = new BogWordList;     //initialize list to be returned
    rlist->numWords = 0;
    rlist->words = NULL;

    //loop counts num of words of approp length to create capacity
    for(int i = 0; i < wordlist->numWords; i++){
	if(appropLen(&wordlist->words[i], len))
	    rlist->numWords++;
    }

    rlist->words = new BogWord [rlist->numWords];	//initialize words
    int wordCount = 0;

    //loop copies words
    for(int i = 0; i < wordlist->numWords; i++){
	if(appropLen(&wordlist->words[i], len)){
	    rlist->words[wordCount].numLetts = len;
	    rlist->words[wordCount].letts = new BogLett [len];
	    //loop copies BogLetts
	    for(int j = 0; j < len; j++){
		rlist->words[wordCount].letts[j] = wordlist->words[i].letts[j];
	    }
	    wordCount++;
	}
    }

    return rlist;
}

//public function
//void
//prints the words in word list -- in HBF
void BogSolver::printWords()
{
    if(wordlist == NULL)
	return;

    for(int i = 0; i < wordlist->numWords; i++){
	cout << "< ";
	for(int j = 0; j < wordlist->words[i].numLetts; j++){
	    cout << wordlist->words[i].letts[j].c;
	    if(wordlist->words[i].letts[j].c == 'Q')		//Q problemz
		cout << 'U';
	    cout << " " << wordlist->words[i].letts[j].row << " "
		 << wordlist->words[i].letts[j].col << " ";
	}
	cout << ">" << endl;
    }
}

//public function
//void
//prints the words in wordlist of a specified length -- in HBF
void BogSolver::printWords(int len)
{
    if(wordlist == NULL)
	return;

    for(int i = 0; i < wordlist->numWords; i++){
	if(appropLen(&wordlist->words[i], len)){
	    cout << "< ";
	    for(int j = 0; j < wordlist->words[i].numLetts; j++){
		cout << wordlist->words[i].letts[j].c;
		if(wordlist->words[i].letts[j].c == 'Q')	//Q problemz
		    cout << 'U';
		cout << " " << wordlist->words[i].letts[j].row << " "
		     << wordlist->words[i].letts[j].col << " ";
	    }
	    cout << ">" << endl;
	}
    }
}

//public function
//void
//prints the words in wordlist
void BogSolver::listWords()
{
    if(wordlist == NULL)
	return;

    for(int i = 0; i < wordlist->numWords; i++){
	for(int j = 0; j < wordlist->words[i].numLetts; j++){
	    cout << wordlist->words[i].letts[j].c;
	    if(wordlist->words[i].letts[j].c == 'Q')		//Q problemz
		cout << 'U';
	}
	cout << endl;
    }
}

//public function
//void
//prints the words in wordlist of a specified length
void BogSolver::listWords(int len)
{
    if(wordlist == NULL)
	return;

    for(int i = 0; i < wordlist->numWords; i++){
	if(appropLen(&wordlist->words[i], len)){
	    for(int j = 0; j < wordlist->words[i].numLetts; j++){
		cout << wordlist->words[i].letts[j].c;
		if(wordlist->words[i].letts[j].c == 'Q')	//Q problemz
		    cout << 'U';
	    }
	    cout << endl;
	}
    }
}

//private function
//boolean
//determines if a bogWord is of the specified length, by taking into account 
//whether it has a Q (and thus, an implied U)
bool BogSolver::appropLen(BogWord* w, int len)
{
    return (((w->numLetts == len) && !QinWord(w)) || 
	    (w->numLetts == len-1 && QinWord(w)));
}

//private function
//boolean
//that determines if there is a q in a BogWord
bool BogSolver::QinWord(BogWord* w)
{
    for(int i = 0; i < w->numLetts; i++){
	if(w->letts[i].c == 'Q')
	    return true;
    }
    return false;
}

//destructor
BogSolver::~BogSolver()
{
    deletewordlist();
}

//private helper function for destructor
//deletes wordlist
void BogSolver::deletewordlist()
{
    for(int i = 0; i < wordlist->numWords; i++){
	delete [] wordlist->words[i].letts;
    }
    delete [] wordlist->words;
    delete wordlist;
}
