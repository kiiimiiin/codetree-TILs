#include <iostream>
#include <tuple>
#include <algorithm>
#include <utility>
#include <queue>
#define X first
#define Y second
using namespace std; 
const int dx[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
const int dy[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
int m, t, px, py;

vector<int> monster[4][4];
vector<int> egg[4][4];
int dead[4][4];

bool OOB(int x, int y) {
	return x < 0 || x >= 4 || y < 0 || y >= 4;
}

void Copy() {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if(!monster[i][j].empty())
				egg[i][j] = monster[i][j];
}

bool ExistPack(int x, int y) {
	return x == px && y == py; 
}

void SubMove(int i, int j, vector<int> tmp[4][4]) { // call by ref
	for (auto d : monster[i][j]) {
		int nd, nx, ny, dd;
		for (dd = 0; dd < 8; dd++) {
			nd = (d + dd) % 8;
			nx = i + dx[nd];
			ny = j + dy[nd]; 
			if (OOB(nx, ny) || ExistPack(nx,ny) || dead[nx][ny]) continue;
			break;
		}
	
		if (dd == 8) {
			tmp[i][j].push_back(d);
			continue;
		}

		tmp[nx][ny].push_back(nd); 
	}
}

void MoveMonster() {

	vector<int> tmp[4][4];


	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (monster[i][j].empty()) continue;
				SubMove(i, j, tmp);
		}
	}

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			monster[i][j] = tmp[i][j];

}

void dfs(int k, int cnt, int x, int y, int& mx, int vis[4][4], int pdrr[3], int mxpdrr[3]) {
	if (k == 3) {
		if (cnt > mx) {
			for (int i = 0; i < 3; i++)
				mxpdrr[i] = pdrr[i];
			mx = cnt;
		}
		return;
	}

	for (int dir : {0, 2, 4, 6}) {
		int nx = x + dx[dir];
		int ny = y + dy[dir];
		if (OOB(nx, ny)) continue;

		pdrr[k] = dir;
		
		if (!vis[nx][ny]) {
			vis[nx][ny]++;
			dfs(k + 1, cnt + monster[nx][ny].size(), nx, ny, mx, vis, pdrr, mxpdrr);
			vis[nx][ny]--;
		}
		else {
			dfs(k + 1, cnt, nx, ny, mx, vis, pdrr, mxpdrr);
		}
	}
}

void MovePack(int pdrr[3]) {

	for (int i = 0; i < 3; i++) {
		int nd = pdrr[i];
		int nx = px + dx[nd];
		int ny = py + dy[nd];
		if (!monster[nx][ny].empty()) {
			monster[nx][ny].clear();
			dead[nx][ny] = 3;
		}
		px = nx;
		py = ny;
	}
}

void UpdateDead() {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (dead[i][j]) dead[i][j]--;
}

void Paste() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			while (!egg[i][j].empty()) {
				monster[i][j].push_back(egg[i][j].back());
				egg[i][j].pop_back();
			}
		}
	}
}

int getLivedMonster() {
	int ret = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			ret += monster[i][j].size();
	return ret;
}
int main() {
	ios::sync_with_stdio(0), cin.tie(0);

	cin >> m >> t;

	cin >> px >> py;
	px--; py--;

	while (m--) {
		int r, c, d;
		cin >> r >> c >> d;
		monster[r - 1][c - 1].push_back(d - 1);
	}

	
	
	while (t--) {
		Copy();
		MoveMonster();

		int pdrr[3], mxpdrr[3];
		int vis[4][4] = {};
		int mx = -0x7f7f7f7f;

		dfs(0, 0, px, py, mx, vis, pdrr, mxpdrr);
		MovePack(mxpdrr);

		UpdateDead();
		Paste();

	}

	int ans = getLivedMonster();
	cout << ans;
}

/*
	몬스터 복제 시도
	몬스터 이동
	팩맨 이동
	몬스터 시체 소멸
	몬스터 복제 완성
*/