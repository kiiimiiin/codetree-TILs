#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#define X first
#define Y second
using namespace std;
int m, t;
int px, py;
int dx[8] = { -1,-1,0,1,1,1,0,-1 };
int dy[8] = { 0,-1,-1,-1,0,1,1,1 };
int monsters[5][5][9]; // 좌표에대한 몬스터 방향별 개수
int dead[5][5]; // 시체 몬스터 보드 

bool Cmp(tuple<int, int, int, int> &t1, tuple<int, int, int, int> &t2) {
	if (get<0>(t1) != get<0>(t2)) return get<0>(t1) > get<0>(t2);
	if (get<1>(t1) != get<1>(t2)) return get<1>(t1) < get<1>(t2);
	if (get<2>(t1) != get<2>(t2)) return get<2>(t1) < get<2>(t2);
	return get<3>(t1) < get<3>(t2);
}

bool OOB(int x, int y) {
	return (x < 0 || x >= 4 || y < 0 || y >= 4);
}
void MonsterMove() {
	int cMonsters[5][5][9] = {};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 8; k++) {
				if (monsters[i][j][k] == 0) continue;
				int md = k;
				int nx = i + dx[md];
				int ny = j + dy[md];
				int cnt = 0;
				while ((nx == px && ny == py) || OOB(nx, ny) || dead[nx][ny]) { // 팩맨이 있거나 격자벗어나거나 시체있으면 회전
					if (cnt >= 8) break;
					md = (md + 1) % 8;
					nx = i + dx[md];
					ny = j + dy[md];
					cnt++;
				}
				if (cnt >= 8) cMonsters[i][j][md]++;
				else cMonsters[nx][ny][md]++;
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 8; k++) {
				monsters[i][j][k] = cMonsters[i][j][k];
			}
		}
	}
}

bool IsVisited(int pnx, int pny, vector<pair<int, int>>& v) {
	for (auto pos : v) {
		if (pnx == pos.X && pny == pos.Y) return true;
	}

	return false;
}

void PackMove() {

	int pdx[4] = { -1, 0, 1, 0 };
	int pdy[4] = { 0, -1, 0 , 1 }; // 상좌하우


	// 모든 경우에서 먹는 우선 경우 택
	vector<tuple<int, int, int, int>> infos; // 먹은수, 방향정보
	for (int cse = 0; cse < (1 << 6); cse++) {
		int tmp = cse;
		int pnx = px;
		int pny = py;
		int eattenCnt = 0;
		int isSkipped = 0;
		vector<int> dirs;
		vector<pair<int, int>> v; // 방문한 배열

		for (int i = 0; i < 3; i++) {
			int dir = tmp % 4;
			tmp = tmp / 4;

			pnx = pnx + pdx[dir];
			pny = pny + pdy[dir];
			if (OOB(pnx, pny)) {
				isSkipped = 1;
				break;
			}

			if (!IsVisited(pnx, pny, v)) {
				for (auto cnt : monsters[pnx][pny]) {
					eattenCnt += cnt;
				}
				v.push_back({ pnx,pny });
			}

			dirs.push_back(dir);
		}

		if (isSkipped) continue;

		infos.push_back({ eattenCnt, dirs[0], dirs[1], dirs[2] });
	}

	// 우선 경우에 대해 실제 몬스터 먹음
	sort(infos.begin(), infos.end(), Cmp);
	int d0, d1, d2;
	tie(ignore, d0, d1, d2) = infos.front();

	int pnx = px;
	int pny = py;
	for (auto dir : { d0, d1, d2 }) {
		pnx = pnx + pdx[dir];
		pny = pny + pdy[dir];

		for (int k = 0; k < 8; k++) {
			if (monsters[pnx][pny] > 0) {
				monsters[pnx][pny][k] = 0;
				dead[pnx][pny] = 3;
			}
		}

	}

	px = pnx;
	py = pny;
}

void DeadReset() {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (dead[i][j] >= 1) dead[i][j]--;
}

void CopyMonster(int cMonsters[5][5][9]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 8; k++) {
				cMonsters[i][j][k] = monsters[i][j][k];
			}
		}
	}
}

void PasteMonster(int cMonsters[5][5][9]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 8; k++) {
				monsters[i][j][k] += cMonsters[i][j][k];
			}
		}
	}
}

int Ans() {
	int ret = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 8; k++) {
				ret += monsters[i][j][k];
			}
		}
	}

	return ret;
}
void Solve() {
	cin >> m >> t;
	cin >> px >> py;
	px = px - 1; py = py - 1;

	while (m--) {
		int r, c, d;

		cin >> r >> c >> d;
		r = r - 1; c = c - 1; d = d - 1;
		monsters[r][c][d]++;
	}

	for (int i = 1; i <= t; i++) {

		int cMonsters[5][5][9];
		CopyMonster(cMonsters);
		MonsterMove();
		PackMove();
		DeadReset();
		PasteMonster(cMonsters);

	}

	int ans = Ans();

	cout << ans;
}

int main(void) {

	Solve();

	return 0;
}