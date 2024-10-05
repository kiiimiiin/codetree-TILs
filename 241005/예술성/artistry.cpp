#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#define X first
#define Y second
const int dx[4] = { -1, 0, 0, 1 };
const int dy[4] = { 0, -1, 1, 0 };
using namespace std;
int n, c;
int board[30][30];
int tmp[30][30];
queue<pair<int, int>> q;

struct Group {
	int num;
	int area;
};

Group group[900];

bool OOB(int x, int y) {
	return x < 0 || x >= n || y < 0 || y >= n;
}

int getArtScore() {
	int ret = 0;
	int vis[30][30] = {};
	vector<tuple<int, int, int>> neighbor; // 그룹, 그룹, 맞닿아 있는 변수

	int idx = 1;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (!vis[i][j]) {
				unordered_map<int, int> m;
				int area = 0;

				q.push({ i,j });
				vis[i][j] = idx;
				while (!q.empty()) {
					auto cur = q.front(); q.pop(); area++;
					for (int dir = 0; dir < 4; dir++) {
						int nx = cur.X + dx[dir];
						int ny = cur.Y + dy[dir];
						if (OOB(nx, ny) || vis[nx][ny] == idx) continue;
						if (vis[nx][ny] > 0) {
							m[vis[nx][ny]]++;
							continue;
						}
						if (board[i][j] == board[nx][ny]) {
							vis[nx][ny] = idx;
							q.push({ nx,ny });
						}
					}
				}

				for (auto e : m)
					neighbor.push_back(make_tuple(e.X, vis[i][j], e.Y));
				
				group[idx].num = board[i][j];
				group[idx].area = area;
				idx++;
			}
		}
	}

	for (auto cse : neighbor) {
		int a, b, side;
		tie(a, b, side) = cse;
		ret += (group[a].area + group[b].area) *
			group[a].num * group[b].num * side;
	}

	return ret;
}

void RotateCross() {
	const int rdx[4] = { 0, 1, 0, -1 };
	const int rdy[4] = { 1, 0, -1, 0 };
	for (int p = 1; p <= c; p++) {
		for (int dir = 0; dir < 3; dir++) {
			int x = c + p * rdx[dir];
			int y = c + p * rdy[dir];
			int nx = c + p * rdx[dir + 1];
			int ny = c + p * rdy[dir + 1];
			swap(board[x][y], board[nx][ny]);
		}
	}
}

void RotateClock(int x, int y){
	for (int i = 0; i < c; i++) {
		for (int j = 0; j < c; j++) {
			tmp[j][c - 1 - i] = board[i + x][j + y];
		}
	}

	for (int i = 0; i < c; i++) {
		for (int j = 0; j < c; j++) {
			board[i + x][j + y] = tmp[i][j];
		}
	}
}

void Rotate() {

	RotateCross();

	for (int x = 0; x < n; x += c + 1) {
		for (int y = 0; y < n; y += c + 1) {
			RotateClock(x, y);
		}
	}
}

int main() {
	ios::sync_with_stdio(0), cin.tie(0);

	cin >> n;

	c = (n - 1) / 2;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> board[i][j];

	int ans = getArtScore();

	for (int t = 1; t <= 3; t++) {
		Rotate();
		ans += getArtScore();
	}
	
	cout << ans;

}