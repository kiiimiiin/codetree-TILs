#include <iostream>
#include <tuple>
#include <algorithm>
#include <utility>
#include <queue>
#define X first
#define Y second
using namespace std;
int k, m;
const int dx[4] = { -1, 0, 1, 0 };
const int dy[4] = { 0, 1, 0, -1 };
int board[5][5], tmp[5][5], vis[5][5];
int rtmp[3][3];

queue<int> eq;
queue<pair<int, int>> q, rq;


void Copy(int desc[5][5], int src[5][5]) {
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			desc[i][j] = src[i][j];
}

void Rotate(int x, int y, int r) {

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if(r == 0)
				rtmp[j][2 - i] = board[x - 1 + i][y - 1 + j];
			else if(r == 1)
				rtmp[2- i][2 - j] = board[x - 1 + i][y - 1 + j];
			else
				rtmp[2-j][i] = board[x - 1 + i][y - 1 + j];
		}
	}
	
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			board[x - 1 + i][y - 1 + j] = rtmp[i][j];
}

bool OOB(int x, int y) {
	return x < 0 || x >= 5 || y < 0 || y >= 5;
}

int getWorth() {

	for (int i = 0; i < 5; i++) fill(vis[i], vis[i] + 5, 0);

	int score = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (!vis[i][j]) {
				int area = 0;
				q.push({ i, j });
				rq.push({ i, j });
				vis[i][j] = 1;
				
				while (!q.empty()) {
					auto cur = q.front(); q.pop(); area++;
					for (int dir = 0; dir < 4; dir++) {
						int nx = cur.X + dx[dir];
						int ny = cur.Y + dy[dir];
						if (OOB(nx, ny) || vis[nx][ny]) continue;
						if (board[nx][ny] != board[cur.X][cur.Y]) continue;
						q.push({ nx,ny });
						rq.push({ nx, ny });
						vis[nx][ny] = 1;
					}
				}

				if (area >= 3) {
					score += area;
					while (!rq.empty()) {
						auto cur = rq.front(); rq.pop();
						board[cur.X][cur.Y] = 0;
					}
				}
				else
					while (!rq.empty()) rq.pop();
			}
		}
	}

	return score;
}
tuple<int,int,int> getTarget() {
	
	
	Copy(tmp, board);

	int mx, tx, ty, tr;
	mx = 0;

	for (int r = 0; r < 3; r++) {
		for (int j = 1; j <= 3; j++) {
			for (int i = 1; i <= 3; i++) {
				Rotate(i, j, r);
				int worth = getWorth();
				if (worth > mx) {
					mx = worth;
					tx = i; ty = j; tr = r;
				}

				Copy(board, tmp);
			}
		}
	}

	if (mx == 0) return make_tuple(-1,-1,-1); // 유물 제거 불가
	return make_tuple(tx, ty, tr);

}

int ProcessWorth() {
	int ret = 0;
	while (1) {
		int score = getWorth();
		if (score == 0) break;

		int idx = 1;
		for (int j = 0; j < 5; j++) {
			for (int i = 4; i >= 0; i--) {
				if (idx > score) break;
				if (board[i][j]) continue;
				int num = eq.front(); eq.pop();
				board[i][j] = num;
				idx++;
			}
		}
		ret += score;
	}

	return ret;
}

int main() {
	ios::sync_with_stdio(0), cin.tie(0);
	cin >> k >> m;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cin >> board[i][j];
		}
	}

	while (m--) {
		int num;
		cin >> num;
		eq.push(num);
	}


	for (int t = 1; t <= k; t++) {
		int tx, ty, tc;
		tie(tx, ty, tc) = getTarget();
		if (tx == -1 && ty == -1 && tc == -1) break;
		Rotate(tx, ty, tc);

		int worthSum = ProcessWorth();
		cout << worthSum << ' ';
	}

}