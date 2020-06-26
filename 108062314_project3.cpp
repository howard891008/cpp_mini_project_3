#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <algorithm>
#include <climits>
struct Point {
	int x, y;
	Point() : Point(0, 0) {}
	Point(int x, int y) : x(x), y(y) {}
	bool operator==(const Point& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Point& rhs) const {
		return !operator==(rhs);
	}
	Point operator+(const Point& rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}
	Point operator-(const Point& rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}
};


int player;
const int SIZE = 8;
const int flip_value = 10;
const int mobility_vallue = 10;
std::array<std::array<int, SIZE>, SIZE> board;
std::vector<Point> next_valid_spots;
std::array<std::array<int, SIZE>, SIZE> evval;
std::vector<int> next_valid_spots_value;
std::array<std::array<int, SIZE>, SIZE> statenow;
std::vector<Point> high_value_spots;
const std::array<Point, 8> directions{ {
		Point(-1, -1), Point(-1, 0), Point(-1, 1),
		Point(0, -1), /*{0, 0}, */Point(0, 1),
		Point(1, -1), Point(1, 0), Point(1, 1)
	} };
//set the evaluating value
void init() {
	evval[0][0] = 350;
	evval[0][1] = -60;
	evval[0][2] = 10;
	evval[0][3] = 10;
	evval[0][4] = 10;
	evval[0][5] = 10;
	evval[0][6] = -60;
	evval[0][7] = 350;
	evval[1][0] = -60;
	evval[1][1] = -80;
	evval[1][2] = 5;
	evval[1][3] = 5;
	evval[1][4] = 5;
	evval[1][5] = 5;
	evval[1][6] = -80;
	evval[1][7] = -60;
	evval[2][0] = 10;
	evval[2][1] = 5;
	evval[2][2] = 1;
	evval[2][3] = 1;
	evval[2][4] = 1;
	evval[2][5] = 1;
	evval[2][6] = 5;
	evval[2][7] = 10;
	evval[3][0] = 10;
	evval[3][1] = 5;
	evval[3][2] = 1;
	evval[3][3] = 1;
	evval[3][4] = 1;
	evval[3][5] = 1;
	evval[3][6] = 5;
	evval[3][7] = 10;
	evval[4][0] = 10;
	evval[4][1] = 5;
	evval[4][2] = 1;
	evval[4][3] = 1;
	evval[4][4] = 1;
	evval[4][5] = 1;
	evval[4][6] = 5;
	evval[4][7] = 10;
	evval[5][0] = 10;
	evval[5][1] = 5;
	evval[5][2] = 1;
	evval[5][3] = 1;
	evval[5][4] = 1;
	evval[5][5] = 1;
	evval[5][6] = 5;
	evval[5][7] = 10;
	evval[6][0] = -60;
	evval[6][1] = -80;
	evval[6][2] = 5;
	evval[6][3] = 5;
	evval[6][4] = 5;
	evval[6][5] = 5;
	evval[6][6] = -80;
	evval[6][7] = -60;
	evval[7][0] = 350;
	evval[7][1] = -60;
	evval[7][2] = 10;
	evval[7][3] = 10;
	evval[7][4] = 10;
	evval[7][5] = 10;
	evval[7][6] = -60;
	evval[7][7] = 350;
}
//return the valid spots of a board to a player
std::vector<Point> count_valid_spots(std::array<std::array<int, SIZE>, SIZE> board, int player) {
	std::vector<Point> result;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] == 0) {
				Point p(i, j);
				for (int k = 0; k < 8; k++) {
					Point tmp = p + directions[k];
					int flag = 0;//the first spots near is different color or not
					if (tmp.x <= 7 && tmp.y <= 7 && tmp.x >= 0 && tmp.y >= 0) {
						while (board[tmp.x][tmp.y] == 3 - player) {
							tmp = tmp + directions[k];
							if (tmp.x > 7 || tmp.y > 7 || tmp.x < 0 || tmp.y < 0) {
								tmp = tmp - directions[k];
								break;
							}
							flag = 1;
						}
					}
					if (tmp.x > 7 || tmp.y > 7 || tmp.x < 0 || tmp.y < 0)
						continue;
					if (board[tmp.x][tmp.y] == player && flag == 1) {
						result.push_back(p);
					}
				}
			}
		}
	}
	return result;
}
//return the board after putting a disc
std::array<std::array<int, SIZE>, SIZE> flip_board(std::array<std::array<int, SIZE>, SIZE> board, Point p, int player) {
	std::array<std::array<int, SIZE>, SIZE> newboard = board;
	newboard[p.x][p.y] = player;
	for (int i = 0; i < 8; i++) {
		std::vector<Point> to_flip;
		Point tmp = p + directions[i];
		if (tmp.x <= 7 && tmp.y <= 7 && tmp.x >= 0 && tmp.y >= 0) {
			while (board[tmp.x][tmp.y] == 3 - player) {
				tmp = tmp + directions[i];
				if (tmp.x > 7 || tmp.y > 7 || tmp.x < 0 || tmp.y < 0) {
					tmp = tmp - directions[i];
					break;
				}
				to_flip.push_back(Point(tmp.x, tmp.y));
			}
		}
		if (tmp.x > 7 || tmp.y > 7 || tmp.x < 0 || tmp.y < 0)
			continue;
		if (board[tmp.x][tmp.y] == player) {
			for (auto c : to_flip) {
				newboard[c.x][c.y] = player;
			}
		}
	}
	return newboard;
}
//return the state value of a board to a player
int state_value(std::array<std::array<int, SIZE>, SIZE> board, int player) {
	std::array<std::array<int, SIZE>, SIZE> spot_value = evval;
	int value=0;
	if (board[0][0] == player) {
		spot_value[0][1] *= -1;
		spot_value[1][0] *= -1;
		spot_value[1][1] *= -1;
	}
	if (board[0][7] == player) {
		spot_value[0][6] *= -1;
		spot_value[1][7] *= -1;
		spot_value[1][6] *= -1;
	}
	if (board[7][0] == player) {
		spot_value[7][1] *= -1;
		spot_value[6][0] *= -1;
		spot_value[6][1] *= -1;
	}
	if (board[7][7] == player) {
		spot_value[7][6] *= -1;
		spot_value[6][7] *= -1;
		spot_value[6][6] *= -1;
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] == player)
				value += spot_value[i][j];
			else if (board[i][j] == 3 - player)
				value -= spot_value[i][j];
		}
	}
	std::vector<Point> valid_spots = count_valid_spots(board, 3 - player);
	int mobility = valid_spots.size();
	value -= mobility * mobility_vallue;
	return value;
}
//minimax search and alpha-beta pruning
int search(std::array<std::array<int, SIZE>, SIZE> board, int dist, int alpha, int beta, int last, bool maximize) {
	std::vector<Point> child_spots = count_valid_spots(board, 3-last);
	if (dist >= 3||child_spots.size()==0)
		return state_value(board, player);
	if (maximize) {
		for (auto c : child_spots) {
			std::array<std::array<int, SIZE>, SIZE> newboard = flip_board(board, c, 3-last);
			alpha = std::max(alpha, search(newboard, dist + 1, alpha, beta, 3-last, false));
			if (beta <= alpha)
				break;
		}
		return alpha;
	}
	else {
		for (auto c : child_spots) {
			std::array<std::array<int, SIZE>, SIZE> newboard = flip_board(board, c, 3-last);
			beta = std::min(beta, search(newboard, dist + 1, alpha, beta, 3-last, true));
			if (beta <= alpha)
				break;
		}
		return beta;
	}
}
void read_board(std::ifstream& fin) {
	fin >> player;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			fin >> board[i][j];
		}
	}
}

void read_valid_spots(std::ifstream& fin) {
	int n_valid_spots;
	fin >> n_valid_spots;
	int x, y;
	for (int i = 0; i < n_valid_spots; i++) {
		fin >> x >> y;
		next_valid_spots.push_back({ x, y });
	}
}

void write_valid_spot(std::ofstream& fout) {
	int n_valid_spots = next_valid_spots.size();
	srand(time(NULL));
	int maxval = INT_MIN;
	// Remember to flush the output to ensure the last action is written to file.
	for (int i = 0; i < n_valid_spots; i++) {
		Point p = next_valid_spots[i];
		std::array<std::array<int, SIZE>, SIZE> newboard = flip_board(board, p, player);
		int value = search(newboard, 0, INT_MIN, INT_MAX, player, false);
		if (value > maxval) {
			maxval = value;
			fout << p.x << " " << p.y << std::endl;
			fout.flush();
		}
	}
}

int main(int, char** argv) {
	std::ifstream fin(argv[1]);
	std::ofstream fout(argv[2]);
	init();
	read_board(fin);
	read_valid_spots(fin);
	write_valid_spot(fout);
	fin.close();
	fout.close();
	return 0;
}
