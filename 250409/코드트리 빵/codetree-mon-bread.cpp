#include <iostream>
#include <queue>
#include <tuple>
#define X first
#define Y second
using namespace std;
int dx[4] = { -1, 0, 0, 1 };
int dy[4] = { 0, -1, 1, 0 }; // 상좌우하
int n, m;
int board[16][16]; // base camp
int dist[16][16];

struct Person {
	int x, y;
	int ex, ey;
	bool isLived = false;
};

queue<pair<int, int>> q;
Person person[32];

bool OOB(int nx, int ny) {
	return nx < 0 || nx >= n || ny < 0 || ny >= n;
}
void GoToBase(int t) {
	if(t > m) return;
	
	for (int i = 0; i < n; i++) fill(dist[i], dist[i] + n, -1);

	q.push({ person[t].ex, person[t].ey });
	dist[person[t].ex][person[t].ey] = 0;
	tuple<int, int, int> mn = { 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f };

	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			if (OOB(nx, ny) || dist[nx][ny] >= 0 || board[nx][ny] == 2) continue;
			q.push({ nx, ny });
			dist[nx][ny] = dist[cur.X][cur.Y] + 1;
			if (mn > make_tuple(dist[nx][ny], nx, ny) && board[nx][ny] == 1) {
				mn = make_tuple(dist[nx][ny], nx, ny);
			}
		}
	}

	int tx, ty;
	tie(ignore, tx, ty) = mn;
	if (tx == 0x7f7f7f7f) cout << "z";
	person[t].x = tx;
	person[t].y = ty;
	person[t].isLived = true;
	board[tx][ty] = 2;
	return;
}

void MoveToMarket() {
	for (int idx = 1; idx <= m; idx++) {
		if (person[idx].isLived == false) continue;
		for (int i = 0; i < n; i++) 
			fill(dist[i], dist[i] + n, -1);
		q.push({ person[idx].ex, person[idx].ey });
		dist[person[idx].ex][person[idx].ey] = 0;
		while (!q.empty()) {
			auto cur = q.front(); q.pop();
			for (int dir = 0; dir < 4; dir++) {
				int nx = cur.X + dx[dir];
				int ny = cur.Y + dy[dir];
				if (OOB(nx, ny) || dist[nx][ny] >= 0) 
					continue;
				if (board[nx][ny] == 2 && !(nx == person[idx].x && ny == person[idx].y))
					continue;
				q.push({ nx,ny });
				dist[nx][ny] = dist[cur.X][cur.Y] + 1;
			}
		}

		
		for (int dir = 0; dir < 4; dir++) {
			int nx = person[idx].x + dx[dir];
			int ny = person[idx].y + dy[dir];
			if (OOB(nx, ny)) continue;
			if (dist[nx][ny]== dist[person[idx].x][person[idx].y] - 1 
				&& dist[nx][ny] >= 0 && board[nx][ny] != 2) {
				person[idx].x = nx;
				person[idx].y = ny;
				break;
			}
		}
	}

	for (int idx = 1; idx <= m; idx++) {
		if (person[idx].isLived == false) continue;
		if (person[idx].x == person[idx].ex &&
			person[idx].y == person[idx].ey) {
			person[idx].isLived = false;
			board[person[idx].x][person[idx].y] = 2;
		}
	}

}

bool IsNoPerson() {
	for (int idx = 1; idx <= m; idx++) 
		if (board[person[idx].ex][person[idx].ey] != 2) return false;
	return true;
}
int main() {
	cin >> n >> m;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}

	for (int idx = 1; idx <= m; idx++) {
		cin >> person[idx].ex >> person[idx].ey;
		person[idx].ex--; person[idx].ey--;
	}

	int t;
	for (t = 1;; t++) {
		//1
		MoveToMarket();
		//2
		if (IsNoPerson()) break;
		GoToBase(t);
	}
	cout << t;
}