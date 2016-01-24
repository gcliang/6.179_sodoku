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
		int row;
		int column;
		Section *section;
	
	public:
		Unit() {
			value = 0;
		}

		Unit(int r, int c, int val) {
			row = r;
			column = c;
			value = val;
		}

		int getValue() {
			return value;
		}

		void setValue(int val) {
			value = val;
		}

		// Row *getRow() {
		// 	return row;
		// }

		// Column *getColumn() {
		// 	return column;
		// }

		// Section *getSection() {
		// 	return section;
		// }
};

class Row {
	private:
		Unit *units[N];
		set<int> values;
	
	public:
		Row() {
			int i;
			for (i = 0; i < N; i++) {
				units[i] = NULL;
			}
		}

		// Unit *getUnits() {
		// 	return units;
		// }

		// void insert(int x, int value) {
		// 	if (units[x] != NULL) {
		// 		units[x]->setValue(value);
		// 	} else {
		// 		units[x] = new Unit(value);
		// 	}
		// 	values.insert(value);
		// }

		void insert(int x, Unit *unit) {
			units[x] = unit;
			values.insert((*unit).getValue());
		}

		bool isValid(int x, int value) {
  			set<int>::iterator it;
  			bool hasValue = false;
  			for (it=values.begin(); it!=values.end(); ++it) {
  				if (*it == value) {
  					hasValue = true;
  					break;
  				}
  			}
			if (hasValue) {
				return units[x]->getValue() == value;
			}
			return true;
		}
};

class Column {
	private:
		Unit *units[N];
		set<int> values;
	
	public:
		Column() {
			int i;
			for (i = 0; i < N; i++) {
				units[i] = NULL;
			}
		}

		// Unit *getUnits() {
		// 	return units;
		// }

		// void insert(int y, int value) {
		// 	if (units[y] != NULL) {
		// 		units[y]->setValue(value);
		// 	} else {
		// 		units[y] = new Unit(value);
		// 	}
		// 	values.insert(value);
		// }

		void insert(int y, Unit *unit) {
			units[y] = unit;
			values.insert((*unit).getValue());
		}

		bool isValid(int y, int value) {
  			set<int>::iterator it;
  			bool hasValue = false;
  			for (it=values.begin(); it!=values.end(); ++it) {
  				if (*it == value) {
  					hasValue = true;
  					break;
  				}
  			}
			if (hasValue) {
				return units[y]->getValue() == value;
			}
			return true;
		}
};

class Section {
	private:
		Unit *units[N][N];
		set<int> values;
	
	public:
		Section() {
			int i;
			for (i = 0; i < N; i++) {
				int j;
				for (j = 0; j < N; j++) {
					units[i][j] = NULL;
				}
			}
		}

		// Unit *getUnits() {
		// 	return units;
		// }

		// void insert(int x, int y, int value) {
		// 	if (units[y][x] != NULL) {
		// 		units[y][x]->setValue(value);
		// 	} else {
		// 		units[y][x] = new Unit(value);
		// 	}
		// 	values.insert(value);
		// }

		void insert(int x, int y, Unit *unit) {
			units[y][x] = unit;
			values.insert((*unit).getValue());
		}

		bool isValid(int x, int y, int value) {
  			set<int>::iterator it;
  			bool hasValue = false;
  			for (it=values.begin(); it!=values.end(); ++it) {
  				if (*it == value) {
  					hasValue = true;
  					break;
  				}
  			}
			if (hasValue) {
				return units[y][x]->getValue() == value;
			}
			return true;
		}
};

class Sudoku {
	private:
		Row *rows[N];
		Column *columns[N];
		Section ***sections;
		Unit ***board;

		void setRows() {
			int i;
			for (i = 0; i < N; i++) {
				Row *row = new Row();
				int j;
				for (j = 0; j < N; j++) {
					rows[i]->insert(j, &(*board)[i][j]);
				}
			}
		}

		void setColumns() {
			
		}

		void setSections() {
			
		}
	
	public:
		Sudoku(int (*intBoard)[N][N]) {
			int i;
			for (i = 0; i < N; i++) {
				int j;
				for (j = 0; j < N; j++) {
					board[i][j] = new Unit(i, j, (*intBoard)[i][j]);
				}
			}
			setRows();
			setColumns();
			setSections();
		}

		Row **getRows() {
			return rows;
		}

		Column **getColumns() {
			return columns;
		}

		Section ***getSections() {
			return sections;
		}

		Unit ***getBoard() {
			return board;
		}

		void insert(int x, int y, int value) {
			if (x >= N || y >= N || x < 0 || y < 0) {
				throw outOfBoundsException;
			} else if (!isValid(x, y, value)) {
				throw sudokuInvariantException;
			}
			// TODO: INSERT INTO APPROPRIATE ROW, COLUMN, AND SECTION
		}

		bool isValid(int x, int y, int value) {
			// TODO: check appropriate row
			bool isValidRow;
			// TODO: check appropriate column
			bool isValidColumn;
			// TODO: check appropriate section
			bool isValidSection;
			return isValidRow && isValidColumn && isValidSection;
		}

		bool isComplete() {
			return true;
		}

		// void displayBoard() {

		// }
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
