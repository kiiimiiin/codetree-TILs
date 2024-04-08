#include <iostream>
using namespace std; 

int n;
int board[502][502];
int dirBoard[502][502];
int cx, cy;
int dx[4] = { 0, 1, 0, -1  };
int dy[4] = { 1, 0, -1, 0 }; // 0우1하2좌3상
int ans;

int dustRatio[4][5][5] = {
	{
		{0, 0, 2, 0, 0},
		{0, 1, 7, 10, 0},
		{0, 0, 0, 0, 5},
		{0, 1, 7, 10, 0},
		{0, 0, 2, 0, 0},
	},
	{
		{0, 0, 0, 0, 0},
		{0, 1, 0, 1, 0},
		{2, 7, 0, 7, 2},
		{0, 10, 0, 10, 0},
		{0, 0, 5, 0, 0},
	},
	{
		{0, 0, 2, 0, 0},
		{0, 10, 7, 1, 0},
		{5, 0, 0, 0, 0},
		{0, 10, 7, 1, 0},
		{0, 0, 2, 0, 0},
	},
	{
		{0, 0, 5, 0, 0},
		{0, 10, 0, 10, 0},
		{2, 7, 0, 7, 2},
		{0, 1, 0, 1, 0},
		{0, 0, 0, 0, 0},
	},
};

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

void SpreadDust(int cnx, int cny, int dir) {
	int curDust = board[cnx][cny];
	board[cnx][cny] = 0;

	int sum = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			int addedDust = curDust * dustRatio[dir][i][j] / 100;
			int snx = cnx + i - 2;
			int sny = cny + j - 2;
			
			sum += addedDust; 
			if (OOB(snx, sny)) ans += addedDust;
			else board[snx][sny] += addedDust;
		}
	}

	int ax = cnx + dx[dir];
	int ay = cny + dy[dir];
	int aDust = curDust - sum;
	if (OOB(ax, ay)) ans += aDust;
	else board[ax][ay] += aDust;
}



void SpreadDust() {
	while (!(cx == 0 && cy == 0)) {
		int cnx = cx + dx[dirBoard[cx][cy]];
		int cny = cy + dy[dirBoard[cx][cy]];

		
		SpreadDust(cnx, cny, dirBoard[cx][cy]);
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