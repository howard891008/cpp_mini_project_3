#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Point {
	int x, y;
};

int player;
const int SIZE = 8;
std::array<std::array<int, SIZE>, SIZE> board;
std::vector<Point> next_valid_spots;
std::array<std::array<int, SIZE>, SIZE> evval;
std::vector<int> next_valid_spots_value;
std::array<std::array<int, SIZE>, SIZE> statenow;
std::vector<Point> high_value_spots;
void init() {
	evval[0][0] = 90;
	evval[0][1] = -60;
	evval[0][2] = 10;
	evval[0][3] = 10;
	evval[0][4] = 10;
	evval[0][5] = 10;
	evval[0][6] = -60;
	evval[0][7] = 90;
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
	evval[7][0] = 90;
	evval[7][1] = -60;
	evval[7][2] = 10;
	evval[7][3] = 10;
	evval[7][4] = 10;
	evval[7][5] = 10;
	evval[7][6] = -60;
	evval[7][7] = 90;
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
	int maxval = -99999;
	// Remember to flush the output to ensure the last action is written to file.
	for (int i = 0; i < n_valid_spots; i++) {
		//statenow = board;
		Point p = next_valid_spots[i];
		if (evval[p.x][p.y] > maxval) {
			maxval = evval[p.x][p.y];
		}
		if (evval[p.x][p.y] == maxval) {
			high_value_spots.push_back(p);
		}
	}
	int n_high_value_spots = high_value_spots.size();
	while (true) {
		int index = (rand() % n_high_value_spots);
		Point p = high_value_spots[index];
		fout << p.x << " " << p.y << std::endl;
		fout.flush();
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
