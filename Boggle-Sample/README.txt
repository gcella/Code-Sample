Comp15: Data Structures
Project Two -- Boggle

Created by: Gabrielle Cella
Created on: 20 April 2013
Last Modified: 2 May 2013

Files Sumbitted: BogWordList.h, trienode.h, trienode.cpp, Dictionary.h, 
      		 Dictionary.cpp, BogBoard.h, BogBoard.cpp, BogScorer.h, 
		 BogScorer.cpp, scorerMain.cpp, BogValidator.h, 
		 BogValidator.cpp, checkerMain.cpp, BogSolver.h, 
		 BogSolver.cpp, solverMain.cpp, README.txt

Programs:
	bbsolver:
		- solves a boggle board
		- input:
		[each word of the dictionary]
		. (<single period to mark end of dictionary)
		#rows #cols
                [each row entered on a separate line with no spaces]
		- output:
		"< [letter] [row number] [col number] >" (for ea letter in word)

	bbchecker:
		- checks to see if a given set of words exists in a given 
		  boggle board
		- input:
		[each word of the dictionary]
		. (< single period to mark end of dictionary)
		#rows #cols
		[each row entered on a separate line with no spaces]
		[each word in the set to be checked entered on a separate line]
		- output:
		listed report in format:
		[OK (or) NO] [word that was guessed]

	bbscorer:
		- takes output from bbchecker and determines a score based
		  on the words guessed and the official boggle rules for points
		  per word
		- input:
		listed report in format:
		[OK (or) NO] [word that was guessed]

Compile:

bbsolver: g++ -Wall -Wextra -g trienode.cpp Dictionary.cpp BogBoard.cpp BogSolver.cpp solverMain.cpp -o bbsolver
bbchecker: g++ -Wall -Wextra -g trienode.cpp Dictionary.cpp BogBoard.cpp BogValidator.cpp checkerMain.cpp -o bbchecker
bbscorer: g++ -Wall -Wextra -g BogScorer.cpp scorerMain.cpp -o bbscorer

Header Files:

	BogWordList.h
		- This file contains the definitions of the struct
		  BogWordList, with its member BogWord, and its member BogLett

	Dictionary.h
		- This file contains Dictionary
		- The code for dictionary was provided by Bruce
		- Functions defined in Dictionary.cpp
		- Included only in BogBoard

	BogBoard.h
		- Includes: Dictionary.h, BogWordList.h
		- This class contains BogBoard, the class that holds the 
		  the board for the boggle program--a 2D array of BogLetts
		- BogBoard is a self-sufficient board that is used to house the
		  the board with which the user is playing boggle. This board 
		  has the capacity to solve itself and look to see if it 
		  contains certain words. In order to do so, the board must
		  occasionally confer with the dictionay of approved words, so 
		  it contains an instance of the class Dictionay, as well.
		  This is the only class with an instance of Dictionary.
		- Functions defined in BogBoard.cpp

	BogScorer.h
		- This class contains BogScorer, the class that is responsible 
		  for scoring the results of a boggle check-report
		- Scores for a word depend on if the word was marked as OK or 
		  NO, and the word's length. BogScorer prints a score report
		  with "total words" and "total points" calculated at the
		  bottom.
		- It is important that this program operates out of a class 
		  incase teh user wishes to compare the score reports of
		  two or more different players. This can be done by declaring
		  more than one BogScorer.
		- Functions defined in BogScorer.cpp

	BogValidator.h
		- Includes: BogBoard.h
		- This class contains BogValidator, the class where a player's 
		  guesses are checked.
		- BogValidator allows the user to check if a given word or a 
		  given set of words exist in the boggle board by ensuring that
		  they are 1) an appropriate word in the boggle dictionay 
		  2) an appropriate length 3) found in the Boggle Board itself 
		  4) have not already been marked as OK in the same checkreport
		- Functions defined in BogValidator.cpp

	BogSovler.h
		- Includes: BogBoard.h, BogWordList.h
		- This class contains BogSolver, the class that solves a 
		  Boggle Board, producing a list of all acceptable words
		- BogSovler confirs with the boggle board and has it return a 
		  BogWordList of all acceptable words in the Boggle Board.
		- This class allows the user to solve the board and request
		  different ways to print or otherwise obtain information about
		  the set of solved words.
		- Functions defined in BogSolver.cpp

Other Notes:

      EXPIRED: The website that the game was initially able to be played on 
      is no longer available
	Play my boggle game at:
	www.eecs.tufts.edu/~gcella01/bog15/
