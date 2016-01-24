#include <iostream>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <set>
#include "sudoku.h"

using namespace std;
const int N = 9;
const int SQRT_N = 3;
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
		int section;
	
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

		int getRow() {
			return row;
		}

		int getColumn() {
			return column;
		}

		int getSection() {
			return section;
		}

		void setSection(int sec) {
			section = sec;
		}
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
		
		Unit **getUnits() {
			return units;
		}

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

		Unit **getUnits() {
			return units;
		}

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
		Unit *units[SQRT_N * SQRT_N];
		set<int> values;
	
	public:
		Section() {
			int i;
			for (i = 0; i < N; i++) {
				int j;
				for (j = 0; j < N; j++) {
					units[i * N + j] = NULL;
				}
			}
		}
		
		Unit **getUnits() {
			return units;
		}

		void insert(int x, int y, Unit *unit) {
			units[y * N + x] = unit;
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
				return units[y * N + x]->getValue() == value;
			}
			return true;
		}
};

class Sudoku {
	private:
		Row *rows[N];
		Column *columns[N];
		Section *sections[SQRT_N * SQRT_N];
		Unit *board[N * N];

		void setRows() {
			int i;
			for (i = 0; i < N; i++) {
				Row *row = new Row();
				rows[i] = row;
				int j;
				for (j = 0; j < N; j++) {
					rows[i]->insert(j, board[i * N + j]);
				}
			}
		}

		void setColumns() {
			int k;
			for (k = 0; k < N; k++) {
				Column *column = new Column();
				columns[k] = column;
			}
			int i;
			for (i = 0; i < N; i++) {
				int j;
				for (j = 0; j < N; j++) {
					columns[j]->insert(i, board[i * N + j]);
				}
			}
		}

		int calculateSection(int i, int j) {
			return (i / SQRT_N) * N + (j / SQRT_N);
		}

		void setSections() {
			int k;
			for (k = 0; k < N; k++) {
				Section *section = new Section();
				sections[k] = section;
			}
			int i;
			for (i = 0; i < N; i++) {
				int j;
				for (j = 0; j < N; j++) {
					sections[calculateSection(i, j)]->insert(j, i, board[i * N + j]);
				}
			}
		}

		void setBoard(int *intBoard) {
			int i;
			for (i = 0; i < N; i++) {
				int j;
				for (j = 0; j < N; j++) {
					board[i * N + j] = new Unit(i, j, intBoard[i * N + j]);
				}
			}
		}
	
	public:
		Sudoku(int *intBoard) {
			setBoard(intBoard);
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

		Section **getSections() {
			return sections;
		}

		Unit **getBoard() {
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
};

void print_board(Sudoku *board) {
	int row;
	Row *cur;
	for (row = 0; row < N; row++) {
		cur = board->getRows()[row];
		Unit **vals;
		vals = (cur->getUnits());
		int col;
		for (col = 0; col < N; col++) {
			cout << vals[col]->getValue();
			if ((col+1) % 3 == 0) {
				cout << " "; // Add horizontal space between sections
			}
		}
		cout << "\n";
		if ((row+1) % 3 ==0) {
			cout << "\n"; // Add vertical space between sections
		}
	}
}

int main() {
	// Determine desired difficulty
	cout << "Welcome to Sudoku.  What is your desired difficulty? \n" << "The options are easy, medium, and hard.\n";
	string difficulty;
	int diff;
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
	int board1[N*N] = {3,9,4,1,7,2,5,8,6,
					   1,5,7,3,8,6,2,4,9,
					   2,8,6,9,4,5,7,1,3,
					   5,3,8,7,9,4,6,2,1,
					   9,4,1,2,6,3,8,7,5,
					   7,6,2,8,5,1,3,9,4,
					   4,1,3,5,2,8,9,6,7,
					   6,2,9,4,3,7,1,5,8,
					   8,7,5,6,1,9,4,3,2
					   };
	int board2[N*N] = {2,4,8,3,9,5,7,1,6,
					   5,7,1,6,2,8,3,4,9,
					   9,3,6,7,4,1,5,8,2,
					   6,8,2,5,3,9,1,7,4,
					   3,5,9,1,7,4,6,2,8,
				       7,1,4,8,6,2,9,5,3,
					   8,6,3,4,1,7,2,9,5,
					   1,9,5,2,8,6,4,3,7,
					   4,2,7,9,5,3,8,6,1
					   };
	int board3[N*N] = {4,1,2,3,6,7,8,5,9,
					   6,5,8,9,4,1,3,2,7,
					   3,9,7,5,2,8,1,6,4,
					   9,6,4,2,3,5,7,8,1,
					   7,3,5,1,8,4,6,9,2,
					   8,2,1,7,9,6,4,3,5,
					   1,4,9,6,5,3,2,7,8,
				       5,8,3,4,7,2,9,1,6,
					   2,7,6,8,1,9,5,4,3
					   };

	int* possibleBoards[COMPLETED_BOARDS] = {board1, board2, board3};

	int randomIndex = rand() % COMPLETED_BOARDS;
	int* chosen;
	chosen = possibleBoards[randomIndex];

	Sudoku *mainBoard = new Sudoku(chosen);

	// randomly remove numbers to reach initial state
	int row, d;
	Row *current;
	for (row = 0; row < N; row++) {
		current = mainBoard->getRows()[row];
		int removed[diff];
		int col;
		for (d = 0; d < diff; d++) {
			col = rand() % N;
			int * found;
			found = find(removed, removed+diff, col);
			while (found == removed+diff) {
				col = rand() % N;
				found = find(removed, removed+diff, col);
			}
			current->insert(col, 0);
		}
	}
	/*
	cout << "Fill in the 0's with the correct number (1-9) by inputing:\n";
	cout << "x-coordinate y-coordinate digit\n";
	cout << "(0,0) is the top left location\n";

	// loop to get inputs and update map
	while (!mainBoard->isComplete()) {
		// print out board
		print_board(mainBoard);

		// wait for output
		string input;
		getline(cin, input);

		stringstream ss(input);
		int x, y, digit;
		ss >> x;
		ss >> y;
		ss >> digit;

		cout << "Attempting to set (" << x << ", " << y << ") to " << digit << "\n";

		mainBoard->insert(x, y, digit);

		// print out board with possible update
		print_board(mainBoard);
	}
	cout << "Congratulations! You solved the puzzle!\n";
	*/
	return 0;
}
