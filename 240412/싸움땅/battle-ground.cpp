#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#define X first
#define Y second
using namespace std;

struct Player {
	int idx;
	int s;
	int gs;
	int dir;
};

int dx[4] = { -1, 0, 1, 0 };
int dy[4] = { 0, 1, 0, -1 }; // 상 우 하 좌
vector<pair<int, int>> players;
Player pBoard[22][22]; // 인덱스, 초기스탯, 건스탯, 방향 
vector<int> gBoard[22][22]; // 총 Board
int score[404];



int n, m, k;

bool OOB(int x, int y) {
	return (x < 0 || x >= n || y < 0 || y >= n);
}

void Loose(Player p){
	auto pos = players[p.idx];
	
	if (p.gs != 0) {
		gBoard[pos.X][pos.Y].push_back(p.gs);
		p.gs = 0;
	}
		
	int nx = pos.X + dx[p.dir];
	int ny = pos.Y + dy[p.dir];
	
	while(OOB(nx, ny) || pBoard[nx][ny].s != 0) { // 벽이거나 다른플레이어
		p.dir = (p.dir + 1) % 4;
		nx = pos.X + dx[p.dir];
		ny = pos.Y + dy[p.dir];
	}

	// 빈칸

	if (!gBoard[nx][ny].empty()) { //총이 있는지 확인
		sort(gBoard[nx][ny].begin(), gBoard[nx][ny].end());

		int sgs = gBoard[nx][ny].back();
		gBoard[nx][ny].pop_back();
		p.gs = sgs;

	}

	players[p.idx] = { nx, ny };
	pBoard[nx][ny] = p;
	pBoard[pos.X][pos.Y].s = 0;
}

void Win(Player p) {
	auto pos = players[p.idx];

	if (!gBoard[pos.X][pos.Y].empty()) { //총이 있는지 확인
		sort(gBoard[pos.X][pos.Y].begin(), gBoard[pos.X][pos.Y].end());

		int sgs = gBoard[pos.X][pos.Y].back();

		if (p.gs != 0) {
			if (sgs > p.gs) {
				gBoard[pos.X][pos.Y].pop_back();
				gBoard[pos.X][pos.Y].push_back(p.gs);
				p.gs = sgs;
			}
		}
		else {
			gBoard[pos.X][pos.Y].pop_back();
			p.gs = sgs;
		}

	}

	pBoard[pos.X][pos.Y] = p;
}

void playerAct() {
	for (int i = 0; i < players.size(); i++) {
		auto pos = players[i];
		Player p = pBoard[pos.X][pos.Y];
		

		// 1방향대로 움직임
		int nx = pos.X + dx[p.dir];
		int ny = pos.Y + dy[p.dir];
		
		if (OOB(nx, ny)) { // 격자에서 벗어남
			p.dir = (p.dir + 2) % 4;
			nx = pos.X + dx[p.dir];
			ny = pos.Y + dy[p.dir];
		}

		if (pBoard[nx][ny].s == 0) { // 이동시 플레이어가 없으면

			if (!gBoard[nx][ny].empty()) { //총이 있는지 확인
				sort(gBoard[nx][ny].begin(), gBoard[nx][ny].end());
	
				int sgs = gBoard[nx][ny].back();
				

				if (p.gs != 0) { // 플레이어가 이미 총이 있다면
					if (sgs > p.gs) { // 격자총이 더 쎌때만
						gBoard[nx][ny].pop_back();
						gBoard[nx][ny].push_back(p.gs);
						p.gs = sgs;
					}
				}
				else { // 플레이어가 총이 없다면
					gBoard[nx][ny].pop_back();
					p.gs = sgs;
				}
			}

			players[i] = { nx, ny };
			pBoard[nx][ny] = p; 
			pBoard[pos.X][pos.Y].s = 0;
			
		} 
		else { // 이동시 플레이어가 있으면
			Player ep = pBoard[nx][ny]; 
			int diff = p.s + p.gs - (ep.s + ep.gs);
			if (diff > 0 || (diff == 0 && p.s > ep.s)) { // 이동하는 플레이어 win
				score[p.idx] += abs(diff);

				pBoard[pos.X][pos.Y].s = 0;

				Loose(ep);

				players[i] = { nx, ny };
				pBoard[nx][ny] = p;

				Win(p);

			}
			else if (diff < 0 || (diff == 0 && ep.s > p.s)) { // 적 win
				score[ep.idx] += abs(diff);

				players[i] = {nx, ny};
				pBoard[nx][ny] = p;
				pBoard[pos.X][pos.Y].s = 0;
				Loose(p);

				Win(ep);
			}
		}
	}
}

void Solve() {
	cin >> n >> m >> k;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int gs;
			cin >> gs;
			if (gs > 0) gBoard[i][j].push_back(gs);
		}
	}

	for(int i = 0 ; i < m; i++){
		int x, y, d, s;
		cin >> x >> y >> d >> s;
		x--; y--;
		players.push_back({ x, y });
		pBoard[x][y] = { i, s, 0, d };
	}

	while (k--) {
		playerAct();
	}



	for (int i = 0; i < m; i++) {
		cout << score[i] << ' ';
	}

}

int main(void) {
	Solve();

	return 0;
}