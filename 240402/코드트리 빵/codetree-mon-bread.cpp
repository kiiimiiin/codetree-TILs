#include <iostream>
#include <vector>
#include <queue>
#define X first
#define Y second
using namespace std; 


// t번 사람이 가고싶은 편의점 좌표, 가고자하는 편의점 루트,
struct Person {
	int x, y;
	int px, py;
	
};
vector<Person> persons;

int board[17][17];
int dx[4] = { -1, 0, 0, 1 };
int dy[4] = { 0, -1, 1, 0 };
int n, m, t;

bool OOB(int x, int y) {
	return (x < 1 || x > n || y < 1 || y > n);
}

void MoveForMarket() {

	vector<pair<int, int>> closed;

	for (int i = 1; i < persons.size(); i++){
		auto p = persons[i];
		if (p.x < 0) continue; // 베이스캠프에 위치 안하거나 편의점도착은 예외
		
		int dist[17][17] = {};
		for(int i = 1; i <= n; i++)
			fill(dist[i] + 1, dist[i] + n + 1, -1);
		
		queue<pair<int, int>> q;
		int findFlag = 0;
		dist[p.x][p.y] = 0;
		q.push({ p.x,p.y });

		while (!q.empty()) {
			auto cur = q.front(); q.pop();
			for (int dir = 0; dir < 4; dir++) {
				int nx = cur.X + dx[dir];
				int ny = cur.Y + dy[dir];
				if (OOB(nx, ny) || dist[nx][ny] >= 0) continue;
				if (board[nx][ny] == 2) continue; // 막힘
				if (nx == p.px && ny == p.py) {
					dist[nx][ny] = dist[cur.X][cur.Y] + 1;
					findFlag = 1;
					break;
				}
				dist[nx][ny] = dist[cur.X][cur.Y] + 1;
				q.push({ nx,ny });
			}
			if (findFlag) break;
		} // 최단거리 편의점 찾음

		while (!q.empty()) q.pop();

		int route[17][17] = {};
		route[p.px][p.py] = 1;
		q.push({ p.px, p.py });

		while (!q.empty()) {
			auto cur = q.front(); q.pop();
			for (int dir = 0; dir < 4; dir++) {
				int nx = cur.X + dx[dir];
				int ny = cur.Y + dy[dir];
				if (OOB(nx, ny) || route[nx][ny]) continue;
				if (dist[nx][ny] == dist[cur.X][cur.Y] - 1
					&& dist[nx][ny] >= 0) {
					q.push({ nx,ny });
					route[nx][ny] = 1;
				}
			}
		} // 최단거리에 해당하는 루트찾음

		for (int dir = 0; dir < 4; dir++) {
			int nx = p.x + dx[dir];
			int ny = p.y + dy[dir];
			if (OOB(nx, ny)) continue;
			if (route[nx][ny]) {
				p.x = nx; p.y = ny;
				break;
			}
 		} // 현재 위치에서 상좌우하 우선순위로 루트를 나섬.

		if (p.x == p.px && p.y == p.py) {
			p.x = -2; p.y = -2; 
			closed.push_back({ p.px, p.py });
		} // 도착
			

		persons[i] = p; // 변화한 위치 반영
	}

	for (auto c : closed)
		board[c.X][c.Y] = 2; // 모든 이동이 끝나고 막힘처리
}

void FindBaseCamp() { // 가고자 하는 편의점과 가까운 베이스캠프
	if (t > m) return;
	auto p = persons[t];
	int dist[17][17] = {};
	for (int i = 1; i <= n; i++)
		fill(dist[i] + 1, dist[i] + n + 1, -1);

	queue<pair<int, int>> q;
	int bx, by, findFlag;

	dist[p.px][p.py] = 0;
	q.push({ p.px,p.py });
	
	bx = by = -1;
	findFlag = 0;
	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			if (OOB(nx, ny) || dist[nx][ny] >= 0) continue;
			if (board[nx][ny] == 2) continue; // 막힘
			if (board[nx][ny] == 1) {
				bx = nx; by = ny;
				findFlag = 1;
				break;
			}
			dist[nx][ny] = dist[cur.X][cur.Y] + 1;
			q.push({ nx,ny });
		}
		if (findFlag) break;
	}

	if (bx == -1) exit(0); // 베이스캠프는 사람보다 많아 이럴일없음

	p.x = bx; p.y = by;
	board[bx][by] = 2; // 베이스캠프로 위치하면서 이제 막히게됨

	persons[t] = p; // 변화한 위치 반영
}

int main(void) {
	
	persons.resize(1); // dummy
	
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> board[i][j];
		}
	}

	for (int i = 1; i <= m; i++) {
		int x, y;
		cin >> x >> y;
		Person person;
		person.x = -1; person.y = -1;
		person.px = x; person.py = y;
		persons.push_back(person);
	}

	t = 1;
	while (1) {

		MoveForMarket();
		FindBaseCamp();
		
		int flag = 1;
		for (int i = 1; i < persons.size(); i++) {
			auto p = persons[i];
			if (!(p.x == -2 && p.y == -2)) flag = 0;
		}
		if (flag) break;
		t++; 
	}
	
	cout << t;
}