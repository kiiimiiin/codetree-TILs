#include <iostream>
#include <utility>
#include <tuple>
#define X first
#define Y second
using namespace std;
int n, m;
int board[17][17];
int growBoard[17][17];
int dx[8] = { 0, -1, -1, -1,0, 1, 1, 1 }; // 1 3 5 7 : 우상 좌상 좌하 우하
int dy[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };// 0 2 4 6 : 우 상 좌 하


pair<int, int> ProcessOOB(int nx, int ny) {
	if (nx >= n) nx = nx - n;
	if (nx < 0) nx = n + nx;
	if (ny >= n) ny = ny - n;
	if (ny < 0) ny = n + ny;

	return { nx, ny };
}

void Move(int d, int p) {
	int cBoard[17][17] = {};

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (growBoard[i][j]) {
				int nx = i + (p % n) * dx[d];
				int ny = j + (p % n) * dy[d];
				tie(nx, ny) = ProcessOOB(nx, ny);
				cBoard[nx][ny] = growBoard[i][j];
			}
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			growBoard[i][j] = cBoard[i][j];
			if (growBoard[i][j]) board[i][j]++; // 영양제 위치 1 성장
		}
	}

}

void Grow() {
	for (int i = 0; i < n; i++) { // 영양제 위치 대각선방향 고려 성장
		for (int j = 0; j < n; j++) {
			if (growBoard[i][j]) {
				int cnt = 0;
				for (int dir : {1, 3, 5, 7}) {
					int nx = i + dx[dir];
					int ny = j + dy[dir];
					if (nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
					if (board[nx][ny] == 0) continue;
					cnt++;
				}
				board[i][j] += cnt;
			}
		}
	}
}

void ResetGrowBoard() {
	int cBoard[17][17] = {};

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (growBoard[i][j]) continue;
			if (board[i][j] >= 2) {
				board[i][j] -= 2;
				cBoard[i][j] = 1;
			}
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			growBoard[i][j] = cBoard[i][j];
		}
	}
}

int Ans() {
	int ret = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			ret += board[i][j];
		}
	}

	return ret;
}


int main(void) {
	cin >> n >> m;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}

	growBoard[n - 1][0] = growBoard[n - 2][0]
		= growBoard[n - 1][1] = growBoard[n - 2][1] = 1;

	while (m--) {
		int d, p;
		cin >> d >> p;
		Move(d-1, p);
		Grow();
		ResetGrowBoard();
	}

	int ans = Ans();
	cout << ans;
}