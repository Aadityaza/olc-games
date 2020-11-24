#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include<thread>
#include<iostream>
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

class TicTacToe : public olc::PixelGameEngine
{
public:
	TicTacToe()
	{
		sAppName = "Example";
	}
private:
	olc::Sprite* board;
	olc::Sprite* player_x;
	olc::Sprite* player_o;
	olc::Sprite* Game_over;
	olc::Sprite* You_won;
	olc::Sprite* Play_again;
	olc::Sprite* exit;

	bool run;	
	char player_mat[3][3];

public:
	void draw_players() {
		for (int i = 0; i != 3; i++) {
			for (int j = 0; j != 3; j++) {
				if (player_mat[i][j] != NULL) {
					if (player_mat[i][j] == 'X')
						DrawSprite((i * 200) + 50, (j * 200) + 50, player_x, 1);
					else
						DrawSprite((i * 200) + 50, (j * 200) + 50, player_o, 1);
				}
			}
		}
		
	}
	bool check(char player) {
		for (int i = 0; i != 3; i++) {
			if (player_mat[i][0] == player_mat[i][1] && player_mat[i][0] == player_mat[i][2] && player_mat[i][0] == player) {
				return true;
			}
		}
		for (int j = 0; j != 3; j++) {
			if (player_mat[0][j] == player_mat[1][j] && player_mat[0][j] == player_mat[2][j] && player_mat[0][j] == player) {
				return true;
			}
		}
		if (player_mat[0][0] == player_mat[1][1] && player_mat[0][0] == player_mat[2][2] && player_mat[0][0] == player) {
			return true;
		}
		if (player_mat[0][2] == player_mat[1][1] && player_mat[0][2] == player_mat[2][0] && player_mat[0][2] == player) {
			return true;
		}
		return false;
	}
	void ClearBoard() {
		for (int x = 0; x < 3; x++) {
			for (int y = 0; y < 3; y++) {
				player_mat[x][y] = NULL;
			}
		}
		run = true;
		//drawing empty board
		Clear(olc::WHITE);
		SetPixelMode(olc::Pixel::ALPHA);
		DrawSprite(0, 0, board, 1);
	}
	bool move_left() {
		for (int i = 0; i != 3; i++) {
			for (int j = 0; j != 3; j++){
				if (player_mat[i][j] == NULL) {
					return true;
				}
			}
		}
		 return false;

	}
	int MinMax(bool isMaxPlayer) { //returns the value of the board
		
		if (check('O')) return 1;
		if (check('X')) return -1;
		if (move_left()==false) return 0;

		if (isMaxPlayer) {
			int best = -100000;
			for (int i = 0; i != 3; i++) {
				for (int j = 0; j != 3; j++) {
					if (player_mat[i][j] == NULL) {
						player_mat[i][j] = 'X';
						best = max(best, MinMax(false));//recursively find outs best value;
						player_mat[i][j] = NULL; //undo change made in board
					}
				}
			}
			return best;
		}
		else {
			int best = 100000;
			for (int i = 0; i != 3; i++) {
				for (int j = 0; j != 3; j++) {
					if (player_mat[i][j] == NULL) {
						player_mat[i][j] = 'O';
						best = min(best, MinMax(true));//recursively find outs best value;
						player_mat[i][j] = NULL; //undo change made in board
					}
				}
			}
			return best;
		}
	}
	void computers_move() {
		int best_value = -1000;
		int best_move=-1;
		
		for (int i = 0; i != 3; i++) {
			for (int j = 0; j != 3; j++) {
				if (player_mat[i][j] == NULL) {
					player_mat[i][j] = 'O';
					int move_value = MinMax(true);//O is maximixing player;
					if (move_value > best_value) {
						best_value = move_value;
						best_move = i * 3 + j;//converting 2D representation to 1D
					}
					player_mat[i][j] = NULL; //undo change made in board
				}
			}
		}
		player_mat[best_move / 3][best_move % 3]='O'; //making best move //again converting 1D indexing to 2D
		std::cout << "best move is :" << best_move / 3 << ","<< best_move % 3 << std::endl << best_value << std::endl;
	}
	

	bool OnUserCreate() override
	{
		//loading sprites
		board =		new olc::Sprite("board.png");
		player_x =  new olc::Sprite("x.png");
		player_o =  new olc::Sprite("o.png");		
		Game_over = new olc::Sprite("Game_over.png");
		You_won = new olc::Sprite("You_won.png");
		Play_again = new olc::Sprite("Play_again.png");
		exit = new olc::Sprite("exit.png");

		// Called once at the start, so create things here
		ClearBoard();
		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		int mouse_x = GetMouseX();
		int mouse_y = GetMouseY();

		if (!run) {//checking if someone has not already won
			Clear(olc::WHITE);
								
			if (check('X')) DrawSprite(90, 100, You_won, 1);
			if (check('O'))DrawSprite(150, 100, Game_over, 1);
			
			DrawSprite(150, 450, Play_again, 1);
			if ((GetMouse(0).bReleased)) {
				if (mouse_x > 150 && mouse_x < (150 + 338) && mouse_y > 450 && mouse_y < 450 + 146) { // to check if clicked on play again
					ClearBoard();
				}
			}
			return true;
			
		}
		else {
			//getting user input
			if ((GetMouse(0).bReleased)) {

				int x = mouse_x / 200;
				int y = mouse_y / 200;

				if (player_mat[x][y] == NULL) {
					player_mat[x][y] = 'X';
					draw_players();
					if (check('X')) run = false;
					if (run && !check('O')) {
						computers_move();
						draw_players();
					}
					if (check('O')) run = false;
					if (!move_left()) run = false;
				}				
			}
		}
		return true;
	}			
		
};


int main()
{
	TicTacToe  demo;
	if (demo.Construct(600, 600, 1, 1))
		demo.Start();
	return 0;
}
