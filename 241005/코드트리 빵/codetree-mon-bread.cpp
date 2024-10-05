#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <tuple>
#include <algorithm>
#define X first
#define Y second
const int dx[4] = { -1, 0, 0, 1 };
const int dy[4] = { 0, -1, 1, 0 };
using namespace std;

struct Person {
	int sx = -1;
	int sy = -1;
	int ex, ey;
	bool isLived = false;
};

int n, m;
Person person[31];
int board[16][16];
int dist[16][16];
queue<pair<int, int>> q;

bool OOB(int x, int y) {
	return x < 0 || x >= n || y < 0 || y >= n;
}
void GoToBase(int idx) {
	if (idx > m) return;

	auto p = person[idx];
	for (int i = 0; i < n; i++) 
		fill(dist[i], dist[i] + n, -1);

	
	q.push({ p.ex, p.ey });
	dist[p.ex][p.ey] = 0;

	tuple<int, int, int> mn = { 0x7f7f7f7f, 0, 0 }; 
	// 도달가능한 베이스중에서 최소거리 찾음
	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			if (OOB(nx, ny) || dist[nx][ny] >= 0) continue;
			if (board[nx][ny] == -1) continue;
			dist[nx][ny] = dist[cur.X][cur.Y] + 1;
			q.push({ nx,ny });

			if (board[nx][ny] == 1)
				mn = min(mn, make_tuple(dist[nx][ny], nx, ny));
		}
	}

	
	tie(ignore, person[idx].sx, person[idx].sy) = mn;
	person[idx].isLived = true;
	board[person[idx].sx][person[idx].sy] = -1;
}

pair<int, int> getNext(int idx) { 
	// 목적지에서부터 시작점 최단거리, 값이 작아지는 순으로
	auto p = person[idx];
	for (int i = 0; i < n; i++)
		fill(dist[i], dist[i] + n, -1);


	q.push({ p.ex, p.ey });
	dist[p.ex][p.ey] = 0;

	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			if (OOB(nx, ny) || dist[nx][ny] >= 0) continue;
			if (board[nx][ny] == -1 && !(nx == p.sx && ny == p.sy)) continue;
			dist[nx][ny] = dist[cur.X][cur.Y] + 1;
			q.push({ nx,ny });
		}
	}

	int nx, ny;
	for (int dir = 0; dir < 4; dir++) {
		nx = p.sx + dx[dir];
		ny = p.sy + dy[dir];
		if (OOB(nx, ny) || board[nx][ny] == -1) continue;
		if (dist[nx][ny] == dist[p.sx][p.sy] - 1)
			break;
	}
	
	return make_pair(nx, ny);
}
void GoToMarket() {
	for (int idx = 1; idx <= m; idx++) {
		if (!person[idx].isLived) continue; 
		int nx, ny;
		tie(nx, ny) = getNext(idx);
		person[idx].sx = nx;
		person[idx].sy = ny;
	}
}

void CloseMarket() {
	for (int idx = 1; idx <= m; idx++) {
		if (!person[idx].isLived) continue;
		if (person[idx].sx == person[idx].ex
			&& person[idx].sy == person[idx].ey) {
			board[person[idx].ex][person[idx].ey] = -1;
			person[idx].isLived = false;
		}
	}
}

bool IsNoPerson() {
	for (int idx = 1; idx <= m; idx++) {
		if (person[idx].isLived) return false;
	}
	return true;
}
int main() {
	ios::sync_with_stdio(0), cin.tie(0);
	
	cin >> n >> m;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> board[i][j];

	for (int idx = 1; idx <= m; idx++) {
		int ex, ey;
		cin >> ex >> ey; 
		person[idx].ex = ex - 1;
		person[idx].ey = ey - 1;
 	}

	
	int t = 1;

	while (1) {
		GoToMarket();
		CloseMarket();
		GoToBase(t);

		if (IsNoPerson()) break;
		t++;
	}

	cout << t;
}