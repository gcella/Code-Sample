// contains main() for the bbchecker program
//
//Created by: Gabrielle Cella
//Created on: 20 April 2013
//Last Modified: 20 April 2013

#include <iostream>
#include "BogValidator.h"
using namespace std;

int main()
{
    BogValidator v;

    v.readDict();
    v.readBoard();
    v.checkWords();
    return 0;
}
