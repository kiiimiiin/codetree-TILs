#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <algorithm>
using namespace std;
struct Player {
	int idx = -1; // 플레이어 번호
	int stat;
	int dir;
	int gunStat;
};
int n, m, k; // 격자크기, 플레이어수, 라운드수
int dx[4] = { -1, 0, 1, 0 };
int dy[4] = { 0, 1, 0, -1 }; // 상 우 하 좌
vector<int> guns[22][22]; // 총관리 벡터
vector<tuple<int, int, int>> players; // 좌표, 점수
Player pBoard[22][22]; // 개인능력치, 방향, 총의 능력치 : 플레이어보드



bool OOB(int x, int y) {
	return (x < 0 || x >= n || y < 0 || y >= n);
}

void Win(int idx) {
	int x, y, score, gunStat;
	tie(x, y, score) = players[idx]; // winner
	gunStat = pBoard[x][y].gunStat;

	// 총이 놓여있는 경우
	if (!guns[x][y].empty()) {
		sort(guns[x][y].begin(), guns[x][y].end());
		int maxGunStat = guns[x][y].back();
		if (maxGunStat > gunStat) {
			guns[x][y].pop_back();
			if (gunStat > 0) // 플레이어가 총을 갖고있는 경우 
				guns[x][y].push_back(gunStat);
			gunStat = maxGunStat;
		}
	}

	pBoard[x][y].gunStat = gunStat;
}

void Lose(int idx) {
	int x, y, score, stat, dir, gunStat;
	tie(x, y, score) = players[idx]; // looser
	stat = pBoard[x][y].stat;
	dir = pBoard[x][y].dir;
	gunStat = pBoard[x][y].gunStat;

	// 격자에 자신의 총 내려놓음
	if (gunStat > 0) guns[x][y].push_back(gunStat);
	gunStat = 0;

	// 이동
	int nx = x + dx[dir];
	int ny = y + dy[dir];
	while(OOB(nx, ny) || pBoard[nx][ny].idx != -1) {
		dir = (dir + 1) % 4;
		nx = x + dx[dir];
		ny = y + dy[dir];
	}

	// 빈칸 찾음

	// 총이 놓여있는 경우
	if (!guns[nx][ny].empty()) {
		sort(guns[nx][ny].begin(), guns[nx][ny].end());
		int maxGunStat = guns[nx][ny].back();
		guns[nx][ny].pop_back(); 
		gunStat = maxGunStat;
	}

	
	pBoard[x][y].idx = -1;
	pBoard[nx][ny].idx = idx;
	pBoard[nx][ny].stat = stat;
	pBoard[nx][ny].dir = dir;
	pBoard[nx][ny].gunStat = gunStat;
	players[idx] = { nx, ny, score };
}



void Simulate() {
	for (int i = 0; i < players.size(); i++) {
		int x, y, score, stat, dir, gunStat;
		tie( x, y, score) = players[i];
		stat = pBoard[x][y].stat;
		dir = pBoard[x][y].dir;
		gunStat = pBoard[x][y].gunStat;
		// 이동
		int nx = x + dx[dir];
		int ny = y + dy[dir]; 
		if (OOB(nx, ny)) {
			dir = (dir + 2) % 4;
			nx = x + dx[dir];
			ny = y + dy[dir];
		}

		// 이동한 방향에 플레이어가 없고 총이 놓여있는 경우
		if (pBoard[nx][ny].idx == -1) {
			if (!guns[nx][ny].empty()) {
				sort(guns[nx][ny].begin(), guns[nx][ny].end());
				int maxGunStat = guns[nx][ny].back(); 
				if (maxGunStat > gunStat) {
					guns[nx][ny].pop_back();
					if (gunStat > 0) // 플레이어가 총을 갖고있는 경우 
						guns[nx][ny].push_back(gunStat);  
					gunStat = maxGunStat;
				}
			}
			pBoard[x][y].idx = -1;
			pBoard[nx][ny].idx = i;
			pBoard[nx][ny].stat = stat;
			pBoard[nx][ny].dir = dir;
			pBoard[nx][ny].gunStat = gunStat;
			players[i] = { nx, ny, score };
		}
		else { // 플레이어가 있는 경우
			int diff = stat + gunStat -
				(pBoard[nx][ny].stat + pBoard[nx][ny].gunStat);

			if (diff > 0 || (diff == 0 && stat > pBoard[nx][ny].stat)) {
				pBoard[x][y].idx = -1; // Lose처리를 위해 없는 사람 취급
				Lose(pBoard[nx][ny].idx);

				pBoard[nx][ny].idx = i;
				pBoard[nx][ny].stat = stat;
				pBoard[nx][ny].dir = dir;
				pBoard[nx][ny].gunStat = gunStat;
				players[i] = { nx , ny , score + abs(diff) };

				Win(i);
			}// 다른플레이어로 둘러싸여져있는 경우도 한번 생각
			else if ( diff < 0 || (diff == 0 && stat <  pBoard[nx][ny].stat)) {
				auto tmp = pBoard[nx][ny];

				pBoard[x][y].idx = -1;
				pBoard[nx][ny].idx = i;
				pBoard[nx][ny].stat = stat;
				pBoard[nx][ny].dir = dir;
				pBoard[nx][ny].gunStat = gunStat;
				players[i] = { nx , ny , score}; 
				Lose(i); // 루저 먼저처리하여 이동시킴
				
				pBoard[nx][ny] = tmp;
				players[pBoard[nx][ny].idx] = { nx, ny, score + abs(diff) };
				Win(pBoard[nx][ny].idx);
			}
		}
	}
}
int main(void) {
	cin >> n >> m >> k;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int init;
			cin >> init;
			if (init >= 1) guns[i][j].push_back(init);
		}
	}

	for (int i = 0; i < m; i++) {
		int x, y, d, s;
		cin >> x >> y >> d >> s;
		players.push_back({ x - 1, y - 1, 0 });
		pBoard[x - 1][y - 1].idx = i;
		pBoard[x - 1][y - 1].dir = d;
		pBoard[x - 1][y - 1].stat = s;
		pBoard[x - 1][y - 1].gunStat = 0;
	}

	
	while (k--) {
		Simulate(); 
	}

	for (auto p : players) 
		cout << get<2>(p) << ' ';
}