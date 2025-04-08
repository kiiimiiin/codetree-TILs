#include <iostream>
#include <tuple>
#include <algorithm>
#include <utility>
#include <queue>
#define X first
#define Y second
using namespace std; 
const int dx[4] = { -1,1,0,0 };
const int dy[4] = { 0,0,-1,1 };
int n, m, k, t, ex , ey;
int board[11][11]; // 벽, 출구
int person[11][11];

bool OOB(int x, int y) {
	return x < 0 || x >= n || y < 0 || y >= n;
}

int DistFromExit(int x, int y) {
	return abs(x - ex) + abs(y - ey);
}
int SubMove(int sx, int sy, int tmp[11][11]) {

	int nx, ny;
	int stFromExit = DistFromExit(sx, sy);
	int ret = 0;
	for (int dir = 0; dir < 4; dir++) {
		nx = sx + dx[dir];
		ny = sy + dy[dir];
		if (OOB(nx, ny) || board[nx][ny]) continue;
		if (DistFromExit(nx, ny) < stFromExit) {
			ret += person[sx][sy];
			if(!(nx == ex && ny == ey))
				tmp[nx][ny] += person[sx][sy];
			person[sx][sy] = 0;
			break;
		}
	}

	return ret;
	
}
int Move() {
	int ret = 0;
	int tmp[11][11] = {};

	bool flag = false;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (person[i][j] == 0) continue;
			ret += SubMove(i, j, tmp);
			flag = true;
		}
	}

	if (flag == false) return -1;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			person[i][j] += tmp[i][j];


	return ret;
}

bool ExitInRec(int i, int j, int size) {
	return ( ex >= i && ex <= i + size - 1)
		&& ( ey >= j && ey <= j + size - 1);
}

bool PersonInRec(int x, int y, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (person[i + x][j + y]) return true;
		}
	}
	return false;
}

tuple<int, int, int> FindRec() {
	int x, y, size;
	for (size = 2; size <= n; size++) {
		for (x = 0; x <= n - size; x++) {
			for (y = 0; y <= n - size; y++) {
				if (ExitInRec(x, y, size) && PersonInRec(x, y, size))
					return make_tuple(x, y, size);
			}
		}
	}
	return make_tuple(-1, -1, -1);
}
void Rotate() {
	int x, y, size;
	tie(x, y, size) = FindRec();

	int cBoard[11][11], cPerson[11][11];
	int nex, ney;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i + x == ex && j + y == ey) {
				nex = j + x;
				ney = size - 1 - i + y;
			}
			cBoard[j][size - 1 - i] 
				= board[i + x][j + y] ? board[i + x][j + y] - 1 : 0;
			cPerson[j][size - 1 - i] = person[i + x][j + y];
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			board[i + x][j + y] = cBoard[i][j];
			person[i + x][j + y] = cPerson[i][j];
		}
	}

	ex = nex;
	ey = ney;
}
int main() {
	ios::sync_with_stdio(0), cin.tie(0);

	cin >> n >> m >> k;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> board[i][j];
	
	int x, y;
	while (m--) {
		cin >> x >> y;
		person[x - 1][y - 1]++;
	}
	cin >> ex >> ey;
	ex--; ey--;
	
	int distSum = 0;
	for(int t = 1; t <= k; t++){
		int dist = Move();
		if (dist < 0) break;
		distSum += dist;
		Rotate();
	}

	cout << distSum << '\n' << ex + 1 << ' ' << ey + 1;
}

/*

	81 * 4, 64 * 9 , 49 * 16, 36 * 25, 25 * 36 , 16 * 49, 8 ,4, 1

	300

*/