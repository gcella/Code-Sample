// BogScorer.cpp contains the definitions of the functions declared in the
// class BogScorer
// BogScorer is a class taht reads in a validity report from the bbchecker
// program and prints out the corresponding score report
//
//Created by: Gabrielle Cella
//Created on: 25 April 2013
//Last Modified: 29 April 2013

#include <iostream>
#include <string>
#include "BogScorer.h"
using namespace std;

const int INIT_SIZE = 5;	//initial size of dynamic array of Reports

const string OK = "OK";
const string NO = "NO";

//constants for the point values
const int THREE_P = 1;
const int FOUR_P = 1;
const int FIVE_P = 2;
const int SIX_P = 3;
const int SEVEN_P = 5;
const int EIGHT_P = 11;

//default constructor
BogScorer::BogScorer()
{
    list = new Report [INIT_SIZE];
    listSize = 0;
    listCapac = INIT_SIZE;
}

//public function
//void
//reads and stores input of the score report
void BogScorer::readReport()
{
    string temp;
    Report r;

    while(cin >> temp){
	if(temp == OK)		//store OK vs NO
	    r.ok = true;
	else
	    r.ok = false;

	cin >> r.word;		//store word

	if(listSize == listCapac)
	    expandList();

	list[listSize] = r;
	listSize++;
    }
}

//public function
//void
//scores the report by assigning point values to each word
void BogScorer::scoreReport()
{
    for(int i = 0; i < listSize; i++){
	if(list[i].ok){
	    if(list[i].word.length() == 3)	//assign appropriate
		list[i].points = THREE_P;	//point values
	    if(list[i].word.length() == 4)
		list[i].points = FOUR_P;
	    if(list[i].word.length() == 5)
		list[i].points = FIVE_P;
	    if(list[i].word.length() == 6)
		list[i].points = SIX_P;
	    if(list[i].word.length() == 7)
		list[i].points = SEVEN_P;
	    if(list[i].word.length() >= 8)
		list[i].points = EIGHT_P;
	}
	else
	    list[i].points = 0;
    }
}

//public function
//void
//prints score report
void BogScorer::printScores()
{
    int total_pts = 0;
    int valid_words = 0;

    for(int i = 0; i < listSize; i++){
	cout << list[i].points << " ";

	if(list[i].ok){
	    cout << OK << " ";
	    valid_words++;		//count total words
	}
	else{
	    cout << NO << " ";
	}

	cout << list[i].word << endl;
	total_pts+=list[i].points;	//count total points
    }
    cout << valid_words << " words " << total_pts << " points" << endl;
}

//private function
//used to expand the dynamic array of Report structs
void BogScorer::expandList()
{
    Report* temp = new Report [listCapac*2];

    for(int i = 0; i < listCapac; i++)
	temp[i] = list[i];

    listCapac*=2;

    delete [] list;
    list = temp;
}

//destructor
BogScorer::~BogScorer()
{
    delete [] list;
}
