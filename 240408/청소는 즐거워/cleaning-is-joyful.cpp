#include <iostream>
using namespace std; 

int n;
int board[502][502];
int dirBoard[502][502];
int cx, cy;
int dx[4] = { 0, 1, 0, -1  };
int dy[4] = { 1, 0, -1, 0 }; // 0우1하2좌3상
int ans;

void MakeSnail() {
	
	for (int i = 0; i < n; i++) fill(dirBoard[i], dirBoard[i] + n, -1); 

	int x, y, dir;
	x = y = dir = 0;
	dirBoard[x][y] = (dir + 2) % 4;

	while (dirBoard[cx][cy] == -1) {
		
		int nx = x + dx[dir];
		int ny = y + dy[dir];

		if (ny >= n || nx >= n || nx < 0 || dirBoard[nx][ny] >= 0) {
			dir = (dir + 1) % 4;
			nx = x + dx[dir];
			ny = y + dy[dir]; 
		}

		dirBoard[nx][ny] = (dir + 2) % 4;
		x = nx; y = ny;
	}

}

bool OOB(int x, int y) {
	return (x < 0 || x >= n || y < 0 || y >= n);
}

void SpreadDust(int cnx, int cny, int sdx[10], int sdy[10]) {

	int curDust = board[cnx][cny];
	int sum = 0;
	board[cnx][cny] = 0;

	for (int dir = 0; dir < 10; dir++) {
		int sx = cnx + sdx[dir];
		int sy = cny + sdy[dir];
		int dDust = 0;

		if (dir == 0 || dir == 1)
			dDust = (curDust * 1) / 100;
		else if (dir == 2 || dir == 3)
			dDust = (curDust * 2) / 100;
		else if (dir == 4)
			dDust = (curDust * 5) / 100;
		else if (dir == 5 || dir == 6)
			dDust = (curDust * 7) / 100;
		else if (dir == 7 || dir == 8)
			dDust = (curDust * 10) / 100;
		else if (dir == 9)
			dDust = curDust - sum;

		sum += dDust;

		if (OOB(sx, sy)) ans += dDust;
		else board[sx][sy] += dDust;
	}
}



void SpreadDust() {
	while (!(cx == 0 && cy == 0)) {
		int cnx = cx + dx[dirBoard[cx][cy]];
		int cny = cy + dy[dirBoard[cx][cy]];

		if (dirBoard[cx][cy] == 0) {
			int sdx[10] = { -1, 1, -2, 2, 0, -1, 1, -1, 1, 0 };
			int sdy[10] = { -1, -1, 0, 0, 2, 0, 0, 1, 1, 1 };
			SpreadDust(cnx, cny, sdx, sdy);
		}
		else if (dirBoard[cx][cy] == 1) {
			int sdx[10] = { -1, -1, 0, 0, 2, 0, 0, 1, 1, 1 };
			int sdy[10] = { -1, 1, -2, 2, 0, -1, 1, -1, 1, 0 };
			SpreadDust(cnx, cny, sdx, sdy);
		}
		else if (dirBoard[cx][cy] == 2) {
			int sdx[10] = { -1, 1, -2, 2, 0, -1, 1, -1, 1, 0 };
			int sdy[10] = { 1, 1, 0, 0, -2, 0, 0, -1, -1, -1 };
			SpreadDust(cnx, cny, sdx, sdy);
		}
		else if (dirBoard[cx][cy] == 3) {
			int sdx[10] = { 1, 1, 0, 0, -2, 0, 0, -1, -1, -1 };
			int sdy[10] = { -1, 1, -2, 2, 0, -1, 1, -1, 1, 0 };
			SpreadDust(cnx, cny, sdx, sdy);
		}
		cx = cnx; cy = cny; 
	}
}


int main(void) {
	cin >> n;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}

	cx = cy = (n - 1) / 2;
	MakeSnail();
	SpreadDust();

	cout << ans;
}