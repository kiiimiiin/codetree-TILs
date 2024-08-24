#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <tuple>
#define X first 
#define Y second
using namespace std;

struct AirCon {
	int x, y, dir;
};
const int dx[4] = { 0, -1, 0, 1 }; // 좌 상 우 하
const int dy[4] = { -1, 0, 1, 0 };
const int airdx[4][3] = {
	{-1, 0, 1},
	{-1, -1, -1},
	{-1,0,1},
	{1,1,1}
};

const int airdy[4][3] = {
	{-1,-1,-1},
	{-1,0,1},
	{1,1,1},
	{-1,0,1},
};

int n, m, k;
int cool[22][22], board[22][22], wall[22][22][4]; 
vector<AirCon> aircon;


bool OOB(int x, int y) {
	return x < 0 || x >= n || y < 0 || y >= n;
}

bool IsFrontWall(int x, int y, int nx , int ny, int adir, int cnt) {
	if (adir == 0) {
		if (cnt == 0 && (wall[x][y][1] || wall[nx][ny][2])) return true;
		if (cnt == 1 && (wall[x][y][0] || wall[nx][ny][2])) return true;
		if (cnt == 2 && (wall[x][y][3] || wall[nx][ny][2])) return true;
	}
	else if (adir == 1) {
		if (cnt == 0 && (wall[x][y][0] || wall[nx][ny][3])) return true;
		if (cnt == 1 && (wall[x][y][1] || wall[nx][ny][3])) return true;
		if (cnt == 2 && (wall[x][y][2] || wall[nx][ny][3])) return true;
	}
	else if (adir == 2) {
		if (cnt == 0 && (wall[x][y][1] || wall[nx][ny][0])) return true;
		if (cnt == 1 && (wall[x][y][2] || wall[nx][ny][0])) return true;
		if (cnt == 2 && (wall[x][y][3] || wall[nx][ny][0])) return true;
	}
	else if (adir == 3) {
		if (cnt == 0 && (wall[x][y][0] || wall[nx][ny][1])) return true;
		if (cnt == 1 && (wall[x][y][3] || wall[nx][ny][1])) return true;
		if (cnt == 2 && (wall[x][y][2] || wall[nx][ny][1])) return true;
	}
	return false;
}
void Wind() {
	
	for (auto a : aircon) {
		int vis[22][22] = {};
		queue<pair<int, int>> q;

		q.push({ a.x + dx[a.dir], a.y + dy[a.dir] });
		vis[a.x + dx[a.dir]][a.y + dy[a.dir]] = 5;
		while (!q.empty()) {
			auto cur = q.front(); q.pop();
			if (vis[cur.X][cur.Y] == 1) break;

			for (int cnt = 0; cnt < 3; cnt++) {
				int nx = cur.X + airdx[a.dir][cnt];
				int ny = cur.Y + airdy[a.dir][cnt];	
				if (OOB(nx, ny) || IsFrontWall(cur.X, cur.Y, nx, ny, a.dir, cnt))
					continue;
				vis[nx][ny] = vis[cur.X][cur.Y] - 1;
				q.push({ nx, ny });
			}
		}

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				cool[i][j] += vis[i][j];
			}
		}
	}
}

void Mix() {
	int temp[22][22] = {};
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (!cool[i][j]) continue;
			for (int dir = 0; dir < 4; dir++) {
				int nx = i + dx[dir];
				int ny = j + dy[dir];
				if (OOB(nx, ny) || wall[i][j][dir]) continue;
				if (cool[i][j] > cool[nx][ny]) {
					temp[i][j] -= (cool[i][j] - cool[nx][ny]) / 4;
					temp[nx][ny] += (cool[i][j] - cool[nx][ny]) / 4;
				}
			}
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cool[i][j] += temp[i][j];
		}
	}
}

void DoSideHot() {
	for (int i = 0; i < n; i++) {
		for (int j : { 0, n - 1 }) {
			if(cool[i][j])	
				cool[i][j]--;
		}
	}

	for (int i : { 0, n - 1}) {
		for (int j = 1; j < n - 1; j++) {
			if (cool[i][j])
				cool[i][j]--;
		}
	}
}

bool IsOfficeCool() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] != 1) continue;
			if (cool[i][j] < k) return false;
		}
	}
	return true;
}
int main() {
	ios::sync_with_stdio(0), cin.tie(0);

	cin >> n >> m >> k;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
			if (board[i][j] >= 2) {
				aircon.push_back({ i, j, board[i][j] - 2 });
			}
		}
	}

	
	for (int i = 0; i < m; i++) {
		int x, y, dir;
		cin >> x >> y >> dir; // 0 상 1 좌
		x--; y--;
		dir = dir == 0 ? 1 : 0;
		wall[x][y][dir] = 1;
		wall[x + dx[dir]][y + dy[dir]][dir + 2] = 1;// 좌상우하
	}

	int t;
	for (t = 0; t <= 100; t++) {
		if(IsOfficeCool()) break;
		Wind();
		Mix();
		DoSideHot();
	}

	cout << (t <= 100 ? t : -1) ;
}