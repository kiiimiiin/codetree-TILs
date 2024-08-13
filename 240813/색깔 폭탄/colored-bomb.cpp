#include <iostream>
#include <queue>
#include <algorithm>
#include <utility>
#include <tuple>
#define X first
#define Y second
using namespace std;
const int dx[4] = { 1, 0, -1, 0 };
const int dy[4] = { 0, 1, 0, -1 };
int n, m;
int board[22][22];


bool OOB(int x, int y) {
	return ( x < 0 || x >= n || y < 0 || y >= n ); 
}

int Remove() {
	queue<pair<int, int>> q;
	queue<pair<int, int>> rq;
	int vis[22][22] = {};
	int cx, cy, color, size; 
	tuple<int, int, int, int> mx = { -1, -1, -1, -1};

	// 가장 크기가 큰 , 빨간 폭탄이 가장 적은,
	// 기준점의 행이 큰, 열이 작은
	// 기준점 : 폭탄 묶음 중 빨간색이 아니며 행이 큰, 열이 작은
	
	// 1. 조건에 맞는 폭탄 택
	for (int i = n - 1; i >= 0; i--) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] > 0 && !vis[i][j]) {
				size = 0;
				color = board[i][j];
				cx = i, cy = j;
				q.push({ i,j });
				vis[i][j] = 1;
				while (!q.empty()) {
					auto cur = q.front(); q.pop(); size++;
					for (int dir = 0; dir < 4; dir++) {
						int nx = cur.X + dx[dir];
						int ny = cur.Y + dy[dir];
						if (OOB(nx, ny) || vis[nx][ny]) continue;
						if (board[nx][ny] == color) {
							q.push({ nx,ny });
							vis[nx][ny] = 1;
						}
						else if (board[nx][ny] == 0) {
							q.push({ nx,ny });
							rq.push({ nx,ny });
							vis[nx][ny] = 1;
						}
					}
				}
				if (size == 1) continue;

				int rsize = rq.size();
				
				while (!rq.empty()) {
					auto pos = rq.front(); rq.pop(); 
					vis[pos.X][pos.Y] = 0;
				}
				
				if (make_tuple(size, -rsize, cx, -cy) > mx) {
					mx = { size, -rsize, cx, -cy };
				}

			}
		}
	} 
	
	// 2. 택한 폭탄 묶음 제거


	tie(size, ignore, cx, cy) = mx;
	if (size == -1) return -1;
	cy = -cy;

	for (int i = 0; i < n; i++) 
		fill(vis[i], vis[i] + n, 0);
	q.push({ cx, cy });
	vis[cx][cy] = 1;
	color = board[cx][cy];
	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		board[cur.X][cur.Y] = -2; 
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			if (OOB(nx, ny) || vis[nx][ny]) continue;
			if (board[nx][ny] == color || board[nx][ny] == 0) {
				q.push({ nx,ny });
				vis[nx][ny] = 1;
			}
		}
	}

	return size * size;
}

void DoGravity() {
	// 떨어뜨려질 기준 cx, cy와 떨어뜨림을 당할 i, j  

	for (int j = 0; j < n; j++) {
		int cx = n;
		for (int i = n - 1; i >= 0; i--) {
			if (board[i][j] == -2) continue;
			if (board[i][j] == -1) {
				cx = i;
			}
			else {
				int tmp = board[i][j];
				board[i][j] = -2;
				board[cx - 1][j] = tmp;
				cx = cx - 1;
			}
		}
	}
}

void Rotate() {
	int temp[22][22];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			temp[n-1-j][i] = board[i][j];
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			board[i][j] = temp[i][j];
		}
	}
}
int main() {
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}
	
	int ans = 0;
	while (1) {
		int size = Remove();
		
		if (size == -1) break;
		else ans += size;

		DoGravity();
		Rotate();
		DoGravity();
	}
	cout << ans;
}