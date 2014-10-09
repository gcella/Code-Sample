// contains main() for the bbscorer program
//
//Created by: Gabirelle Cella
//Created on: 25 April 2013
//Last Modified: 25 April 2013

#include <iostream>
#include "BogScorer.h"
using namespace std;

int main()
{
    BogScorer s1;

    s1.readReport();
    s1.scoreReport();
    s1.printScores();

    return 0;
}
