#include <iostream>
#include <vector>
#include <string>

#pragma warning (disable: 4996)
#define NEXTLINE cout<<endl;
using namespace std;
/*
2017-05-20~2017-05-29
양지용_2013726091
Visual Studio 2015_Windows 7_C++
*/

//Player's, Mino's Direction enum 
enum Direction { Up, Down, Left, Right, Stay };

//define wall, load Code
static const int LOAD = 1;
static const int WALL = 0;

int Sx, Sy;		//Start Location
int Ex, Ey;		//End Location

//벽돌 -> 0
//길 -> 1
//M -> 미노타우루스 좌표 기억
//S -> 시작지점 기억
//E -> 도착지점 기억

//map
int map[30][30];
//map width, height
int width, height;
//For Player's Load tracking
vector<char> all_Direct;

void Map_Setting(int& Px, int& Py, int& Mx, int& My, FILE* file, int map[][30]);
int find_Path(int Px, int Py, int Mx, int My, Direction direc, int flag, bool**** exMap);
int move_Minotaur(int Px, int Py, int& Mx, int& My, int Pdirec, int flag, bool**** exMap);
int approved_Minotaur_Move(int Px, int Py, int Mx, int My);

void main() {
	bool**** exMap = nullptr;
	FILE* file[5];
	for (int i = 0; i < 5; i++) {
		string tempstr = "Puzzle" + to_string(i + 1);
		tempstr += ".txt";
		const char* mapname = tempstr.c_str();
		file[i] = fopen(mapname, "r");
	}

	//Dynamic Allocation exMap
	exMap = new bool***[30];
	for (int i = 0; i < 30; i++) {
		exMap[i] = new bool**[30];
		for (int j = 0; j < 30; j++) {
			exMap[i][j] = new bool*[30];
			for (int k = 0; k < 30; k++) {
				exMap[i][j][k] = new bool[30];
				for (int m = 0; m < 30; m++) {
					exMap[i][j][k][m] = true;
				}
			}
		}
	}

	int Px, Py;		//Player Location
	int Mx, My;		//Minotaur Location
	for (int n = 0; n < 5; n++) {
		cout << "Puzzle" << n << endl;
		Map_Setting(Px, Py, Mx, My, file[n], map);

		find_Path(Px, Py, Mx, My, Stay, 2, exMap);
		for each (char entry in all_Direct)
		{
			cout << entry;
		}
		NEXTLINE;
		all_Direct.clear();

		//setting default setting_exMap
		for (int i = 0; i < 30; i++) {
			for (int j = 0; j < 30; j++) {
				for (int k = 0; k < 30; k++) {
					for (int m = 0; m < 30; m++) {
						exMap[i][j][k][m] = true;
					}
				}
			}
		}
		//setting default setting_map
		for (int i = 0; i < 30; i++) {
			for (int j = 0; j < 30; j++) {
				map[i][j] = WALL;
			}
		}
	}

	//delete exMap
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			for (int k = 0; k < 30; k++)
				delete[] exMap[i][j][k];
		}
	}

	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++)
			delete[] exMap[i][j];
	}

	for (int i = 0; i < 30; i++)
		delete[] exMap[i];

	delete[] exMap;
}

//If Player dead, return 0; alived, return 1
int move_Minotaur(int Px, int Py, int& Mx, int& My, int Pdirec, int flag, bool**** exMap) {
	//수평으로 움직였을 경우 플레이어와 가까워 질 수 있는지 판단.
	//만일 가능하다면 수평으로 움직임.
	//수평으로 움직일 수 없다면 수직으로 가까워 질 수 있는지 판단.
	//만일 가능하다면 수직으로 움직임.
	//수평, 수직 모두 움직일 수 없다면 가만히 있는다.
	int tempDirection[2];
	int tempMx = Mx, tempMy = My;

	for (int i = 0; i < 2; i++) {
		tempDirection[i] = approved_Minotaur_Move( Px, Py, Mx, My);

		//enum Direction { Up, Down, Left, Right, Stay };
		switch (tempDirection[i])
		{
		case Up:
			My -= 2;
			break;
		case Right:
			Mx += 2;
			break;
		case Down:
			My += 2;
			break;
		case Left:
			Mx -= 2;
			break;
		case Stay:
			break;
		}

		//if you gotten
		if ((Px == Mx) && (Py == My)) {
			return 0;
		}
	}

	//Check non Proming_1
	//if Mino's don't Move and Player's don't Move -> non Proming
	if (tempMx == Mx && tempMy == My && tempDirection[1] == Stay && Pdirec==Stay ) {
		return 0;
	}

	//Check non Proming_2
	//if already uses Mino's Location, Player's Location -> non Proming
	if (exMap[Py][Px][My][Mx] == true) {
		exMap[Py][Px][My][Mx] = false;
		return 1;
	}
	else {
		return 0;
	}
}

int approved_Minotaur_Move(int Px, int Py, int Mx, int My) {

	//x,y좌표 차이 기억
	//양수면 오른쪽 이동 또는 아래쪽 이동
	//음수면 왼쪽 이동 또는 위쪽 이동
	int dx = Px - Mx;
	int dy = Py - My;

	if (dx > 0) {
		if (map[My][Mx + 1] == 1)
			return Right;
	}
	if (dx < 0) {
		if (map[My][Mx - 1] == 1)
			return Left;
	}
	if (dy > 0) {
		if (map[My + 1][Mx] == 1)
			return Down;
	}
	if (dy < 0) {
		if (map[My - 1][Mx] == 1)
			return Up;
	}
		return Stay;
}

int approved_Player_Move(int Px, int Py) {
	int return_value = (int)00010000;
	//map[][] == 1 -> path
	//UP * 2^3
	if (map[Py-1][Px] == 1) {
		return_value += (int)00001000;
	}
	//Down * 2^2
	if (map[Py+1][Px] == 1) {
		return_value += (int)00000100;
	}
	//Left * 2^1
	if (map[Py][Px-1] == 1) {
		return_value += (int)00000010;
	}
	//Right * 2^0
	if (map[Py][Px+1] == 1) {
		return_value += (int)00000001;
	}

	return return_value;
}

//enum Direction { Up, Down, Left, Right, Stay };
int find_Path(int Px, int Py, int Mx, int My, Direction direc, int flag, bool**** exMap) {
	
	//if arrived end Point 1, if you die return 0
	int approved_Success = 0;
	if ((Px == Ex) && (Py == Ey)) {
		return 1;
	}

	int dieOrLive = 1;
	if (flag != 2) {
		dieOrLive = move_Minotaur(Px, Py, Mx, My, direc, flag, exMap);
	}
	//Escape Function Don't find End Point
	if (dieOrLive != 1) {
		return 0;
	}

	//Value Can move
	int decision_Move;
	//x, y좌표 기억하는 Point 클래스 생성하여 Mino, Player의 좌표 기억
	//find_Path에 Px, Py를 Point 형으로 받는것으로 수정하기.
	decision_Move = approved_Player_Move(Px, Py);
	if (decision_Move & 00000010) {
		all_Direct.push_back('L');
		approved_Success = find_Path(Px - 2, Py, Mx, My, Left, 1, exMap);
		if (approved_Success == 1) return 1;
		all_Direct.pop_back();
	}
	if (decision_Move & 00001000) {
		all_Direct.push_back('U');
		approved_Success = find_Path(Px, Py - 2, Mx, My, Up, 1, exMap);
		if (approved_Success == 1) return 1;
		all_Direct.pop_back();
	}
	if (decision_Move & 00000001) {
		all_Direct.push_back('R');
		approved_Success = find_Path(Px + 2, Py, Mx, My, Right, 1, exMap);
		if (approved_Success == 1) return 1;
		all_Direct.pop_back();
	}
	if (decision_Move & 00000100) {
		all_Direct.push_back('D');
		approved_Success = find_Path(Px, Py + 2, Mx, My, Down, 0, exMap);
		if (approved_Success == 1) return 1;
		all_Direct.pop_back();
	}
	if (decision_Move & 00010000) {
		all_Direct.push_back('S');
		approved_Success = find_Path(Px, Py, Mx, My, Stay, 0, exMap);
		if (approved_Success == 1) return 1;
		all_Direct.pop_back();
	}

	/*
	switch (direc)
	{
	case Up:
		decision_Move = approved_Player_Move(Px, Py);
		if (decision_Move & 00000010) {
			all_Direct.push_back('L');
			approved_Success = find_Path(Px - 2, Py, Mx, My, Left, 1, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00001000) {
			all_Direct.push_back('U');
			approved_Success = find_Path(Px, Py - 2, Mx, My, Up, 1, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00000001) {
			all_Direct.push_back('R');
			approved_Success = find_Path(Px + 2, Py, Mx, My, Right, 1, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00000100) {
			all_Direct.push_back('D');
			approved_Success = find_Path(Px, Py + 2, Mx, My, Down, 0, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00010000) {
			all_Direct.push_back('S');
			approved_Success = find_Path(Px, Py, Mx, My, Stay,  0, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		break;
	case Down:
		decision_Move = approved_Player_Move( Px, Py);
		if (decision_Move & 00000001) {
			all_Direct.push_back('R');
			approved_Success = find_Path(Px + 2, Py, Mx, My, Right,  1, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00000100) {
			all_Direct.push_back('D');
			approved_Success = find_Path(Px, Py + 2, Mx, My, Down,  1, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00000010) {
			all_Direct.push_back('L');
			approved_Success = find_Path(Px - 2, Py, Mx, My, Left,  1, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00001000) {
			all_Direct.push_back('U');
			approved_Success = find_Path(Px, Py - 2, Mx, My, Up,  0, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00010000) {
			all_Direct.push_back('S');
			approved_Success = find_Path(Px, Py, Mx, My, Stay,  0, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		break;
	case Left:
		decision_Move = approved_Player_Move( Px, Py);
		if (decision_Move & 00000100) {
			all_Direct.push_back('D');
			approved_Success = find_Path(Px, Py + 2, Mx, My, Down,  1, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00000010) {
			all_Direct.push_back('L');
			approved_Success = find_Path(Px - 2, Py, Mx, My, Left,  1, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00001000) {
			all_Direct.push_back('U');
			approved_Success = find_Path(Px, Py - 2, Mx, My, Up,  1, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00000001) {
			all_Direct.push_back('R');
			approved_Success = find_Path(Px + 2, Py, Mx, My, Right,  0, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00010000) {
			all_Direct.push_back('S');
			approved_Success = find_Path(Px, Py, Mx, My, Stay,  0, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		break;
	case Right:
		decision_Move = approved_Player_Move( Px, Py);
		if (decision_Move & 00001000) {
			all_Direct.push_back('U');
			approved_Success = find_Path(Px, Py - 2, Mx, My, Up,  1, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00000001) {
			all_Direct.push_back('R');
			approved_Success = find_Path(Px + 2, Py, Mx, My, Right,  1, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00000100) {
			all_Direct.push_back('D');
			approved_Success = find_Path(Px, Py + 2, Mx, My, Down,  1, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00000010) {
			all_Direct.push_back('L');
			approved_Success = find_Path(Px - 2, Py, Mx, My, Left,  0, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00010000) {
			all_Direct.push_back('S');
			approved_Success = find_Path(Px, Py, Mx, My, Stay,  0, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		break;
	case Stay:
		decision_Move = approved_Player_Move( Px, Py);
		if (decision_Move & 00000010) {
			all_Direct.push_back('L');
			approved_Success = find_Path(Px - 2, Py, Mx, My, Left,  1, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00001000) {
			all_Direct.push_back('U');
			approved_Success = find_Path(Px, Py - 2, Mx, My, Up,  1, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00000001) {
			all_Direct.push_back('R');
			approved_Success = find_Path(Px + 2, Py, Mx, My, Right,  1, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00000100) {
			all_Direct.push_back('D');
			approved_Success = find_Path(Px, Py + 2, Mx, My, Down,  1, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		if (decision_Move & 00010000) {
			all_Direct.push_back('S');
			approved_Success = find_Path(Px, Py, Mx, My, Stay,  0, exMap);
			if (approved_Success == 1) return 1;
			all_Direct.pop_back();
		}
		break;
	}
	*/
	return 0;
}

void Map_Setting(int& Px, int& Py, int& Mx, int& My, FILE* file, int map[][30]) {

	char mapentry;
	if (file == nullptr) {
		cout << "not exist File\n";
		return;
	}
	//Read, width, height
	fscanf(file, "%d %d", &height, &width);

	for (int h = 0; h < height; h++) {
		for (int w = 0; w <= width; w++) {
			if (feof(file)) {
				fclose(file);
				return;
			}
			fscanf(file, "%c", &mapentry);
			switch (mapentry) {
				//Wall
			case '@':
				map[h][w] = 0;
				break;
				//Load
			case ' ':
				map[h][w] = 1;
				break;
				//start Location
			case 'S':
				map[h][w] = 1;
				Px = w; Py = h;
				break;
				//Minotaur Location
			case 'M':
				map[h][w] = 1;
				Mx = w; My = h;
				break;
				//End Location
			case 'E':
				map[h][w] = 1;
				Ex = w; Ey = h;
				break;
			case '\n':
				break;
			}
		}
	}
	fclose(file);
	return;
}
