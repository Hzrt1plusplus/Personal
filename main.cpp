#define PDC_WIDE
#include "PDCurses-3.8/curses.h"
#include <stdlib.h>
#include <time.h>
#include <string>
#define SCREEN_W (COLS / 4)
#define SCREEN_H (LINES / 3)
static int VERTICAL_DELAY = static_cast<int>(3000.0f / SCREEN_H);
static int HORIZONTAL_DELAY = static_cast<int>(6000.0f / SCREEN_W);
void update_delays()
{
	VERTICAL_DELAY = static_cast<int>(3000.0f / (COLS / 4));
	HORIZONTAL_DELAY = static_cast<int>(6000.0f / (LINES / 3));
}
inline int max(int a, int b) noexcept
{
	if (a > b)
		return a;
	return b;
}
inline int min(int a, int b) noexcept
{
	if (a < b)
		return a;
	return b;
}
inline int _abs(int a) noexcept
{
	if (a >= 0)
		return a;
	return -a;
}
bool has_collision(int x1, int y1, int x2, int y2, int w1 = 4, int h1 = 3, int w2 = 4, int h2 = 3)
{
	int _h = min(h1, h2);
	int _w = min(w1, w2);
	int delta_x = _abs(x1 - x2);
	int delta_y = _abs(y1 - y2);
	return (delta_x <= _w && delta_y <= _h);
}
void game_over()
{
	const char *BANNER[] = {

		                                                                                                                        
                          "     +#@@@@@@@@@.          ##@@@@@@       ##@@@@@@        #@*@%@#@ ##@@@@@@@@@@@@",                           
                          "   ##@@@@@@@@@@@@@@       ##@@@@@@@@      ##@@@@@@@      #+@*@@@@@ ##@@@@@@@@@@@@",                           
                          "  ##@@@@@@@@@@@@@@@@     :#@@@@@@@@@-     ##@@@@@@@@    ##@*@@@@@@ ##@@@@@       ",                           
                          " ##@@@@@                 ##@@@@%@@@@@.    ##@@@@@@@@.. =#@*@@@@@@@ ##@@@@@        ",                          
                          " ##@@@@..               ##@@@@@ *@@@@@.   ##@@@@@@@@@  ##@@@@@@@@@ ##@@@@@@@@@@@.",                           
                          " ##@@@@  ##@@@@@@@@@@  ##@@@@@. #@@@@@+   ##@@@@@=@@@% #@@@@ @@@@@ ##@@@@@@@@@@@.",                           
                          " ##@@@@@ ##@@@@@@@@@@  ##@@@@@  ##@@@@@   ##@@@@@ @@@@+@@@@: @@@@@ ##@@@@@.      ",                           
                          " :#@@@@@@:    #%@@@@%. #@@@@@@@@@@@@@@@@. *#@@@@@. @@@@@@@@  @@@@@  ##@@@@=      ",                           
                          "  ##@@@@@@@@@@@@@@@: ##@@@@@@@@@@@@@@@@@-.##@@@@@ #@@@@@@@  #@@@@@ ##@@@@@@@@@@@@",                        
                          "    ##@@@@@@@@@@@+   ##@@@@@      ##@@@@@  #@@@@@ ##@@@@@: ##@@@@@ ##@@@@@@@@@@@@",                           
                          "         ::-                                                                     ",                           
                          "                                                                                 ",                           
                          "                                                                                 ",                           
                          "                                                                                 ",                           
                          "     ##@@@@@@@@@@@#.   ##@@@@@@.      #@@@@@@  #@@@@@@@@@@@@@  #@@@@@@@@@@@@@@   ",                           
                          "   ##@@@@@@@@@@@@@@@@   ##@@@@@-     ##@@@@@@.##@@@@@@@@@@@@@  #@@@@@@@@@@@@@@@@.",                           
                          "  #@@@@@@@@  #@@@@@@@@. *#@@@@@@    .#@@@@@@  ##@@@@@.        ##@@@@@+  ##@@@@@@.",                           
                          " ##@@@@@@.     ##@@@@@@  ##@@*#@@   ##@@@@@:  ##@@@@@:        ##@@@@@+   ##@@@@@*",                           
                          "##@@@@@@        #@@@@@@   #%##@*@  ##@@@@@@   ##@@@@@@@@@@@@. ##@@@@@+  ##@@@@@@ ",                           
                          "##@@@@@:        #%@@@@:   #*%@@@@: #%@@@@+    ##@@@@@@@@@@@-. ##@@@@@@@@@@@@@@=  ",                           
                          " #@@@@@@       ##@@@@@@    *#@@@@@..@@@@@%    ##@@@@@:        ##@@@@@@@@@@@@..   ",                           
                          " ##@@@@@@.    ##@@@@@@.     #@@@@@@%@@@@@     ##@@@@@:        ##@@@@@* @@@@@@    ",                           
                          "    ==*++**+===+++++--.       ==++++***+-:      +=+*%:.         ==***+.  =**++.. ",                           
                          "   .#@@@@@@@@@@@@@@%         ##@@@@@@@@@.     ##@@@@@@@@@@@@@  #@@@@@+ ##@@@@@@ .",                           
                          "      +#@@@@@@@@@:           +#@@@@@@@@       ##@@@@@@@@@@@@@  #@@@@@*  ##@@@@@@ "                           
                                                                                                              
	};

	int i = 1;
	int X = (COLS - 81) / 2;
	int Y = (LINES - 25) / 2;


	while (true)
	{
		attron(COLOR_PAIR(i));
		for (int j = 0; j < 25; j++)
			mvprintw(Y + j, X, BANNER[j]);
		mvprintw(Y + 30, (COLS - 35) / 2, "Press 'R' or 'r' to restart please!");
		attroff(COLOR_PAIR(i));
		refresh();
		int c = getch();
		if (c == 'R' || c == 'r')
			return;
		napms(300);
		i += 1;
		if (i == 4)
			i = 1;
	}
}

enum Direction
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Point
{
	int x, y;

public:
	Point()
	{
		spawn();
	}
	inline void spawn()
	{
		x = rand() % SCREEN_W;
		y = rand() % SCREEN_H;
	}
	bool control(int x0, int y0) const
	{
		return has_collision(x0, y0, x*4, y*3);
	}
	void render() const
	{
		attron(COLOR_PAIR(4));
		mvprintw(y*3, x*4, "XXXX");
		mvprintw(y*3 + 1, x*4, "XXXX");
		mvprintw(y*3 + 2, x*4, "XXXX");
		attroff(COLOR_PAIR(4));
	}
};

class Snake
{
public:
	int blocks[256][2];
	int length;
	Direction dir;
	Snake() : length(1), dir{RIGHT}
	{
		blocks[0][0] = 5;
		blocks[0][1] = 5;
	}
	bool collapsed() const noexcept
	{
		for (int i = 4; i < length; i++)
			if (has_collision(blocks[0][0], blocks[0][1], blocks[i][0], blocks[i][1]))
				return true;
		return false;
	}
	void change_dir(Direction nd)
	{
		if (dir == RIGHT || dir == LEFT)
		{
			if (nd != RIGHT && nd != LEFT)
				dir = nd;
		}
		if (dir == UP || dir == DOWN)
		{
			if (nd != UP && nd != DOWN)
				dir = nd;
		}
	}
	void enlarge()
	{
		blocks[length][0] = (blocks[length - 1][0] - 4);
		blocks[length][1] = blocks[length - 1][1];
		length++;
	}
	void update()
	{
		for (int i = length - 1; i >= 1; i--)
		{
			blocks[i][0] = blocks[i - 1][0];
			blocks[i][1] = blocks[i - 1][1];
		}
		switch (dir)
		{
		case RIGHT:
			blocks[0][0] += 3;
			if (blocks[0][0] >= (COLS - 4))
				blocks[0][0] = 0;
			break;
		case LEFT:
			blocks[0][0] -= 3;
			if (blocks[0][0] <= -4)
				blocks[0][0] = (COLS - 4);
			break;
		case UP:
			blocks[0][1] -= 3;
			if (blocks[0][1] <= -3)
				blocks[0][1] = (LINES - 4);
			break;
		case DOWN:
			blocks[0][1] += 3;
			if (blocks[0][1] >= LINES)
				blocks[0][1] = 0;
			break;
		}
	}
	void render() const
	{
		attron(COLOR_PAIR(6));
		for (int i = 0; i < length; i++)
		{
			mvprintw(blocks[i][1], blocks[i][0], "$$$$");
			mvprintw(blocks[i][1] + 1, blocks[i][0], "$$$$");
			mvprintw(blocks[i][1] + 2, blocks[i][0], "$$$$");
		}
		attroff(COLOR_PAIR(6));
	}
};

int show_start_screen()
{
	clear();
	int x = (COLS - 20) / 2;
	int y = (LINES - 2) / 2;
	attron(COLOR_PAIR(2));
	mvprintw(y, x, "1. Start");
	attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(1));
	mvprintw(y + 1, x, "2. Exit");
	attroff(COLOR_PAIR(1));
	refresh();
	while (true)
	{
		int c = getch();
		switch (c)
		{
		case '1':
			return 1;
		case '2':
			return 2;
		}
		napms(50);
	}
}

int gameloop()
{
	Point p{};
	Snake s{};
	int score = 0;
	bool running = true;
	while (running)
	{
		clear();
		int c = getch();
		switch (c)
		{
		case 'q':
			running = false;
			break;
		case KEY_LEFT:
			s.change_dir(LEFT);
			break;
		case KEY_RIGHT:
			s.change_dir(RIGHT);
			break;
		case KEY_UP:
			s.change_dir(UP);
			break;
		case KEY_DOWN:
			s.change_dir(DOWN);
			break;
		}
		if (p.control(s.blocks[0][0], s.blocks[0][1]))
		{
			p.spawn();
			s.enlarge();
			score++;
		}
		if (s.collapsed())
			return 2;
		s.update();
		p.render();
		s.render();
		attron(COLOR_PAIR(7));
		std::string score_str = std::string("Score: ") + std::to_string(score);
		mvprintw(0, 0, score_str.c_str());
		attroff(COLOR_PAIR(7));
		refresh();
		if (s.dir == RIGHT || s.dir == LEFT)
			napms(80); // FPS = 1000 / HORIZONTAL_DELAY
		else
			napms(160);
	}
	return 0;
}

int main()
{
	srand(time(NULL));
	initscr();
	notimeout(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	noecho();
	keypad(stdscr, TRUE);
	start_color();
	curs_set(0);
	init_pair(1, COLOR_RED, COLOR_BLACK); // BG - BLACK, FG - RED
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_BLACK, COLOR_RED); // vice versa
	init_pair(5, COLOR_BLACK, COLOR_GREEN);
	init_pair(6, COLOR_BLACK, COLOR_BLUE);
	init_pair(7, COLOR_YELLOW, COLOR_BLACK);
	while (true)
	{
		int code = show_start_screen();
		if (code == 2)
			break;
		int c2 = gameloop();
		if (c2 == 0)
			break;
		game_over();
	}
	endwin();
}