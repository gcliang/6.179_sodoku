#include <iostream>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <set>
#include <ctime>
#include "sudoku.h"
#include "graphics.h"

using namespace std;

const int N = 9;
const int SQRT_N = 3;
const int EASY = 5;
const int MEDIUM = 6;
const int HARD = 7;
const int COMPLETED_BOARDS = 5;

class OutOfBoundsException : public exception {
public:
	virtual const char* what() const throw() {
		return "";
	}
} outOfBoundsException;

class PositionOutOfBoundsException : public OutOfBoundsException {
public:
	virtual const char* what() const throw() {
		return "That position is out of bounds! Please try again.";
	}
} positionOutOfBoundsException;

class ValueOutOfBoundsException : public OutOfBoundsException {
public:
	virtual const char* what() const throw() {
		return "That value is out of bounds! Please try again.";
	}
} valueOutOfBoundsException;

class ImmutableUnitException : public exception {
	public:
		virtual const char* what() const throw() {
			return "That position is immutable! Please insert into another position.";
	}
} immutableUnitException;

class SudokuInvariantException : public exception {
public:
	virtual const char* what() const throw() {
		return "";
	}
} sudokuInvariantException;

class RowInvariantException : public SudokuInvariantException {
	public:
		virtual const char* what() const throw() {
			return "That value already exists in that row! Please try again.";
	}
} rowInvariantException;

class ColumnInvariantException : public SudokuInvariantException {
	public:
		virtual const char* what() const throw() {
			return "That value already exists in that column! Please try again.";
	}
} columnInvariantException;

class SectionInvariantException : public SudokuInvariantException {
	public:
		virtual const char* what() const throw() {
			return "That value already exists in that section! Please try again.";
	}
} sectionInvariantException;

class Unit {
	private:
		int value;
		int row;
		int column;
		int section;
		bool isMutable;
	
	public:
		Unit(int r, int c, int s, int val, bool isMut) {
			row = r;
			column = c;
			section = s;
			value = val;
			isMutable = isMut;
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

		bool getMutability() {
			return isMutable;
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
			if (units[x] == NULL) {
				units[x] = unit;
			}
			values.insert(unit->getValue());
		}

		void insert(int value) {
			values.insert(value);
		}

		void remove(int value) {
			set<int>::iterator it;
			it = values.find(value);
			if (it != values.end()) {
				values.erase(values.find(value));
			}
		}

		bool isValid(int value) {
			if (value == 0) {
				return true;
			}
			set<int>::iterator it;
			it = values.find(value);
			return it == values.end();
		}

		// void insert(int x, Unit *unit) {
		// 	if (units[x] != NULL) {
		// 		// cout << "This value: " << getUnits()[x]->getValue() << endl;
		// 		// cout << "Value in values: " << std::to_string(values.find(units[x]->getValue()) != values.end()) << endl;
		// 		if (values.find(units[x]->getValue()) != values.end()){
		// 			values.erase(values.find(units[x]->getValue()));
		// 			// cout << "Deleting val=" << *values.find(units[x]->getValue()) << endl;
		// 		}
		// 	} else {
		// 		units[x] = unit;
		// 	}
		// 	if (unit->getValue() != 0) {
		// 		// cout << "Inserting value" << endl;
		// 		values.insert(unit->getValue());
		// 	}
		// }

		// bool isValid(int x, int value) {
  // 			set<int>::iterator it;
		// 	it = values.find(value);
		// 	std::cout << "Row isValid... val=" << *it << " stored_val=" << getUnits()[x]->getValue() << endl;
		// 	if (it != values.end() && value != 0) {
		// 		return getUnits()[x]->getValue() == value;
		// 	}
		// 	return true;
		// 	// return it == values.end();
		// }
};

class Column {
	private:
		// Unit *units[N];
		set<int> values;
	
	public:
		// Column() {
		// 	int i;
		// 	for (i = 0; i < N; i++) {
		// 		units[i] = NULL;
		// 	}
		// }

		void insert(int value) {
			values.insert(value);
		}

		void remove(int value) {
			set<int>::iterator it;
			it = values.find(value);
			if (it != values.end()) {
				values.erase(values.find(value));
			}
		}

		bool isValid(int value) {
			if (value == 0) {
				return true;
			}
			set<int>::iterator it;
			it = values.find(value);
			return it == values.end();
		}

		// Unit **getUnits() {
		// 	return units;
		// }

		// void insert(int y, Unit *unit) {
		// 	if (units[y] != NULL) {
		// 		values.erase(units[y]->getValue());
		// 	} else {
		// 		units[y] = unit;
		// 	}
		// 	if (unit->getValue() != 0) {
		// 		values.insert(unit->getValue());
		// 	}
		// }

		// bool isValid(int y, int value) {
  // 			set<int>::iterator it;
		// 	it = values.find(value);
		// 	if (it != values.end() && value != 0) {
		// 		return getUnits()[y]->getValue() == value;
		// 	}
		// 	return true;
		// 	// return it == values.end();
		// }
};

class Section {
	private:
		// Unit *units[SQRT_N * SQRT_N];
		set<int> values;

		// int calculateIdx(int x, int y) {
		// 	return (y % SQRT_N) * SQRT_N + (x % SQRT_N);
		// }

	public:
		// Section() {
		// 	int i;
		// 	for (i = 0; i < SQRT_N; i++) {
		// 		int j;
		// 		for (j = 0; j < SQRT_N; j++) {
		// 			units[i * SQRT_N + j] = NULL;
		// 		}
		// 	}
		// }

		void insert(int value) {
			values.insert(value);
		}

		void remove(int value) {
			set<int>::iterator it;
			it = values.find(value);
			if (it != values.end()) {
				values.erase(values.find(value));
			}
		}

		bool isValid(int value) {
			if (value == 0) {
				return true;
			}
			set<int>::iterator it;
			it = values.find(value);
			return it == values.end();
		}
		
		// Unit **getUnits() {
		// 	return units;
		// }

		// void insert(int x, int y, Unit *unit) {
		// 	if (units[calculateIdx(x, y)] != NULL) {
		// 		values.erase(units[calculateIdx(x, y)]->getValue());
		// 	} else {
		// 		units[calculateIdx(x, y)] = unit;
		// 	}
		// 	if (unit->getValue() != 0) {
		// 		values.insert(unit->getValue());
		// 	}
		// }

		// bool isValid(int x, int y, int value) {
  // 			set<int>::iterator it;
		// 	it = values.find(value);
		// 	if (it != values.end() && value != 0) {
		// 		return getUnits()[y * N + x]->getValue() == value;
		// 	}
		// 	return true;
		// 	// return it == values.end();
		// }
};

class Sudoku {
	private:
		Unit *board[N * N];
		Row *rows[N];
		Column *columns[N];
		Section *sections[SQRT_N * SQRT_N];
		
		void setBoard(int *intBoard) {
			int i;
			for (i = 0; i < N; i++) {
				int j;
				for (j = 0; j < N; j++) {
					if (intBoard[i * N + j] == 0) {
						board[i * N + j] = new Unit(i, j, calculateSection(i, j), intBoard[i * N + j], true);
					} else {
						board[i * N + j] = new Unit(i, j, calculateSection(i, j), intBoard[i * N + j], false);
					}
				}
			}
		}

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
					columns[j]->insert(board[i * N + j]->getValue());
				}
			}
		}

		int calculateSection(int i, int j) {
			return (i / SQRT_N) * SQRT_N + (j / SQRT_N);
		}

		void setSections() {
			int k;
			for (k = 0; k < N; k++) {
				Section *section = new Section();
				sections[k] = section;
			}
			cout << "Setup sections" << endl;
			int i;
			for (i = 0; i < N; i++) {
				int j;
				for (j = 0; j < N; j++) {
					int currentSection = calculateSection(i, j);
					sections[currentSection]->insert(board[i * N + j]->getValue());
				}
				cout << "Populated one section" << endl;
			}
		}

		// void setBoard(int *intBoard) {
		// 	int i;
		// 	for (i = 0; i < N; i++) {
		// 		int j;
		// 		for (j = 0; j < N; j++) {
		// 			if (intBoard[i * N + j] == 0) {
		// 				board[i * N + j] = new Unit(i, j, calculateSection(i, j), intBoard[i * N + j], true);
		// 			} else {
		// 				board[i * N + j] = new Unit(i, j, calculateSection(i, j), intBoard[i * N + j], false);
		// 			}
		// 		}
		// 	}
		// }

		// void setRows() {
		// 	int i;
		// 	for (i = 0; i < N; i++) {
		// 		Row *row = new Row();
		// 		rows[i] = row;
		// 		int j;
		// 		for (j = 0; j < N; j++) {
		// 			rows[i]->insert(j, board[i * N + j]);
		// 		}
		// 	}
		// }

		// void setColumns() {
		// 	int k;
		// 	for (k = 0; k < N; k++) {
		// 		Column *column = new Column();
		// 		columns[k] = column;
		// 	}
		// 	int i;
		// 	for (i = 0; i < N; i++) {
		// 		int j;
		// 		for (j = 0; j < N; j++) {
		// 			columns[j]->insert(i, board[i * N + j]);
		// 		}
		// 	}
		// }

		// int calculateSection(int i, int j) {
		// 	return (i / SQRT_N) * SQRT_N + (j / SQRT_N);
		// }

		// void setSections() {
		// 	int k;
		// 	for (k = 0; k < N; k++) {
		// 		Section *section = new Section();
		// 		sections[k] = section;
		// 	}
		// 	cout << "Setup sections" << endl;
		// 	int i;
		// 	for (i = 0; i < N; i++) {
		// 		int j;
		// 		for (j = 0; j < N; j++) {
		// 			int currentSection = calculateSection(i, j);
		// 			sections[currentSection]->insert(j, i, board[i * N + j]);
		// 		}
		// 		cout << "Populated one section" << endl;
		// 	}
		// }

	public:
		Sudoku(int *intBoard) {
			setBoard(intBoard);
			cout << "Board set" << endl;
			setRows();
			cout << "Rows set" << endl;
			setColumns();
			cout << "Columns set" << endl;
			setSections();
			cout << "Sections set" << endl;
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
				throw positionOutOfBoundsException;
			} else if (value < 0 || value > N) {
				throw valueOutOfBoundsException;
			} else if (!board[y * N + x]->getMutability()) {
				throw immutableUnitException;
			} else if (!isValid(x, y, value)) {
				bool isValidRow = getRows()[y]->isValid(value);
				bool isValidColumn = getColumns()[x]->isValid(value);
				bool isValidSection = getSections()[calculateSection(y, x)]->isValid(value);
				if (!isValidRow) {
					throw rowInvariantException;
				} else if (!isValidColumn) {
					throw columnInvariantException;
				} else if (!isValidSection) {
					throw sectionInvariantException;
				}
			}
			getRows()[y]->remove(board[y * N + x]->getValue());
			getColumns()[x]->remove(board[y * N + x]->getValue());
			getSections()[calculateSection(y, x)]->remove(board[y * N + x]->getValue());
			
			board[y * N + x]->setValue(value);

			getRows()[y]->insert(board[y * N + x]->getValue());
			getColumns()[x]->insert(board[y * N + x]->getValue());
			getSections()[calculateSection(y, x)]->insert(board[y * N + x]->getValue());
		}

		bool isValid(int x, int y, int value) {
			bool isValidRow = getRows()[y]->isValid(value);
			bool isValidColumn = getColumns()[x]->isValid(value);
			bool isValidSection = sections[calculateSection(y, x)]->isValid(value);
			cout << "isValidRow: " << isValidRow << endl;
			cout << "isValidColumn: " << isValidColumn << endl;
			cout << "isValidSection: " << isValidSection << endl;
			return isValidRow && isValidColumn && isValidSection;
		}

		bool isComplete() {
			int i;
			for (i = 0; i < N; i++) {
				int j;
				for (j = 0; j < N; j++) {
					if (board[i * N + j]->getValue() == 0) {
						return false;
					}
				}
			}
			return true;
		}

		// void insert(int x, int y, int value) {
		// 	if (x >= N || y >= N || x < 0 || y < 0) {
		// 		throw positionOutOfBoundsException;
		// 	} else if (value < 0 || value > N) {
		// 		throw valueOutOfBoundsException;
		// 	} else if (!board[y * N + x]->getMutability()) {
		// 		throw immutableUnitException;
		// 	} else if (!isValid(x, y, value)) {
		// 		bool isValidRow = getRows()[y]->isValid(x, value);
		// 		bool isValidColumn = getColumns()[x]->isValid(y, value);
		// 		bool isValidSection = getSections()[calculateSection(y, x)]->isValid(x, y, value);
		// 		if (!isValidRow) {
		// 			throw rowInvariantException;
		// 		} else if (!isValidColumn) {
		// 			throw columnInvariantException;
		// 		} else if (!isValidSection) {
		// 			throw sectionInvariantException;
		// 		}
		// 	}
		// 	cout << "Before Insert" << endl;
			
		// 	cout << "After Setting Value" << endl;
		// 	getRows()[y]->insert(x, board[y * N + x]);
		// 	cout << "After Row Insert" << endl;
		// 	getColumns()[x]->insert(y, board[y * N + x]);
		// 	cout << "After Column Insert" << endl;
		// 	getSections()[calculateSection(y, x)]->insert(x, y, board[y * N + x]);
		// 	cout << "After Section Insert" << endl;
		// 	board[y * N + x]->setValue(value);
		// }

		// bool isValid(int x, int y, int value) {
		// 	bool isValidRow = getRows()[y]->isValid(x, value);
		// 	bool isValidColumn = getColumns()[x]->isValid(y, value);
		// 	bool isValidSection = sections[calculateSection(y, x)]->isValid(x, y, value);
		// 	cout << "isValidRow: " << isValidRow << endl;
		// 	cout << "isValidColumn: " << isValidColumn << endl;
		// 	cout << "isValidSection: " << isValidSection << endl;
		// 	return isValidRow && isValidColumn && isValidSection;
		// }

		// bool isComplete() {
		// 	int i;
		// 	for (i = 0; i < N; i++) {
		// 		int j;
		// 		for (j = 0; j < N; j++) {
		// 			if (board[i * N + j]->getValue() == 0) {
		// 				return false;
		// 			}
		// 		}
		// 	}
		// 	return true;
		// }
};

typedef SDL_Texture* Texture;

SDL_Renderer* renderer;

Texture number_0_image, number_1_image, number_2_image, number_3_image, number_4_image,
		number_5_image, number_6_image, number_7_image, number_8_image, number_9_image;

unsigned BOX_GAP = 6;
unsigned BOX_WIDTH = 60;
unsigned BOX_HEIGHT = 60;
unsigned WINDOW_WIDTH = BOX_WIDTH * N + BOX_GAP * (N+1) + 2 * BOX_GAP;
unsigned WINDOW_HEIGHT = BOX_HEIGHT * N + BOX_GAP * (N+1) + 2 * BOX_GAP;

Texture loadImage(char* image) {
   SDL_Surface *loadedImage = IMG_Load(image);
   if(!loadedImage) {
      cout << "Failed to load image: " << SDL_GetError() << endl;
      SDL_Quit();
      exit(1);
   }
   Texture texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
   SDL_FreeSurface(loadedImage);
   if(!texture) {
      cout << "Failed to create texture: " << SDL_GetError() << endl;
      SDL_Quit();
      exit(1);
   }
   return texture;
}

void displayTexture(Texture t, unsigned x, unsigned y, unsigned width, unsigned height, SDL_RendererFlip flip){
   SDL_Rect tex_size;
   tex_size.x = 0;
   tex_size.y = 0;
   SDL_Rect toplace;
   toplace.x = x;
   toplace.y = y;
   toplace.w = width;
   toplace.h = height;
   SDL_QueryTexture(t, NULL, NULL, &tex_size.w, &tex_size.h);
   SDL_RenderCopyEx(renderer,t,&tex_size,&toplace,0,NULL,flip);
}

void print_board(Sudoku *board, Texture n[10], SDL_Rect rects[N*N]) {
	int r, c;
	Row *cur;
	for (r = 0; r < N; r++) {
		cur = board->getRows()[r];
		Unit **vals;
		vals = (cur->getUnits());
		int value, box_num;
		for (c = 0; c < N; c++) {
			value = vals[c]->getValue();
			if (value != 0) {
				box_num = (r * N) + c;
				displayTexture(n[value], rects[box_num].x, rects[box_num].y, BOX_WIDTH, BOX_HEIGHT, SDL_FLIP_NONE);
			}
		}
	}

	//	int row;
	//	Row *cur;
	//	cout << "-----------" << endl;
	//	for (row = 0; row < N; row++) {
	//		cur = board->getRows()[row];
	//		Unit **vals;
	//		vals = (cur->getUnits());
	//		int col;
	//		for (col = 0; col < N; col++) {
	//			cout << vals[col]->getValue();
	//			if ((col+1) % SQRT_N == 0) {
	//				cout << " "; // Add horizontal space between sections
	//			}
	//		}
	//		cout << "\n";
	//		if ((row+1) % SQRT_N ==0) {
	//			cout << endl; // Add vertical space between sections
	//		}
	//	}
}

void calculate_section(unsigned m_x, unsigned m_y, SDL_Rect rec[N*N], unsigned coords[2]) {
	int vert, horz;
	for (vert = 0; vert < N; vert++) {
		unsigned left_edge = rec[vert].x;
		if (m_x >= left_edge && m_x <= (left_edge + BOX_WIDTH)) {
			coords[0] = vert;
			break;
		}
	}
	for (horz = 0; horz < N; horz++) {
		unsigned top_edge = rec[horz*N].y;
		if (m_y >= top_edge && m_y <= (top_edge + BOX_HEIGHT)) {
			coords[1] = horz;
			break;
		}
	}
}

Sudoku* sudoku_main() {
	// Determine desired difficulty
	cout << "Welcome to Sudoku.  What is your desired difficulty?" << endl << "The options are easy, medium, and hard." << endl;
	string difficulty;
	int diff;
	getline(cin, difficulty);

	while (difficulty != "easy" && difficulty != "medium" && difficulty != "hard") {
		cout << "Invalid difficulty, please try again." << endl;
		getline(cin, difficulty);
	}

	if (difficulty == "easy") {
		diff = EASY;
	} else if (difficulty == "medium") {
		diff = MEDIUM;
	} else if (difficulty == "hard") {
		diff = HARD;
	}

	cout << "You have chosen " << difficulty << endl << "Building your puzzle..." << endl;
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

	int board4[N*N] = {2,9,6,3,1,8,5,7,4,
					   5,8,4,9,7,2,6,1,3,
					   7,1,3,6,4,5,2,8,9,
					   6,2,5,8,9,7,3,4,1,
					   9,3,1,4,2,6,8,5,7,
					   4,7,8,5,3,1,9,2,6,
					   1,6,7,2,5,3,4,9,8,
					   8,5,9,7,6,4,1,3,2,
					   3,4,2,1,8,9,7,6,5
					   };

	int board5[N*N] = {5,3,4,6,7,8,9,1,2,
					   6,7,2,1,9,5,3,4,8,
					   1,9,8,3,4,2,5,6,7,
					   8,5,9,7,6,1,4,2,3,
					   4,2,6,8,5,3,7,9,1,
					   7,1,3,9,2,4,8,5,6,
					   9,6,1,5,3,7,2,8,4,
					   2,8,7,4,1,9,6,3,5,
					   3,4,5,2,8,6,1,7,9
					   };

	int* possibleBoards[COMPLETED_BOARDS] = {board1, board2, board3, board4, board5};

	srand(time(NULL));
	int randomIndex = rand() % COMPLETED_BOARDS;
	int* chosen;
	chosen = possibleBoards[randomIndex];

	cout << "Board chosen" << endl;

	// randomly remove numbers to reach initial state
	int row, d;
	int empty = 0;
	for (row = 0; row < N; row++) {
		int removed[diff];
		int col;
		for (d = 0; d < diff; d++) {
			col = rand() % N;
			int * found;
			found = find(removed, removed+diff, col);
			while (found != removed+diff) {
				col = rand() % N;
				found = find(removed, removed+diff, col);
			}
			removed[d] = col;
			chosen[row*N + col] = empty;
		}
	}

	Sudoku *mainBoard = new Sudoku(chosen);

	cout << "Sudoku constructed" << endl;

	return mainBoard;
	/*
	cout << "Fill in the 0's with the correct number (1-9) by inputing:" << endl;
	cout << "x-coordinate y-coordinate digit" << endl;
	cout << "(0,0) is the top left location" << endl;
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

		cout << "Attempting to set (" << x << ", " << y << ") to " << digit << endl;
		try {
			mainBoard->insert(x, y, digit);
		} catch (const OutOfBoundsException& o) {
			cout << o.what() << endl;
		} catch (const SudokuInvariantException& s) {
			cout << s.what() << endl;
		} catch (const ImmutableUnitException& i) {
			cout << i.what() << endl;
		}
	}
	cout << "Congratulations! You solved the puzzle!" << endl;
	print_board(mainBoard);
	return 0;
	*/
}

int main(int argc, char* argv[]) {
	SDL_Window* window;

	// create sudoku board
	Sudoku* sudoku_board = sudoku_main();

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			return 1;
	}

	// Create the window where we will draw.
	window = SDL_CreateWindow("Sudoku",
					SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
					WINDOW_WIDTH, WINDOW_HEIGHT,
					0);

	// We must call SDL_CreateRenderer in order for draw calls to affect this window.
	renderer = SDL_CreateRenderer(window, -1, 0);

	// Declare images
	number_0_image = loadImage("number_0_blue.png");
	number_1_image = loadImage("number_1_blue.png");
	number_2_image = loadImage("number_2_blue.png");
	number_3_image = loadImage("number_3_blue.png");
	number_4_image = loadImage("number_4_blue.png");
	number_5_image = loadImage("number_5_blue.png");
	number_6_image = loadImage("number_6_blue.png");
	number_7_image = loadImage("number_7_blue.png");
	number_8_image = loadImage("number_8_blue.png");
	number_9_image = loadImage("number_9_blue.png");

	Texture numbers[10] = {number_0_image, number_1_image, number_2_image, number_3_image, number_4_image, number_5_image, number_6_image,
							number_7_image, number_8_image, number_9_image};

	// Select the color for drawing. It is set to white here.
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	// Clear the entire screen to our selected color.
	SDL_RenderClear(renderer);

	// Draw rectangles
	SDL_Rect rectangles[81];
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	int row, col;
	for (row = 0; row < N; row++) {
		SDL_Rect box;
		box.y = row * BOX_HEIGHT + row * BOX_GAP + (row/3) * BOX_GAP + BOX_GAP;
		box.w = BOX_WIDTH;
		box.h = BOX_HEIGHT;
		for (col = 0; col < N; col++) {
			box.x = col * BOX_WIDTH + col * BOX_GAP + (col/3) * BOX_GAP + BOX_GAP;
			rectangles[row * N + col] = box;
		}
	}

	SDL_RenderFillRects(renderer, rectangles, N*N);

	// display numbers
	print_board(sudoku_board, numbers, rectangles);

	// Up until now everything was drawn behind the scenes.
	// This will show the new, red contents of the window.
	SDL_Event e;
	int quit = 0;
//	SDL_StartTextInput();
	while (!quit) {
		 while (SDL_PollEvent(&e)){
			 if (e.type == SDL_QUIT) {
				 quit = 1;
			 } else if (e.type == SDL_MOUSEBUTTONDOWN ) {
				 if (e.button.button == SDL_BUTTON_LEFT) {
					 unsigned mouse_x, mouse_y, section_x, section_y;
					 unsigned wrong = 11;
					 unsigned coordinates[2] = {wrong, wrong};
					 mouse_x = e.button.x;
					 mouse_y = e.button.y;
//					 cout << "X:" << mouse_x << " Y:" << mouse_y << endl;

					 calculate_section(mouse_x, mouse_y, rectangles, coordinates);
					 if (coordinates[0] == wrong || coordinates[1] == wrong) {
						 cout << "Invalid location. Please try again." << endl;
						 break;
					 }
					 section_x = coordinates[0];
					 section_y = coordinates[1];
					 cout << "You have chosen section (" << section_x << ", " << section_y << "). Is this correct (y/n)?" << endl;
					 string response;
					 getline(cin, response);
					 while (response != "n" && response != "y") {
						 cout << "Invalid choice. (y/n)?" << endl;
						 getline(cin, response);
					 }
					 if (response == "n") {
						 break;
					 }

					 // Read in digit
					 cout << "Desired digit?:" << endl;
					 string input;
					 int digit;
					 getline(cin, input);

					 stringstream ss(input);
					 ss >> digit;

					 try {
					 	 sudoku_board->insert(section_x, section_y, digit);
					 } catch (const OutOfBoundsException& o) {
						 cout << o.what() << endl;
					 } catch (const SudokuInvariantException& s) {
						 cout << s.what() << endl;
					 } catch (const ImmutableUnitException& i) {
						 cout << i.what() << endl;
					 }

					 print_board(sudoku_board, numbers, rectangles);
					 if (sudoku_board->isComplete()) {
						 cout << "Congratulations! You solved the puzzle!" << endl;
						 quit = 1;
					 }
				 }
			 }
		 }
		 SDL_RenderPresent(renderer);
	}
	// Give us time to see the window.
	//        SDL_Delay(1000);

	// Always be sure to clean up
	SDL_Quit();
	return 0;
}
