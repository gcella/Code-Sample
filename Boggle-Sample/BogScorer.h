// BogScorer.h contains the declaration of the class BogScorer. 
// BogScorer is a class that reads in a validity report from the bbchecker
// program and prints out the corresponding score report
// It is important that this have its own class incase the user wishes to
// compare the score reports of different players, by declaring more than one
// BogScorer
//
//Created by: Gabrielle Cella
//Created on: 25 April 2013
//Last Modified: 29 April 2013

#ifndef SCORE_H
#define SCORE_H

#include <iostream>
#include <string>
using namespace std;

//struct Report contains the data for each word in the score report, including
//its validity, the word itself, and its point value
struct Report{
    bool ok;
    string word;
    int points;
};

class BogScorer
{
public:

    //default constructor
    BogScorer();

    //destructor
    ~BogScorer();

    //reads in a score report and stores the data
    void readReport();

    //scores the report by assigning appropriate point values
    void scoreReport();

    //prints the score report
    void printScores();

private:

    //the list of score reports
    Report* list;
    int listSize;
    int listCapac;

    //expands the dynamic array of Reports
    void expandList();

};

#endif
