#include <iostream>
#include <tuple>
#include <vector>
#define X first
#define Y second
using namespace std;
int n, m, h, k; // m : 도망자 수 , h : 나무수 , k : 턴수
int board[101][101];
int treeBoard[101][101]; 
vector<int> targetBoard[101][101];
vector<pair<int, int>> trees; 
int hdx[4] = { 0, 1, 0, -1 }; // 우하좌상 
int hdy[4] = { 1, 0, -1, 0 }; 
int sdx[4] = { 1, 0, -1, 0 }; // 하 우 상 좌
int sdy[4] = { 0, 1, 0, -1 };
int dirBoard[101][101];
int rdirBoard[101][101]; 
int t;
int cx, cy;
int rflag; 


bool OOB(int x, int y) {
	return (x < 0 || x >= n || y < 0 || y >= n);
}

void SubMoveTarget(vector<pair<int,int>>& posForMoved) {
	vector<int> cBoard[101][101];


	for (auto pos : posForMoved) { // 도망자 있는 좌표에 대해
		for (auto td : targetBoard[pos.X][pos.Y]) { // 바뀐 도망자 위치 복사
			int nx = pos.X + hdx[td];
			int ny = pos.Y + hdy[td];
			if (!OOB(nx, ny)) {
				if (nx == cx && ny == cy)
					cBoard[pos.X][pos.Y].push_back(td);
				else 
					cBoard[nx][ny].push_back(td);
			}
			else {
				td = (td + 2) % 4;
				nx = pos.X + hdx[td];
				ny = pos.Y + hdy[td];
				if (nx == cx && ny == cy)
					cBoard[pos.X][pos.Y].push_back(td);
				else
					cBoard[nx][ny].push_back(td);
			}
		}
		targetBoard[pos.X][pos.Y].clear();
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (cBoard[i][j].empty()) continue;
			for (int td : cBoard[i][j]) {
				targetBoard[i][j].push_back(td);
			}
		}
	}
}


void MakeSnailDir() {
	for (int i = 0; i < n; i++) {
		fill(dirBoard[i], dirBoard[i] + n, -1);
		fill(rdirBoard[i], rdirBoard[i] + n, -1);
	}

	int x, y, nx, ny, dir;
	x = y = 0;
	dir = 0; 

	dirBoard[0][0] = ( dir + 2 ) % 4;
	

	while (!(x == (n - 1)/2 && y == (n - 1) /2 )){
		nx = x + sdx[dir];
		ny = y + sdy[dir];
		if (nx >= n || ny >= n || nx < 0 || dirBoard[nx][ny] >= 0) {
			dir = (dir + 1) % 4;
			continue;
		}
		dirBoard[nx][ny] = ( dir + 2 ) % 4;
		x = nx;
		y = ny; 
	}

	x = y = 0;
	dir = 0;
	
	while (!(x == (n - 1) / 2 && y == (n - 1) / 2)) {
		nx = x + sdx[dir];
		ny = y + sdy[dir];
		if (nx >= n || ny >= n || nx < 0 || rdirBoard[nx][ny] >= 0) {
			dir = (dir + 1) % 4;
			continue;
		}
		rdirBoard[x][y] = dir;
		x = nx;
		y = ny;
	}

	rdirBoard[x][y] = 0;

}

void MoveTarget() {
	int cdx[25] = { 
		3,
		2, 2, 2,
		1, 1, 1, 1, 1, 
		0, 0, 0, 0, 0, 0, 0,
		-1, -1,-1,-1, -1,
		-2, -2, -2,
		-3,
	};
	int cdy[25] = { 
		0,
		-1, 0, 1,
		-2, -1, 0, 1, 2,
		-3, -2, -1, 0, 1, 2, 3,
		-2, -1, 0, 1, 2,
		-1, 0, 1,
		0,
	};

	vector<pair<int, int>> posForMoved; 
	for (int dir = 0; dir < 25; dir++) {
		int nx = cx + cdx[dir];
		int ny = cy + cdy[dir];
		if (OOB(nx, ny)) continue;
		if (!targetBoard[nx][ny].empty()) {
			posForMoved.push_back({ nx,ny });
		}
	}

	SubMoveTarget(posForMoved);

}

int MoveRemover() {
	
	int cd = rflag ? rdirBoard[cx][cy] : dirBoard[cx][cy];
	int score = 0;

	cx = cx + sdx[cd];
	cy = cy + sdy[cd];

	if (cx == 0 && cy == 0) rflag = 1;
	else if (cx == (n - 1) / 2 && cy == (n - 1) / 2) rflag = 0;
	

	cd = rflag ? rdirBoard[cx][cy] : dirBoard[cx][cy] ;
	
	for (int s = 0; s <= 2; s++) {
		int fx = cx + s * sdx[cd];
		int fy = cy + s * sdy[cd];
		if (OOB(fx, fy)) continue;
		if (!targetBoard[fx][fy].empty() && !treeBoard[fx][fy]) {
			score += t * targetBoard[fx][fy].size(); 
			targetBoard[fx][fy].clear();
		}
	}

	return score;
}
int main(void) {

	cin >> n >> m >> h >> k;

	cx = cy = (n - 1) / 2;

	for (int i = 0; i < m; i++) {
		int x, y, dir;
		cin >> x >> y >> dir; // dir 1: 우 dir 2 :하
		targetBoard[x - 1][y - 1].push_back(dir - 1);
	}

	for (int i = 0; i < h; i++) {
		int x, y;
		cin >> x >> y;
		treeBoard[x - 1][y - 1] = 1;
	}

	MakeSnailDir();

	int ans = 0;
	for (t = 1; t <= k; t++) {
		MoveTarget();
		ans += MoveRemover();
	}

	cout << ans;
}