#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#define X first
#define Y second
using namespace std; 
const int dx[4] = { -1,0,1,0 };
const int dy[4] = { 0,1,0,-1 };
int n, m, k;
vector<int> gun[21][21];

struct Person {
	int x, y, d, s, gs, score;
};

Person person[31];
int board[21][21];

// 이동 -> 플레이어 x
//		-> 플레이어 o -> 진 플레이어는 또 이동
//					-> 이긴 플레이어는 그자리에서 총줍기

bool OOB(int x, int y) {
	return x < 0 || x >= n || y < 0 || y >= n;
}

bool IsNoPerson(int x, int y) {
	return board[x][y] == 0;
}

tuple<int,int, int> getWinner(int idx1, int idx2) {
	auto p1 = person[idx1];
	auto p2 = person[idx2];
	int score = p1.s + p1.gs - p2.s - p2.gs;
	int winIdx, loseIdx;

	if (score > 0)
		winIdx = idx1;
	else if (score < 0)
		winIdx = idx2;
	else {
		if (p1.s > p2.s)
			winIdx = idx1;
		else
			winIdx = idx2;
	}

	loseIdx = ( winIdx == idx1 ? idx2 : idx1);

	return make_tuple(winIdx, loseIdx, abs(score));
}

void getGun(int idx,int nx, int ny) {
	if (person[idx].gs > 0) gun[nx][ny].push_back(person[idx].gs);
	sort(gun[nx][ny].begin(), gun[nx][ny].end());
	person[idx].gs = gun[nx][ny].back();
	gun[nx][ny].pop_back();
}

void LoserAct(int idx) {
	gun[person[idx].x][person[idx].y].push_back(person[idx].gs);
	person[idx].gs = 0;

	int nx, ny, nd;
	for (int dd = 0; dd < 4; dd++) {
		nd = person[idx].d + dd;
		nx = person[idx].x + dx[nd];
		ny = person[idx].y + dy[nd];
		if (OOB(nx, ny) || board[nx][ny])
			continue;
		break;
	}

	if (!gun[nx][ny].empty()) {
		getGun(idx, nx, ny);
	}
	board[nx][ny] = idx;
	person[idx].x = nx;
	person[idx].y = ny;
	person[idx].d = nd;
	
}

void WinAct(int idx) {

	if (!gun[person[idx].x][person[idx].y].empty()) {
		getGun(idx, person[idx].x, person[idx].y);
	}

	board[person[idx].x][person[idx].y] = idx;
}

void Simulate() {
	for (int idx = 1; idx <= m; idx++) {
		int nx = person[idx].x + dx[person[idx].d];
		int ny = person[idx].y + dy[person[idx].d];

		if (OOB(nx, ny)) {
			person[idx].d = (person[idx].d + 2) % 4;
			nx = person[idx].x + dx[person[idx].d];
			ny = person[idx].y + dy[person[idx].d];
		}
		
		if (IsNoPerson(nx, ny)) {
			if (!gun[nx][ny].empty()) { // 2 - 1
				getGun(idx, nx, ny);				
			}
			board[person[idx].x][person[idx].y] = 0;
			board[nx][ny] = idx;
			person[idx].x = nx;
			person[idx].y = ny;
			
		}
		else {

			int winIdx, loseIdx, score;
			tie(winIdx, loseIdx, score) = getWinner(idx, board[nx][ny]);
			board[person[idx].x][person[idx].y] = 0;
			board[nx][ny] = 0;
			person[idx].x = nx; person[idx].y = ny;
			
			LoserAct(loseIdx);
			WinAct(winIdx);

			person[winIdx].score += score; 
		}
	}
}
int main() {
	ios::sync_with_stdio(0), cin.tie(0);

	cin >> n >> m >> k;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int gs;
			cin >> gs;
			gun[i][j].push_back(gs);
		}
	}

	for (int idx = 1; idx <= m; idx++) {
		int x, y, d, s;
		cin >> x >> y >> d >> s;
		person[idx] = { x - 1, y - 1, d, s , 0};
		board[x - 1][y - 1] = idx;
	}


	while (k--) {
		Simulate();
	}

	
	for (int idx = 1; idx <= m; idx++)
		cout << person[idx].score << ' ';

}