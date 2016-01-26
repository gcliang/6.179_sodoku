#include <iostream>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <set>
#include <ctime>
#include "sudoku.h"
#include "graphics.h"
// #include "SDL2/SDL_image.h"
// #include "SDL2/SDL_mixer.h"


using namespace std;

const int N = 9;
const int SQRT_N = 3;
const int EASY = 5;
const int MEDIUM = 6;
const int HARD = 7;
const int COMPLETED_BOARDS = 3;

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
			units[x] = unit;
			if (unit->getValue() != 0) {
				values.insert(unit->getValue());
			}
		}

		bool isValid(int x, int value) {
  			set<int>::iterator it;
			it = values.find(value);
			if (it != values.end()) {
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
			if (unit->getValue() != 0) {
				values.insert(unit->getValue());
			}
		}

		bool isValid(int y, int value) {
  			set<int>::iterator it;
			it = values.find(value);
			if (it != values.end()) {
				return units[y]->getValue() == value;
			}
			return true;
		}
};

class Section {
	private:
		Unit *units[SQRT_N * SQRT_N];
		set<int> values;

		int calculateIdx(int x, int y) {
			return (y % SQRT_N) * SQRT_N + (x % SQRT_N);
		}

	public:
		Section() {
			int i;
			for (i = 0; i < SQRT_N; i++) {
				int j;
				for (j = 0; j < SQRT_N; j++) {
					units[i * SQRT_N + j] = NULL;
				}
			}
		}
		
		Unit **getUnits() {
			return units;
		}

		void insert(int x, int y, Unit *unit) {
			units[calculateIdx(x, y)] = unit;
			if (unit->getValue() != 0) {
				values.insert(unit->getValue());
			}
		}

		bool isValid(int x, int y, int value) {
  			set<int>::iterator it;
			it = values.find(value);
			if (it != values.end()) {
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
			return (i / SQRT_N) * SQRT_N + (j / SQRT_N);
		}

		void setSections() {
			int k;
			for (k = 0; k < N; k++) {
				Section *section = new Section();
				sections[k] = section;
			}
			cout << "Setup sections" << '\n';
			int i;
			for (i = 0; i < N; i++) {
				int j;
				for (j = 0; j < N; j++) {
					int currentSection = calculateSection(i, j);
					sections[currentSection]->insert(j, i, board[i * N + j]);
				}
				cout << "Populated one section" << '\n';
			}
		}

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
				bool isValidRow = getRows()[y]->isValid(x, value);
				bool isValidColumn = getColumns()[x]->isValid(y, value);
				bool isValidSection = sections[calculateSection(y, x)]->isValid(x, y, value);
				if (!isValidRow) {
					throw rowInvariantException;
				} else if (!isValidColumn) {
					throw columnInvariantException;
				} else if (!isValidSection) {
					throw sectionInvariantException;
				}
			}
			cout << "Before Insert" << endl;
			board[y * N + x]->setValue(value);
			cout << "After Setting Value" << endl;
			getRows()[y]->insert(x, board[y * N + x]);
			cout << "After Row Insert" << endl;
			getColumns()[x]->insert(y, board[y * N + x]);
			cout << "After Column Insert" << endl;
			getSections()[calculateSection(y, x)]->insert(x, y, board[y * N + x]);
			cout << "After Section Insert" << endl;
		}

		bool isValid(int x, int y, int value) {
			bool isValidRow = getRows()[y]->isValid(x, value);
			bool isValidColumn = getColumns()[x]->isValid(y, value);
			bool isValidSection = sections[calculateSection(y, x)]->isValid(x, y, value);
			cout << "isValidRow: " << isValidRow << '\n';
			cout << "isValidColumn: " << isValidColumn << '\n';
			cout << "isValidSection: " << isValidSection << '\n';
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
};

void print_board(Sudoku *board) {
	int row;
	Row *cur;
	cout << "-----------" << endl;
	for (row = 0; row < N; row++) {
		cur = board->getRows()[row];
		Unit **vals;
		vals = (cur->getUnits());
		int col;
		for (col = 0; col < N; col++) {
			cout << vals[col]->getValue();
			if ((col+1) % SQRT_N == 0) {
				cout << " "; // Add horizontal space between sections
			}
		}
		cout << "\n";
		if ((row+1) % SQRT_N ==0) {
			cout << endl; // Add vertical space between sections
		}
	}
}

int sudoku_main() {
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

	int* possibleBoards[COMPLETED_BOARDS] = {board1, board2, board3};

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
}

typedef SDL_Texture* Texture;

unsigned window_x = 800;
unsigned window_y = 600;

unsigned window_start_x = 200;
unsigned window_start_y = 100;

char* window_name = "Sudoku";

double player1 = 0.5;
double player2 = 0.5;
double player_height = .15;
double player_width  = .03;
double player_speed = 0.01;

int player1_up = 0;
int player2_up = 0;
int player1_down = 0;
int player2_down = 0;

SDL_Renderer *renderer;

Texture loadImage(char* image){
   SDL_Surface *loadedImage = IMG_Load(image);
   if(!loadedImage) {
      printf("Failed to load image: %s\n", SDL_GetError() );
      SDL_Quit();
      exit(1);
   }
   Texture texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
   SDL_FreeSurface(loadedImage);
   if(!texture) {
      printf("Failed to create texture: %s\n", SDL_GetError() );
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

Texture background_image, player_image;

int main(int argc, char* argv[] ){
   if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
      printf("SDL_Init failed: %s\n", SDL_GetError());
      return 1;
   }

   SDL_Window *window = SDL_CreateWindow(window_name,window_start_x,window_start_y, window_x,window_y,SDL_WINDOW_SHOWN);
   if (!window){
      printf("Window creation failed: %s\n", SDL_GetError());
      SDL_Quit();
      return 1;
   }

   renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
   if (!renderer){
      printf("Renderer creation failed: %s\n", SDL_GetError());
      SDL_Quit();
      return 1;
   }

   if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) {
        printf("Audio creation failed %s\n", Mix_GetError());
        return 1;
   }

   Mix_Chunk *laser = Mix_LoadWAV("laser.wav");
   if(!laser){
        printf("Could not open sound effect %s\n", Mix_GetError());
   }

   background_image = loadImage("snowcow.png");
   player_image = loadImage("player_image2.png");

   //Our event structure
   SDL_Event e;
   int quit = 0;
   while (!quit){
      while (SDL_PollEvent(&e)){
         if (e.type == SDL_QUIT) quit = 1;
         else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            switch(e.key.keysym.sym){
               case SDLK_1:
                  if(e.type == SDL_KEYDOWN) Mix_PlayChannel( -1, laser, 0 ); break;
               case SDLK_UP:
                  player2_down = (e.type == SDL_KEYDOWN); break;
               case SDLK_DOWN:
                  player2_up = (e.type == SDL_KEYDOWN); break;
               case SDLK_w:
                  player1_down = (e.type == SDL_KEYDOWN); break;
               case SDLK_s:
                  player1_up = (e.type == SDL_KEYDOWN); break;
            }
         }
      }

      player1+= (player1_up - player1_down) * player_speed;
      if(player1 > 1.0 - player_height) player1 = 1.0 - player_height ;
      else if(player1 < 0) player1 = 0;

      player2+= (player2_up - player2_down) * player_speed;
      if(player2 > 1.0 - player_height ) player2 = 1.0 - player_height;
      else if(player2 < 0) player2 = 0;


      SDL_RenderClear(renderer);

      SDL_SetRenderDrawColor(renderer,255,255,255,255);
      SDL_Rect rectangle;
      rectangle.x = 0;
      rectangle.y = 0;
      rectangle.w = window_x;
      rectangle.h = window_y;
      SDL_RenderFillRect(renderer, &rectangle);

      displayTexture(background_image, 200, 200, 200, 200, SDL_FLIP_NONE);
      displayTexture(player_image, 0, (int)((player1)*window_y),
         (int)(player_width*window_x), (int)(player_height*window_y),SDL_FLIP_NONE);
      displayTexture(player_image, window_x-(int)(player_width*window_x), (int)((player2)*window_y),
         (int)(player_width*window_x), (int)(player_height*window_y),SDL_FLIP_HORIZONTAL);
      SDL_RenderPresent(renderer);
   }
   return 0;
}
