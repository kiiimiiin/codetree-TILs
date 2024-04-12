#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <tuple>
#include <algorithm>
#define X first
#define Y second
using namespace std;


struct Person {
	int x = -1;
	int y = -1; 
	int mx, my;
};

vector<Person> persons(1);
int board[17][17];
int dx[4] = { -1, 0, 0, 1 };
int dy[4] = { 0, -1, 1, 0 };
int n, m;

bool OOB(int nx, int ny) {
	return (nx < 0 || nx >= n || ny < 0 || ny >= n);
}

void GoToBase(int t) {
	if (t > m) return;
	auto p = persons[t];

	// t번사람의 편의점에서 가장 가까운 캠프탐색
	queue<pair<int, int>> q;
	vector<tuple<int, int, int>> infos; // 거리, 베이스캠프 좌표
	int dist[17][17];
	for (int i = 0; i < n; i++) fill(dist[i], dist[i] + n, -1);
	
	q.push({ p.mx, p.my });
	dist[p.mx][p.my] = 0;

	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			if (OOB(nx, ny) || dist[nx][ny] >= 0) continue;
			if (board[nx][ny] == 2) continue; // 막힌공간
			dist[nx][ny] = dist[cur.X][cur.Y] + 1;
			q.push({ nx, ny });
			if (board[nx][ny] == 1) 
				infos.push_back({ dist[nx][ny], nx, ny });
		}
	}

	sort(infos.begin(), infos.end());
	int bx, by;
	tie(ignore, bx, by) = infos.front(); // 우선순위 베이스캠프 

	board[bx][by] = 2; 
	p.x = bx; p.y = by; 

	persons[t] = p; 
	
}

void GoToMarket() {
	vector<pair<int, int>> closed;
	for (int i = 1; i < persons.size(); i++) {
		if (persons[i].x < 0) continue; // 격자밖이거나 도착완료 스킵
		auto p = persons[i];
		// 편의점에서 현재 위치까지 최단거리값 계산 
		// 역으로 사람이 편의점으로 이동하도록

		queue<pair<int, int>> q;
		int dist[17][17];
		for (int i = 0; i < n; i++) fill(dist[i], dist[i] + n, -1);

		q.push({ p.mx, p.my });
		dist[p.mx][p.my] = 0;

		while (!q.empty()) {
			auto cur = q.front(); q.pop();
			for (int dir = 0; dir < 4; dir++) {
				int nx = cur.X + dx[dir];
				int ny = cur.Y + dy[dir];
				if (OOB(nx, ny) || dist[nx][ny] >= 0) continue;
				if (board[nx][ny] == 2 && !(nx == p.x && ny == p.y)) 
					continue; // 현재 위치 제외한 막힌공간
				dist[nx][ny] = dist[cur.X][cur.Y] + 1;
				q.push({ nx, ny });
			}
		}

		// 최단루트 중에서 우선순위 방향으로 한칸이동
		for (int dir = 0; dir < 4; dir++) {
			int nx = p.x + dx[dir];
			int ny = p.y + dy[dir];
			if (OOB(nx, ny)) continue;
			if (dist[nx][ny] >= 0 && (dist[nx][ny] == dist[p.x][p.y] - 1)) {
				p.x = nx;
				p.y = ny;
				break;
			}
		}

		// 편의점 도착
		if (p.x == p.mx && p.y == p.my) {
			closed.push_back({ p.mx, p.my }); 
			p.x = -2; // 격자밖으로 
		}

		persons[i] = p;
	}

	for (auto c : closed) {
		board[c.X][c.Y] = 2; 
	}
}

bool IsAllClear() {
	for (int i = 1; i < persons.size(); i++) {
		if (persons[i].x != -2) return false;
	}
	return true;
}
void Solve() {
	cin >> n >> m;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}

	for(int i =  0; i < m; i++){
		int mx, my;
		cin >> mx >> my;
		mx--; my--;
		persons.push_back({ -1, -1, mx,my });
	}

	int t = 1;
	while (1) {
		GoToMarket();
		GoToBase(t);

		if (IsAllClear()) break;
		t++;
	}
	
	cout << t;
}



int main(void) {
	Solve();

	return 0;
}