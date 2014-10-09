// contains main() for the bbsolver program
//
//Created by: Gabrielle Cella
//Created on: 20 April 2013
//Last Modified: 20 April 2013

#include <iostream>
#include "BogSolver.h"
using namespace std;

int main()
{
    BogSolver solver;

    solver.readDict();
    solver.readBoard();
    solver.solve();
    solver.printWords();
    return 0;
}
