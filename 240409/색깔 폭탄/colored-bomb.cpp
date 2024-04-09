#include <iostream>
#include <algorithm>
#include <queue>
#include <utility>
#include <tuple>
#define X first 
#define Y second 
using namespace std;
int n, m;
int board[22][22];
int dx[4] = { 1, 0, -1, 0 };
int dy[4] = { 0, 1, 0, -1 };

bool OOB(int nx, int ny) {
	return (nx < 0 || nx >= n || ny < 0 || ny >= n);
}

bool Cmp(pair<int, int>& p1, pair<int, int>&p2) { // 행, 열
	if (p1.first != p2.first) return p1.first > p2.first;
	return p2.second < p2.second; 
}

bool InfoCmp(tuple<int, int, int, int>& t1, tuple<int, int, int, int>& t2) {
	// 크기, 빨간폭탄수, 기준점 x, y
	if (get<0>(t1) != get<0>(t2)) return get<0>(t1) > get<0>(t2);
	if (get<1>(t1) != get<1>(t2)) return get<1>(t1) < get<1>(t2);
	if (get<2>(t1) != get<2>(t2)) return get<2>(t1) > get<2>(t2);
	return get<3>(t1) < get<3>(t2);
}

tuple<int,int,int,int> bfs(int x, int y, int vis[22][22]) {
	queue<pair<int, int>> q;
	vector<pair<int, int>> colorPos;
	vector<pair<int, int>> redPos;
	q.push({ x, y });
	vis[x][y] = 1;
	colorPos.push_back({ x,y });
	int color = board[x][y];
	int redCnt = 0;
	int area = 0;
	int sx, sy;

	while (!q.empty()) {
		auto cur = q.front(); q.pop(); area++;
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			if (OOB(nx, ny) || vis[nx][ny]) continue;
			if (board[nx][ny] == 0 || board[nx][ny] == color) { // 빨간색이거나 같은 유색
				q.push({ nx, ny });
				vis[nx][ny] = 1;
				if (board[nx][ny] == 0) {
					redPos.push_back({ nx,ny });
				}
				else if (board[nx][ny] == color) {
					colorPos.push_back({ nx,ny });
				}
			}
		}
	}

	redCnt = redPos.size();
	for (auto pos : redPos) { // 다른 경우 방문을 위한 빨간색 방문 초기화
		vis[pos.X][pos.Y] = 0;
	}

	// 유색 구슬 기준좌표 찾기 위한 sort
	sort(colorPos.begin(), colorPos.end(), Cmp);
	tie(sx, sy) = colorPos.front(); 
	
	return { area, redCnt, sx, sy }; // 크기, 빨간폭탄수, 기준점 x, y
}

int BombGroup(int sx, int sy) {
	int color = board[sx][sy];
	int area = 0;
	queue<pair<int, int>> q;
	q.push({ sx, sy });
	board[sx][sy] = -2;

	while (!q.empty()) {
		auto cur = q.front(); q.pop(); area++;
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			if (OOB(nx, ny)) continue;
			if (board[nx][ny] == 0 || board[nx][ny] == color) { // 빨간색이거나 같은 유색
				q.push({ nx, ny });
				board[nx][ny] = -2;
				
			}
		}
	}

	return area;
}

int ProcessGroup() {
	int vis[22][22] = {};
	// 1. 그룹핑
	vector<tuple<int, int, int, int>> infos;
	int sx, sy;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] >= 1 && !vis[i][j]) { // 그룹핑 되지 않은 유색구슬에 대해 
				int area, redCnt, sx, sy;
				tie(area, redCnt, sx, sy) = bfs(i, j, vis);
				if(area >= 2)
					infos.push_back({ area, redCnt, sx, sy });
			}
		}
	}

	if (infos.size() == 0) return -1;

	sort(infos.begin(), infos.end(), InfoCmp);

	tie(ignore, ignore, sx, sy) = infos.front();
	// 2. 그룹 폭발
	int area = BombGroup(sx, sy);
	return area;
}

void doGravity() {
	for (int j = 0; j < n; j++) {
		for (int i = n - 2; i >= 0; i--) {
			if (board[i][j] >= 0) {
				int ny = i + 1;
				int tmp = board[i][j];
				board[i][j] = -2;
				while (ny <= n && board[ny][j] == -2) {
					ny++;
				}
				board[ny - 1][j] = tmp;
			}
		}
	}
}

void Rotate() {
	int cBoard[22][22] = {}; 
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cBoard[n - 1 - j][i] = board[i][j];
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			board[i][j] = cBoard[i][j];
		}
	}
}
void UpdateBoard() {
	doGravity();
	Rotate();
	doGravity();
}


int main(void) {
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}

	int ans = 0;

	while (1) {
		int bombedArea = ProcessGroup();
		if (bombedArea == -1) break;
		ans += bombedArea * bombedArea;
		UpdateBoard();
	}

	cout << ans;
}