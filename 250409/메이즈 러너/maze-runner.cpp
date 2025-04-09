#include <iostream>
#include <queue>
#include <tuple>
#define X first
#define Y second
using namespace std;
int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0, -1, 1 }; // 상하좌우
int n, m, k;
int board[12][12]; // wall
int guest[12][12];
int tmp1[12][12];
int tmp2[12][12];
int ex, ey;

int getMnDist(int x1, int x2, int y1, int y2) {
	return abs(x1 - x2) + abs(y1 - y2);
}

bool OOB(int nx, int ny) {
	return nx < 0 || nx >= n || ny < 0 || ny >= n; 
}

int Move(int& existGuest) {
	int tmp[12][12] = {};
	int ret = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (guest[i][j]) {
				int mnDist = getMnDist(i, ex, j, ey);
				int tx = -1;
				int ty = -1;
				for (int dir = 0; dir < 4; dir++) {
					int nx = i + dx[dir];
					int ny = j + dy[dir];
					if (OOB(nx, ny) || board[nx][ny]) continue;
					int nmnDist = getMnDist(nx, ex, ny, ey);
					if ( nmnDist < mnDist) {
						tx = nx; ty = ny;
						mnDist = nmnDist;
					}
				}
				if (tx == -1 && ty == -1) {
					tmp[i][j] += guest[i][j];
					guest[i][j] = 0;
					continue;
				}
				
				if (!(tx == ex && ty == ey))
					tmp[tx][ty] += guest[i][j];
				else
					existGuest = existGuest - guest[i][j];

				ret += guest[i][j];
				guest[i][j] = 0;
			}
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			guest[i][j] = tmp[i][j];
		}
	}
	return ret;

}

bool IsChoosed(int x, int y, int l) {
	bool existExit = false;
	bool existGuest = false;
	for (int i = x; i < x + l; i++) {
		for (int j = y; j < y + l; j++) {
			if (guest[i][j])
				existGuest = true;
			if (i == ex && j == ey)
				existExit = true;
			if (existGuest && existExit)
				return true;
		}
	}
	return false;
}
tuple<int,int, int> FindMaze() {
	
	for (int l = 2; l <= n; l++) {
		for (int i = 0; i <= n - l; i++) {
			for (int j = 0; j <= n - l; j++) {
				if (IsChoosed(i, j, l)) {
					return make_tuple(i, j, l);
				}			
			}
		}
	}
	return make_tuple(-1, -1, -1);
}

void Rotate(int r, int c, int l) {
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < l; j++) {
			tmp1[j][l - 1 - i] = board[r + i][c + j] ? board[r + i][c + j] - 1 : 0;
			tmp2[j][l - 1 - i] = guest[r + i][c + j];
		}
	}

	for (int i = 0; i < l; i++) {
		for (int j = 0; j < l; j++) {
			board[r + i][c + j] = tmp1[i][j];
			guest[r + i][c + j] = tmp2[i][j];
		}
	}

	int x = ey - c;
	int y = l - 1 - (ex - r);
	
	ex = r + x;
	ey = c + y;
}
int main() {
	cin >> n >> m >> k;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}

	int x, y;
	for (int i = 0; i < m; i++) {
		cin >> x >> y;
		guest[x - 1][y - 1]++;
	}

	cin >> ex >> ey;
	ex--; ey--;

	int existGuest = m;
	int moveSum = 0;
	for (int t = 1; t <= k; t++) {
		moveSum += Move(existGuest);
		if (existGuest  <= 0) break;
		int r, c, l;
		tie(r, c, l) = FindMaze();
		if (r == -1 && c == -1) cout << "what";
		Rotate(r, c, l);
	}

	cout << moveSum << '\n' << ex + 1 << ' ' << ey + 1;
}