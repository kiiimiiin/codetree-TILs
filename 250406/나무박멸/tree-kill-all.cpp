#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <queue>
#define X first
#define Y second
using namespace std;
int dx[4] = { 1, 0, -1, 0 };
int dy[4] = { 0, 1, 0, -1 };
int n, m, k, c;
int board[22][22];
int gas[22][22];

bool OOB(int nx, int ny) {
	return nx < 0 || nx >= n || ny < 0 || ny >= n;
}
void Grow() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] > 0) {
				int cnt = 0;
				for (int dir = 0; dir < 4; dir++) {
					int nx = i + dx[dir];
					int ny = j + dy[dir];
					if (OOB(nx, ny)) continue;
					if (board[nx][ny] > 0)
						cnt++;
				}
				board[i][j] += cnt;
			}
		}
	}


}

int Remove() {
	
	int mx = -0x7f7f7f7f;
	int mr = -1;
	int mc = -1;
	
	int rdx[4] = { -1, -1, 1, 1, };
	int rdy[4] = { -1, 1, -1, 1 };

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] > 0) {
				int removed = board[i][j];
				for (int dir = 0; dir < 4; dir++) {
					for (int p = 1; p <= k; p++) {
						int nx = i + p * rdx[dir];
						int ny = j + p * rdy[dir];
						if (OOB(nx, ny) || board[nx][ny] <= 0)
							break;
						removed += board[nx][ny];
					}
				}
				if (removed > mx) {
					mx = removed;
					mr = i; mc = j;
				}
			}
			
		}
	}

	if (mx == -0x7f7f7f7f) return 0;
	
	gas[mr][mc] = c + 1;
	board[mr][mc] = 0;

	for (int dir = 0; dir < 4; dir++) {
		for (int p = 1; p <= k; p++) {
			int nx = mr + p * rdx[dir];
			int ny = mc + p * rdy[dir];
			if (OOB(nx, ny) || board[nx][ny] == -1) break;
			if (board[nx][ny] == 0) {
				gas[nx][ny] = c + 1;
				break;
			}
			gas[nx][ny] = c + 1;
			board[nx][ny] = 0;
		}
	}
	return mx;
}

void UpdateGas() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (gas[i][j])
				gas[i][j]--;
		}
	}
}
void Copy() {
	int tmp[22][22] = {};

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] > 0) {
				int cnt = 0;
				for (int dir = 0; dir < 4; dir++) {
					int nx = i + dx[dir];
					int ny = j + dy[dir];
					if (OOB(nx, ny)) continue;
					if (board[nx][ny] == 0 && gas[nx][ny] == 0)
						cnt++;
				}
				for (int dir = 0; dir < 4; dir++) {
					int nx = i + dx[dir];
					int ny = j + dy[dir];
					if (OOB(nx, ny)) continue;
					if (board[nx][ny] == 0 && gas[nx][ny] == 0 )
						tmp[nx][ny] += board[i][j] / cnt;
				}
			}
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (tmp[i][j] == 0) continue;
			board[i][j] += tmp[i][j];
			tmp[i][j] = 0;
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
		Copy();
		ans += Remove();
		UpdateGas();
	}
	cout << ans;
}

