#include <iostream>
#include <algorithm>
#define X first
#define Y second
const int dx[4] = { 1, 0, -1 ,0 };
const int dy[4] = { 0, 1, 0, -1 };
using namespace std;
int n, m, k, c; 
int board[22][22];
int removed[22][22]; 

bool OOB(int x, int y) {
	return (x < 0 || x >= n || y < 0 || y >= n);
}

void Grow() {
	// 1. 성장
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] > 0) {
				int cnt = 0; // 주변 나무 수 
				for (int dir = 0; dir < 4; dir++) {
					int nx = i + dx[dir];
					int ny = j + dy[dir];
					if (OOB(nx, ny)) continue;
					if (board[nx][ny] > 0) cnt++;
				}
				board[i][j] += cnt;
			}
		}
	}
	// 2. 번식
	int temp[22][22] = {}; 
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] > 0) {
				int cnt = 0; // 주변 빈칸 수
				for (int dir = 0; dir < 4; dir++) {
					int nx = i + dx[dir];
					int ny = j + dy[dir];
					if (OOB(nx, ny) || removed[nx][ny]) continue;
					if (board[nx][ny] == 0) { 
						cnt++;
					}
				}
				for (int dir = 0; dir < 4; dir++) {
					int nx = i + dx[dir];
					int ny = j + dy[dir];
					if (OOB(nx, ny) || removed[nx][ny]) continue;
					if (board[nx][ny] == 0) { 
						temp[nx][ny] += board[i][j] / cnt;
					}
				}
			}
		}
	}
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			board[i][j] += temp[i][j];
		}
	}
}

int Remove() {
	// 1. 칸에 대한 제초제 박멸 총 그루수 확인 
	int rdx[4] = { 1, 1, -1, -1 };
	int rdy[4] = { 1, -1, 1, -1 };
	pair<int, int> pos = { -1, -1 };
	int mx = -0x7f7f7f7f; 

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] > 0) {
				int cnt = board[i][j]; // 제거 나무 그루 수
				for (int dir = 0; dir < 4; dir++) {
					for (int s = 1; s <= k; s++) {
						int nx = i + s * rdx[dir];
						int ny = j + s * rdy[dir];
						if (OOB(nx, ny)) break;
						if (board[nx][ny] <= 0) break;
						if (board[nx][ny] > 0) {
							cnt += board[nx][ny];
						}
					}
				}
				if (cnt > mx) {
					mx = cnt;
					pos = { i, j };
				}
			}
		}
	}

	// if (mx == -0x7f7f7f7f) return 0; // 박멸하는 경우가 없음

	// 2. 박멸
	board[pos.X][pos.Y] = 0; 
	removed[pos.X][pos.Y] = c + 1 ;

	for (int dir = 0; dir < 4; dir++) {
		for (int s = 1; s <= k; s++) {
			int nx = pos.X + s * rdx[dir];
			int ny = pos.Y + s * rdy[dir];
			if (OOB(nx, ny)) break;
			if (board[nx][ny] <= 0) {
				removed[nx][ny] = c + 1;
				break;
			}
			if (board[nx][ny] > 0) {
				board[nx][ny] = 0;
				removed[nx][ny] = c + 1;
			}
		}
	}

	return mx;
}

void UpdateRemoved() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (removed[i][j]) removed[i][j]--;
		}
	}
}

int main() {
	cin >> n >> m >> k >> c;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}

	int ans = 0;
	while (m--) {
		Grow();
		ans += Remove();
		UpdateRemoved();
	}

	cout << ans;
}