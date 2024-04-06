#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <tuple>
#define X first
#define Y second
using namespace std; 
struct Person {
	int sx, sy;
	int ex, ey;
};
int cx, cy;
int n, m, c;
int board[22][22]; // 벽, 빈칸
int dx[4] = { 1,0, -1, 0 };
int dy[4] = { 0, 1, 0, -1 };
vector<Person> persons(1);

bool OOB(int x, int y) {
	return (x < 0 || x >= n || y < 0 || y >= n); 
}

bool Move() {
	// 1. Move For person
	queue <pair<int, int>> q;
	int dist[22][22];
	for (int i = 0; i < n; i++) fill(dist[i], dist[i] + n, -1);

	q.push({ cx,cy });
	dist[cx][cy] = 0;
	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			if (OOB(nx, ny) || board[nx][ny] == 1) continue;
			if (dist[nx][ny] >= 0) continue; 
			q.push({ nx,ny });
			dist[nx][ny] = dist[cur.X][cur.Y] + 1;
		}
	} // 최단거리값 계산

	int mn = 0x7f7f7f7f;
	int tsx, tsy, tIdx;
	tsx = tsy = 0x7f7f7f7f;
	for (int i = 1; i <= m; i++) {
		if (persons[i].sx == -1) continue;
		auto p = persons[i];
		if ((make_tuple(dist[p.sx][p.sy], p.sx, p.sy)
			< make_tuple(mn, tsx, tsy)) && dist[p.sx][p.sy] != -1) {
			mn = dist[p.sx][p.sy];
			tsx = p.sx;
			tsy = p.sy;
			tIdx = i;
		}
	}

	if (mn == 0x7f7f7f7f) return false;
	else if (c <= mn) return false ;
	
	c -= mn;
	cx = tsx;
	cy = tsy;

	// 2. Move For Dest 
	auto target = persons[tIdx];
	for (int i = 0; i < n; i++) fill(dist[i], dist[i] + n, -1);
	q.push({ cx,cy });
	dist[cx][cy] = 0;
	
	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			if (OOB(nx, ny) || board[nx][ny] == 1) continue;
			if (dist[nx][ny] >= 0) continue;
			q.push({ nx,ny });
			dist[nx][ny] = dist[cur.X][cur.Y] + 1;
		}
	} // 최단거리값 계산

	if (dist[target.ex][target.ey] == -1) return false;
	else if (c < dist[target.ex][target.ey]) return false;

	c -= dist[target.ex][target.ey];
	c += 2 * dist[target.ex][target.ey];
	cx = target.ex;
	cy = target.ey;
	persons[tIdx].sx = -1;
	return true;
}

bool CanAllMove() {
	for (int i = 1; i <= m; i++) {
		if (persons[i].sx != -1) return false;
	}
	return true; 
}

int main(void) {
	cin >> n >> m >> c;
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}

	cin >> cx >> cy;
	cx = cx - 1; cy = cy - 1; 
	for (int i = 1; i <= m; i++) {
		int sx, sy, ex, ey;
		cin >> sx >> sy >> ex >> ey;
		persons.push_back({ sx - 1,sy - 1,ex - 1,ey - 1 });
	}
	 
	int success = 0;
	while (1) {
		if (!Move()) {
			break;
		}
		if (CanAllMove()) {
			success = 1;
			break;
		}
	}

	cout << (success ? c : -1);
}