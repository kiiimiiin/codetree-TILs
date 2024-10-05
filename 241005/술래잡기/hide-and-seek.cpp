#include <iostream>
#include <vector>
#include <queue>
#include <utility>
using namespace std;
const int dx[4] = { 1, 0, -1, 0 }; // 하 우 상 좌
const int dy[4] = { 0, 1, 0, -1 };
int tree[100][100];
int dirBoard[100][100];
int rdirBoard[100][100];
vector<int> target[100][100];
vector<int> tmp[100][100];
int mx, my, md, n, m, h, k;
bool dirFlag = true;


int getDist(int x, int y) { // 몬스터에서부터 타겟까지 거리
	return abs(x - mx) + abs(y - my);
}

bool OOB(int x, int y) {
	return x < 0 || x >= n || y < 0 || y >= n;
}

bool ExistMonster(int x, int y) {
	return x == mx && y == my;
}

void SubMove(int i, int j) {
	const int dx[4] = { 0, 0, 1, -1 }; // 좌우하상
	const int dy[4] = { -1, 1, 0, 0 };
	
	for (auto dir : target[i][j]) {
		int nx = i + dx[dir];
		int ny = j + dy[dir];
		if (!OOB(nx, ny)) {
			if (ExistMonster(nx, ny))
				tmp[i][j].push_back(dir);
			else
				tmp[nx][ny].push_back(dir);
		}
		else {
			if (dir == 0 || dir == 2) dir++;
			else dir--;
			nx = i + dx[dir];
			ny = j + dy[dir];
			if (ExistMonster(nx, ny))
				tmp[i][j].push_back(dir);
			else
				tmp[nx][ny].push_back(dir);
		}
	}
	target[i][j].clear();
}
void MoveTarget() {
	
	for (int i = mx - 3; i <= mx + 3; i++) {
		for (int j = my - 3; j <= my + 3; j++) {
			if (OOB(i, j) || getDist(i, j) > 3) continue;
			if (target[i][j].empty()) continue;
			SubMove(i, j);
		}
	}

	for (int i = mx - 4; i <= mx + 4; i++) {
		for (int j = my - 4; j <= my + 4; j++) {
			if (OOB(i, j)) continue;
			if (tmp[i][j].empty()) continue;
			while (!tmp[i][j].empty()) {
				target[i][j].push_back(tmp[i][j].back());
				tmp[i][j].pop_back();
			}
		}
	}
}

void Set() {

	for (int i = 0; i < n; i++) {
		fill(dirBoard[i], dirBoard[i] + n, -1);
		fill(rdirBoard[i], rdirBoard[i] + n, -1);
	}

	int x, y, dir;
	x = y = dir = 0;
	dirBoard[x][y] = dir + 2;
	rdirBoard[x][y] = dir;
	while (!(x == (n - 1) / 2 && y == (n - 1) / 2)) {
		int nx = x + dx[dir];
		int ny = y + dy[dir];
		if (OOB(nx, ny) || dirBoard[nx][ny] >= 0) {
			dir = (dir + 1) % 4;
			continue;
		}
		dirBoard[nx][ny] = (dir + 2) % 4;
		rdirBoard[x][y] = dir;
		x = nx; y = ny;
	}
}

void MoveMonster() {
	int nx = mx + dx[md];
	int ny = my + dy[md];

	if (nx == 0 && ny == 0 && dirFlag) dirFlag = false;
	if (nx == (n - 1) / 2 && ny == (n - 1) / 2 && !dirFlag) dirFlag = true;

	if (dirFlag)
		md = dirBoard[nx][ny];
	else
		md = rdirBoard[nx][ny];
	
	mx = nx;
	my = ny;

}

int CatchTarget() {
	int ret = 0;
	for (int s = 0; s <= 2; s++) {
		int nx = mx + s * dx[md];
		int ny = my + s * dy[md];
		if (OOB(nx, ny) || tree[nx][ny]) continue;
		if (target[nx][ny].empty()) continue;
		ret += target[nx][ny].size();
		target[nx][ny].clear();
	}
	return ret;
}

int main() {
	ios::sync_with_stdio(0), cin.tie(0);
	cin >> n >> m >> h >> k;
	
	mx = my = (n - 1) / 2;
	md = 2;

	Set();

	while (m--) {
		int x, y, d;
		cin >> x >> y >> d; // 1 좌우, 2 상하
		target[x - 1][y - 1].push_back(d);
	}

	while (h--) {
		int x, y;
		cin >> x >> y;
		tree[x - 1][y - 1] = 1;
	}

	int ans = 0;
	for (int t = 1; t <= k; t++) {
		MoveTarget();
		MoveMonster();
		int cnt = CatchTarget();
		ans += t * cnt; 
	}

	cout << ans;
}