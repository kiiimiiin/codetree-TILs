#include <iostream>
using namespace std;
int n, m, k, c; // k : 제초제 확산 범위, c : 제초제 남아있는 년 수
int board[22][22]; // 나무 및 제초제 등
int rBoard[22][22]; // 제초제 존재시간 보드
int dx[4] = { 1, 0, -1, 0 };
int dy[4] = { 0, -1, 0, 1 }; 


bool OOB(int x, int y) {
	return (x < 0 || x >= n || y < 0 || y >= n);
}
void Grow() {
	
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			if (board[x][y] > 0) {
				int nearTree = 0;
				for (int dir = 0; dir < 4; dir++) {
					int nx = x + dx[dir];
					int ny = y + dy[dir];
					if (OOB(nx, ny)) continue;
					if (board[nx][ny] > 0)	nearTree++;
				}
				board[x][y] += nearTree; 
			}
		}
	}
}

void Copy() {
	int addedBoard[22][22] = {};
	
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			if (board[x][y] > 0) {
				int nearEmpty = 0;
				for (int dir = 0; dir < 4; dir++) {
					int nx = x + dx[dir];
					int ny = y + dy[dir];
					if (OOB(nx, ny)) continue;
					if (board[nx][ny] == 0 && !rBoard[nx][ny]) nearEmpty++;
				}

				for (int dir = 0; dir < 4; dir++) {
					int nx = x + dx[dir];
					int ny = y + dy[dir];
					if (OOB(nx, ny)) continue;
					if (board[nx][ny] == 0 && !rBoard[nx][ny])
						addedBoard[nx][ny] += board[x][y] / nearEmpty;
				}
			}
		}
	}

	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			board[x][y] += addedBoard[x][y];
		}
	}
}


int Remove() {
	// 1.최대 박멸 칸 택
	int rdx[4] = { 1, 1, -1, -1 };
	int rdy[4] = { 1, -1, 1, -1 };
	
	int cx, cy;
	int mx = -0x7f7f7f7f;

	for (int x = n - 1; x >= 0; x--) {
		for (int y = n -1; y >= 0; y--) {
			if (board[x][y] > 0) {
				int removedSum = board[x][y];
				
				for (int dir = 0; dir < 4; dir++) {
					for (int s = 1; s <= k; s++) {
						int rnx = x + s * rdx[dir];
						int rny = y + s * rdy[dir];
						if (OOB(rnx, rny) || !(board[rnx][rny] > 0)) break;
						removedSum += board[rnx][rny];
					}
				}
				if (removedSum >= mx) {
					mx = removedSum;
					cx = x;
					cy = y;
				}
			}
		}
	}

	// 2. 제초제 뿌림 , 뿌리기전 제초제 업데이트
	
	

	for (int x = 0; x < n; x++)
		for (int y = 0; y < n; y++)
			if (rBoard[x][y]) rBoard[x][y]--;

	if (mx == -0x7f7f7f7f) return 0; // EdheCase 확인!! 
	
	rBoard[cx][cy] = c;
	board[cx][cy] = 0;
	for (int dir = 0; dir < 4; dir++) {
		for (int s = 1; s <= k; s++) {
			int cnx = cx + s * rdx[dir];
			int cny = cy + s * rdy[dir];
			if (OOB(cnx, cny)) break;
			rBoard[cnx][cny] = c;
			
			if (board[cnx][cny] <= 0) break;
			else board[cnx][cny] = 0;
		}
	}
	
	return mx;
}

void Test() {
	cout << "\nTestBoard:\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << board[i][j] << ' ';
		}
		cout << '\n';
	}

	cout << "\nTestKiller:\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << rBoard[i][j] << ' ';
		}
		cout << '\n';
	}

}
int main(void) {
	cin >> n >> m >> k >> c;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}
		

	int ans = 0;
	while (m--) {
		
		Grow(); 
		Copy();
		ans += Remove();
	}

	cout << ans; 
}