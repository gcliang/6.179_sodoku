#include <iostream>
#include <map>
#include <set>
#include "sudoku.h"

using namespace std;

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
		char value;
		Row *row;
		Column *column;
		Section *section;
	
	public:
		Unit() {
			value = '#';
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
		map<int, Unit *> units;
		set<int> values;
	
	public:
		map<int, Unit *> getUnits() {
			return units;
		}

		void insert(int position, int value) {
			units[position] = new Unit(value);
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
				return units[position]->getValue() == value + '0';
			}
			return true;
		}
};

class Column {
	private:
		map<int, Unit *> units;
		set<int> values;
	
	public:
		map<int, Unit *> getUnits() {
			return units;
		}

		void insert(int position, int value) {
			units[position] = new Unit(value);
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
				return units[position]->getValue() == value + '0';
			}
			return true;
		}
};

class Section {

};

class Sudoku {

};

int main() {
  return 0;
}