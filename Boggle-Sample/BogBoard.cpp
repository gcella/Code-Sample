// BogBoard.cpp contains the definitions of the functions declared in the 
// class BogBoard. 
// BogBoard is a self-sufficient board that contains all of the data in the 
// user's boggle board and solves itself.
//
//Created by: Gabrielle Cella
//Created on: 20 April 2013
//Last Modified: 30 April 2013

#include <iostream>
#include <string>
#include "BogBoard.h"
#include "Dictionary.h"
using namespace std;

const int INIT_SIZE = 5; 	//initial size of boardWords->words

//default constructor
BogBoard::BogBoard()
{
    board = NULL;
    spotsUsed = NULL;

    boardWords = new BogWordList;
    boardWords->numWords = 0; 
    boardWords->words = new BogWord [INIT_SIZE];
    wordListCapac = INIT_SIZE;
    for(int i = 0; i < wordListCapac; i++){
	boardWords->words[i].letts = NULL;
    }
}

//public
//void
//builds a board based on the user's input number of rows and columns
//calls function to declare spotsUsed 2D array
void BogBoard::setBoards(int rowNum, int colNum)
{
    row = rowNum;
    col = colNum;
    board = new BogLett* [row];		//initialize dynamic 2D BogLett array

    for(int i = 0; i < row; i++){
	board[i] = new BogLett [col];
    }

    for(int i = 0; i < row; i++){	//fill indicies of BogLetts
	for(int j = 0; j < col; j++){
	    board[i][j].row = i;
	    board[i][j].col = j;
	}
    }
    declareSpotsUsed();			//declare helper 2D array
}

//private
//void
//builds the 2Dboard of bools that will be used by all recursive functions to 
//help determine what indicies have already been used in the formation of a 
//given word
void BogBoard::declareSpotsUsed()
{
    spotsUsed = new bool* [row];	//initialize dynamic 2D bool array
    for(int i = 0; i < row; i++){
	spotsUsed[i] = new bool [col];
    }
    initializeAllFalse();		//initialize all elements as false
}

//private
//void
//initializes all indicies in spotsUsed to be false
void BogBoard::initializeAllFalse()
{
    for(int i = 0; i < row; i++){	//initialize 2D array as false
	for(int j = 0; j < col; j++){
	    spotsUsed[i][j] = false;
	}
    }
}

//public
//void
//user inserts the characters for the 2D board of BogLetts, row by row
//the parameters for this function are the row number, and the letters
//that go into that row
void BogBoard::boardInsertByRow(int rowNum, string rowletters)
{
    for(int j = 0; j < col; j++){
	board[rowNum][j].c = rowletters[j];
    }
}

//public
//boolean
//user inserts words for the boggle dictionary
bool BogBoard::dictInsert(string s)
{
    return dict.insert(s);
}

//public
//boolean
//boolean lookup function for the boggle dictionary--is it in the boggle dict?
bool BogBoard::isInDict(string s)
{
    return dict.isWord(s);
}

//public
//bool
//boolean lookup for existance of a word in the BogBoard 
bool BogBoard::isInBoard(string s)
{
    for(int i = 0; i < row; i++){
	for(int j = 0; j < col; j++){
	    if(board[i][j].c == s[0]){	//if the first letter is a match
		initializeAllFalse();
		if(isWordHere(i, j, s, 0))//search to see if the word is in the
		    return true;	//board beginning with that letter
	    }
	}
    }
    return false;
}

//private
//bool
//helper function for isInBoard()
//starts at the given index and recursively checks to see if there is a word
//starting at that index that matches the given string
bool BogBoard::isWordHere(int rNum, int cNum, const string& s, unsigned sIndex)
{
    if(sIndex == s.length())	//got past end of s!
	return true;

    if(board[rNum][cNum].c != s[sIndex])	//no match
	return false;
    else
	spotsUsed[rNum][cNum] = true;		//using this index as part
						//of the word

    //recursions below
    if(rNum-1 >= 0 && cNum-1 >= 0 && !spotsUsed[rNum-1][cNum-1]){
	if(isWordHere(rNum-1, cNum-1, s, sIndex+1))
	    return true;
    }
    if(rNum-1 >= 0 && !spotsUsed[rNum-1][cNum]){
	if(isWordHere(rNum-1, cNum, s, sIndex+1))
	    return true;
    }
    if(rNum-1 >= 0 && cNum+1 < col && !spotsUsed[rNum-1][cNum+1]){
	if(isWordHere(rNum-1, cNum+1, s, sIndex+1))
	    return true;
    }
    if(cNum-1 >= 0 && !spotsUsed[rNum][cNum-1]){
	if(isWordHere(rNum, cNum-1, s, sIndex+1))
	    return true;
    }
    if(cNum+1 < col && !spotsUsed[rNum][cNum+1]){
	if(isWordHere(rNum, cNum+1, s, sIndex+1))
	    return true;
    }
    if(rNum+1 < row && cNum-1 >= 0 && !spotsUsed[rNum+1][cNum-1]){
	if(isWordHere(rNum+1, cNum-1, s, sIndex+1))
	    return true;
    }
    if(rNum+1 < row && !spotsUsed[rNum+1][cNum]){
	if(isWordHere(rNum+1, cNum, s, sIndex+1))
	    return true;
    }
    if(rNum+1 < row && cNum+1 < col && !spotsUsed[rNum+1][cNum+1]){
	if(isWordHere(rNum+1, cNum+1, s, sIndex+1))
	    return true;
    }

    //at this point you can't recurse anymore
    spotsUsed[rNum][cNum] = false;	//done using spot
    if(sIndex == s.length()-1)		//got to end of word!
	return true;
    else				//did not get to end!
	return false;
}

//public
//returns a BogWordList* of all legal words in the BogBoard
BogWordList* BogBoard::solve()
{
    for(int i = 0; i < row; i++){
	for(int j = 0; j < col; j++){
	    initializeAllFalse();
	    goThruBoard(i, j, NULL);	//go through board and find words 
	}				//starting at index i, j
    }
    return getWordListCopy(boardWords);	//return deep copy of boardWords
}

//private
//void
//helper function for solve()
//starts at given index and builds all possible strings found in the board
//and adds them to the list boardWords if they are found in the dictionary
void BogBoard::goThruBoard(int rNum, int cNum, LNode* list)
{
    int len = 0;

    if(list == NULL){
        list = new LNode;
	list->data = board[rNum][cNum];
	list->next = NULL;
    }
    else {
	len = 2;		//one for current char, one we're about to add
	LNode* iter = list;
	LNode* next = iter->next;
	while(next != NULL){
	    len++;		//count the rest
	    iter = iter->next;
	    next = iter->next;
	}
	next = new LNode;
	next->data = board[rNum][cNum];
	next->next = NULL;
	iter->next = next;
    }

    spotsUsed[rNum][cNum] = true;	//using this spot

    if((len >= 3 || (len >= 2 && qExists(list)))	//proper length
       && dict.isWord(listToString(list))){		//and a word
	addToWordList(list);				//so its a word
    }

    //recursive calls -- only recurse if it's a known prefix
    if(dict.isPrefix(listToString(list))){
	if(rNum-1 >= 0 && cNum-1 >=0 && !spotsUsed[rNum-1][cNum-1])
	    goThruBoard(rNum-1, cNum-1, list);
	if(rNum-1 >=0 && !spotsUsed[rNum-1][cNum])
	    goThruBoard(rNum-1, cNum, list);
	if(rNum-1 >=0 && cNum+1 < col && !spotsUsed[rNum-1][cNum+1])
	    goThruBoard(rNum-1, cNum+1, list);
	if(cNum-1 >= 0 && !spotsUsed[rNum][cNum-1])
	    goThruBoard(rNum, cNum-1, list);
	if(cNum+1 < col && !spotsUsed[rNum][cNum+1])
	    goThruBoard(rNum, cNum+1, list);
	if(rNum+1 < row && cNum-1 >= 0 && !spotsUsed[rNum+1][cNum-1])
	    goThruBoard(rNum+1, cNum-1, list);
	if(rNum+1 < row && !spotsUsed[rNum+1][cNum])
	    goThruBoard(rNum+1, cNum, list);
	if(rNum+1 < row && cNum+1 < col && !spotsUsed[rNum+1][cNum+1])
	    goThruBoard(rNum+1, cNum+1, list);
    }
    LNode* current = list;
    LNode* next = current->next;
    if(next != NULL){
	while(next->next != NULL){
	    current = current->next;
	    next = current -> next;
	}
	delete next;			//delete the last LNode
	current->next = NULL;
    }
    else
	delete list;			//if there's only one LNode, delete it

    spotsUsed[rNum][cNum] = false;	//done using spot
}

//private function
//boolean
//takes in a linked list of BogLetts and returns true if one of them is a 'Q'
bool BogBoard::qExists(LNode* s)
{
    LNode* iter = s;

    while(iter != NULL){
	if(iter->data.c == 'Q')
	    return true;
	iter = iter->next;
    }
    return false;
}

//private function
//returns a string
//takes in a linked list of BogLetts and returns a string spelled out by the
//nodes
string BogBoard::listToString(LNode* s)
{
    string rvalue = "";
    LNode* iter = s;

    while(iter != NULL){
	rvalue+=iter->data.c;
	iter = iter->next;
    }
    return rvalue;
}

//private function
//void
//adds a linked list of BogLetts to the BogWordList boardWords
void BogBoard::addToWordList(LNode* list)
{
    string s = listToString(list);

    //only add it to the wordlist if it's not already in the dictionary
    //of solved words. otherwise, add it to the dictionary of solved words
    //and to the wordlist
    if(solvedDict.isWord(s))
	return;
    else
	solvedDict.insert(s);

    if(boardWords->numWords == wordListCapac-1)
	expandWordList();

    int stringSize = s.length();
    LNode* iter = list;

    //build array of BogLetts
    boardWords->words[boardWords->numWords].letts = new BogLett[stringSize];
    boardWords->words[boardWords->numWords].numLetts = stringSize;
    for(int i = 0; i < stringSize; i++){
	boardWords->words[boardWords->numWords].letts[i] = iter->data;
	iter = iter->next;
    }
    boardWords->numWords++;	//update numWords
}

//public function
//returns a deep copy of the parameter BogWordList
BogWordList* BogBoard::getWordListCopy(BogWordList* original)
{
    if(original == NULL)
	return NULL;
    else{
	BogWordList* temp = new BogWordList;
	temp->numWords = original->numWords;
	temp->words = new BogWord [temp->numWords];
	for(int i = 0; i < original->numWords; i++){
	    temp->words[i].numLetts = original->words[i].numLetts;
	    temp->words[i].letts = new BogLett [temp->words[i].numLetts];
	    for(int j = 0; j < temp->words[i].numLetts; j++){
		temp->words[i].letts[j].c = original->words[i].letts[j].c;
		temp->words[i].letts[j].row = original->words[i].letts[j].row;
		temp->words[i].letts[j].col = original->words[i].letts[j].col;
	    }
	}
	return temp;
    }
}

//private function
//void
//expand function for BogWords expands the array in boardWords->words
void BogBoard::expandWordList()
{
    BogWord* temp = new BogWord [wordListCapac*2];

    for(int i = 0; i < wordListCapac; i++)	//copy
	temp[i] = boardWords->words[i];

    wordListCapac*=2;

    int i = boardWords->numWords;
    while(i < wordListCapac){			//initialize
	temp[i].numLetts = 0;
	temp[i].letts = NULL;
	i++;
    }

    delete [] boardWords->words;
    boardWords->words = temp;			//transfer
}

//destructor
BogBoard::~BogBoard()
{
    //delete main BogBoard of BogLett
    for(int i = 0; i < row; i++){
	delete [] board[i];
    }
    delete [] board;

    //delete spotsUsed board of bools
    for(int i = 0; i < row; i++){
	delete [] spotsUsed[i];
    }
    delete [] spotsUsed;

    //delete boardWords for solver program, but not for checker program
    //(where it isn't used)
    if(boardWords != NULL)
	deleteBoardWords();
}

//private function
//void
//helper function for default destructor
//delete BogWordList boardWords
void BogBoard::deleteBoardWords()
{
    for(int i = 0; i < boardWords->numWords; i++){
	    delete [] boardWords->words[i].letts;
	}
    delete [] boardWords->words;
    delete boardWords;
}
