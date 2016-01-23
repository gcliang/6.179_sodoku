#include <iostream>
#include <set>
#include <cstdlib>
#include <sstream>
#include "sudoku.h"

using namespace std;
const int N = 9;
const int EASY = 5;
const int MEDIUM = 6;
const int HARD = 7;
const int COMPLETED_BOARDS = 3;

class OutOfBoundsException : public exception {
	virtual const char* what() const throw() {
		return "That position is out of bounds!";
	}
} outOfBoundsException;

class SudokuInvariantException : public exception {
	virtual const char* what() const throw() {
		return "That move violates the Sudoku invariant. Try again!";
	}
} sudokuInvariantException;

class Unit {
	private:
		int value;
		Row *row;
		Column *column;
		Section *section;
	
	public:
		Unit() {
			value = 0;
		}

		Unit(int val) {
			value = val;
		}

		int getValue() {
			return value;
		}

		void setValue(int val) {
			value = val;
		}

		Row *getRow() {
			return row;
		}

		Column *getColumn() {
			return column;
		}

		Section *getSection() {
			return section;
		}
};

class Row {
	private:
		Unit *units[N];
		set<int> values;
	
	public:
//		Unit *getUnits() {
//			return units;
//		}

		void insert(int position, int value) {
			if (units[position] != NULL) {
				units[position]->setValue(value);
			} else {
				units[position] = new Unit(value);
			}
			values.insert(value);
		}

		bool isValid(int position, int value) {
  			set<int>::iterator it;
  			bool hasValue = false;
  			for (it=values.begin(); it!=values.end(); ++it) {
  				if (*it == value) {
  					hasValue = true;
  					break;
  				}
  			}
			if (hasValue) {
				return units[position]->getValue() == value;
			}
			return true;
		}
};

class Column {
	private:
		Unit *units[N];
		set<int> values;
	
	public:
//		Unit *getUnits() {
//			return units;
//		}

		void insert(int position, int value) {
			if (units[position] != NULL) {
				units[position]->setValue(value);
			} else {
				units[position] = new Unit(value);
			}
			values.insert(value);
		}

		bool isValid(int position, int value) {
  			set<int>::iterator it;
  			bool hasValue = false;
  			for (it=values.begin(); it!=values.end(); ++it) {
  				if (*it == value) {
  					hasValue = true;
  					break;
  				}
  			}
			if (hasValue) {
				return units[position]->getValue() == value;
			}
			return true;
		}
};

class Section {
	private:
		Unit *units[N];
		set<int> values;
	
	public:
//		Unit *getUnits() {
//			return units;
//		}

		void insert(int position, int value) {
			if (units[position] != NULL) {
				units[position]->setValue(value);
			} else {
				units[position] = new Unit(value);
			}
			values.insert(value);
		}

		bool isValid(int position, int value) {
  			set<int>::iterator it;
  			bool hasValue = false;
  			for (it=values.begin(); it!=values.end(); ++it) {
  				if (*it == value) {
  					hasValue = true;
  					break;
  				}
  			}
			if (hasValue) {
				return units[position]->getValue() == value;
			}
			return true;
		}
};

class Sudoku {

};

int main() {
	// Determine desired difficulty
	cout << "Welcome to Sudoku.  What is your desired difficulty? \n" << "The options are easy, medium, and hard.\n";
	string difficulty;
	int diff;
	bool done = false;
	getline(cin, difficulty);

	while (difficulty != "easy" && difficulty != "medium" && difficulty != "hard") {
		cout << "Invalid difficulty, please try again.\n";
		getline(cin, difficulty);
	}

	if (difficulty == "easy") {
		diff = EASY;
	} else if (difficulty == "medium") {
		diff = MEDIUM;
	} else if (difficulty == "hard") {
		diff = HARD;
	}

	cout << "You have chosen " << difficulty << ".\nBuilding your puzzle...\n";

	// Chose inital sudoku board from selected completed board
	int board1[N][N] = {{3,9,4,1,7,2,5,8,6},
						{1,5,7,3,8,6,2,4,9},
						{2,8,6,9,4,5,7,1,3},
						{5,3,8,7,9,4,6,2,1},
						{9,4,1,2,6,3,8,7,5},
						{7,6,2,8,5,1,3,9,4},
						{4,1,3,5,2,8,9,6,7},
						{6,2,9,4,3,7,1,5,8},
						{8,7,5,6,1,9,4,3,2}
						};
	int board2[N][N];
	int board3[N][N];

	int (*possibleBoards[COMPLETED_BOARDS])[N][N] = {&board1, &board2, &board3};

	int randomIndex = rand() % COMPLETED_BOARDS;
	int (*chosen)[9][9] = (possibleBoards[randomIndex]);

	Sudoku mainBoard;
	// randomly remove numbers to reach initial state

	cout << "Fill in the 0's with the correct number (1-9) by inputing:\nx-coordinate y-coordinate digit";

	// loop to get inputs and update map
	while (!done) {
		// print out board

		// wait for output
		string input;
		getline(cin, input);

		stringstream ss(input);
		int x, y, digit;
		ss >> x;
		ss >> y;
		ss >> digit;

		// print out board with possible update
	}

	return 0;
}
