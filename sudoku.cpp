#include <iostream>
#include <set>
#include "sudoku.h"

using namespace std;
const int N = 9;

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
		// Unit *getUnits() {
		// 	return units;
		// }

		void insert(int x, int value) {
			if (units[x] != NULL) {
				units[x]->setValue(value);
			} else {
				units[x] = new Unit(value);
			}
			values.insert(value);
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
		// Unit *getUnits() {
		// 	return units;
		// }

		void insert(int y, int value) {
			if (units[y] != NULL) {
				units[y]->setValue(value);
			} else {
				units[y] = new Unit(value);
			}
			values.insert(value);
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
		// Unit *getUnits() {
		// 	return units;
		// }

		void insert(int x, int y, int value) {
			if (units[y][x] != NULL) {
				units[y][x]->setValue(value);
			} else {
				units[y][x] = new Unit(value);
			}
			values.insert(value);
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
		Section *sections[N];
		Unit *board[N][N];
	
	public:
		Sudoku(int *board[N][N]) {
			
		}

		Row *getRows() {

		}

		void setRows() {

		}

		Column *getColumns() {
			
		}

		void setColumns() {

		}

		Section *getSections() {
			
		}

		void setSections() {
			
		}

		Unit *getBoard() {
			return board;
		}

		void insert(int x, int y, int value) {

		}

		bool isValid(int x, int y, int value) {

		}

		void displayBoard() {

		}

		bool isComplete() {

		}
};

int main() {
  return 0;
}