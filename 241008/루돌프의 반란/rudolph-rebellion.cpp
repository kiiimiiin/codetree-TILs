#include <iostream>
#include <tuple>
#include <algorithm>
#include <utility>
#include <queue>
#define SANTA 1
#define RUDOLPH 0
const int rdx[8] = { -1,-1,-1, 0, 0, 1,1,1 };
const int rdy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
const int sdx[4] = { -1,0,1,0 };
const int sdy[4] = { 0, 1,0,-1 };
using namespace std; 
int n, m, p, c, d;

struct Rudolph {
	int x, y;
};

struct Santa {
	int x, y, t;
	int isLived = false;
	int score = 0;
};

Rudolph r;
Santa santa[32];

int board[52][52];

int getDist(int x1, int y1, int x2, int y2) {
	int dx = x1- x2;
	int dy = y1 - y2;
	return dx * dx + dy * dy;
}

bool OOB(int x, int y) {
	return x < 0 || x >= n || y < 0 || y >= n;
}
bool IsSameSpace(int x1, int y1, int x2, int y2) {
	return x1 == x2 && y1 == y2;
}

void Interaction(int snx, int sny, int idx, int dir, int isSanta) {
	if (board[snx][sny] > 0) {
		queue<int> q;
		q.push(board[snx][sny]);

		board[snx][sny] = idx;
		santa[idx].x = snx;
		santa[idx].y = sny;

		while (!q.empty()) {
			int cur = q.front(); q.pop();
			int nx, ny;
			if (isSanta) {
				nx = santa[cur].x + sdx[dir];
				ny = santa[cur].y + sdy[dir];
			}
			else {
				nx = santa[cur].x + rdx[dir];
				ny = santa[cur].y + rdy[dir];
			}
			
			if (OOB(nx, ny)) {
				santa[cur].isLived = false;
				break;
			}

			if (board[nx][ny] > 0)
				q.push(board[nx][ny]);

			board[nx][ny] = cur;
			santa[cur].x = nx;
			santa[cur].y = ny;
		}
	}
	else {
		board[snx][sny] = idx;
		santa[idx].x = snx;
		santa[idx].y = sny;
	}
}


void Acident(int idx, int snx, int sny, int dir, int isSanta) {
	
	santa[idx].t = 2;
	if (isSanta) {
		santa[idx].score += d;
		dir = (dir + 2) % 4;
		snx = snx + d * sdx[dir];
		sny = sny + d * sdy[dir];
	}
	else {
		santa[idx].score += c;
		snx = snx + c * rdx[dir];
		sny = sny + c * rdy[dir];
	}

	if (OOB(snx, sny)) {
		santa[idx].isLived = false;
		return;
	}

	Interaction(snx, sny, idx, dir, isSanta);

}

void MoveRudolph() {
	int tDist, tx, ty, tIdx;
	tDist = 0x7f7f7f7f;
	for (int idx = 1; idx <= p; idx++) {
		if (!santa[idx].isLived) continue;
		Santa s = santa[idx];
		int dist = getDist(r.x, r.y, s.x, s.y);
		if (dist < tDist || (dist == tDist && s.x > tx) ||
			(dist == tDist && s.x == tx && s.y > ty)) {
			tDist = dist;
			tx = s.x;
			ty = s.y;
			tIdx = idx;
		}
	}

	int mn, rnx, rny, rnd;
	mn = 0x7f7f7f7f;
	for (int dir = 0; dir < 8; dir++) {
		int nx = r.x + rdx[dir];
		int ny = r.y + rdy[dir];
		if (OOB(nx, ny)) continue;
		int dist = getDist(nx, ny, tx, ty);
		if (dist < mn) {
			mn = dist;
			rnx = nx;
			rny = ny;
			rnd = dir;
		}
	}

	board[r.x][r.y] = 0;
	if (board[rnx][rny] > 0)
		Acident(board[rnx][rny], rnx, rny, rnd, RUDOLPH);
	board[rnx][rny] = -1;
		
	 
	r.x = rnx;
	r.y = rny;
}





void MoveSanta() {
	for (int idx = 1; idx <= p; idx++) {
		if (!santa[idx].isLived || santa[idx].t > 0) continue;
		Santa s = santa[idx];
		int mn, snx, sny, snd;
		mn = getDist(s.x, s.y, r.x, r.y);

		for (int dir = 0; dir < 4; dir++) {
			int nx = s.x + sdx[dir];
			int ny = s.y + sdy[dir];
			if (OOB(nx, ny) || board[nx][ny] > 0) continue;
			int dist = getDist(nx, ny, r.x, r.y);
			if (dist < mn) {
				mn = dist;
				snx = nx;
				sny = ny;
				snd = dir;
			}
		}

		if (mn == getDist(s.x, s.y, r.x, r.y))
			continue;

		board[s.x][s.y] = 0;
		if (board[snx][sny] == -1)
			Acident(idx, snx, sny, snd, SANTA);
		else {
			board[snx][sny] = idx;
			santa[idx].x = snx;
			santa[idx].y = sny;
		}
			
		
		 // 충돌 구현 필요 
	}
}


void UpdateInfo() {
	for (int idx = 1; idx <= p; idx++) {
		if (!santa[idx].isLived) continue;
		if (santa[idx].t > 0) santa[idx].t--;
		santa[idx].score++;
	}
}

bool NoSanta() {
	for (int idx = 1; idx <= p; idx++) {
		if (santa[idx].isLived) return false;
	}
	return true;
}
int main() {
	ios::sync_with_stdio(0), cin.tie(0);
	cin >> n >> m >> p >> c >> d;
	
	cin >> r.x >> r.y;
	r.x--; r.y--;
	board[r.x][r.y] = -1;

	for(int i = 0; i < p; i++){
		int idx, x, y;
		cin >> idx >> x >> y;
		x--; y--; 
		santa[idx].x = x;
		santa[idx].y = y;
		santa[idx].isLived = true;
		board[x][y] = idx;
	}

	while (m--) {
		if (NoSanta()) break;
		MoveRudolph();
		MoveSanta();
		UpdateInfo();
	}

	for (int idx = 1; idx <= p; idx++) {
		cout << santa[idx].score << ' ';
	}
}