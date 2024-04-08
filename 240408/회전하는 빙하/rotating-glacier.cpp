#include <iostream>
#include <utility>
#include <queue>
#include <algorithm>
#include <tuple>
#define X first
#define Y second
using namespace std;
int board[66][66];
int cBoard[66][66];
int dx[4] = { 0, 1, -1, 0 };
int dy[4] = { 1, 0, 0, -1 }; // 우 하 상 좌 (격자내 부분격자 시계회전위함)
int n, q;

void SubRotate(int x, int y , int l) {
	
	int ti, tj;
	
	int s = 1 << (l - 1);
	int dir = 0; 
	for (int i = 0; i < (1 << l); i += (1 << (l - 1))) {
		for (int j = 0; j < (1 << l); j += (1 << (l - 1))) {
			
			for (int r = i; r < i + (1 << (l - 1)); r++) {
				for (int c = j; c < j + (1 << (l - 1)); c++) {
					cBoard[r + s * dx[dir]][c + s * dy[dir]] 
						= board[x + r][y + c];
				}
			}
			dir++;
		}
	}

	for (int i = 0; i < (1 << l); i++) {
		for (int j = 0; j < (1 << l); j++) {
			board[x + i][y + j] = cBoard[i][j];
		}
	}

}

void Rotate(int l) {
	if (l == 0) return;
	for (int i = 0; i < (1 << n); i += (1 << l)) {
		for (int j = 0; j < (1 << n); j += (1 << l)) {
			SubRotate(i, j, l);
		}
	}
}

bool OOB(int nx, int ny) {
	return (nx < 0 || nx >= (1 << n) || ny < 0 || ny >= (1 << n));
}
void Melt() {
	int meltingBoard[66][66] = {}; // 얼음 melt 동시처리

	for (int i = 0; i < (1 << n); i++) {
		for (int j = 0; j < (1 << n); j++) {
			if (board[i][j] > 0) {
				int cnt = 0;
				for (int dir = 0; dir < 4; dir++) {
					int nx = i + dx[dir];
					int ny = j + dy[dir];
					if (OOB(nx, ny) || board[nx][ny] == 0) continue;
					cnt++;
				}

				if (cnt < 3) meltingBoard[i][j] = 1;
			}
		}
	}

	for (int i = 0; i < (1 << n); i++) {
		for (int j = 0; j < (1 << n); j++) {
			if (meltingBoard[i][j]) board[i][j]--;
		}
	}

}

pair <int,int> Ans() {
	int vis[66][66] = {};
	int mxArea = -0x7f7f7f7f;
	int sum = 0;

	for (int i = 0; i < (1 << n); i++) {
		for (int j = 0; j < (1 << n); j++) {
			if (board[i][j] && !vis[i][j]) {
				queue<pair<int, int>> q;
				int area = 0;
				q.push({ i,j });
				vis[i][j] = 1;
				while (!q.empty()) {
					auto cur = q.front(); q.pop();
					sum += board[cur.X][cur.Y];
					area++;
					for (int dir = 0; dir < 4; dir++) {
						int nx = cur.X + dx[dir];
						int ny = cur.Y + dy[dir];
						if (OOB(nx, ny) || vis[nx][ny]) continue;
						if (board[nx][ny] == 0) continue;
						q.push({ nx,ny });
						vis[nx][ny] = 1;
					}

					mxArea = max(mxArea, area);
				}
			}
		}
	}

	if (mxArea == -0x7f7f7f7f || mxArea == 1) mxArea = 0; // 얼음군집 존재 x
	return { sum ,mxArea };
}

int main(void) {
	cin >> n >> q;

	for (int i = 0; i < (1 << n); i++) {
		for (int j = 0; j < (1 << n); j++) {
			cin >> board[i][j];
			
		}
	}

	while (q--) {
		int l;
		cin >> l;
		Rotate(l);
		Melt();
	}

	int sum, mxArea; 
	tie(sum, mxArea) = Ans();

	cout << sum << '\n' << mxArea; 
}