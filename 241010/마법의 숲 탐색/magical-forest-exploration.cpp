#include <iostream>
#include <tuple>
#include <algorithm>
#include <utility>
#include <queue>
#define X first
#define Y second
using namespace std;
int r, c, k;
const int dx[4] = { -1, 0, 1, 0 };
const int dy[4] = { 0, 1, 0, -1 };
int board[75][72];
int vis[75][72];

struct Gollem {
	int c, d;
	int score = 0;
};

Gollem gollem[1002];

bool OOB(int x, int y) {
	return x < 0 || x >= r || y < 0 || y >= c;
}

bool IsLeftPossible(int x, int y) {
	const int lgdx[5] = { -1, 0, 1, 1, 2 };
	const int lgdy[5] = { -1, -2, -1, -2, -1};

	for (int dir = 0; dir < 5; dir++) {
		int nx = x + lgdx[dir];
		int ny = y + lgdy[dir];
		if (OOB(nx, ny) || board[nx][ny]) return false;
	}
	return true;
}

bool IsRightPossible(int x, int y) {
	const int rgdx[5] = { -1, 0, 1, 1, 2 };
	const int rgdy[5] = { 1, 2, 1, 2, 1 };

	for (int dir = 0; dir < 5; dir++) {
		int nx = x + rgdx[dir];
		int ny = y + rgdy[dir];
		if (OOB(nx, ny) || board[nx][ny]) return false;
	}
	return true;
}

void getDownPos(int& x, int& y) {
	queue<pair<int, int>> q;
	const int dgdx[3] = { 1, 2, 1 };
	const int dgdy[3] = { -1, 0, 1 };
	q.push({ x, y });

	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		bool findSpace = false;
		for (int dir = 0; dir < 3; dir++) {
			int nx = cur.X + dgdx[dir];
			int ny = cur.Y + dgdy[dir];
			if (OOB(nx, ny) || board[nx][ny]) {
				findSpace = true;
				break;
			}
		}

		if (findSpace) {
			x = cur.X;
			y = cur.Y;
			break;
		}
		else q.push({ cur.X + 1, cur.Y });
	}

}

int getScore(int x, int y, int idx) {
	// 다른 idx랑 연결이 되어있으면 그 점수 중 가장 큰놈
	int score = x + 1;

	for (int dir = 0; dir < 4; dir++) {
		int nx = x + dx[dir];
		int ny = y + dy[dir];
		if (board[nx][ny] == -idx) {
			for (int ndir = 0; ndir < 4; ndir++) {
				int nnx = nx + dx[ndir];
				int nny = ny + dy[ndir];
				if (OOB(nnx, nny)) continue;
				if (board[nnx][nny] != idx && board[nnx][nny] != 0) {
					int nidx = abs(board[nnx][nny]);
					score = max(score, gollem[nidx].score);
				}
			}
		}
	}

	
	return score;
}
int Down(int idx) {
	auto g = gollem[idx];
	
	//1.  먼저 남으로 쭉이동
	
	int x = 1;
	int y = g.c;
	int d = g.d;

	getDownPos(x, y);
	
	
	while (1) {
		if (IsLeftPossible(x, y)) {
			y = y - 1;
			d = (d - 1) % 4;
			getDownPos(x, y);
		}
		else if (IsRightPossible(x, y)) {
			y = y + 1;
			d = (d + 1) % 4;
			getDownPos(x, y);
		}
		else
			break;
	}

	if (x >= 0 && x <= 3)
		return -1;
	
	
	for (int dir = 0; dir < 4; dir++) {
		int nx = x + dx[dir];
		int ny = y + dy[dir];
		if(dir == d) board[nx][ny] = -idx;
		else board[nx][ny] = idx;
	}
	board[x][y] = idx;

	int score = getScore(x, y, idx);
	gollem[idx].score = score;
	score -= 2;
	return score;

}

void Reset() {
	for (int i = 0; i < r; i++)
		fill(board[i], board[i] + c, 0);
}
int main() {
	ios::sync_with_stdio(0), cin.tie(0);
	cin >> r >> c >> k;
	r = r + 3;

	for (int idx = 1; idx <= k; idx++) {
		int c, d;
		cin >> c >> d;
		gollem[idx].c = c - 1;
		gollem[idx].d = d;
	}

	int ans = 0;
	for (int idx = 1; idx <= k; idx++) {
		int score = Down(idx);
		if (score < 0) Reset();
		else ans += score;
	}
	cout << ans;
}