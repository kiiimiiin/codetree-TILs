#include <iostream>
#include <vector>
#include <queue>
using namespace std;
int n, m, cx, cy;
int dx[4] = { 0, 1, 0, -1 }; // 우 하 좌 상
int dy[4] = { 1, 0, -1, 0 };
int board[27][27]; // 해당 좌표의 몬스터
int numBoard[27][27]; // 해당 좌표가 몇번째 칸인지
int monsterByNum[627]; // 칸에 해당하는 몬스터가 무엇인지
int ans = 0;


void Init() {
	for (int i = 0; i < n; i++) fill(numBoard[i], numBoard[i] + n, -1);
	int x, y, dir;
	x = y = dir = 0;
	numBoard[x][y] = n * n - 1;
	monsterByNum[numBoard[x][y]] = board[x][y];

	while (numBoard[cx][cy] == -1) {
		int nx = x + dx[dir];
		int ny = y + dy[dir];
		if (ny >= n || nx >= n || ny < 0 || numBoard[nx][ny] >= 0) {
			dir = (dir + 1) % 4;
			nx = x + dx[dir];
			ny = y + dy[dir];
		}
		numBoard[nx][ny] = numBoard[x][y] - 1;
		monsterByNum[numBoard[nx][ny]] = board[nx][ny];
		x = nx;
		y = ny;
	}
	
}

void Push(int fNum) {
	queue<int> pushedMonster;
	for (int num = fNum; num < n * n; num++) {
		if (monsterByNum[num] >= 1)
			pushedMonster.push(monsterByNum[num]);
	}

	for (int num = fNum; num < n * n; num++) {
		if (!pushedMonster.empty()) {
			monsterByNum[num] = pushedMonster.front();
			pushedMonster.pop();
		}
		else {
			monsterByNum[num] = 0;
		}
	}
}

int Simulate(int d, int p) {

	// 1. 방향 공격
	// 공격지점 몬스터 빈칸 처리
	int score = 0;
	for (int s = 1; s <= p; s++) { 
		int nx = cx + s * dx[d];
		int ny = cy + s * dy[d];
		score += monsterByNum[numBoard[nx][ny]];
		monsterByNum[numBoard[nx][ny]] = 0;

	}

	
	// 공격지점 몬스터 push
	int fx = cx + dx[d];
	int fy = cy + dy[d];
	int fNum = numBoard[fx][fy];
	Push(fNum);
	
	

	// 2. 중첩 몬스터 처리
	
	
	
	while (1) {
		int fNum = -0x7f7f7f7f;
		int fFlag = 1;
		int idx;
		for (int num = 1; num < n * n; num += idx) {
			if (monsterByNum[num] == 0) break;
			idx = 1;
			while ((num + idx < n * n)
				&& monsterByNum[num + idx] == monsterByNum[num]) {
				idx++;
			}

			if (idx >= 4) {
				for (int i = num; i < num + idx; i++) {
					if (fFlag) { fNum = i; fFlag = 0; }
					score += monsterByNum[i];
					monsterByNum[i] = 0;
				}
			}
		}
		

		if (fFlag) break;
		Push(fNum);
	}
	
	// 3. 보드 업데이트
	queue<int> newMonster;
	int idx;
	for (int num = 1; num < n * n; num += idx) {
		if (monsterByNum[num] == 0) break;
		idx = 1;
		while ((num + idx < n * n)
			&& monsterByNum[num + idx] == monsterByNum[num]) {
			idx++;
		}
		newMonster.push(idx);
		newMonster.push(monsterByNum[num]);
	}

	for (int num = 1; num < n * n; num++) {
		if (!newMonster.empty()) {
			monsterByNum[num] = newMonster.front();
			newMonster.pop();
		}
		else {
			monsterByNum[num] = 0;
		}
	}

	return score;
}

int main(void) {
	cin >> n >> m;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}

	int ans = 0;
	cx = cy = (n - 1) / 2;
	
	Init(); 

	while (m--) {
		int d, p;
		cin >> d >> p;
		ans += Simulate(d, p);
	}

	cout << ans;
}