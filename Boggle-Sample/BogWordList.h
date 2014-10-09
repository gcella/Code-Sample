// BogWordList.h contains the definition of a BogWordList
//
//Created by: Gabrielle Cella
//Created on: 20 April 2013
//Last Modified: 20 April 2013

#ifndef WORD_L
#define WORD_L

#include <iostream>
using namespace std;

struct BogLett{
    char c;
    int row;
    int col;
};

struct BogWord{
    int numLetts;
    BogLett* letts;
};

struct BogWordList{
    int numWords;
    BogWord* words;
};

#endif
