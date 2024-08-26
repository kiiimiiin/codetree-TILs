#include <iostream>
#include <vector>
using namespace std; 

struct Sullae {
	int x, y;
	int d = 0;
	bool isOuter = true;
};

int board[102][102]; // 나무, 술래
int runner[102][102][4];
int outer[102][102]; // 술래의 경로확인용
int inner[102][102];
const int dx[4] = { 1,0,-1,0 }; // 하 우 상 좌
const int dy[4] = { 0, 1, 0, -1 };

int n, m, h, k, c, t;
Sullae s;

int getDistFromSullae(int i, int j) {
	return abs(i - s.x) + abs(j - s.y);
}

bool IsEmptyRunner(int i, int j) {
	for (int k = 0; k < 4; k++) 
		if (runner[i][j][k]) return false;
	return true;
}

bool ExistSullae(int i, int j) {
	return i == s.x && j == s.y;
}

bool OOB(int x, int y) {
	return x < 0 || x >= n || y < 0 || y >= n;
}

void testRunner() {
	cout << '\n';
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (IsEmptyRunner(i, j)) cout << "0 ";
			else cout << "1 ";
		}
		cout << '\n';
	}
	
}

void MoveRunner() {
	int temp[102][102][4] = {};
	const int dx[4] = { 0,0,1,-1 }; // 좌우하상
	const int dy[4] = { -1,1,0,0 };


	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (IsEmptyRunner(i, j)) continue;
			if (getDistFromSullae(i, j) > 3) continue;
			
			for (int k = 0; k < 4; k++) {
				int nx = i + dx[k];
				int ny = j + dy[k];
				if (!OOB(nx, ny)) {
					if (ExistSullae(nx, ny))
						temp[i][j][k] += runner[i][j][k];
					else
						temp[nx][ny][k] += runner[i][j][k];
				}
				else {
					int nd;
					if (k == 1 || k == 3) nd = k - 1;
					else nd = k + 1;
					nx = i + dx[nd];
					ny = j + dy[nd];

					if (ExistSullae(nx, ny))
						temp[i][j][nd] += runner[i][j][k];
					else
						temp[nx][ny][nd] += runner[i][j][k];
				}
			}
		}
	}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) 
			for (int k = 0; k < 4; k++) 
				runner[i][j][k] = temp[i][j][k];

	// testRunner();

}

int Catch() {
	int ret = 0;
	for (int p = 0; p < 3; p++) {
		int nx = s.x + p * dx[s.d];
		int ny = s.y + p * dy[s.d];
		if (OOB(nx, ny)) break;
		if (board[nx][ny] == 2 || IsEmptyRunner(nx,ny)) continue;
		for (int k = 0; k < 4; k++) {
			ret += runner[nx][ny][k];
			runner[nx][ny][k] = 0;
		}
			
	}
	ret *= t;
	return ret;
}

int MoveSullae() {
	int ret = 0;
	if (s.isOuter) {
		int nx = s.x + dx[outer[s.x][s.y]];
		int ny = s.y + dy[outer[s.x][s.y]];
		int nd = outer[nx][ny];
		s.x = nx; s.y = ny; s.d = nd;
		if (s.x == 0 && s.y == 0) s.isOuter = false;
	}
	else {
		int nx = s.x + dx[inner[s.x][s.y]];
		int ny = s.y + dy[inner[s.x][s.y]];
		int nd = inner[nx][ny];
		s.x = nx; s.y = ny; s.d = nd;
		if (s.x == c && s.y == c) s.isOuter = true;
	}

	ret += Catch();
	return ret;
	
}

void SetSnail() {
	
	int x, y, d;
	
	
	for (int i = 0; i < n; i++) {
		fill(outer[i], outer[i] + n, -1);
		fill(inner[i], inner[i] + n, -1);
	}
	
	x = y = 0;
	d = 0;
	outer[0][0] = 0;

	while(!(x == c && y == c)) {
		int nx = x + dx[d];
		int ny = y + dy[d];
		if (OOB(nx,ny) || outer[nx][ny] >= 0) {
			d = (d + 1) % 4;
			continue;
		}	
		outer[nx][ny] = (d + 2) % 4;
		x = nx;
		y = ny;
	}

	x = y = 0;
	d = 0;
	
	while (!(x == c && y == c)) {
		int nx = x + dx[d];
		int ny = y + dy[d];
		if (OOB(nx, ny) || inner[nx][ny] >= 0) {
			d = (d + 1) % 4;
			continue;
		}
		inner[x][y] = d;
		x = nx;
		y = ny;
	}
	
	inner[c][c] = 2;

}

int main() {
	cin >> n >> m >> h >> k;
	c = (n - 1) / 2;
	for (int i = 0; i < m; i++) {
		int x, y, d;
		cin >> x >> y >> d;
		x--; y--;
		runner[x][y][d]++;
	}

	for (int i = 0; i < h; i++) {
		int x, y;
		cin >> x >> y;
		x--; y--;
		board[x][y] = 2;
	}
	
	s.x = s.y = c;

	SetSnail();

	int ans = 0;
	for (t = 1; t <= k; t++) {
		MoveRunner();
		ans += MoveSullae();
	}

	cout << ans;
}

/*
	1 -> 0 -> 1 
	우 좌
	2 -> 3 -> 2
	하  상
*/