#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <queue>
#define X first
#define Y second
using namespace std;
struct Warriors {
	int x, y;
	int h, w;
};
int dx[4] = { -1, 0, 1, 0 }; // 상우하좌
int dy[4] = { 0, 1, 0, -1 }; 
int board[42][42]; // 빈칸,함정,벽 보드
int wBoard[42][42]; // 워리어 보드
int hp[32]; // 기사번호에 대한 체력 
int dmgSum[32]; // 기사가 받은 데미지합
int l, n, q;
vector<Warriors> warriors;



bool OOB(int x, int y) {
	return (x < 0 || x >= l || y < 0 || y >= l);
}

bool IsMovedSpace(int idx, vector<int>& movedIdxList) {
	if (idx == 0) return false;

	for (auto movedIdx : movedIdxList) {
		if (idx == movedIdx) return true;
	}
	return false;
}

void Move(int wIdx, int dir, int movedIdx[32]) {
	auto w = warriors[wIdx];
	movedIdx[wIdx] = 1;

	if (dir == 0) { // 상
		for (int j = w.y; j <= w.y + w.w; j++) {
			if (board[w.x + dx[dir]][j] == 2 || OOB(w.x + dx[dir], j)) {
				movedIdx[0] = 1;
				return;
			}
			else if (wBoard[w.x + dx[dir]][j] > 0) {
				if (movedIdx[wBoard[w.x + dx[dir]][j]]) continue;
				Move(wBoard[w.x + dx[dir]][j], dir, movedIdx);
			}
		}
	}
	else if (dir == 1) { // 우
		for (int i = w.x; i <= w.x + w.h; i++) {
			if (board[i][w.y + w.w + dy[dir]] == 2 || OOB(i, w.y + w.w + dy[dir])) {
				movedIdx[0] = 1;
				return;
			}
			else if (wBoard[i][w.y + w.w + dy[dir]] > 0) {
				if (movedIdx[wBoard[i][w.y + w.w + dy[dir]]]) continue;
				Move(wBoard[i][w.y + w.w + dy[dir]], dir, movedIdx);
			}
		}
	}
	else if (dir == 2) { // 하
		for (int j = w.y; j <= w.y + w.w; j++) {
			if (board[w.x + w.h + dx[dir]][j] == 2 || OOB(w.x + w.h + dx[dir], j)){
				movedIdx[0] = 1;
				return;
			}
			else if (wBoard[w.x + w.h + dx[dir]][j] > 0) {
				if (movedIdx[wBoard[w.x + w.h + dx[dir]][j]]) continue;
				Move(wBoard[w.x + w.h + dx[dir]][j], dir, movedIdx);
			}
		}
	}
	else { // 좌
		for (int i = w.x; i <= w.x + w.h; i++) {
			if (board[i][w.y + dy[dir]] == 2 || OOB(i, w.y + dy[dir])) {
				movedIdx[0] = 1;
				return;
			}
			else if (wBoard[i][w.y + dy[dir]] > 0) {
				if (movedIdx[wBoard[i][w.y + dy[dir]]]) continue;
				Move(wBoard[i][w.y + dy[dir]], dir, movedIdx);
			}
		}
	}
}

void Simulate(int wIdx, int dir) {
	// 1. 기사이동

	if (hp[wIdx] > 0) {
		

		int movedIdx[32] = {};
		
		Move(wIdx, dir,  movedIdx);
		
		if (movedIdx[0] == 0) { // 이동가능하면
			int movedWBoard[42][42] = {};
			vector<int> movedIdxList;
			for (int i = 1; i <= n; i++)
				if (movedIdx[i]) movedIdxList.push_back(i);

			for (int i = 0; i < l; i++) { // 동시 이동을 위해 새로운보드에 이동
				for (int j = 0; j < l; j++) {
					if (IsMovedSpace(wBoard[i][j], movedIdxList)) {
						movedWBoard[i + dx[dir]][j + dy[dir]] = wBoard[i][j];
						wBoard[i][j] = 0;
					}
				}
			}

			for (int i = l -1 ; i >= 0; i--) { // 이동한 기사 원복, 밀쳐진 기사 데미지
				for (int j = l - 1; j >= 0; j--) {
					if (movedWBoard[i][j]) {
						wBoard[i][j] = movedWBoard[i][j];
						warriors[wBoard[i][j]].x = i;
						warriors[wBoard[i][j]].y = j; // x,y 위치 수정
						if ((wBoard[i][j] && wBoard[i][j] != wIdx) && board[i][j] == 1) {
							hp[wBoard[i][j]]--;
							dmgSum[wBoard[i][j]]++;
						} // 움직여진 위치만큼 데미지 반영
					}
				}
			}

			for (int idx = 1; idx <= n; idx++) { // 밀린 이후에 데미지 처리
				if (hp[idx] <= 0) {
					for (int i = 0; i < l; i++) {
						for (int j = 0; j < l; j++) {
							if (wBoard[i][j] == idx)
								wBoard[i][j] = 0;
						}
					}
				}
			}
		}
	}
}

int main(void) {
	cin >> l >> n >> q;
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < l; j++) {
			cin >> board[i][j];
		}
	}

	warriors.resize(1);
	for(int wIdx = 1; wIdx <= n; wIdx++){
		int r, c, h, w, k;
		cin >> r >> c >> h >> w >> k;
		r = r - 1; c = c - 1; // 0,0 기준으로
		h = h - 1; w = w - 1;
		warriors.push_back({ r, c , h, w});
		hp[wIdx] = k;
		
		for (int i = r; i <= r + h; i++)
			for (int j = c; j <= c + w; j++)
				wBoard[i][j] = wIdx;
		
	}

	while (q--) {
		int wIdx, dir;
		cin >> wIdx >> dir;
		Simulate(wIdx, dir); 
	}

	// 생존한 기사의 데미지 합만 출력
	int ans = 0;
	for (int wIdx = 1; wIdx <= n; wIdx++) {
		if (hp[wIdx] > 0) ans += dmgSum[wIdx];
	}
	cout << ans;
}