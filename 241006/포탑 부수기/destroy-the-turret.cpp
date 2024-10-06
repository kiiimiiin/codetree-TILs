#include <iostream>
#include <tuple>
#include <algorithm>
#include <utility>
#include <queue>
#define X first
#define Y second
using namespace std; 
const int dx[4] = { 0,1,0,-1 };
const int dy[4] = { 1,0,-1,0 };
int n, m, k, t;
struct Potab { // 공격력, 최근 공격시점
	int a = 0;
	int t = 0;
};

Potab board[12][12]; 
Potab tmp[12][12];


tuple<int, int, int, int> getPoint() {

	tuple<int, int, int, int> mn = { 0x7f7f7f7f, 0,0,0 };
	tuple<int, int, int, int> mx = { -0x7f7f7f7f, 0,0,0 }; 

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (board[i][j].a == 0) continue;
			auto p = board[i][j];

			mn = min(mn, make_tuple(p.a, -p.t, -i - j, -j));
			mx = max(mx, make_tuple(p.a, -p.t, -i - j, -j));
		}
	}

	int wx, wy, sx, sy, xysum, y;
	tie(ignore, ignore, xysum, y) = mn;
	xysum = -xysum;
	y = -y;
	wx = xysum - y;
	wy = y;

	tie(ignore, ignore, xysum, y) = mx;
	xysum = -xysum;
	y = -y;
	sx = xysum - y;
	sy = y;
	
	return make_tuple(wx, wy, sx, sy);
	
}

void ProcessOOB(int& x, int& y) {
	if (x >= n) x = x - n;
	else if (x < 0) x = n + x;

	if (y >= m) y = y - m;
	else if (y < 0) y = m + y;

}
bool Laser(int ax, int ay, int ex, int ey) {
	queue<pair<int, int>> q;
	int dist[12][12];
	for (int i = 0; i < n; i++) fill(dist[i], dist[i] + m, -1);

	dist[ex][ey] = 0;
	q.push({ ex, ey });
	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			ProcessOOB(nx, ny);
			if (board[nx][ny].a == 0 || dist[nx][ny] >= 0) continue;
			q.push({ nx, ny });
			dist[nx][ny] = dist[cur.X][cur.Y] + 1;
		}
	}

	if (dist[ax][ay] == -1) return false;
	

	q.push({ ax, ay });
	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			ProcessOOB(nx, ny);
			if (dist[nx][ny] == dist[cur.X][cur.Y] - 1 && dist[nx][ny] >= 0) {
				q.push({ nx, ny });
				if(nx == ex && ny == ey)
					board[nx][ny].a -= board[ax][ay].a;
				else
					board[nx][ny].a -= board[ax][ay].a / 2;

				if (board[nx][ny].a < 0) board[nx][ny].a = 0;
				break;
			}
		}
	}

	return true;
}

void Bomb(int ax, int ay, int ex, int ey) {
	const int dx[8] = { -1, -1, -1, 0, 0, 1,1, 1 };
	const int dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	
	board[ex][ey].a -= board[ax][ay].a;
	if (board[ex][ey].a < 0) board[ex][ey].a = 0;

	for (int dir = 0; dir < 8; dir++) {
		int nx = ex + dx[dir];
		int ny = ey + dy[dir];
		ProcessOOB(nx, ny);
		if (board[nx][ny].a == 0 || nx == ax && ny == ay) continue;
		board[nx][ny].a -= board[ax][ay].a / 2;
		if (board[nx][ny].a < 0) board[nx][ny].a = 0;
	}
}

void Attack(int ax, int ay, int ex, int ey) {
	if (!Laser(ax, ay, ex, ey))
		Bomb(ax, ay, ex, ey);
}


void Copy(Potab desc[12][12], Potab src[12][12]) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			desc[i][j] = src[i][j];
}

void Repair(Potab tmp[12][12], Potab board[12][12], int ax , int ay) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (i == ax && j == ay) continue;
			if (board[i][j].a == tmp[i][j].a && board[i][j].a > 0) {
				board[i][j].a++;
			}
		}
	}
		
}
int main() {
	ios::sync_with_stdio(0), cin.tie(0);

	cin >> n >> m >> k;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			int a;
			cin >> a;
			board[i][j] = { a, 0 };
		}
	}
		

	for (t = 1; t <= k; t++) {
		int ax, ay, ex, ey;
		tie(ax, ay, ex, ey) = getPoint();
		//if (ax == ex && ay == ey) break;
		
		board[ax][ay].a += n + m;

		Copy(tmp, board);
		Attack(ax, ay, ex, ey);
		board[ax][ay].t = t;

		Repair(tmp, board, ax, ay);
	}

	int sx, sy;
	tie(ignore, ignore, sx, sy) = getPoint();
	cout << board[sx][sy].a;

}