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
		Unit *getUnits() {
			return units;
		}

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
		Unit *getUnits() {
			return units;
		}

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
		Unit *getUnits() {
			return units;
		}

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
  return 0;
}