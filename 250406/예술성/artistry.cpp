#include <iostream>
#include <queue>
#define X first
#define Y second
using namespace std;
int dx[4] = { 1, 0, -1, 0 };
int dy[4] = { 0, -1, 0, 1 }; // 하 좌 상 우
int n, c;
int board[30][30];
int tmp[30][30];

struct Group {
	int area;
	int num;
};



bool OOB(int nx, int ny) {
	return nx < 0 || nx >= n || ny < 0 || ny >= n; 
}

int getScore() {

	queue<pair<int, int>> q; 
	queue<pair<int, int>> cse;
	Group group[900];
	int near[900] = {};
	int vis[30][30] = {};
	int idx = 1;
	int ret = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (vis[i][j]) continue;
			int num = board[i][j];
			int area = 0;
			fill(near, near + idx, 0);
			q.push({ i,j });
			vis[i][j] = idx;
			
			while (!q.empty()) {
				auto cur = q.front(); q.pop(); area++;
				for (int dir = 0; dir < 4; dir++) {
					int nx = cur.X + dx[dir];
					int ny = cur.Y + dy[dir];
					if (OOB(nx, ny) || vis[nx][ny] == idx) continue;
					if (board[nx][ny] == num) {
						q.push({ nx, ny });
						vis[nx][ny] = idx;
					}
					else if (board[nx][ny] != num && vis[nx][ny]) {
						near[vis[nx][ny]]++;
					}
				}
			}
			group[idx].area = area; 
			group[idx].num = num;
			for (int f = 1; f < idx; f++) {
				if (near[f] == 0) continue;
				ret += (group[f].area + group[idx].area)
					* group[f].num * group[idx].num
					* near[f];
			}
			idx++;
		}
	}


	return ret;
}
void SubRotate(int x, int y) {
	for (int i = 0; i < c; i++)
		for (int j = 0; j < c; j++)
			tmp[j][c - 1 - i] = board[i + x][j + y];

	for (int i = 0; i < c; i++)
		for (int j = 0; j < c; j++)
			board[i + x][j + y] = tmp[i][j];
}
void Rotate() {
	for (int p = 1; p <= c; p++) {
		for (int dir = 0; dir < 3; dir++) {
			int nx = c + p * dx[dir];
			int ny = c + p * dy[dir];
			int nnx = c + p * dx[(dir + 1) % 4];
			int nny = c + p * dy[(dir + 1) % 4];
			
			int tmp = board[nnx][nny];
			board[nnx][nny] = board[nx][ny];
			board[nx][ny] = tmp;
		}
	}


	for (int i = 0; i < n; i += (c + 1))
		for (int j = 0; j < n; j += (c + 1))
			SubRotate(i, j);
}

int main() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}

	c = (n - 1) / 2;

	int ans = getScore();
	for (int i = 0; i < 3; i++) {
		Rotate();
		ans += getScore();
	}
	cout << ans;
}

/*
(그룹 a에 속한 칸의 수 + 그룹 b에 속한 칸의 수 )
x 그룹 a를 이루고 있는 숫자 값 x 그룹 b를 이루고 있는 숫자 값 
x 그룹 a와 그룹 b가 서로 맞닿아 있는 변의 수
*/

