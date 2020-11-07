#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include<thread>
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
	short run = 1;
	olc::Sprite* board;
	olc::Sprite* player_x;
	olc::Sprite* player_o;
	
	char player_mat[3][3] = { {NULL,NULL, NULL},{NULL,NULL, NULL},{NULL,NULL, NULL} };

public:
	void computers_move() {
		for (int i = 0; i != 3; i++) {
			for (int j = 0; j != 3; j++) {
				if (player_mat[i][j] == NULL) {
					player_mat[i][j] = 'O';
					return;
				}
			}
		}
	}
	bool draw_players() {
		for (int i = 0; i != 3; i++) {
			for (int j = 0; j != 3; j++) {
				if (player_mat[i][j] != NULL) {
					if(player_mat[i][j]=='X')
						DrawSprite((i * 200)+50,( j * 200)+50, player_x, 1);
					else 
						DrawSprite((i*200)+50, (j*200)+50, player_o, 1);
				}
			}
		}
		return true;
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
		for (int x = 0; x < 3; x++)
			for (int y = 0; y < 3; y++)
				player_mat[x][y] = NULL;
	}
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		ClearBoard();
		//loading sprites
		board = new olc::Sprite("board.png");
		player_x = new olc::Sprite("x.png");
		player_o= new olc::Sprite("o.png");

		//draw board
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(255,255,255));

		SetPixelMode(olc::Pixel::ALPHA); 
		DrawSprite(0, 0, board, 1);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		int mouse_x = GetMouseX();
		int mouse_y = GetMouseY();

		//getting user input
		if ((GetMouse(0).bHeld)) {
			int x = mouse_x / 200;
			int y = mouse_y / 200;
			if (!run) {
				sleep_until(system_clock::now() + 5s);
				return false;
			}

			if (player_mat[x][y] == NULL) {
				player_mat[x][y] = 'X';
				if (check('X')) {
					draw_players();
					DrawString(250, 200, "X has won!");
					std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!  X has WON  Congralutations  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
					run--;
				}
				computers_move();
				if (check('O')) {
					draw_players();
					DrawString(250, 200, "O has won!");
					std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!  O has WON  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
					run--;
				}
			}
			draw_players();
		}

				
		return true;
		
	}
};


int main()
{
	TicTacToe  demo;
	if (demo.Construct(600, 600, 1, 1))
		demo.Start();

	std::cout << "Type Y to play again" << std::endl;
	char player_choice;
	std::cin >> player_choice;

	if (player_choice == 'Y' || player_choice == 'y')
		if (demo.Construct(600, 600, 1, 1))
			demo.Start();

	return 0;
}
