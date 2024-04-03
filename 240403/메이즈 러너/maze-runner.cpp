#include <iostream>
#include <vector>
using namespace std;
int n, m, k;
int ex, ey;
int persons[12][12];
int board[12][12];
int dx[4] = { -1, 1, 0, 0 }; // 상하좌우
int dy[4] = { 0, 0, -1, 1 }; 

// 참가자는 한칸에 중복될 수 있다.
// 움직일 수 있는 칸이 2개 이상이라면, 상하로 움직이는 것을 우선시합니다.


bool OOB(int x, int y) {
	return (x < 0 || x >= n || y < 0 || y >= n);
}
int Move() {
	int cPersons[12][12] = {};

	int movedDist = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (persons[i][j] == 0) continue; 
			int curMinDist = abs(i - ex) + abs(j - ey); 

			int moveFlag = 0;
			for (int dir = 0; dir < 4; dir++) {
				int nx = i + dx[dir];
				int ny = j + dy[dir];
				if (OOB(nx, ny) || board[nx][ny] > 0) continue;
				int nMinDist = abs(nx - ex) + abs(ny - ey); 
				if (nMinDist == 0) {
					// 탈출
					m -= persons[i][j];
					movedDist += persons[i][j];
					moveFlag = 1; break; 
				}
				else if (nMinDist < curMinDist) {
					// 이동
					movedDist += persons[i][j];
					cPersons[nx][ny] += persons[i][j];
					moveFlag = 1; break; 
				}
			}
			if (moveFlag) continue;
			// 움직이지 못함
			cPersons[i][j] += persons[i][j];
		}
	}

	// 동시처리를 위한 보드 카피
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			persons[i][j] = cPersons[i][j];

	return movedDist;

}

void Rotate() {
	int diff = 1; // 변의 길이 2

	int fx, fy;

	int findFlag = 0;
	while (!findFlag) { // 가장 작은 기준 사각형 좌상단 모서리 찾기 
		for (int x = ex - diff; x <= ex; x++) {
			for (int y = ey - diff; y <= ey; y++) {
				if (OOB(x, y)) continue;
				// 좌상단 기준 x, y


				for (int rx = x; rx <= x + diff; rx++) {
					for (int ry = y; ry <= y + diff; ry++) {
						if (OOB(rx, ry)) continue;
						if (persons[rx][ry]) {
							findFlag = 1;
							fx = x; fy = y;
							break;
						}
					}
					if (findFlag) break;
				}

				if (findFlag) break;
			}
			if (findFlag) break;
		}
		if (findFlag) break;
		diff++; 
	} 
	
	// 시계방향 90도 회전
	int cBoard[17][17] = {};
	int cPersons[17][17] = {};

	for (int i = 0; i <= diff; i++) {
		for (int j = 0; j <= diff; j++) {
			cBoard[j][diff - i] = board[i + fx][j + fy];
			cPersons[j][diff - i] = persons[i + fx][j + fy];
		}
	}

	for (int i = 0; i <= diff; i++) {
		for (int j = 0; j <= diff; j++) {
			board[i + fx][j + fy] = cBoard[i][j];
			persons[i + fx][j + fy] = cPersons[i][j];

			if (board[i + fx][j + fy] == -1) {
				ex = i + fx; ey = j + fy;
			}
			else if (board[i + fx][j + fy] > 0) {
				board[i + fx][j + fy]--;
			}
		}
	}
}

int main(void) {
	cin >> n >> m >> k;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
			
		}
	}

	for(int i = 0 ; i < m; i++){
		int x, y;
		cin >> x >> y;
		persons[x - 1][y - 1]++;
	}

	int x, y;
	cin >> x >> y;
	ex = x - 1; ey = y - 1;
	board[ex][ey] = -1; // 출구

	
	int sum = 0;
	for (int i = 1; i <= k; i++) {
		sum += Move();
		if (m == 0) break;
		Rotate();
	}

	cout << sum << '\n';
	cout << ex + 1 << ' ' << ey + 1;
}