#include <iostream>
#include <vector>
using namespace std;
int board[22][22];
pair<int,int> markerBoard[22][22]; // 마크한 플레이어번호, 남은 턴수
int dx[4] = { -1,1,0,0 };
int dy[4] = { 0, 0, -1, 1 };
int n, m, k;
int t;

struct Player {
	int x = -1;
	int y = -1;
	int dir = -1; 
	int priority[4][4] = {};
};

vector<Player> players(400);


void Mark() {
	for (int i = 1; i <= m; i++) {
		if (players[i].x == -1) continue;
		auto p = players[i];
		markerBoard[p.x][p.y] = { i, k };
	}
}

bool OOB(int x, int y) {
	return (x < 0 || x >= n || y < 0 || y >= n); 
}

void Move() {
	vector<int> playerNums[22][22];

	for (int i = 1; i <= m; i++) {
		if (players[i].x == -1) continue;
		auto p = players[i];

		// 1. 움직임 (빈칸 -> 자신영역)
		int findEmpty = 0;
		for (int pri = 0; pri < 4; pri++) {
			int nx = p.x + dx[p.priority[p.dir][pri]];
			int ny = p.y + dy[p.priority[p.dir][pri]];
			if (OOB(nx, ny)) continue; 
			if (markerBoard[nx][ny].first == 0) {
				playerNums[nx][ny].push_back(i);
				p.x = nx; p.y = ny;
				p.dir = p.priority[p.dir][pri];
				players[i] = p;
				findEmpty = 1;
				break; 
			}
		}
		if (findEmpty) continue;

		for (int pri = 0; pri < 4; pri++) {
			int nx = p.x + dx[p.priority[p.dir][pri]];
			int ny = p.y + dy[p.priority[p.dir][pri]];
			if (OOB(nx, ny)) continue;
			if (markerBoard[nx][ny].first == i) {
				playerNums[nx][ny].push_back(i);
				p.x = nx; p.y = ny;
				p.dir = p.priority[p.dir][pri];
				players[i] = p;
				break;
			}
		}
	}

	// 2. 중첩 플레이어 처리
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (playerNums[i][j].size() >= 2) {
				for (int k = 1; k <= playerNums[i][j].size()-1; k++) {
					int num = playerNums[i][j][k];
					players[num].x = -1;
					players[num].y = -1;
					players[num].dir = -1;
				}
			}
		}
	}

	// 3. 기존 mark 수명감소
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (markerBoard[i][j].first > 0) {
				markerBoard[i][j].second--;
				if (markerBoard[i][j].second == 0) {
					markerBoard[i][j].first = 0;
				}
			}
		}
	}
}

bool IsOnlyOne() {
	for (int i = 2; i <= m; i++) {
		if (players[i].x != -1) return false;
	}
	return true;
}

int main(void) {
	cin >> n >> m >> k;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
			if (board[i][j] >= 1) {
				players[board[i][j]].x = i;
				players[board[i][j]].y = j;
			}	
		}
	}

	for (int i = 1; i <= m; i++) {
		int fd;
		cin >> fd;
		players[i].dir = fd - 1;
	}

	for (int i = 1; i <= m; i++) {
		for (int dir = 0; dir < 4; dir++) {
			for (int pdir = 0; pdir < 4; pdir++) {
				int d;
				cin >> d;
				players[i].priority[dir][pdir] = d - 1;
			}
		}
	}

	Mark();

	for (t = 1; t < 1000; t++) {
		Move();
		Mark();
		if (IsOnlyOne()) break;
	}

	cout << (t < 1000 ? t : -1);
}