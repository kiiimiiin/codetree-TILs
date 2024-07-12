#include <iostream>
#include <vector>
#include <tuple>
#include <utility>
#define X first
#define Y second
int dx[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };// 1 3 5 7 : 대각선
int dy[8] = { 0,1, 1, 1, 0, -1, -1, -1 }; // 0 2 4 6 : 상우하좌
using namespace std;
int n, m, k;
vector<tuple<int,int,int>> board[52][52]; // 질량,속력,방향

pair<int, int> ProcessOOB(int nx, int ny) {
	if (nx < 0) nx = n + nx;
	if (nx >= n) nx = nx - n;
	if (ny < 0) ny = n + ny;
	if (ny >= n) ny = ny - n;
	return { nx, ny };
}

void Simulate() {

	vector<tuple<int, int, int>> cBoard[52][52];
	// 이동
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j].empty()) continue;
			for (auto a : board[i][j]) {
				int m, s, dir;
				tie(m, s, dir) = a;
				int nx = i + ( s % n ) * dx[dir];
				int ny = j + ( s % n ) * dy[dir];
				tie(nx, ny) = ProcessOOB(nx, ny);
				cBoard[nx][ny].push_back({ m, s, dir });
			}
		}
	}

	// 원자 합치기
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (cBoard[i][j].size() >= 2) {
				int nm, ns, dirFlag, aNum;
				nm = ns = dirFlag = 0;
				aNum = cBoard[i][j].size();

				for (auto a : cBoard[i][j]) {
					int m, s, dir; 
					tie(m, s, dir) = a;
					nm += m;
					ns += s;
					dirFlag += dir % 2;
				}
				nm = nm / 5; 
				ns = ns / aNum; 
				cBoard[i][j].clear(); 
				if (nm < 1) continue;
				
				if (dirFlag == 0 || dirFlag == aNum) {
					for (int ndir : {0, 2, 4, 6})
						cBoard[i][j].push_back({ nm, ns, ndir });
				}
				else {
					for (int ndir : {1, 3, 5, 7})
						cBoard[i][j].push_back({ nm, ns, ndir });
				}

			}
		}
	}

	// 보드 복사
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			board[i][j] = cBoard[i][j];
		}
	}
}

int Ans() {
	int ret = 0; 
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j].empty()) continue; 
			for (auto a : board[i][j]) {
				ret += get<0>(a);
			}
		}
	}

	return ret;
}
int main(void) {
	cin >> n >> m >> k;
	while (m--) {
		int x, y, m, s, d;
		cin >> x >> y >> m >> s >> d;
		x = x - 1;
		y = y - 1;
		board[x][y].push_back({ m,s,d });
	}


	for (int i = 1; i <= k; i++) {
		Simulate();
	}
	
	int ans = Ans();
	cout << ans;
}