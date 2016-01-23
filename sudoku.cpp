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
		Column() {
			int i;
			for (i = 0; i < N; i++) {
				units[i] = NULL;
			}
		}

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
		Section ***sections;
		Unit ***board;

		void setRows() {
			int i;
			for (i = 0; i < N; i++) {
				Row row = new Row();
				int j;
				for (j = 0; j < N; j++) {
					row->insert;
					rows[i]->insert(j, (*board)[i][j]);
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
			if (x > N or y > N) {

			}
		}

		bool isValid(int x, int y, int value) {
			return true;
		}

		void displayBoard() {

		}

		bool isComplete() {
			return true;
		}
};

int main() {
  return 0;
}